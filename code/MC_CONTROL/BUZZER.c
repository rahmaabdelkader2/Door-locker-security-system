/*
 * BUZZER.c
 *
 *  Created on: Oct 31, 2021
 *      Author: Rahma Abdelkader
 */

#include "BUZZER.h"
#include "gpio.h"
void BUZZER_init()
{
	GPIO_setupPinDirection(BUZZER_PORT,BUZZER_PIN,PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT,BUZZER_PIN,LOGIC_LOW);
}
void BUZZER_on()
{
	GPIO_writePin(BUZZER_PORT,BUZZER_PIN,LOGIC_HIGH);
}
void BUZZER_off()
{
	GPIO_writePin(BUZZER_PORT,BUZZER_PIN,LOGIC_LOW);
}
