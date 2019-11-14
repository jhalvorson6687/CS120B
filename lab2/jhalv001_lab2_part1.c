/*	Author: jhalv001
 *  Partner(s) Name: Chris Diaz
 *	Lab Section: 22
 *	Assignment: Lab 2  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

    /* Insert your solution below */
	unsigned char Door = 0x00; //PA0
	unsigned char Light = 0x00; //PA1
	unsigned char LED = 0x00; //PB0

	while(1) {
		// Read Input
		Door = PINA & 0x01;
		Light = PINA & 0x02;

		// Illuminate LED if PA1 = 1 && PA0 = 1
		if ((Door == 0x01) && (Light == 0x00)) {
			LED = 0x01; // set rightmost bit = 1
		}
		else {
			LED = LED & 0x00; // set LED to 0
		}
		PORTB = LED; // set Port
	}
	return 0;
}
