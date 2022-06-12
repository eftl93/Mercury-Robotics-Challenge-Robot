/*
 * main.h
 *
 *  Created on: May 21, 2022
 *      Author: eder0
 */

#ifndef MAIN_H_
#define MAIN_H_

#define MIN_DUTY 32
#define MAX_DUTY 65
#define NEUTRAL_DUTY (MIN_DUTY + (MAX_DUTY-MIN_DUTY)/2)
#define COUNTER_COMPARE 500

//struct is meant to store flags to pending action and current state
struct flags {
    uint8_t toggle;
    uint8_t set;
    uint8_t clear;
    uint8_t state;
};

extern struct flags led0_flag, servo0_flag, servo1_flag, servo2_flag, servo3_flag;
extern volatile uint16_t timer_count;
extern uint16_t servo0_duty;
extern uint16_t servo1_duty;
extern uint16_t servo2_duty;
extern uint16_t servo3_duty;

#endif /* MAIN_H_ */
