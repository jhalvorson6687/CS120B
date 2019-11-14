/* Author: jhalv001
 * Partner(s) Name: Chris Diaz
 * Lab Section: 22
 * Assignment: Lab 9  Exercise 1
 * Exercise Description:  3 buttons play different notes on speaker.
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void set_PWM(double frequency) {
	static double current_frequency;
	
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; }
		else { TCCR3B |= 0x03; }
			
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		else if (frequency > 31250) { OCR3A = 0x0000; }
		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }
			
		TCNT3 = 0;
		current_frequency = frequency;
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3A = 0x00;
}

unsigned char tmpA;
unsigned char tmpB;
enum States {WAIT, C4, D4, E4} state;

void tick() {
	tmpA = ~PINA & 0xFF;
	tmpB = 0x00;
	
	switch (state) {
		case WAIT:
			if (tmpA == 0x01) { state = C4; }
			else if (tmpA == 0x02) { state = D4; }
			else if (tmpA == 0x04) { state = E4; }
			else { state = WAIT; }
			break;

		case C4:
			if (tmpA == 0x01) { state = C4; }
			else { state = WAIT; }
			break;

		case D4:
			if (tmpA == 0x02) { state = D4; }
			else { state = WAIT; }
			break;
			
		case E4:
			if (tmpA == 0x04) { state = E4; }
			else { state = WAIT; }
			break;
			
		default:
			state = WAIT;
			break;
	}

	switch (state) {
		case WAIT:
			set_PWM(0);
			tmpB = 0x00;
			break;
		
		case C4:
			set_PWM(261.63);
			tmpB = 0x40;
			break;
			
		case D4:
			set_PWM(293.66);
			tmpB = 0x40;
			break;
			
		case E4:
			set_PWM(329.63);
			tmpB = 0x40;
			break;
	}
}

int main(void) {
    DDRB = 0xFF; PORTB = 0x00;
    DDRA = 0x00; PORTA = 0xFF;
    state = WAIT;
    PWM_on();
    while (1) {
	tick();
	PORTB = tmpB;
    }
}
