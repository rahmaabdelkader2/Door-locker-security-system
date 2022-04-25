/*
 * Timer0.c
 *
 *  Created on: Oct 31, 2021
 *      Author: Rahma Abdelkader
 */
#include "timer.h"
#include <avr/interrupt.h> // To use ISR
#include <avr/io.h>// To use registers

static volatile void (*g_t_0_ovf_callBackPtr)(void) = NULL_PTR; /*Timer0,overflow mode*/
static volatile void (*g_t_0_ctc_callBackPtr)(void) = NULL_PTR; /*Timer0,compare mode*/

void Timer0_init(Timer_Config* config)
{
	/*Configure the timer to work in a non-PWM mode*/
	TCCR0=(1<<FOC0);

	/*Configure the timer clock prescaler*/
	TCCR0=(TCCR0 & 0xF8) | (config->prescaler & 0x07);

	/*Set timer initial value*/
	TCNT0=config->Initial_value;

	/*Configure the timer to work in the Normal mode:
	 * WGM00=0 & WGM01=0
	 * COM00=0 & COM01=0
	 * Enable Timer Overflow Interrupt TOIE0=1
	 */
	TIMSK|=config->N_INT & 0x01;

	/*Configure the timer to work in the Normal mode:
	 * WGM00=0 & WGM01=1
	 * COM00=0 & COM01=0
	 * Set timer compare match value OCR0
	 * Enable Timer output compare match Interrupt OCIE0=1
	 */
	TCCR0=(TCCR0 & 0xF7)|((config->mode & 0x02)<<2);
	OCR0=config->Compare_value;
	TIMSK|=(config->CTC_INT & 0x01)<<1;
}

void Timer0_Ovf_setCallBack(void(*a_t0_ovf_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_t_0_ovf_callBackPtr = a_t0_ovf_ptr;
}

/*Function to set the Call Back function address for Timer0, compare mode*/
void Timer0_CTC_setCallBack(void(*a_t0_ctc_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_t_0_ctc_callBackPtr = a_t0_ctc_ptr;
}
void Timer0_DeInit()
{
	TCCR0=0;
	TCNT0=0;
}
/******************************************************************************
 * 							Interrupt service routines
 ******************************************************************************/
ISR(TIMER0_OVF_vect)
{
	if(g_t_0_ovf_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after each overflow */
		(*g_t_0_ovf_callBackPtr)();
		/* another method to call the function using pointer to function t_0_ovf_callBackPtr(); */
	}
}
ISR(TIMER0_COMP_vect)
{
	if(g_t_0_ctc_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application
		 * after the timer reach the compare value
		 */
		(*g_t_0_ctc_callBackPtr)();
		/* another method to call the function using pointer to function t_0_ctc_callBackPtr(); */
	}
}
