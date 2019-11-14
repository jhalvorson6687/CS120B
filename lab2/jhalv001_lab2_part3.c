/*	Author: jhalv001
 *  Partner(s) Name: Chris Diaz
 *	Lab Section: 22
 *	Assignment: Lab 2  Exercise 3
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
	DDRC = 0xFF; PORTC = 0x00;

    /* Insert your solution below */
	unsigned char cntavail = 0x00;
	unsigned char tmpA = 0x00;
	unsigned char i = 0;

	while(1) {
		tmpA = 0x0F & PINA;

		for(i = 0; i < 4; i++) {
			if ((tmpA & 0x01) == 0x00) {
				cntavail++;
			}
			tmpA = tmpA >> 1;
		}
		if(cntavail == 0x00) {
			cntavail = 0x80;
			PORTC = cntavail;
		}
		else {
			PORTC = cntavail;
		}
		cntavail = 0x00;
	}
	return 0;
}
