/*	Author: jhalv001
 *  Partner(s) Name: Chris Diaz
 *	Lab Section: 22
 *	Assignment: Lab 3  Exercise 3
 *	Exercise Description: Fasten Seatbelt Light
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
    DDRC = 0xFF; PORTC = 0x00;
    	
    /* Insert your solution below */
    unsigned tmpA = 0x00;
    unsigned tmpB = 0x00;
    unsigned tmpC = 0x00;

    while (1) {

	tmpA = PINA & 0xFF;

	tmpB = tmpA >> 4;
	tmpC = tmpA << 4;

	PORTB = tmpB;
	PORTC = tmpC;
    }
    return 1;
}
