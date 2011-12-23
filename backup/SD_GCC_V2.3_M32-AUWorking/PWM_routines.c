
#include<avr/io.h>
#include "PWM_routines.h"



void init_pwm()
{
	/* old
	TCCR0 |= (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00);

	DDRB |= (1<<PB3);

	*/


	/* use OC1A pin as output */
    DDRD = _BV(PD5);

    /*
    * clear OC1A on compare match
    * set OC1A at BOTTOM, non-inverting mode
    * Fast PWM, 8bit
    */
    TCCR1A = _BV(COM1A1) | _BV(WGM10);
   
    /*
    * Fast PWM, 8bit
    * Prescaler: clk/1 = 8MHz
    * PWM frequency = 8MHz / (255 + 1) = 31.25kHz
    */
    TCCR1B = _BV(WGM12) | _BV(CS10);
   
    /* set initial duty cycle to zero */
    OCR1A = 0;
   

};


//uint8_t
void send_pwm(int duty)
{
	// OCR0=duty;	//old

	OCR1A = duty;
};
