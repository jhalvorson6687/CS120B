/* Author: jhalv001
 * Partner(s) Name: Chris Diaz
 * Lab Section: 22
 * Assignment: Lab 6  Exercise 1
 * Exercise Description: Sequence LEDS 
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1ms

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

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

unsigned char tmpB;
enum States { START, INIT, NEXT_LED } state;
	
void tick() {
	
	switch (state) { // State Transitions
		case START:
			state = INIT;
			break;
			
		case INIT:
			state = NEXT_LED;
			break;
		
		case NEXT_LED:
			state = NEXT_LED;
			break;
			
		default:
			state = INIT;
			break;
	}

	switch (state) { // State Actions
		case START:
			break;
			
		case INIT:
			tmpB = 0x01;
			break;
			
		case NEXT_LED:
			if (tmpB == 0x04) {tmpB = 0x01;}
			else {tmpB = tmpB << 1;}
			break;
			
		default:
			break;
			
	}
				
}

int main(void) {
	
	DDRB = 0xFF; // Set port B to output 
	PORTB = 0x00; // Init port B to 0s
	TimerSet(1000);
	TimerOn();
	state = START;
	tmpB = 0x00;
	while(1) {
		tick();
		PORTB = tmpB;
		while (!TimerFlag);	// Wait 1 sec
		TimerFlag = 0;
	}
}

