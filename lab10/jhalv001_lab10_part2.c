/*	Author: jhalv001
 *  Partner(s) Name: Chris Diaz
 *	Lab Section: 22
 *	Assignment: Lab 10  Exercise 1
 *	Exercise Description: Create concurrent synchSM's ThreeLEDs and Blinking LED. ThreeLEDs
 *						  should blink three LEDs in sequence 1 sec each. BlinkLED should 
 *						  set bit 3 on and off every 1 sec. A third SM (combine) should combine 
 *						  the two state machines to work together.
 *
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


int main(void) {
	unsigned long BL_elapsedTime = 1000;
	unsigned long TL_elapsedTime = 300;
	const unsigned long timerPeriod = 100;
	
    // init I/O Ports
	DDRB = 0xFF; PORTB = 0x00;
	
	TimerSet(timerPeriod);
	TimerOn();
	
	BL_State = BL_START;
	TL_State = TL_START;
	
    while (1) {
		if (BL_elapsedTime >= 1000) { 
			BL_tick();
			BL_elapsedTime = 0;
		}
		if (TL_elapsedTime >= 300) {
			TL_tick();
			TL_elapsedTime = 0;
		}
		
		PORTB = threeLEDs | blinkingLED;
		while (!TimerFlag);	// Wait for timerPeriod
		TimerFlag = 0;
		BL_elapsedTime += timerPeriod;
		TL_elapsedTime += timerPeriod;
    }
}

