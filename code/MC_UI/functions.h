/*
 * functions.h
 *
 *  Created on: Nov 9, 2021
 *      Author: Rahma Abdelkader
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_


void MC1_Get_password(uint8* password);
void MC1_pass_creating();
void MC1_send_password(const uint8 *array);
uint8 Check_pass();
void MC1_Main_options();
void MC1_Door_open();
void MC1_First_open();
void MC1_CallBackForError();
void MC1_error_msgDisplay();
void MC1_Calculate_time();
void MC1_First_Run();

#endif /* FUNCTIONS_H_ */
