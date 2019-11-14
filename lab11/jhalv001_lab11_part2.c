/*	Author: jhalv001
 *  Partner(s) Name: Chris Diaz
 *	Lab Section: 22
 *	Assignment: Lab 11  Exercise 2
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
unsigned char cursorCount = 1;
//---------End Shared Variables---------

enum LCD_States {START, SCROLL};
int lcdTick(int state) {
	// Local State Variables
	const unsigned char lcdOutput[67] = { ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
										  ' ',' ',' ',' ','C','S','1','2','0','B',' ',
										  'i','s',' ','L','e','g','e','n','d','.','.',
										  '.','w','a','i','t',' ','f','o','r',' ','i',
										  't',' ','D','A','R','Y','!',' ',' ',' ',' ',
										  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
	
	switch(state) { //Transitions
		case START:
			state = SCROLL;
			break;
		case SCROLL:
			state = SCROLL;
			break;
		default:
			state = START; break;
	}
	switch(state) { // Actions
		case START:
			break;
		case SCROLL:
			for(int j = 1; j <= 16; j++){
				LCD_Cursor(j);
				LCD_WriteData(lcdOutput[cursorCount + (j-2)]);
				if(cursorCount + (j+1) == 69) { cursorCount = 1; }
			}
			cursorCount++;
			break;
		default:
			break;
	}
	return state;
}


int main(void) {
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	// Declare an array of tasks
	static task task1;
	task *tasks[] = {&task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	unsigned long int gcd = 0;
	
	// Task 1 (LCD output)
	task1.state = START;
	task1.period = 500;
	task1.elapsedTime = task1.period;
	task1.tick = &lcdTick;
	
	//gcd = findGCD();
	gcd = 500;
	// Set the timer and turn it on
	TimerSet(gcd);
	TimerOn();
	
	LCD_init();
	
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
