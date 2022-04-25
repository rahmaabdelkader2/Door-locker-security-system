
/*
 * main1.c
 *
 *  Created on: Nov 7, 2021
 *      Author: Rahma abdelkader
 */
#include "keypad.h"
#include "LCD.h"
#include"uart.h"
#include "timer.h"
#include "gpio.h"
#include "commands.h"
#include "functions.h"
#include <avr/io.h>
#include<avr/interrupt.h>
#include <avr/delay.h>

/*******Global Variables*******/


uint8 Fisrt_pass [10]; //save first entered password
uint8 Second_pass[10] ; //save second entered password
uint8 PasswordCheck[10];
uint32 g_ticks=0; /*Global variable indicates number of overflows*/
uint8 return_flag=1; /*Global variable used to know if the password has been entered for 3 times wrong or not*/
uint8 counter_error =0; /*Global variable indicates number of times the entered password was wrong*/
uint8 key=0;
uint8 g_error=0;
uint8 checked=0;
uint8 g_check=0;

int main(void)
{
	//uart initialization

	Uart_ConfigType config={EIGHT_BIT,DISABLED,ONE_BIT,9600};
	UART_init(&config);

	// lcd initialization
	LCD_init();

	SREG|=1<<7; // interrupt enable

	MC1_First_Run();
	while(1)
	{
		if(return_flag==FLAG_FIRSTRUN)
			MC1_Main_options();
	}
}
