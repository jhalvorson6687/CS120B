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

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02; 
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;

	SREG |= 0x80; 
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) { 
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

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

double Notes [8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
unsigned char note = 0;
unsigned char tmpA;

enum State {OFF, OFF_RELEASE, ON, ON_RELEASE, UP, DOWN, RELEASE} state;

void tick() {
	tmpA = ~PINA & 0xFF;
	
	switch (state) {
		case OFF:
			if (tmpA == 0x00) { state = OFF; }
			else if (tmpA == 0x01) { state = OFF_RELEASE; }
			else { state = OFF; }
			break;
			
		case OFF_RELEASE:
			if (tmpA == 0x00) {state = ON; }
			else { state = OFF_RELEASE; }
			break; 
		
		case ON:
			if (tmpA == 0x01) { state = ON_RELEASE; }
			else if (tmpA == 0x02) { state = UP; }
			else if (tmpA == 0x04) { state = DOWN; }
			else { state = ON; }
			break;
			
		case ON_RELEASE:
			if (tmpA == 0x00) { state = OFF; }
			else { state = ON_RELEASE; }
			break;
			
		case UP:
			state = RELEASE;
			break;
			
		case DOWN:
			state = RELEASE;
			break;
			
		case RELEASE:
			if (tmpA == 0x00) { state = ON; }
			else { state = RELEASE; }
			break;
			
		default:
			state = OFF;
			break;
	}
	
	switch (state) {
		case OFF:
			set_PWM(0);
			break;
			
		case ON:
			set_PWM(Notes[note]);
			break;
			
		case UP:
			if (note < 7) { note++; }
			break;
			
		case DOWN:
			if (note > 0) { note--; }
			break;
			
		case RELEASE:
			break;

		default:
			break;
	}
	
}

int main(void) {
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	TimerSet(150);
	TimerOn();
	state = OFF;
	note = 0;
	PWM_on();
	while (1) {
		tick();
		while (!TimerFlag);	// Wait 150 ms
		TimerFlag = 0;
		
	}
}
