/*	miscellaneous functions
 *	part of Soldering Station project
 *  written in 2019 by Marcel Meyer-Garcia
 *  see LICENCE.txt
 */
#include "misc.h"
#include <avr/io.h>
#include "tm1637.h"

void init_led1(){
	DDRA |= (1<<DDA4);
	PORTA &=~(1<<4);
}

void set_led1(bool state){
	PORTA &=~ (1<<4);
	PORTA |= (state<<4);
}

void init_standby_sensor(){
	// setup pin PA0 as input with pullup
	PORTA |= (1<<0);
	DDRA &=~(1<<DDA0);	
}

bool read_standby_sensor(){
	return (bool) (PINA & (1<<PINA0));
}
// display the temperature on the TM1637 display
void display_temperature(int16_t temperature){
	uint8_t digit[4];
	digit[0] = temperature/1000;
	digit[1] = temperature/100;
	digit[2] = (temperature % 100)/10;
	digit[3] = temperature % 10;
	uint8_t digit_index = 0;
	// no leading zeros for values > 1000, i.e. e.g. 45 should be displayed as '45' and not as '0045'
	// but 0 should still be displayed as '0'
	while( (digit[digit_index] == 0) && (digit_index < 3) ){
		TM1637_display_segments(digit_index, 0x00); // display nothing
		++digit_index;
	}
	while( digit_index <= 3 ){
		TM1637_display_digit(digit_index, digit[digit_index]);
		++digit_index;
	}
}

void display_error(){
	// display " ERR"
	TM1637_display_segments(0, 0x00);
	TM1637_display_segments(1, 0x79);
	TM1637_display_segments(2, 0x77);
	TM1637_display_segments(3, 0x77);
}

void display_okay(){
		// display "OHAY"
		TM1637_display_segments(0, 0x3F);
		TM1637_display_segments(1, 0x76);
		TM1637_display_segments(2, 0x77);
		TM1637_display_segments(3, 0x66);
}

// convert raw ADC value to temperature in °C
uint16_t adc2celsius(uint16_t adc_value){
	/*	amplifier gain is 101, adc reference is 1.1V, adc range is 0..1023
		so we have 1.1V/101/1024LSB = 10.636 µV/LSB
		thermocouple voltage is about 7.093mV @ 450°C so we have approx. 15.762 µV/°C
		we assume 20°C ambient temperature
		so we have approximately celsius = 20°C + 15.762 µV/°C 
		this leads to the conversion factor 10.636 µV/LSB / 15.762 µV/°C = 0.6748 °C/LSB
		this is approximately 44224/(2^16)=0,674805 so we can use a bit-shift instead of a division*/
	uint32_t celsius = ((uint32_t) adc_value) * 44224;
	celsius = (celsius>>16) + 20;
	return ( (uint16_t)(celsius & 0xFFFF) );
}

int16_t limit(int16_t value, int16_t lower_limit, int16_t upper_limit){
	if( value < lower_limit ){
		return lower_limit;
	}
	if( value > upper_limit ){
		return upper_limit;
	}
	return value;
}

uint16_t ulimit(uint16_t value, uint16_t lower_limit, uint16_t upper_limit){
	if( value < lower_limit ){
		return lower_limit;
	}
	if( value > upper_limit ){
		return upper_limit;
	}
	return value;
}