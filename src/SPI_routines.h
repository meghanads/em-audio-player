

//**************************************************
// ***** HEADER FILE : SPI_routines.h ******
//**************************************************



#ifndef _SPI_ROUTINES_H_
#define _SPI_ROUTINES_H_

#define SPI_SD					SPCR = 0x52
#define SPI_HIGH_SPEED_6M		SPCR = 0x50; SPSR |= (1<<SPI2X)
#define SPI_HIGH_SPEED_3M		SPCR = 0x50; SPSR |= (0<<SPI2X)


void init_spi(void);
unsigned char SPI_transmit(unsigned char);
unsigned char SPI_receive(void);

#endif
