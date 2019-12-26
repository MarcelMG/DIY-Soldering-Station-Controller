/*	Soldering Station Controller
 *	main program
 *  written in 2019 by Marcel Meyer-Garcia
 *  see LICENCE.txt
 */

#define MAX_TEMPERATURE 450
#define STANDBY_TEMPERATURE 100
/* P-Controller factor is P = MULTIPLIER/(2^BITSHIFT) e.g. P = 3/(2^1)=3/2=1.5 */
#define P_CONTROLLER_GAIN_MULTIPLIER 3
#define P_CONTROLLER_GAIN_BITSHIFT 1
#define DISPLAY_REFRESH_MS 300
#define DISPLAY_TIME_TARGET_TEMP 500
#define PUSHBUTTON_SAVE_TEMPERATURE_MS 1000

#include <stdlib.h>
#include <util/delay.h>
#include <avr/eeprom.h>
//#include "soft_uart.h"
#include "adc.h"
#include "encoder.h"
#include "tm1637.h"
#include "pwm.h"
#include "misc.h"
#include "time.h"

/* EEPROM variable */
uint16_t EEMEM default_temperature = 25;

int main(void)
{
	/* variable declaration */
	uint16_t target_temperature = 0;
	uint16_t current_temperature = 0;
	uint8_t duty_cycle = 0;
	uint32_t last_display_refresh_time = 0;
	uint32_t last_encoder_change_time = 0;
	
	setup_time(); // system time in µs and ms
	setup_adc(); // setup ADC for thermocouple readout
	//soft_uart_setup(); // software UART TX for debugging (optional)
	TM1637_init(1, 3); // initialize display with brightness level 3 (0..7)
	TM1637_display_colon(false); // dont't display colon between 2nd and 3rd digit
	setup_pwm(); // setup PWM for heater control
	pwm_set_duty_cycle(0); // turn off heater
	init_standby_sensor(); // set pin for standby-detection sensor as input with pullup
	init_led1(); // set LED1 pin as output
	
	// read default temperature from EEPROM
	eeprom_busy_wait();
	target_temperature = eeprom_read_word(&default_temperature);
	// value sanity check
	target_temperature = ulimit(target_temperature, 0, MAX_TEMPERATURE);
	// setup encoder knob, initialize enconder count i.e. target temperature with default value from EEPROM
	encoder_setup(target_temperature);
	display_temperature(target_temperature);
	_delay_ms(500);
	
	while (1){
		
		// check if it's time to perform a AD-conversion (when the PWM output is low, i.e. the heater turned off)
		if( new_adc_reading_ready == true ){
			new_adc_reading_ready = false; // clear flag
			// this readings take about 945µs (timed with oscilloscope), 2^4=16 samples, 52µs per AD-conversion plus CPU cycles for calculations
			current_temperature = adc2celsius(adc_read_average(4));
			// P-controller to control the soldering iron temperature
			if( current_temperature >= target_temperature ){
				duty_cycle = 0; // turn heater off
			}else{ // if( current_temperature < target_temperature )
				// note: we need a temporary 16bit variable to perform the computation
				uint16_t tmp_duty_cycle = target_temperature - current_temperature; // compute difference
				// apply gain factor without using floating-point math by first multiplying a factor, then dividing by 2^N
				tmp_duty_cycle *= P_CONTROLLER_GAIN_MULTIPLIER;
				tmp_duty_cycle = (tmp_duty_cycle>>P_CONTROLLER_GAIN_BITSHIFT); // do right bit-shift instead of division, equivalent to tmp_duty_cycle=tmp_duty_cycle/(2^P_CONTROLLER_GAIN_BITSHIFT)
				// limit duty-cycle
				tmp_duty_cycle = ulimit(tmp_duty_cycle, 0, MAX_PWM);
				duty_cycle = (uint8_t) tmp_duty_cycle;
			}
			pwm_set_duty_cycle(duty_cycle); // apply new computed duty-cycle
		}
		

		
		// check if iron is in stand (standby mode)
		if( read_standby_sensor() == STANDBY_SENSOR_STANDBY ){
			target_temperature = STANDBY_TEMPERATURE;
			set_led1(true);
		}else{
			// read target temperature from encoder knob, limit value
			uint16_t encoder_count_local = (uint16_t) limit(get_encoder_count(), 0, MAX_TEMPERATURE);
			// check if the encoder knob has been turned since the last time we checked and if yes, save the time
			if( encoder_count_local != target_temperature ){
				last_encoder_change_time = get_time_ms();
			}
			// set encoder value as target temperature
			target_temperature = (uint16_t) limit(get_encoder_count(), 0, MAX_TEMPERATURE);
			set_encoder_count((int16_t) target_temperature); // limit encoder count		
			set_led1(false);	
		}
		
		// if we the encoder hasn't been turned recently (e.g. in the last 1s), show current temperature, else show target temperature
		if( ( get_time_ms() - last_encoder_change_time ) < DISPLAY_TIME_TARGET_TEMP ){
			// refresh display instantly, show target temperature
			display_temperature(target_temperature);
		}else if( (get_time_ms() - last_display_refresh_time) > DISPLAY_REFRESH_MS ){
			// show current temperature, but refresh display only from time to time (e.g. every 100ms) to avoid flickering of the last digit
			display_temperature(current_temperature);
			last_display_refresh_time = get_time_ms();
		}

		if( get_encoder_button_state() == BUTTON_PRESSED ){
			pwm_set_duty_cycle(0); // turn heater off
			uint32_t t = get_time_ms();
			while(get_encoder_button_state() == BUTTON_PRESSED){
				// if button has been pressed for longer than a certain time
				if( (get_time_ms() - t) >= PUSHBUTTON_SAVE_TEMPERATURE_MS ){
					TM1637_clear(); // clear display
					// save current target temperature as default temperature in non-volatile EEPROM
					eeprom_busy_wait();
					eeprom_write_word(&default_temperature, target_temperature);
					display_okay();
					_delay_ms(2000);
					break;
				}
			}
			pwm_set_duty_cycle(duty_cycle); // turn heater on again
		}

    }
}

