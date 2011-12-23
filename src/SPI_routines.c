

//	SPI routines




#include <avr/io.h>
#include "SPI_routines.h"

//SPI initialize for SD card

void init_spi(void)
{
	SPCR = 0x52; 	// Master mode,
					// SPI enable
					// SPI mode 0
					// sck=fosc/64
	SPSR = 0x00;
};


unsigned char SPI_transmit(unsigned char data)
{
	// Start transmission
	SPDR = data;

	// Wait for transmission complete
	while(!(SPSR & (1<<SPIF)));
	data = SPDR;

	return(data);
};



unsigned char SPI_receive(void)
{
	unsigned char data;
	// Wait for reception complete

	SPDR = 0xff;
	while(!(SPSR & (1<<SPIF)));
	data = SPDR;

	// Return data register
	return data;
}

//------------------------------ END -----------------------------
