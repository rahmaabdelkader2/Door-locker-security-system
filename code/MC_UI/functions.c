/*
 * functions.c
 *
 *  Created on: Nov 9, 2021
 *      Author: Rahma Abdelkader
 */
#include "keypad.h"
#include "LCD.h"
#include"uart.h"
#include "timer.h"
#include "gpio.h"
#include "commands.h"
#include <avr/io.h>
#include<avr/interrupt.h>
#include <avr/delay.h>
/*******Global Variables*******/


extern uint8 Fisrt_pass [10]; //save first entered password
extern uint8 Second_pass[10] ; //save second entered password
extern uint8 PasswordCheck[10];
extern uint32 g_ticks; /*Global variable indicates number of overflows*/
extern uint8 return_flag; /*Global variable used to know if the password has been entered for 3 times wrong or not*/
extern uint8 counter_error; /*Global variable indicates number of times the entered password was wrong*/
extern uint8 key;
extern uint8 g_error;
extern uint8 checked;
extern uint8 g_check;


// ******** functions **********
void MC1_Get_password(uint8* password)
{
	uint8 digit_count=0;
	uint8 key;
	while(digit_count<6)
	{
		key=KEYPAD_getPressedKey(); //get the pressed key from keypad
		_delay_ms(800);
		if(key >=0 && key<=9)
		{
			password[digit_count]=key;
			LCD_displayStringRowColumn(1, digit_count, "*");/* Display'*'to hide the password for security */
		}
		else
		{
			_delay_ms(1000);
			break;
		}
		_delay_ms(1000);
		digit_count++;
	}
}

void MC1_pass_creating()
{
	LCD_clearScreen(); //first clear screen
	LCD_displayStringRowColumn(0,0,"Enter new Pass:");
	MC1_Get_password(Fisrt_pass);
	UART_sendByte(ready);
	while(UART_receiveByte() != MC2_READY){};

	MC1_send_password(Fisrt_pass);

	while(UART_receiveByte() != received){};
	LCD_clearScreen();        //clear screen
	LCD_displayStringRowColumn(0,0,"re_Enter the Pass:");
	MC1_Get_password(Second_pass);
	UART_sendByte(REPASSWORD);
	while(UART_receiveByte() != send){};
	MC1_send_password(Second_pass);

	while(UART_receiveByte() != received){};
}

void MC1_send_password(const uint8 *array)
{
	for(uint8 digit_count=0;digit_count<5;digit_count++)
	{
		UART_sendByte(array[digit_count]);
	}
}


uint8 Check_pass()
{
	LCD_clearScreen();
	LCD_displayString("Please Enter pass:");
	MC1_Get_password(PasswordCheck);
	UART_sendByte(CHECK_PASS);
	while(UART_receiveByte()!=CHECK_PASS);
	MC1_send_password(PasswordCheck);
	while(UART_receiveByte()!=result);
	UART_sendByte(result);
	g_check=UART_receiveByte();
	return g_check;
}


void MC1_Main_options()
{
	UART_sendByte(start);
	while(UART_receiveByte()!=start);

	LCD_clearScreen(); //clear lcd

	// display options

	LCD_displayStringRowColumn(0,0,"+:Open door");
	LCD_displayStringRowColumn(1,0,"- :change password");
	key=KEYPAD_getPressedKey();
	_delay_ms(1000);
	if(key=='+')
	{
		UART_sendByte(send);
		UART_sendByte(plus);
		return_flag=0;
		if(Check_pass()==1)
		{
			MC1_First_open();
			g_check=0;
		}
		else
		{
			g_error++;
			UART_sendByte(ERROR_inPass);
			while(UART_receiveByte()!=ERROR_inPass);
			while(g_error<3)
			{
				if(Check_pass()==1)
				{
					MC1_First_open();
					g_check=0;
					break;
				}
				else
				{
					g_error++;
				}
			}
			if(g_error==3)
			{
				UART_sendByte(ERROR_inPass);
				while(UART_receiveByte()!=ERROR_inPass);
				MC1_error_msgDisplay();
			}
			g_error=0;
		}
	}
	else if(key=='-')
	{
		return_flag=0;
		UART_sendByte(send);
		UART_sendByte(minus);
		if(Check_pass()==1)
		{
			MC1_First_Run();
			return_flag=1;
			g_check=0;
		}
		else
		{
			g_error++;
			UART_sendByte(ERROR_inPass);
			while(UART_receiveByte()!=ERROR_inPass);
			while(g_error<3)
			{
				if(Check_pass()==1)
				{
					MC1_First_Run();
					g_check=0;
					return_flag=1;
					break;
				}
				else
				{
					g_error++;
				}
			}
			if(g_error==3)
			{
				UART_sendByte(ERROR_inPass);
				while(UART_receiveByte()!=ERROR_inPass);
				MC1_error_msgDisplay();
			}
			g_error=0;
		}
	}
}


void MC1_Door_open()
{
	g_ticks++;
	if(g_ticks==450)
	{
		UART_sendByte(Hold);
		while(UART_receiveByte()!=Hold);
		LCD_clearScreen();
		LCD_displayString("Door is Open");
	}
	else if(g_ticks==550)
	{
		UART_sendByte(ClosingDoor);
		while(UART_receiveByte()!=ClosingDoor);
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"locking door");
	}

	else if(g_ticks==1000)
	{
		Timer0_DeInit();
		g_ticks=0;
		return_flag=1;
	}
}

// function to run fist app

void MC1_First_open()
{
	UART_sendByte(startTimer);
	while(UART_receiveByte()!=startTimer);

	Timer0_Ovf_setCallBack(MC1_Door_open);
	MC1_Calculate_time();
	while(UART_receiveByte()!=UNLOCK_DOOR);
	UART_sendByte(UNLOCK_DOOR);
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Unlocking door");
}

// error callback function

void MC1_CallBackForError()
{
	g_ticks++;
	if(g_ticks>1800)
	{
		LCD_clearScreen();
		g_ticks=0; /*Put tick=0 to start counting new second*/
		Timer0_DeInit();
		return_flag=1;
	}
}

// displaying error msg on screen

void MC1_error_msgDisplay()
{
	LCD_clearScreen();
	LCD_displayString("Wrong pass 0 try");
	UART_sendByte(startTimer);
	while(UART_receiveByte()!=startTimer);
	Timer0_Ovf_setCallBack(MC1_CallBackForError);
	MC1_Calculate_time();

}
// function used to calculate time

void MC1_Calculate_time()
{
	Timer_Config timer_ptr={NORMAL,F_CPU_1024,0,0,ENABLE_N,DISABLE};
	Timer0_init(&timer_ptr);
}

// function fist run application

void MC1_First_Run()
{
	do
	{
		MC1_pass_creating();
		while(UART_receiveByte()!=send);
		UART_sendByte(send);
		checked=UART_receiveByte();

	}while(checked==0);
}

