
#ifndef _UART_ROUTINES
#define _UART_ROUTINES

#include <stdio.h>
#include "options.h"

#define CHAR 0
#define INT  1
#define LONG 2


// UART transmit
int xmit_uart( char, FILE* );

// UART receive
int rcv_uart( FILE* );

//initialize UART
void init_uart(void);

#ifdef EN_UART_IOF

#define TX_NEWLINE {transmitByte(0x0d); transmitByte(0x0a);}

#endif


unsigned char receiveByte(void);
void transmitByte(unsigned char);
void transmitString_F(char*);
void transmitString(unsigned char*);
void transmitHex( unsigned char dataType, unsigned long data );




#endif
