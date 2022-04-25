/*
 * DC_motor.c
 *
 *  Created on: Nov 4, 2021
 *      Author: Rahma abdelkader
 */
#include "DC_motor.h"
#include "gpio.h"


void DcMotor_Init(void)
{
	GPIO_setupPinDirection(Dc_MOTOR_PORT,Dc_MOTOR_PIN0,PIN_OUTPUT);
	GPIO_setupPinDirection(Dc_MOTOR_PORT,Dc_MOTOR_PIN1,PIN_OUTPUT);
	GPIO_setupPinDirection(Dc_MOTOR_PORT,Dc_MOTOR_ENABLE_PIN,PIN_OUTPUT);
	GPIO_writePin(Dc_MOTOR_PORT,Dc_MOTOR_PIN0,LOGIC_LOW);
	GPIO_writePin(Dc_MOTOR_PORT,Dc_MOTOR_PIN1,LOGIC_LOW);
	GPIO_writePin(Dc_MOTOR_PORT,Dc_MOTOR_ENABLE_PIN,LOGIC_LOW);
}

void Rotate_clockwise(void)
{
	GPIO_writePin(Dc_MOTOR_PORT,Dc_MOTOR_PIN1,LOGIC_HIGH);
	GPIO_writePin(Dc_MOTOR_PORT,Dc_MOTOR_PIN0,LOGIC_LOW);
	GPIO_writePin(Dc_MOTOR_PORT,Dc_MOTOR_ENABLE_PIN,LOGIC_HIGH);
}
void Rotate_Anticlockwise(void)
{
	GPIO_writePin(Dc_MOTOR_PORT,Dc_MOTOR_PIN0,LOGIC_HIGH);
	GPIO_writePin(Dc_MOTOR_PORT,Dc_MOTOR_PIN1,LOGIC_LOW);
	GPIO_writePin(Dc_MOTOR_PORT,Dc_MOTOR_ENABLE_PIN,LOGIC_HIGH);
}
void MOTOR_STOP()
{
	GPIO_writePin(Dc_MOTOR_PORT,Dc_MOTOR_ENABLE_PIN,LOGIC_LOW);
}
