/*
 * timer.c
 *
 *  Created on: Oct 5, 2021
 *      Author: fhdtr
 */

#include "timer.h"

static unsigned int TIMER_CYCLE; // Timer interrupt period
static unsigned int NO_OF_MIL_TIMERS; // number of timer run in millisecond
static unsigned int NO_OF_SEC_TIMERS; // number of timer run in second

// array to store counter
static unsigned int* timer_mil_counter;
static unsigned int* timer_sec_counter;

// array to store timeout flag
static uint8_t* timer_mil_flag;
static uint8_t* timer_sec_flag;

// array to store stop flag
static uint8_t* timer_mil_stop;
static uint8_t* timer_sec_stop;


/**
  * @brief  Initialize second based timers and millisecond based timers
  * @note   None
  *
  * @param  tim: Timer structure to get the Period and Prescaler
  * @param  num_sec: number of second based timers
  * @param  num_mil: number of millisecond based timers
  * @retval None
  */
void init_timer(TIM_HandleTypeDef *tim, unsigned int num_sec, unsigned int num_mil) {
	// init parameter for software timer
	TIMER_CYCLE = (tim->Instance->ARR+1) * (tim->Instance->PSC+1) / TIMER_CLOCK_FREQ;
	NO_OF_MIL_TIMERS = num_mil;
	NO_OF_SEC_TIMERS = num_sec;

	// init array for counter and flag
	timer_mil_counter = (unsigned int*)malloc(NO_OF_MIL_TIMERS * sizeof(unsigned int));
	timer_sec_counter = (unsigned int*)malloc(NO_OF_SEC_TIMERS * sizeof(unsigned int));
	timer_mil_flag = (uint8_t*)malloc(NO_OF_MIL_TIMERS * sizeof(uint8_t));
	timer_sec_flag = (uint8_t*)malloc(NO_OF_SEC_TIMERS * sizeof(uint8_t));
	timer_mil_stop = (uint8_t*)malloc(NO_OF_MIL_TIMERS * sizeof(uint8_t));
	timer_sec_stop = (uint8_t*)malloc(NO_OF_SEC_TIMERS * sizeof(uint8_t));

	// init value for array
	for(int i = 0; i < NO_OF_MIL_TIMERS; i++) {
		timer_mil_counter[i] = 0;
		timer_mil_flag[i] = 1;
		timer_mil_stop[i] = START_TIMER;
	}
	for(int i = 0; i < NO_OF_SEC_TIMERS; i++) {
		timer_sec_counter[i] = 0;
		timer_sec_flag[i] = 1;
		timer_sec_stop[i] = START_TIMER;
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
uint8_t setMilTimer(uint8_t timer, unsigned int value) {
	if((value * 1000) % TIMER_CYCLE != 0) {
		return 0;
	}
	if(timer >= NO_OF_SEC_TIMERS) {
		exit(-1);
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
uint8_t setSecTimer(uint8_t timer, unsigned int value) {
	if((value * 1000) % TIMER_CYCLE != 0) {
		return 0;
	}
	if(timer >= NO_OF_SEC_TIMERS) {
		exit(-1);
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
uint8_t getMilFlag(uint8_t timer) {
	if(timer >= NO_OF_SEC_TIMERS) {
		exit(-1);
	}
	return timer_mil_flag[timer];
}

/**
  * @brief  check if the timer is done its counting down
  * @note   None
  *
  * @param  timer: a specific timer
  * @retval return 1 if the timer is done its counting down, otherwise 0
  */
uint8_t getSecFlag(uint8_t timer) {
	if(timer >= NO_OF_SEC_TIMERS) {
		exit(-1);
	}
	return timer_sec_flag[timer];
}

/**
  * @brief  get its current counting value
  * @note   None
  *
  * @param  timer: a specific timer
  * @retval return counting value
  */
unsigned int getMilCounter(uint8_t timer) {
	if(timer >= NO_OF_MIL_TIMERS) {
		exit(-1);
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
unsigned int getSecCounter(uint8_t timer) {
	if(timer >= NO_OF_SEC_TIMERS) {
		exit(-1);
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
void resetMilFlag(uint8_t timer) {
	if(timer >= NO_OF_SEC_TIMERS) {
		exit(-1);
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
void resetSecFlag(uint8_t timer) {
	if(timer >= NO_OF_SEC_TIMERS) {
		exit(-1);
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
void controlMilTimer(uint8_t timer, ControlTimer control) {
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
void controlSecTimer(uint8_t timer, ControlTimer control) {
	timer_sec_stop[timer] = control;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
	button_reading();
	for(int i = 0; i < NO_OF_MIL_TIMERS; i++) {
		if(timer_mil_stop[i] == STOP_TIMER) continue;
		if(timer_mil_counter[i] > 0) {
			timer_mil_counter[i]--;
			if(timer_mil_counter[i] == 0) {
				timer_mil_flag[i] = 1;
			}
		}
	}

	for(int i = 0; i < NO_OF_SEC_TIMERS; i++) {
		if(timer_sec_stop[i] == STOP_TIMER) continue;
		if(timer_sec_counter[i] > 0) {
			timer_sec_counter[i]--;
			if(timer_sec_counter[i] == 0) {
				timer_sec_flag[i] = 1;
			}
		}
	}
}

