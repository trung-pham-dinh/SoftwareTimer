/*
 * timer.c
 *
 *  Created on: Nov 25, 2021
 *      Author: fhdtr
 */


#include "timer.h"

static unsigned int TIMER_CYCLE; // Timer interrupt period

// array to store counter
static unsigned int timer_mil_counter[NO_OF_MIL_TIMERS];
static unsigned int timer_sec_counter[NO_OF_SEC_TIMERS];

// array to store timeout flag
static uint8_t timer_mil_flag[NO_OF_MIL_TIMERS];
static uint8_t timer_sec_flag[NO_OF_SEC_TIMERS];

// array to store stop flag
static uint8_t timer_mil_stop[NO_OF_MIL_TIMERS];
static uint8_t timer_sec_stop[NO_OF_SEC_TIMERS];


/**
  * @brief  Initialize second based timers and millisecond based timers
  * @note   None
  *
  * @param  tim: Timer structure to get the Period and Prescaler
  * @param  num_sec: number of second based timers
  * @param  num_mil: number of millisecond based timers
  * @retval None
  */
void TM_init_timer(TIM_HandleTypeDef *tim) {
	// init parameter for software timer
	TIMER_CYCLE = (tim->Instance->ARR+1) * (tim->Instance->PSC+1) / TIMER_CLOCK_FREQ;

	// init value for array
	for(int i = 0; i < NO_OF_MIL_TIMERS; i++) {
		timer_mil_counter[i] = 0;
		timer_mil_flag[i] = 1;
		timer_mil_stop[i] = TM_START_TIMER;
	}
	for(int i = 0; i < NO_OF_SEC_TIMERS; i++) {
		timer_sec_counter[i] = 0;
		timer_sec_flag[i] = 1;
		timer_sec_stop[i] = TM_START_TIMER;
	}
}


/**
  * @brief  Begin a specified timer with a countdown value
  * @note   None
  *
  * @param  timer: a specific timer
  * @param  value: time interval
  * @retval None
  */
uint8_t TM_setMilTimer(uint8_t timer, unsigned int value) {
	if((value * 1000) % TIMER_CYCLE != 0) {
		return 0;
	}
	if(timer >= NO_OF_SEC_TIMERS) {
		return 0;
	}

	timer_mil_counter[timer] = value / TIMER_CYCLE;
	timer_mil_flag[timer] = 0;
	return 1;
}


/**
  * @brief  Begin a specified timer with a countdown value
  * @note   None
  *
  * @param  timer: a specific timer
  * @param  value: time interval
  * @retval None
  */
uint8_t TM_setSecTimer(uint8_t timer, unsigned int value) {
	if((value * 1000) % TIMER_CYCLE != 0) {
		return 0;
	}
	if(timer >= NO_OF_SEC_TIMERS) {
		return 0;
	}

	timer_sec_counter[timer] = value*1000 / TIMER_CYCLE;
	timer_sec_flag[timer] = 0;
	return 1;
}


/**
  * @brief  check if the timer is done its counting down
  * @note   None
  *
  * @param  timer: a specific timer
  * @retval return 1 if the timer is done its counting down, otherwise 0
  */
uint8_t TM_getMilFlag(uint8_t timer) {
	if(timer >= NO_OF_SEC_TIMERS) {
		return 0;
	}
	if(timer_mil_flag[timer]) {
		timer_mil_flag[timer] = 0;
		return 1;
	}
	return 0;
}

/**
  * @brief  check if the timer is done its counting down
  * @note   None
  *
  * @param  timer: a specific timer
  * @retval return 1 if the timer is done its counting down, otherwise 0
  */
uint8_t TM_getSecFlag(uint8_t timer) {
	if(timer >= NO_OF_SEC_TIMERS) {
		return 0;
	}

	if(timer_sec_flag[timer]) {
		timer_sec_flag[timer] = 0;
		return 1;
	}
	return 0;
}

/**
  * @brief  get its current counting value
  * @note   None
  *
  * @param  timer: a specific timer
  * @retval return counting value
  */
unsigned int TM_getMilCounter(uint8_t timer) {
	if(timer >= NO_OF_MIL_TIMERS) {
		return 0;
	}
	return timer_mil_counter[timer];
}


/**
  * @brief  get its current counting value
  * @note   None
  *
  * @param  timer: a specific timer
  * @retval return counting value
  */
unsigned int TM_getSecCounter(uint8_t timer) {
	if(timer >= NO_OF_SEC_TIMERS) {
		return 0;
	}
	return timer_sec_counter[timer] * TIMER_CYCLE / 1000;
}


/**
  * @brief  clear the done counting flag
  * @note   None
  *
  * @param  timer: a specific timer
  * @retval None
  */
void TM_resetMilFlag(uint8_t timer) {
	if(timer >= NO_OF_SEC_TIMERS) {
		return;
	}
	timer_mil_flag[timer] = 0;
}

/**
  * @brief  clear the done counting flag
  * @note   None
  *
  * @param  timer: a specific timer
  * @retval None
  */
void TM_resetSecFlag(uint8_t timer) {
	if(timer >= NO_OF_SEC_TIMERS) {
		return;
	}
	timer_sec_flag[timer] = 0;
}

/**
  * @brief  This function use to stop or continue a specified timer
  * @note   None
  *
  * @param  timer: a specific timer
  * @param  control: stop or continue command. This can be START_TIMER or STOP_TIMER
  * @retval None
  */
void TM_controlMilTimer(uint8_t timer, ControlTimer control) {
	timer_mil_stop[timer] = control;
}



/**
  * @brief  This function use to stop or continue a specified timer
  * @note   None
  *
  * @param  timer: a specific timer
  * @param  control: stop or continue command. This can be START_TIMER or STOP_TIMER
  * @retval None
  */
void TM_controlSecTimer(uint8_t timer, ControlTimer control) {
	timer_sec_stop[timer] = control;
}

void TM_timerRun() {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
	for(int i = 0; i < NO_OF_MIL_TIMERS; i++) {
		if(timer_mil_stop[i] == TM_STOP_TIMER) continue;
		if(timer_mil_counter[i] > 0) {
			timer_mil_counter[i]--;
			if(timer_mil_counter[i] == 0) {
				timer_mil_flag[i] = 1;
			}
		}
	}

	for(int i = 0; i < NO_OF_SEC_TIMERS; i++) {
		if(timer_sec_stop[i] == TM_STOP_TIMER) continue;
		if(timer_sec_counter[i] > 0) {
			timer_sec_counter[i]--;
			if(timer_sec_counter[i] == 0) {
				timer_sec_flag[i] = 1;
			}
		}
	}
}
