/*	Author: jhalv001
 *  Partner(s) Name: Chris Diaz
 *	Lab Section: 22
 *	Assignment: Lab 11  Exercise 1
 *	Exercise Description:  
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include "io.h"
#include "timer.h"
#include "bit.h"
#include "keypad.h"
#include "scheduler.h"

//-----------Shared Variables-----------
unsigned char keypadOutput = 0x00;
//---------End Shared Variables---------

enum keypad_States {NONE, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, A, B, C, D, STAR, ZERO, SHARP, ERROR};
int keypadTick(int state) {
	unsigned char x;
	x = GetKeypadKey();
	switch(x) {
		case '\0':
			state = NONE; break;
		case '1': 
			state = ONE; break;
		case '2': 
			state = TWO; break;
		case '3': 
			state = THREE; break;
		case '4': 
			state = FOUR; break;
		case '5': 
			state = FIVE; break;
		case '6': 
			state = SIX; break;
		case '7': 
			state = SEVEN; break;
		case '8': 
			state = EIGHT; break;
		case '9': 
			state = NINE; break;
		case 'A': 
			state = A; break;
		case 'B': 
			state = B; break;
		case 'C':
			state = C; break;
		case 'D': 
			state = D; break;
		case '*': 
			state = STAR; break;
		case '0': 
			state = ZERO; break;
		case '#': 
			state = SHARP; break;
		default: 
			state = ERROR; break;
	}
	switch (state) {
		case NONE:
			keypadOutput = 0x1F; break;
		case ONE:
			keypadOutput = 0x01; break;
		case TWO:
			keypadOutput = 0x02; break;
		case THREE:
			keypadOutput = 0x03; break;
		case FOUR:
			keypadOutput = 0x04; break;
		case FIVE:
			keypadOutput = 0x05; break;
		case SIX:
			keypadOutput = 0x06; break;
		case SEVEN:
			keypadOutput = 0x07; break;
		case EIGHT:
			keypadOutput = 0x08; break;
		case NINE:
			keypadOutput = 0x09; break;
		case A:
			keypadOutput = 0x0A; break;
		case B:
			keypadOutput = 0x0B; break;
		case C:
			keypadOutput = 0x0C; break;
		case D:
			keypadOutput = 0x0D; break;
		case STAR:
			keypadOutput = 0x0E; break;
		case ZERO:
			keypadOutput = 0x00; break;
		case SHARP:
			keypadOutput = 0x0F; break;
		case ERROR:
			keypadOutput = 0x1B; break;
		default:
			keypadOutput = 0x1B; break;
	}
	PORTB = keypadOutput;
	return state;
}

int main(void) {
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;
	
	// Declare an array of tasks
	static task task1;
	task *tasks[] = {&task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	unsigned long int gcd = 0;
	
	// Task 1 (keypad input)
	task1.state = NONE;
	task1.period = 50;
	task1.elapsedTime = task1.period;
	task1.tick = &keypadTick;
	/*
	// Task 2 (toggleLED0)
	task2.state = LED0_WAIT;
	task2.period = 500;
	task2.elapsedTime = task2.period;
	task2.tick = &LED0Tick;
	// Task 3 (toggleLED1)
	task3.state = LED1_WAIT;
	task3.period = 1000;
	task3.elapsedTime = task3.period;
	task3.tick = &LED1Tick;
	// Task 4 (displaySM)
	task4.state = display_display;
	task4.period = 10;
	task4.elapsedTime = task4.period;
	task4.tick = &displayTick;
	*/
	
	//gcd = findGCD();
	gcd = 50;
	// Set the timer and turn it on
	TimerSet(gcd);
	TimerOn();
	
	unsigned short i; // Scheduler for-loop iterator
	while(1) {
		for (i = 0; i < numTasks; ++i) {
			if (tasks[i]->elapsedTime == tasks[i]->period) {
				tasks[i]->state = tasks[i]->tick(tasks[i]->state); // set next state
				tasks[i]->elapsedTime = 0; // reset elapsedTime
			}
			tasks[i]->elapsedTime += gcd;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 0; // Error: Program should not exit
}

