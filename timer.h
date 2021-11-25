/*
 * timer.h
 *
 *  Created on: Oct 5, 2021
 *      Author: fhdtr
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "main.h"
#include <stdlib.h>

#define TIMER_CLOCK_FREQ 8000

typedef enum {
	START_TIMER = 0,
	STOP_TIMER = 1
} ControlTimer;

void init_timer(TIM_HandleTypeDef *tim, unsigned int num_sec, unsigned int num_mil);
uint8_t setMilTimer(uint8_t timer, unsigned int value);
uint8_t setSecTimer(uint8_t timer, unsigned int value);
uint8_t getMilFlag(uint8_t timer);
uint8_t getSecFlag(uint8_t timer);
unsigned int getMilCounter(uint8_t timer);
unsigned int getSecCounter(uint8_t timer);
void resetMilFlag(uint8_t timer);
void resetSecFlag(uint8_t timer);
void controlMilTimer(uint8_t timer, ControlTimer control);
void controlSecTimer(uint8_t timer, ControlTimer control);

#endif /* INC_TIMER_H_ */
