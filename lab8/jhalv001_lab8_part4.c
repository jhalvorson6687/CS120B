/*	Author: jhalv001
 *  Partner(s) Name: Chris Diaz
 *	Lab Section:
 *	Assignment: Lab 8  Exercise 4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

int main(void) {
	/* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	ADC_init();
	unsigned short adcVal = 0x00;
	unsigned char MAX = 0x70;
	unsigned char tmpB = 0x00;
	unsigned char rangeOne = MAX / 8;
	unsigned char rangeTwo = 2 * ( MAX/8);
	unsigned char rangeThree = 3 * ( MAX/8);
	unsigned char rangeFour = 4 * ( MAX/8);
	unsigned char rangeFive = 5 * ( MAX/8);
	unsigned char rangeSix = 6 * ( MAX/8);
	unsigned char rangeSeven = 7 * ( MAX/8);
	unsigned char rangeEight = 8 * ( MAX/8);

	while (1) {
		adcVal = ADC;
		tmpB = (char)(adcVal & 0xFF);
		if ( tmpB < rangeOne ) { PORTB = 0x01; }
		else if ( (tmpB >= rangeOne) && (tmpB < rangeTwo) ) { PORTB = 0x03; }
		else if ( (tmpB >= rangeTwo) && (tmpB < rangeThree) ) { PORTB = 0x07; }
		else if ( (tmpB >= rangeThree) && (tmpB < rangeFour) ) { PORTB = 0x0F; }
		else if ( (tmpB >= rangeFour) && (tmpB < rangeFive) ) { PORTB = 0x1F; }
		else if ( (tmpB >= rangeFive) && (tmpB < rangeSix) ) { PORTB = 0x3F; }
		else if ( (tmpB >= rangeSix) && (tmpB < rangeSeven) ) { PORTB = 0x7F; }
		else if ( (tmpB >= rangeSeven) && (tmpB < rangeEight) ) { PORTB = 0xFF; }
		else { PORTB = 0xFF; }

	}
	return 1;
}
