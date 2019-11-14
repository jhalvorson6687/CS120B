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

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00;	PORTA = 0xFF;
    	DDRB = 0xFF;	PORTB = 0x00; 

    /* Insert your solution below */
    unsigned char tmp = 0x00;
    unsigned char LED_0 = 0x00;
    unsigned char LED_1 = 0x00;
    enum States {start, B0_ON, B1_ON, waitB0, waitB1} state;

    while (1) {
	
    	    tmp = PINA & 0xFF;
	    LED_0 = 0x00;
	    LED_1 = 0x00;

	    switch(state){

		    case start:
			    state = B0_ON;
			    break;
		    case B0_ON:
			    if(tmp == 0x01) {state = waitB1;}
			    else {state = B0_ON;}
			    break;
		    case B1_ON:
			    if(tmp == 0x01) {state = waitB0;}
			    else {state = B1_ON;}
			    break;
		    case waitB1:
			    if (tmp == 0x01) {state = waitB1;}
			    else {state = B1_ON;}
		            break;
		    case waitB0:
			    if (tmp == 0x01) {state = waitB0;}
			    else {state = B0_ON;}
			    break;
		    default:
			    state = B0_ON;
			    break;
	    }

	    switch(state){
		    case start:
			break;
	
		    case B0_ON:
			    LED_0 = 0x01;
			    LED_1 = 0x00;
			    break;

		    case B1_ON:
			    LED_0 = 0x00;
			    LED_1 = 0x02;
			    break;
		    case waitB1:
			    LED_0 = 0x00;
			    LED_1 = 0x02;
			    break;
		    case waitB0:
			    LED_0 = 0x01;
			    LED_1 = 0x00;
			    break;
		    default:
			    break;
	    }

	    PORTB = LED_0 | LED_1;
    }
    return 1;
}
