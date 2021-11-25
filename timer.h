/*
 * timer.h
 *
 *  Created on: Nov 25, 2021
 *      Author: fhdtr
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "main.h"

#define TIMER_CLOCK_FREQ 8000
#define	NO_OF_SEC_TIMERS 1
#define NO_OF_MIL_TIMERS 0

typedef enum {
	TM_START_TIMER = 0,
	TM_STOP_TIMER = 1
} ControlTimer;


void TM_init_timer(TIM_HandleTypeDef *tim);
uint8_t TM_setMilTimer(uint8_t timer, unsigned int value);
uint8_t TM_setSecTimer(uint8_t timer, unsigned int value);
uint8_t TM_getMilFlag(uint8_t timer);
uint8_t TM_getSecFlag(uint8_t timer);
unsigned int TM_getMilCounter(uint8_t timer);
unsigned int TM_getSecCounter(uint8_t timer);
void TM_resetMilFlag(uint8_t timer);
void TM_resetSecFlag(uint8_t timer);
void TM_controlMilTimer(uint8_t timer, ControlTimer control);
void TM_controlSecTimer(uint8_t timer, ControlTimer control);
void TM_timerRun();
#endif /* INC_TIMER_H_ */
