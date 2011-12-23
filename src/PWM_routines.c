
#include<avr/io.h>
#include "PWM_routines.h"



void init_pwm()
{
	


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
    * Prescaler: clk/1 = 12MHz
    * PWM frequency = 12MHz / (255 + 1) = 46.875kHz
    */
    TCCR1B = _BV(WGM12) | _BV(CS10);
   
    /* set initial duty cycle to zero */
    OCR1A = 0;
   

};



void send_pwm(int duty)
{

	OCR1A = duty;
};
