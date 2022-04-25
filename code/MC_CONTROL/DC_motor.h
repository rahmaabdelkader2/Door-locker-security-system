/*
 * DC_motor.h
 *
 *  Created on: Nov 4, 2021
 *      Author: Rahma Abdelkader
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

/************************************************************************************
 *                                Definitions
 ************************************************************************************/
#define Dc_MOTOR_PIN0 PIN6_ID
#define Dc_MOTOR_PIN1 PIN7_ID
#define Dc_MOTOR_PORT PORTD_ID
#define Dc_MOTOR_ENABLE_PIN PIN5_ID
/************************************************************************************
 * 								 Types Declaration
 ************************************************************************************/
typedef enum
{
	CW,ACW
}DcMotor_Direction;

/************************************************************************************
 * 						Function Prototypes
 ************************************************************************************/
/*
 * Function to initialize the Dc-motor pins and stop the motor at the beginning
 */
void DcMotor_Init(void);
void Rotate_clockwise(void);
void Rotate_Anticlockwise(void);
void MOTOR_STOP(void);


#endif /* DC_MOTOR_H_ */
