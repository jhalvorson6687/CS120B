/*	Author: jhalv001
 *  Partner(s) Name: Chris Diaz
 *	Lab Section:
 *	Assignment: Lab 7  Exercise 1
 *	Exercise Description: [optional - Extended LED stop game. Score kept on LCD.
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "/header/io.h"
#define tmpA (~PINA & 0x01)
volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	IMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt
	
	//Initialize avr counter
	TCNT1=0;

	avr_timer_cntcurr = _avr_timer_M; // TimerISR will be called every _avr_timer_cntcurr milliseconds

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
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

unsigned char tmpB = 0;
unsigned char score = 0;
unsigned char LED = 1;
unsigned char cnt = 0;
enum States { START, INIT, NEXT_LED, STOP, STOP_RELEASE, WAIT_RESET, RESET, UPDATE, WON } state;

void tick() {
	
	switch (state) { // State Transitions
		case START:
			state = INIT;
			break;
			
		case INIT:
			state = NEXT_LED;
			break;
		
		case NEXT_LED:
			if (tmpA) {state = STOP;}
			else {state = NEXT_LED;}
			break;
			
		case STOP:
			state = STOP_RELEASE;
			break;
			
		case STOP_RELEASE:
			if (tmpA) {state = STOP_RELEASE;}
			else {state = UPDATE;}
			break;
			
		case WAIT_RESET:
			if (!tmpA) {state = WAIT_RESET;}
			else {state = RESET;}
			break;
			
		case RESET:
			state = NEXT_LED;
			break;
			
		case UPDATE:
			if (score == 0x09) { state = WON; }
			else { state = WAIT_RESET; }
			break;
			
		case WON:
			if (cnt < 10) { state = WON; }
			else { state = INIT; }
			break;
			
		default:
			state = START;
			break;
	}
	
	switch (state) { // State Actions
		case START:
			break;
			
		case INIT:
			tmpB = 0x01;
			LED = 1;
			score = 5;
			LCD_ClearScreen();
			LCD_Cursor(1);
			LCD_WriteData(score + '0');
			break;
			
		case NEXT_LED:
			if (tmpB == 0x04) {tmpB = 0x01;}
			else {tmpB = tmpB << 1;}
			if (LED == 3) { LED = 1; }
			else { ++LED; }
			break;
			
		case STOP:
			break;
		
		case STOP_RELEASE:
			break;
		
		case WAIT_RESET:
			break;
		
		case RESET:
			break;
			
		case UPDATE:
			if (LED == 0x02) {
				if (score < 9) { score++; }
			}
			else {
				if (score > 0) { score--; }
			}
			LCD_ClearScreen();
			LCD_Cursor(1);
			LCD_WriteData(score + '0');
			break;
		
		case WON:
			if (cnt < 10) { cnt++; }
			else { cnt = 0; }
			tmpB = 0x7; 
			LCD_ClearScreen();
			LCD_DisplayString(1, "YOU WON!");
			break;
		
		default:
			break;
			
	}
				
}

int main(void) {
	
	DDRA = 0x00; // Set port A to input
	PORTA = 0xFF; // Init port A to 1s
	DDRB = 0xFF; // Set port B to output 
	PORTB = 0x00; // Init port B to 0s
	DDRD = 0xFF; PORTD = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	TimerSet(300);
	TimerOn();
	state = START;
	tmpB = 0x00;
	cnt = 0;
	LCD_init();
	
	while(1) {
		tick();
		PORTB = tmpB;
		while (!TimerFlag);	// Wait 300 ms
		TimerFlag = 0;
	}
}
