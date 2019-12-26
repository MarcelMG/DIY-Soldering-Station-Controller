/*	system time functions
 *	part of Soldering Station project
 *  written in 2019 by Marcel Meyer-Garcia
 *  see LICENCE.txt
 */
#include "time.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint16_t tim1_ovf_count = 0;

ISR(TIM1_OVF_vect){
	++tim1_ovf_count;
}

void setup_time(){
	// setup timer1 in normal mode
	TCCR1A = 0x00;
	TCCR1C = 0x00;
	// enable overflow interrupt
	TIMSK1 = (1<<TOIE1);
	// globally enable interrupts
	sei();
	// enable timer1 with prescaler of 8 CS1[2:0]=010
	TCCR1B = (1<<CS11);
}

void reset_time(){
	// disable timer1
	TCCR1B = 0x00;
	TCNT1 = 0x0000;
	tim1_ovf_count = 0;
	// re-enable timer1
	TCCR1B = (1<<CS11);
}

// return time in microseconds
uint32_t get_time_us(){
	// disable timer1 OVF interrupt
	TIMSK1 = 0x00;
	// read value
	uint32_t microseconds = ( 0x0000FFFF * (uint32_t) tim1_ovf_count ) + (uint32_t) TCNT1;
	// re-enable timer1 OVF interrupt
	TIMSK1 = (1<<TOIE1);
	return microseconds;
}

// return time in milliseconds
uint32_t get_time_ms(){
	return ( get_time_us() / 1000 );
}