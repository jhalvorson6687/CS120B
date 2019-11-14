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
    DDRC = 0xFF; PORTC = 0x00;
    	
    /* Insert your solution below */
    unsigned fuelLevel  = 0x00;
    unsigned char tmp = 0x00;
    unsigned char lowFuel = 0;
    unsigned char fstBelt = 0x00;

    while (1) {

        fuelLevel = 0x00;
        tmp = 0xFF & PINA;
	lowFuel = 0x00;
	fstBelt = 0x00;

	if ((tmp == 0x01) || (tmp == 0x02)) {
		fuelLevel = 0x20;
		lowFuel = 0x40;
	}
	else if ((tmp == 0x03) || (tmp == 0x04)) {
		fuelLevel = 0x30;
		lowFuel = 0x40;
	}
	else if ((tmp == 0x05) || (tmp ==0x06)) {
		fuelLevel = 0x38;
	}
	else if ((tmp == 0x07) || (tmp == 0x08) || (tmp == 0x09)) {
		fuelLevel = 0x3C;
	}
	else if ((tmp == 0x0A) || (tmp == 0x0B) || (tmp == 0x0C)) {
		fuelLevel = 0x3E;
	}
	else {
		fuelLevel = 0x3F;
	}
	if ((tmp & 0x70) == 0x30) {
		fstBelt = 0x80;
	}
	
	PORTC = (fuelLevel | lowFuel) | fstBelt;
	tmp = 0x00;
    }
    return 1;
}
