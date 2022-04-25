/*
 * functions.h
 *
 *  Created on: Nov 9, 2021
 *      Author: Rahma Abdelkader
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

void MC2_Receive_pass(uint8 *str);
void MC2_Pass_create();
uint8 MC2_CheckPassword(uint8 *Password,uint8 *RePassword);
void MC2_Save_exeeprom();
void MC2_GET_exeeprom();
uint8 MC2_check_ineeprom();
void MC2_mainOptions();
void MC2_error_buzzer();
void BuzzerCallBack();
void MC2_Buzzer_trigger();
void MC2_Door_opening();
void MC2_openDoor();
void MC2_Timer0_start();
void NewPassMC2();


#endif /* FUNCTIONS_H_ */
