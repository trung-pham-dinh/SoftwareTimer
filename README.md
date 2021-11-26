# SoftwareTimer
STM32 using HAL library

Edit these Macros in timer.h:
#define TIMER_CLOCK_FREQ 8000 // Frequency of clock fed to timer(APBx), in (KHz)
#define	NO_OF_SEC_TIMERS 1 // number of second based timers
#define NO_OF_MIL_TIMERS 0 // number of millisecond based timers

Call TM_init_timer() in the beginning and pass the pointer to the timer handler as parameter
Call TM_timerRun() in timer ISR (HAL_TIM_PeriodElapsedCallback())

