/*--------------------------------------------------------------------------*/
/*	Meghanad:	UART functions*/
/*-------------------------------------------------------------------------*/

#include<avr/io.h>
#include<avr/interrupt.h> 
#include<stdio.h>
#include<stdlib.h>
#include<util/delay.h>
#include"UART_routines.h"
#include "options.h"
#include <avr/pgmspace.h>


/*-----------------------------------------------------------------
*	Uart initialization
*	baudrate:	9600
*	char size:	8
*	stop bits:	2
*	fosc:		12000000
*	parity:		none
*-----------------------------------------------------------------*/

//#define FILE MYFILE

#define val_load	77	//value tobe loaded

#ifdef EN_UART_IOF

// UART transmit
int xmit_uart( char data, FILE *fd )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) );
	
	/* Put data into buffer, sends the data */
	UDR = data;
	return 0;
};


// UART receive
int rcv_uart( FILE *fp)
{
	/* Wait for data to be received */
	char c;

	while ( !(UCSRA & (1<<RXC)) );

	c = UDR;

	//echo character
	xmit_uart(c,NULL);
	//printf("%c",c);

	/* Get and return received data from buffer */
	return UDR;
};

#endif

//initialize UART
void init_uart()
{
	

	/* Set baud rate */
	UBRRH = (unsigned char)(val_load>>8);
	UBRRL = (unsigned char)val_load;


	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);

	/* Set frame format: 8data, 2 stop bit */
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);


	#ifdef EN_UART_IOF

	// Enable printf()/scanf() call-back
	  fdevopen(&xmit_uart, &rcv_uart);

	#endif   

	//Enable the interrupts globally. 
  	SREG |= _BV(SREG_I); 


};


//**************************************************
//Function to receive a single byte
//*************************************************
unsigned char receiveByte( void )
{
	unsigned char data, status;
	
	while(!(UCSRA & (1<<RXC))); 	// Wait for incomming data
	
	status = UCSRA;
	data = UDR;
	
	return(data);
};


//***************************************************
//Function to transmit a single byte
//***************************************************
void transmitByte( unsigned char data )
{
	while ( !(UCSRA & (1<<UDRE)) )
		; 			                /* Wait for empty transmit buffer */
	UDR = data; 			        /* Start transmition */
};


//***************************************************
//Function to transmit hex format data
//first argument indicates type: CHAR, INT or LONG
//Second argument is the data to be displayed
//***************************************************
void transmitHex( unsigned char dataType, unsigned long data )
{
unsigned char count, i, temp;
unsigned char dataString[] = "0x        ";

if (dataType == CHAR) count = 2;
if (dataType == INT) count = 4;
if (dataType == LONG) count = 8;

for(i=count; i>0; i--)
{
  temp = data % 16;
  if((temp>=0) && (temp<10)) dataString [i+1] = temp + 0x30;
  else dataString [i+1] = (temp - 10) + 0x41;

  data = data/16;
}

transmitString (dataString);
}

//***************************************************
//Function to transmit a string in Flash
//***************************************************
void transmitString_F(char* string)
{
  while (pgm_read_byte(&(*string)))
   transmitByte(pgm_read_byte(&(*string++)));
}

//***************************************************
//Function to transmit a string in RAM
//***************************************************
void transmitString(unsigned char* string)
{
  while (*string)
   transmitByte(*string++);
}

//-------------------------------- END --------------------------------
