/*
 * BUZZER.h
 *
 *  Created on: Oct 31, 2021
 *      Author: Rahma Abdelkader
 */

#ifndef BUZZER_H_
#define BUZZER_H_


#define BUZZER_PORT PORTC_ID
#define BUZZER_PIN PIN3_ID

void BUZZER_init();
void BUZZER_on();
void BUZZER_off();
#endif /* BUZZER_H_ */
