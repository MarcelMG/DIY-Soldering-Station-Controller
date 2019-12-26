/*	system time functions
 *	part of Soldering Station project
 *  written in 2019 by Marcel Meyer-Garcia
 *  see LICENCE.txt
 */


#ifndef TIME_H_
#define TIME_H_

#include <stdint.h>

volatile uint16_t tim1_ovf_count;

void setup_time();
void reset_time();
uint32_t get_time_us();
uint32_t get_time_ms();

#endif /* TIME_H_ */