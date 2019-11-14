/*	Author: jhalv001
 *  Partner(s) Name: Chris Diaz
 *	Lab Section:
 *	Assignment: Lab 5  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char tmpA;
unsigned char tmpC;
unsigned char cnt;
enum States {WAIT, IN, MID, OUT, RELEASE} state;
	
void tick() {
	
	tmpA = 0xFF & ~PINA;
	
	switch (state) { // state transitions
		case WAIT:
			if ((tmpA == 0x01) && (cnt == 0x00)) {state = MID;}
			else if ((tmpA == 0x01) && (cnt == 0x01)) {state = OUT;}
			else if ((tmpA == 0x01) && (cnt == 0x02)) {state = MID;}
			else if ((tmpA == 0x01) && (cnt == 0x03)) {state = IN;}
			else {state = WAIT;}
			break;
			
		case IN:
			state = RELEASE;
			break;
			
		case MID:
			state = RELEASE;
			break;
			
		case OUT:
			state = RELEASE;
			break;
			
		case RELEASE:
			if (tmpA == 0x00) {state = WAIT;}
			else {state = RELEASE;}
			break;
			
		default:
			state = WAIT;
			break;
	}
	
	switch (state) { // state actions
		case WAIT:
			break;
		
		case IN:
			if (cnt == 0x03) {cnt = 0x00;}
			else {cnt++;}
			tmpC = 0x0C;
			break;
		
		case MID:
			cnt++;
			tmpC = 0x12;
		break;
		
		case OUT:
			cnt++;
			tmpC = 0x21;
			break;
		
		case RELEASE:
			break;
		
		default:
			break;
	}
	
}

int main(void) {
	/* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	
	/* Insert your solution below */
	state = WAIT;
	tmpC = 0x00;
	cnt = 0x00;
	PORTC = 0x0C;
	
	while (1) {
		tick();
		PORTC = tmpC;
	}
}

