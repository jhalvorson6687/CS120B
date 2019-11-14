/*	Author: jhalv001
 *  Partner(s) Name: Chris Diaz
 *	Lab Section: 22
 *	Assignment: Lab 3  Exercise 1
 *	Exercise Description: Count the number of 1's on Ports A and B and outpu			      t that number on Port C. 
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
    DDRB = 0x00; PORTB = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    	
    /* Insert your solution below */
    unsigned char oneCnt = 0x00;
    unsigned char tmp = 0x00;
    unsigned char i = 0;

    while (1) {

        PORTC = 0x00;
        tmp = 0xFF & PINA;

	for (i = 0; i < 8; i++) { 
		if ((tmp & 0x01) == 0x01) {
			oneCnt++;
		}
		tmp = tmp >> 1;
	}
	
	tmp = 0xFF & PINB;

	for (i = 0; i < 8; i++) {
		if ((tmp & 0x01) == 0x01) {
			oneCnt++;
		}
		tmp = tmp >> 1;
	}
	
	PORTC = oneCnt;
	oneCnt = 0x00;      
    }
    return 1;
}
