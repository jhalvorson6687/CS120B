/*	Author: jhalv001
 *  Partner(s) Name: Chris Diaz
 *	Lab Section:
 *	Assignment: Lab 4  Exercise 3
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
unsigned char tmpB;
enum States {START, WAITL, LOCKED, WAITY, YBUTTON, UNLOCKED} state;

void tick() {
    
    tmpA = 0xFF & PINA;

    switch (state) { //state transitions
	case START:
		state = LOCKED;
		break;
	case WAITL:
		if(tmpA == 0x00) {state = LOCKED;}
		else {state = WAITL;}
		break;
	case LOCKED:
	 	if (tmpA == 0x04) {state = WAITY;}
		else {state = LOCKED;}
		break;
	case WAITY:
		if (tmpA == 0x00) {state = YBUTTON;}
		else if (tmpA == 0x04) {state = WAITY;}
		else {state = LOCKED;}
		break;
	case YBUTTON:
		if (tmpA == 0x02) {state = UNLOCKED;}
		else if (tmpA == 0x00) {state = YBUTTON;}
		else {state = WAITL;}
		break;
	case UNLOCKED:
		if (tmpA == 0x80) {state = LOCKED;}
		else {state = UNLOCKED;}
		break;
	default:
		state = LOCKED;
		break;
		
    }
    switch (state) {
        case WAITL:
		tmpB = 0x00;
		break;
	case LOCKED:
		tmpB = 0x00;		
                break;
        case WAITY:
		tmpB = 0x00;
		break;
	case YBUTTON:
		tmpB = 0x00;
                break;
        case UNLOCKED:
		tmpB = 0x01;
                break;
        default:
                break;
    }	    	
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00; 

    /* Insert your solution below */
    state = LOCKED;  

    while (1) {
	tick();
	PORTB = tmpB;
    }
    return 1;
}
