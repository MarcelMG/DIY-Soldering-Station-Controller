/*	PWM functions to control the heating element in the soldering iron tip
 *	part of Soldering Station project
 *  written in 2019 by Marcel Meyer-Garcia
 *  see LICENCE.txt
 */

#ifndef PWM_H_
#define PWM_H_

#include <stdint.h>
#include <stdbool.h>

#define MAX_PWM 200

volatile bool new_adc_reading_ready;

void setup_pwm();
void pwm_set_duty_cycle(uint8_t duty_cycle);


#endif /* PWM_H_ */