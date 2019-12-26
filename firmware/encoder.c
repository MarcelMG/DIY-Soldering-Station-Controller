/*	library for rotary-encoder with integrated pushbutton
 *	part of Soldering Station project
 *  written in 2019 by Marcel Meyer-Garcia
 *  see LICENCE.txt
 */
#include "encoder.h"
#include "time.h"

volatile bool encoder_button_state;
volatile int8_t encoder_last_state = 0;
volatile int16_t encoder_counter = 0;
volatile bool encoder_button_state = 0; // 0==button released, 1==button pressed

ISR(PCINT0_vect){
	// read encoder state
	// convert 2bit gray-code to binary
	/*	A B  binary
		----------
		0 0 | 00 (0)
		1 0 | 11 (3)
		1 1 | 10 (2)
		0 1 | 01 (1)	*/
	int8_t encoder_state = 0;
	if( PINA & (1<<PINA1) ) encoder_state = 0b11;
	if( PINA & (1<<PINA2) ) encoder_state ^= 0b01;
	if( (encoder_state - encoder_last_state) != 0 ){
	/*	the difference of current and last encoder state can be
		1 (0b00000001) or -3 (0b11111101) (2-complement) for one direction of rotation or
		-1 (0b11111111) (2-complement) or 3 (0b00000011) for the other direction
		we can see that the 2nd LSB gives the direction in which the encoder was turned
	*/
	if( (encoder_state - encoder_last_state) & 0b10 ) ++encoder_counter;
	else --encoder_counter;
	}
	encoder_last_state = encoder_state;
	/* check button state */
	// riding edge->button released ; falling edge->button pressed
	encoder_button_state = !(PINA & (1<<PINA3));
}

void encoder_setup(int16_t encoder_counter_start){
	// initialize encoder counter
	encoder_counter = encoder_counter_start;
	// pins PA1, PA2 and PA3 as inputs without pull-ups
	DDRA &=~((1<<DDA3)|(1<<DDA2)|(1<<DDA1));
	PORTA &=~((1<<PORTA3)|(1<<PORTA2)|(1<<PORTA1));
	// enable pin change interrupt on pins PCINT0..7
	GIMSK |= (1<<PCIE0);
	// unmask pin change interrupt for pins PA1(PCI1), PA2(PCI2) and PA3(PCI3)
	PCMSK0 |= (1<<PCINT3)|(1<<PCINT2)|(1<<PCINT1);
	// initialize encoder state
	// convert 2bit gray-code to binary
	encoder_last_state = 0;
	if( PINA & (1<<PINA1) ) encoder_last_state = 0b11;
	if( PINA & (1<<PINA2) ) encoder_last_state ^= 0b01;
	// globally enable interrupts
	sei();
}

int16_t get_encoder_count(){
	// disable pin change interupt 0 (so variable doesn't change if interrupt occurs while reading it)
	GIMSK &=~(1<<PCIE0);
	int16_t encoder_count = encoder_counter;
	// re-enable pin change interupt 0
	GIMSK |= (1<<PCIE0);
	return encoder_count;
}

void set_encoder_count(int16_t count){
	// disable pin change interupt 0 (so variable doesn't change if interrupt occurs while reading it)
	GIMSK &=~(1<<PCIE0);
	encoder_counter = count;
	// re-enable pin change interupt 0
	GIMSK |= (1<<PCIE0);
}

// returns true if button is pressed
bool get_encoder_button_state(){
	// disable pin change interrupt 0 (so variable doesn't change if interrupt occurs while reading it)
	GIMSK &=~(1<<PCIE0);
	bool button_pressed = encoder_button_state;
	// re-enable pin change interrupt 0
	GIMSK |= (1<<PCIE0);
	return button_pressed;
}
