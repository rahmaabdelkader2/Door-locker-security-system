/*
 * MC2_main.c
 *
 *  Created on: Nov 9, 2021
 *      Author: Rahma Abdelkader
 */


/*****************************************************************************/

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
uint8 Fisrt_pass [10]; //save first entered password
uint8 Second_pass[10] ; //save second entered password
uint8 PasswordCheck[10];
uint8 ex_eeprom[10];
uint8 Check_var=0;
uint32 Ticks_Num=0;
uint8 Error_num=0;
uint8 checked_var=0;
uint8 choice;
uint8 newPassError=0;
uint8 return_flag=1;

int main(void)
{
	SREG|=1<<7; // global interrupt enable

	DcMotor_Init();
	TWI_CONFIG config_i2c={0b00000001,400000};
	TWI_init(&config_i2c);

	BUZZER_init();


	Uart_ConfigType config={EIGHT_BIT,DISABLED,ONE_BIT,9600};
	UART_init(&config);



	NewPassMC2();

	while(1)
	{
		if(return_flag==1)
			MC2_mainOptions();
	}
}
