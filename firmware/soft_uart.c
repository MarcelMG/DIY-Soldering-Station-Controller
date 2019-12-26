/*	Simple Software UART TX for AVR
 *	uses neither interrupts nor timers (blocking)
 *	if you use interrupts, disable them while transmitting
 *	(c)2019 by Marcel Meyer Garcia
 *	see LICENSE.txt
*/
#include "soft_uart.h"
#include <avr/interrupt.h>

void soft_uart_setup(){
	UART_TX_SETOUTPUT(); // set pin as output
}

// a simple software UART-TX
void dbg_print_char(char c){
	cli();
	UART_TX_LOW(); // START bit
	_delay_us(BAUD_DELAY_US);
	for(uint8_t k = 0; k != 8; ++k){
		if( c & 0x01 ) UART_TX_HIGH(); else UART_TX_LOW();
		c >>= 1;
		_delay_us(BAUD_DELAY_US);
	}
	UART_TX_HIGH(); // STOP bit
	_delay_us(BAUD_DELAY_US);
	sei();
}

void dbg_print_str(char* s){
	while(*s){
		dbg_print_char(*s);
		++s;
	}
}