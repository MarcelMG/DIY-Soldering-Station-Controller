/*	miscellaneous functions
 *	part of Soldering Station project
 *  written in 2019 by Marcel Meyer-Garcia
 *  see LICENCE.txt
 */


#ifndef MISC_H_
#define MISC_H_

#include <stdint.h>
#include <stdbool.h>

#define STANDBY_SENSOR_STANDBY false // the input is low when the iron is in the stand and touches the input line, grounding it
#define STANDBY_SENSOR_ACTIVE true

void init_led1();
void set_led1(bool state);
void init_standby_sensor();
bool read_standby_sensor();
void display_temperature(int16_t temperature);
void display_error();
void display_okay();
uint16_t adc2celsius(uint16_t adc_value);
int16_t limit(int16_t value, int16_t lower_limit, int16_t upper_limit);
uint16_t ulimit(uint16_t value, uint16_t lower_limit, uint16_t upper_limit);

#endif /* MISC_H_ */