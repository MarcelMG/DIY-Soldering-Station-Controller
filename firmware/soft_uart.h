#ifndef SOFT_UART_H
#define SOFT_UART_H

/*	Simple Software UART TX for AVR
 *	uses neither interrupts nor timers (blocking)
 *	if you use interrupts, disable them while transmitting
 *	(c)2019 by Marcel Meyer Garcia
 *	see LICENSE.txt
*/
#include <avr/io.h>
#include <util/delay.h>

// use pin PB0 for UART TX, change these 3 defines to use another pin
#define UART_TX_LOW() PORTA&=~(1<<5)
#define UART_TX_HIGH() PORTA|=(1<<5)
#define UART_TX_SETOUTPUT() DDRA|=(1<<5)

#define BAUD 38400
#define ERROR_COMP -0.02 // for clock freq. deviation error compensation
#define BAUD_DELAY_US 1000000UL*(1+ERROR_COMP)/BAUD

void soft_uart_setup(void);
void dbg_print_char(char c);
void dbg_print_str(char* s);

#endif