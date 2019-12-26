/*	library for rotary-encoder with integrated pushbutton
 *	part of Soldering Station project
 *  written in 2019 by Marcel Meyer-Garcia
 *  see LICENCE.txt
 */


#ifndef ENCODER_H_
#define ENCODER_H_


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define BUTTON_PRESSED true

volatile int8_t encoder_last_state;
volatile int16_t encoder_counter;
volatile bool encoder_button_state;

void encoder_setup(int16_t encoder_counter_start);
int16_t get_encoder_count();
void set_encoder_count(int16_t count);
bool get_encoder_button_state();



#endif /* ENCODER_H_ */