/*	Author: jhalv001
 *  Partner(s) Name: Chris Diaz
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
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
enum States {START, WAIT, INCREMENT, DECREMENT, RELEASE, RESET} state;

void tick() {
    
    tmpC = 0xFF & PORTC;
    tmpA = 0xFF & ~PINA;

    switch (state) { //state transitions
	case START:
	    state = WAIT;
	    break;

	case WAIT:
	    if ((tmpC < 0x09) && (tmpA == 0x01)) {state = INCREMENT;}
	    else if ((tmpC > 0x00) && (tmpA == 0x02)) {state = DECREMENT;}
	    else if (tmpA == 0x03) {state = RESET;}
	    else {state = WAIT;}
	    break;

	case INCREMENT:
	    if (tmpA == 0x03) {state = RESET;}
	    else {state = RELEASE;}
	    break;

	case DECREMENT:
	    if (tmpA == 0x03) {state = RESET;}
	    else {state = RELEASE;}
	    break;

	case RESET:
	    state = WAIT;
	    break;

	case RELEASE:
	    if (tmpA == 0x00) {
		state = WAIT;
	    }
	    else if (tmpA == 0x03) {
		state = RESET;
	    }
	    else {
		state = RELEASE;
	    }
	    break;

	default:
	    state = START;
	    break;	    

    }

    switch (state) { //state actions
	case START:
	    tmpC = 0x07;
	    break;
	case WAIT:
	    break;
	case INCREMENT:
	    tmpC = tmpC + 0x01;
	    break;
	case DECREMENT:
	    tmpC = tmpC - 0x01;
	    break;
	case RELEASE:
	    break;
	case RESET:
	    tmpC = 0x00;
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
    PORTC = 0x07;
    tmpC = 0xFF & PORTC;

    while (1) {
	tick();
	PORTC = tmpC;
    }
    return 1;
}
