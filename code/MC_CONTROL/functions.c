/*
 * functions.c
 *
 *  Created on: Nov 9, 2021
 *      Author: Rahma Abdelkader
 */

#include "commands.h"
#include "i2c.h"
#include "external_eeprom.h"
#include "uart.h"
#include "timer.h"
#include "DC_motor.h"
#include <avr/io.h>
#include "BUZZER.h"
#include "functions.h"
#include "commands.h"
#include <avr/delay.h>
/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
extern uint8 Fisrt_pass [10]; //save first entered password
extern uint8 Second_pass[10] ; //save second entered password
extern uint8 PasswordCheck[10];
extern uint8 ex_eeprom[10];
extern uint8 Check_var;
extern uint32 Ticks_Num;
extern uint8 Error_num;
extern uint8 checked_var;
extern uint8 choice;
extern uint8 newPassError;
extern uint8 return_flag;
/*******************************************************************************
 *							Function Definitions
 *******************************************************************************/

// function to receive pass from mc1

void MC2_Receive_pass(uint8 *str)
{
	uint8 count_pass=0;
	while(count_pass<5)
	{
		str[count_pass]=UART_receiveByte();
		count_pass++;
	}
}



// function to create the passsword



void MC2_Pass_create()
{
	while(UART_receiveByte()!=ready);
	UART_sendByte(MC2_READY);
	MC2_Receive_pass(Fisrt_pass);
	UART_sendByte(received);
	while(UART_receiveByte()!=RE_pass);
	UART_sendByte(send);
	MC2_Receive_pass(Second_pass);

	UART_sendByte(received);
}



// function to check two passwords

uint8 MC2_CheckPassword(uint8 *Password,uint8 *RePassword)
{
	uint8 i=0;
	while(i<5)
	{
		if(Password[i] != RePassword[i])
		{
			return ERROR;
		}
		i++;
	}
	return SUCCESS;
}

void MC2_Save_exeeprom()
{
	for(uint8 i=0;i<5;i++)
	{
		EEPROM_writeByte(0x0311+i,Second_pass[i]);
		_delay_ms(200);
	}
}

void MC2_GET_exeeprom()
{
	for(uint8 i=0;i<5;i++)
	{
		EEPROM_readByte(0x0311+i,ex_eeprom+i);
	}
}

uint8 MC2_check_ineeprom()
{
	while(UART_receiveByte()!=check);
	UART_sendByte(check);
	MC2_Receive_pass(PasswordCheck);

	MC2_GET_exeeprom();

	Check_var=MC2_CheckPassword(PasswordCheck,ex_eeprom);
	UART_sendByte(result);
	while(UART_receiveByte()!=result);

	UART_sendByte(Check_var);
	return Check_var;
}


void MC2_mainOptions()
{
	while(UART_receiveByte()!=start);
	UART_sendByte(start);

	while(UART_receiveByte()!=send);

	choice=UART_receiveByte();
	MC2_check_ineeprom();
	return_flag=0;
	if (Check_var==1)
	{
		if(choice==plus)
		{
			MC2_openDoor();
		}
		else if (choice==minus)
		{

			NewPassMC2();
			return_flag=1;
		}
		Check_var=0;
	}
	else
	{
		Error_num++;
		while(UART_receiveByte()!=error);
		UART_sendByte(error);

		while(Error_num<3)
		{
			if(MC2_check_ineeprom()==1)
			{
				if(choice==plus)
				{
					MC2_openDoor();
				}
				else if (choice==minus)
				{
					NewPassMC2();
				}
				Check_var=0;
				return_flag=1;
				break;
			}
			else
			{
				Error_num++;
			}
		}
		if(Error_num==3)
		{
			MC2_error_buzzer();
		}
		Error_num=0;
	}
}

// buzzer sound in error pass
void MC2_error_buzzer()
{
	while(UART_receiveByte()!=error);
	UART_sendByte(error);
	MC2_Buzzer_trigger();
}

// call back function for buzzer

void BuzzerCallBack()
{
	Ticks_Num++;
	if(Ticks_Num>1831)
	{
		Ticks_Num=0;
		Timer0_DeInit();
		BUZZER_off();
		return_flag=1;
	}
}

// function to trigger buzzer

void MC2_Buzzer_trigger()
{
	while(UART_receiveByte()!=Timer_start);
	UART_sendByte(Timer_start);
	Timer0_Ovf_setCallBack(BuzzerCallBack);
	BUZZER_on();
	MC2_Timer0_start();

}
// door opening function
void MC2_Door_opening()
{
	Ticks_Num++;
	if(Ticks_Num==458)
	{
		while(UART_receiveByte()!=HoldDoor);
		UART_sendByte(HoldDoor);
		MOTOR_STOP();
	}

	else if(Ticks_Num==550)
	{
		while(UART_receiveByte()!=ClosingDoor);
		UART_sendByte(ClosingDoor);
		Rotate_Anticlockwise();
	}

	else if(Ticks_Num==1008)
	{
		Timer0_DeInit();
		Ticks_Num=0;
		MOTOR_STOP();
		return_flag=1;
	}
}

// func to open door

void MC2_openDoor()
{
	while(UART_receiveByte()!=Timer_start);
	UART_sendByte(Timer_start);
	Timer0_Ovf_setCallBack(MC2_Door_opening);
	MC2_Timer0_start();
	UART_sendByte(OpeningDoor);
	while(UART_receiveByte()!=OpeningDoor);
	Rotate_clockwise();
}

//timer0 initialization

void MC2_Timer0_start()
{
	Timer_Config config_t={NORMAL,F_CPU_1024,0,0,ENABLE_N,DISABLE};
	Timer0_init(&config_t);
}

// function to ceate new pass for mc2

void NewPassMC2()
{
	do
	{
		MC2_Pass_create();
		checked_var=MC2_CheckPassword(Fisrt_pass,Second_pass);
		UART_sendByte(send);

		while(UART_receiveByte()!=send);

		UART_sendByte(checked_var);

	}while(checked_var==0);
	MC2_Save_exeeprom();
}
