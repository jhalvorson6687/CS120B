/*	Author: jhalv001
 *  Partner(s) Name: Chris Diaz
 *	Lab Section:
 *	Assignment: Lab 8  Exercise 1
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
	DDRD = 0xFF; PORTD = 0x00;

	ADC_init();
	unsigned short adcVal = 0x00;
	unsigned char tmpB = 0x00;
	unsigned char tmcD = 0x00;
	while (1) {
		adcVal = ADC;
		tmpB = (char)(adcVal & 0xFF);
		tmpD = (char)(adcVal >> 8) & 0x03; 
		PORTB = tmpB;
		PORTD = tmpD;

	}
	return 1;
}
