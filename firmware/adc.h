/*
 * adc.h
 *
 * Created: 15.12.2019 12:01:39
 *  Author: Marcel
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>

void setup_adc();
uint16_t adc_read();
uint16_t adc_read_average(uint8_t N);


#endif /* ADC_H_ */