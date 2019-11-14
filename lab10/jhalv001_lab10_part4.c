/*	Author: jhalv001
 *  Partner(s) Name: Chris Diaz
 *	Lab Section: 22
 *	Assignment: Lab 10  Exercise 4
 *	Exercise Description: Extend part 3 to allow adjustable frequency
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.h"

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M; // TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
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

// states for BlinkLED SM
enum BL_States {BL_START, BL_OFF, BL_ON} BL_State;
unsigned char blinkingLED;
void BL_tick() {
	switch (BL_State) { // state transitions
		case BL_START:
		BL_State = BL_OFF;
		break;
		
		case BL_OFF:
		BL_State = BL_ON;
		break;
		
		case BL_ON:
		BL_State = BL_OFF;
		break;
		
		default:
		BL_State = BL_START;
		break;
	}
	
	switch (BL_State) { // state actions
		case BL_START:
		blinkingLED = 0x00;
		break;
		
		case BL_OFF:
		blinkingLED = 0x00; // turn LED off
		break;
		
		case BL_ON:
		blinkingLED = 0x08; // turn bit 3 on
		break;
		
		default:
		break;
	}
}

// states for ThreeLED SM
enum TL_States {TL_START, NEXT_LED} TL_State;
unsigned char threeLEDs;
void TL_tick() {
	switch (TL_State) { // state transitions
		case TL_START:
		if (threeLEDs != 0) { TL_State = NEXT_LED; }
		else {TL_State = TL_START; }
		break;
		
		case NEXT_LED:
		TL_State = NEXT_LED;
		break;
		
		default:
		TL_State = TL_START;
		break;
	}
	
	switch (TL_State) { // state actions
		case TL_START:
		threeLEDs = 0x01;
		break;
		
		case NEXT_LED:
		if (threeLEDs == 0x04) {threeLEDs = 0x01;}
		else {threeLEDs = threeLEDs << 1;}
		break;
		
		default:
		break;
	}
}

// states for adjFreq
enum State {WAIT, UP, DOWN, RELEASE} state;
double Notes [8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
unsigned char note = 0;
unsigned char tmpA;
unsigned char speakerSwitch = 0;
	
void adjFreq_tick() {
	tmpA = ~PINA & 0x03;
	speakerSwitch = ~PINA & 0x04;
	if (speakerSwitch == 0x04) {
		switch (state) {
			case WAIT:
				if (tmpA == 0x01) { state = UP; }
				else if (tmpA == 0x02) { state = DOWN; }
				else { state = WAIT; }
				break;
			
			case UP:
				state = RELEASE;
				break;
			
			case DOWN:
				state = RELEASE;
				break;
			
			case RELEASE:
				if (tmpA == 0x00) { state = WAIT; }
				else { state = RELEASE; }
				break;
			
			default:
				state = WAIT;
				break;
		}
	
		switch (state) {
			case WAIT:
				set_PWM(Notes[note]);
				break;
			
			case UP:
				if (note < 7) { note++; }
				break;
			
			case DOWN:
				if (note > 0) { note--; }
				break;
			
			case RELEASE:
				//set_PWM(Notes[note]);
				break;
			
			default:
				break;
		}
	}
	else { set_PWM(0); }
	
}

int main(void) {
	unsigned long BL_elapsedTime = 1000;
	unsigned long TL_elapsedTime = 300;
	const unsigned long timerPeriod = 2;
	
	// init I/O Ports
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	TimerSet(timerPeriod);
	TimerOn();
	
	BL_State = BL_START;
	TL_State = TL_START;
	state = WAIT;
	note = 0;
	
	PWM_on();
	
	while (1) {
		if (BL_elapsedTime >= 1000) {
			BL_tick();
			BL_elapsedTime = 0;
		}
		if (TL_elapsedTime >= 300) {
			TL_tick();
			TL_elapsedTime = 0;
		}
		adjFreq_tick();
		PORTB = threeLEDs | blinkingLED;
		while (!TimerFlag);	// Wait for timerPeriod
		TimerFlag = 0;
		BL_elapsedTime += timerPeriod;
		TL_elapsedTime += timerPeriod;
		
	}
}

