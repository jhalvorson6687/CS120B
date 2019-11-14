/* Author: jhalv001
 * Partner(s) Name: Chris Diaz
 * Lab Section: 22
 * Assignment: Lab 6  Exercise 3
 * Exercise Description: Two buttons connected to A0 & A1, increment & decrement PORTB. If holding one down go to inc/decr once per sec. Pressing both resets B to 0.
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

unsigned char tmpA;
unsigned char tmpB;
unsigned char cnt;
enum States {START, WAIT, INCREMENT, DECREMENT, INC_HOLD, DEC_HOLD, RELEASE, RESET} state;	
void tick() {
	
	tmpB = 0xFF & PORTB;
	tmpA = 0xFF & ~PINA;

	switch (state) { // State Transitions
		case START:
			state = WAIT;
			break;

		case WAIT:
			if ((tmpB < 0x09) && (tmpA == 0x01)) {state = INCREMENT;}
			else if ((tmpB > 0x00) && (tmpA == 0x02)) {state = DECREMENT;}
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
			
		case INC_HOLD:
			if ( (tmpA == 0x01) && (tmpB < 0x09) ) {
				if (cnt < 10) { // Wait 1 sec
					state = INC_HOLD;
				}
				else {
					state = INCREMENT;
				}
			}
			else if (tmpA == 0x03) {state = RESET;}
			else {state = WAIT;}
			break;
			
		case DEC_HOLD:
			if ( (tmpA == 0x02) && (tmpB > 0x00) ) {
				if (cnt < 10) { // Wait 1 sec
					state = DEC_HOLD;
				}
				else {
					state = DECREMENT;
				}
			}
			else if (tmpA == 0x03) {state = RESET;}
			else {state = WAIT;}
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
			else if ((tmpA == 0x01 ) && (tmpB < 0x09)) { 
				state = INC_HOLD;
			}
			else if ((tmpA == 0x02 ) && (tmpB > 0x00)) {
				state = DEC_HOLD;
			}
			else {
				state = WAIT;
			}
			break;

		default:
			state = START;
			break;	
	}

	switch (state) { // State Actions
		case START:
			tmpB = 0x07;
			break;
		case WAIT:
			break;
		case INCREMENT:
			tmpB = tmpB + 0x01;
			++cnt;
			break;
		case DECREMENT:
			tmpB = tmpB - 0x01;
			++cnt;
			break;
			
		case INC_HOLD:
			if (cnt < 10) {++cnt;}
			else {cnt = 0x00;}
			break;
			
		case DEC_HOLD:
			if (cnt < 10) {++cnt;}
			else {cnt = 0x00;}
			break;
			
		case RELEASE:
			break;
			
		case RESET:
			tmpB = 0x00;
			break;
			
		default:
			break;	
	}
				
}

int main(void) {
	/* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(100);
	TimerOn();
	state = WAIT;
	PORTB = 0x07;
	tmpB = 0xFF & PORTB;
	cnt = 0x00;

	while (1) {
		tick();
		PORTB = tmpB;
		while (!TimerFlag);	// Wait 100 ms
		TimerFlag = 0;
	}
	return 1;
}

