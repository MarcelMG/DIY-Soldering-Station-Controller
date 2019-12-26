/*	PWM functions to control the heating element in the soldering iron tip
 *	part of Soldering Station project
 *  written in 2019 by Marcel Meyer-Garcia
 *  see LICENCE.txt
 */
#include "pwm.h"
#include <avr/interrupt.h>
#include <avr/cpufunc.h>

volatile bool new_adc_reading_ready = false;

/*	
	this interrupt occurs always when the PWM output is low (i.e. heater is turned off)
	thus we can sample the thermocouple sensor while the heater is turned off
	when the duty-cycle is at maximum (200) we have ~1,8ms time for the AD-conversion
	until the heater output goes on again
	      ______         ______         ______
	_____|      |___#___|      |___#___|      |___#
			    interrupt      interrupt
*/
ISR(TIM0_OVF_vect){
	new_adc_reading_ready = true;
}

void setup_pwm(){
	// pin PB2 as output, default low
	PORTB &=~(1<<2);
	DDRB |= (1<<2);
	// reset counter and compare value
	TCNT0 = 0;
	OCR0A = 0;
	// waveform geneartion mode 1 (phase correct PWM with TOP=0xFF) WGM[2..0] = 001
	// set OC0A on Compare Match when up-counting. Clear OC0A on Compare Match when down-counting (inverted mode) COM0A[1:0]=11
	// clock prescaler = 256
	TCCR0A = (1<<COM0A1)|(1<<COM0A0)|(0<<WGM01)|(1<<WGM00);
	TCCR0B = (0<<WGM02)|(1<<CS02)|(0<<CS01)|(0<<CS00);
	// enable overflow interrupt
	TIMSK0 = (1<<TOIE0);
	// globally enable interrupts
	sei();
}

void pwm_set_duty_cycle(uint8_t duty_cycle){
	if( duty_cycle > MAX_PWM ) duty_cycle = MAX_PWM;
	OCR0A = 0xFF - duty_cycle;
}