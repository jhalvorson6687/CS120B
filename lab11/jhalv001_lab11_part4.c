/*	Author: jhalv001
 *  Partner(s) Name: Chris Diaz
 *	Lab Section: 22
 *	Assignment: Lab 11  Exercise 4
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
unsigned char cursorPos = 1;
unsigned char inputDetected = 0;
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
			inputDetected = 0;
			break;
		case ONE:
			inputDetected = 1;
			keypadOutput = '1'; break;
		case TWO:
			inputDetected = 1;
			keypadOutput = '2'; break;
		case THREE:
			inputDetected = 1;
			keypadOutput = '3'; break;
		case FOUR:
			inputDetected = 1;
			keypadOutput = '4'; break;
		case FIVE:
			inputDetected = 1;
			keypadOutput = '5'; break;
		case SIX:
			inputDetected = 1;
			keypadOutput = '6'; break;
		case SEVEN:
			inputDetected = 1;
			keypadOutput = '7'; break;
		case EIGHT:
			inputDetected = 1;
			keypadOutput = '8'; break;
		case NINE:
			inputDetected = 1;
			keypadOutput = '9'; break;
		case A:
			inputDetected = 1;
			keypadOutput = 'A'; break;
		case B:
			inputDetected = 1;
			keypadOutput = 'B'; break;
		case C:
			inputDetected = 1;
			keypadOutput = 'C'; break;
		case D:
			inputDetected = 1;
			keypadOutput = 'D'; break;
		case STAR:
			inputDetected = 1;
			keypadOutput = '*'; break;
		case ZERO:
			inputDetected = 1;
			keypadOutput = '0'; break;
		case SHARP:
			inputDetected = 1;
			keypadOutput = '#'; break;
		case ERROR:
			inputDetected = 0;
			keypadOutput = 0x00; break;
	}
	return state;
}

enum Display_States {WAIT, RELEASE, displayKeypad};
int lcdTick(int state) {
	switch (state) {
		case WAIT:
			if (inputDetected) { state = displayKeypad; }
			else { state = WAIT; }
			break;
		case RELEASE:
			if (inputDetected) { state = RELEASE; }
			else { state = WAIT; }
			break;
		case displayKeypad:
			state = RELEASE;
			break;
		default:
			state = WAIT;
	}
	switch (state) {
		case displayKeypad:
			LCD_Cursor(cursorPos);
			LCD_WriteData(keypadOutput);
			if (cursorPos == 16) { cursorPos = 1; }
			else { ++cursorPos; }
	}
	return state;
}

int main(void) {
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;
    
	// Declare an array of tasks
	static task task1, task2;
	task *tasks[] = {&task1, &task2};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	unsigned long int gcd = 0;
	
	// Task 1 (LCD output)
	task1.state = WAIT;
	task1.period = 100;
	task1.elapsedTime = task1.period;
	task1.tick = &lcdTick;
	
	// Task 2 (keypadInput)
	task2.state = NONE;
	task2.period = 50;
	task2.elapsedTime = task2.period;
	task2.tick = &keypadTick;
	
	gcd = findGCD(task1.period, task2.period);
	
	TimerSet(gcd);
	TimerOn();
	
	LCD_init();
	LCD_DisplayString(1, "Congratulations!");
	
	unsigned short i; // Scheduler for-loop iterator
    while (1) {
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

