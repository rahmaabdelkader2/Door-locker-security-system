/*
 * uart.c
 *
 *  Created on: Nov 4, 2021
 *      Author: Rahma abdelkader
 */
//
#include"uart.h"
#include "avr/io.h"
#include "common_macros.h"
//
//
//
//void UART_init( const Uart_configType*config_ptr)
//{
//
//	uint16 ubrr_value = 0;
//
//	UCSRA = (1<<U2X); //double transmission speed
//
//	uint8 Baud_rate= config_ptr->baud_rate; //insert value of baud rate
//
//	UCSRB = (1<<RXEN); //receiver enable
//
//	UCSRB=  (1<<TXEN) ; //transmitter enable
//
//	UCSRC = (1<<URSEL); //uart register select
//
//	UCSRC =(UCSRC & 0XFC) |(config_ptr->bit_rate) ; //select bite rate 5,6,7,8  bs mn8er 9
//
//	UCSRC =(UCSRC & 0XCF) |((config_ptr->parity)<<4) ; //select parity mode
//
//	UCSRC =(UCSRC & 0XF7 ) |((config_ptr->stop_bit)<<3) ; //select stop bite
//
//	///* Calculate the UBRR register value */
//
//	ubrr_value = (uint16)(((F_CPU / (Baud_rate* 8UL))) - 1);
//
//	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
//
//	    UBRRH = ubrr_value>>8;
//		UBRRL = ubrr_value;
//
//
//
//}
///*
// * Description :
// * Functional responsible for send byte to another UART device.
// */
//void UART_sendByte(const uint8 data)
//{
//	/*
//	 * UDRE flag is set when the Tx buffer (UDR) is empty and ready for
//	 * transmitting a new byte so wait until this flag is set to one
//	 */
//	while(BIT_IS_CLEAR(UCSRA,UDRE)){}
//
//	/*
//	 * Put the required data in the UDR register and it also clear the UDRE flag as
//	 * the UDR register is not empty now
//	 */
//	UDR = data;
//
//	/************************* Another Method *************************
//	UDR = data;
//	while(BIT_IS_CLEAR(UCSRA,TXC)){} // Wait until the transmission is complete TXC = 1
//	SET_BIT(UCSRA,TXC); // Clear the TXC flag
//	*******************************************************************/
//}
//
///*
// * Description :
// * Functional responsible for receive byte from another UART device.
// */
//uint8 UART_recieveByte(void)
//{
//	/* RXC flag is set when the UART receive data so wait until this flag is set to one */
//	while(BIT_IS_CLEAR(UCSRA,RXC)){}
//
//	/*
//	 * Read the received data from the Rx buffer (UDR)
//	 * The RXC flag will be cleared after read the data
//	 */
//    return UDR;
//}
//
///*
// * Description :
// * Send the required string through UART to the other UART device.
// */
//void UART_sendString(const uint8 *Str)
//{
//	uint8 i = 0;
//
//	/* Send the whole string */
//	while(Str[i] != '\0')
//	{
//		UART_sendByte(Str[i]);
//		i++;
//	}
//	/************************* Another Method *************************
//	while(*Str != '\0')
//	{
//		UART_sendByte(*Str);
//		Str++;
//	}
//	*******************************************************************/
//}
//
///*
// * Description :
// * Receive the required string until the '#' symbol through UART from the other UART device.
// */
//void UART_receiveString(uint8 *Str)
//{
//	uint8 i = 0;
//
//	/* Receive the first byte */
//	Str[i] = UART_recieveByte();
//
//	/* Receive the whole string until the '#' */
//	while(Str[i] != '#')
//	{
//		i++;
//		Str[i] = UART_recieveByte();
//	}
//
//	/* After receiving the whole string plus the '#', replace the '#' with '\0' */
//	Str[i] = '\0';
//}
/*******************************************************************************
 *                         		    INCLUDES                                   *
 *******************************************************************************/

#include "uart.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/*Flag used when 9-bits data are used, to handle the transmitting and receiving in this case*/
uint8 flag_9bits =0;

/*******************************************************************************
 *                            FUNCTIONS DEFINITIONS                            *
 *******************************************************************************/

/*
 * Function that initialize the uart
 * 1. Set the required parity
 * 2. Set the required stop bits
 * 3. Set the required number of bits
 * 4. Set the required baudrate
 */
void UART_init(Uart_ConfigType * config)
{
	uint16 ubrr_value = 0;
	/*U2X=1 for double speed*/
	UCSRA=(1<<U2X);
	/*
	 * Configuring the UCSRB register:
	 * RXCIE=0 to disable receive interrupt
	 * TXCIE=0 to disable transmit interrupt
	 * RXEN=1 receive enable
	 * TXEN=1 transmit enable
	 * UDRIE=0 to Disable USART Data Register Empty Interrupt Enable
	 * RXB8 not used for 8-bit data mode
	 * TXB8 not used for 8-bit data mode
	 */
	UCSRB=(1<<RXEN)|(1<<TXEN);
	/*Setup UCSZ2 for the number of bits*/
	UCSRB=(UCSRB & 0xFB)|(config->bits & 0x04);

	/*
	 * Configuring the UCSRC register:
	 * URSEL=1 to enable writing in the UCSRC register
	 * UMSEL=0 for asynchronous mode
	 * UCPOL not used for asynchronous mode
	 */
	UCSRC=(1<<URSEL);
	/*Setup the parity bits : UPM1 & UPM0 bits*/
	UCSRC=(UCSRC & 0xCF)|((config->parity & 0x03)<<4);
	/*Setup the number of stop-bits : USBS bit*/
	UCSRC=(UCSRC & 0xF7)|((config->stop & 0x01)<<3);
	/*Setup the number of bits : UCSZ1 & UCSZ0 bits*/
	UCSRC=(UCSRC & 0xF9)|((config->bits & 0x03)<<1);

	/*Calculating the UBRR register value*/
	ubrr_value = (uint16)(((F_CPU / (config->BaudRate * 8UL))) - 1);

	UBRRH=ubrr_value>>8;
	UBRRL=ubrr_value;
}
/*Function responsible for sending the given data*/
void UART_sendByte(uint8 data)
{
	UDR=data;
	while(BIT_IS_CLEAR(UCSRA,TXC));
	SET_BIT(UCSRA,TXC);
}

/*Function responsible for receiving the data*/
uint8 UART_receiveByte(void)
{
	while(BIT_IS_CLEAR(UCSRA,RXC));
	return UDR;
}


/*Function responsible for sending a string*/
void UART_sendString(uint8 *str)
{
	uint8 i=0;
	while(str[i]!='\0')
	{
		UART_sendByte(str[i]);
		i++;
	}
}

/*Function responsible for receiving a string*/
void UART_receiveString(uint8 *str)
{
	uint8 i=0;
	/*Receive the first byte*/
	str[i]=UART_receiveByte();
	while(str[i]!='#')
	{
		i++;
		str[i]=UART_receiveByte();
	}
	str[i]='\0';
}
