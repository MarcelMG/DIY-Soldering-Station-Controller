/*	ADC routines
 *	part of Soldering Station project
 *  written in 2019 by Marcel Meyer-Garcia
 *  see LICENCE.txt
 */
#include "adc.h"

void setup_adc(){
	// disable digital input buffer for pin PA7
	DIDR0 |= (1<<ADC7D);
	// select internal 1.1V voltage reference REFS[1:0]=10
	// ADC7 (PA7) as single-ended input MUX[5:0]=000111
	ADMUX = (1<<REFS1)|(0<<REFS0)|0b000111;
	// set prescaler to 32, so one conversion will take 1/8MHz*32*13=52µs ADPS[2:0]=101
	ADCSRA = (1<<ADPS2)|(0<<ADPS1)|(1<<ADPS0);
	ADCSRB = 0;
	// enable ADC and start a first conversion to perform initialization
	ADCSRA |= (1<<ADEN)|(1<<ADSC);
	// wait until conversion done
	while( ADCSRA & (1<<ADSC) );
}

uint16_t adc_read(){
	// start conversion
	ADCSRA |= (1<<ADSC);
	// wait until conversion done	
	while( ADCSRA & (1<<ADSC) );
	return ADC;
}

// read 2^N samples and average them
uint16_t adc_read_average(uint8_t N){
	uint32_t result = 0;
	uint16_t num_samples = (1<<N);
	for(uint16_t i=0; i<num_samples; ++i){
		result += adc_read();
	}
	return (uint16_t) (result>>N);
}