/*
 * timera.c
 *
 *  Created on: May 21, 2022
 *      Author: eder0
 */
#include <msp430.h>
#include <stdint.h>
#include "main.h"
#include "msp430_timera.h"

volatile uint16_t timer_count;
extern uint16_t claw_duty;
extern uint16_t arm_duty;
extern uint16_t cam_tilt_duty;
extern uint16_t cam_pan_duty;

void timera_cc_init(uint16_t capt_comp)
{
    TA0CTL |= 0x0200;    //TASSEL = 01; SOURCE CLK IS ACLK## 0b10 for SMCLK
    TA0CTL |= 0x0010;    //MCx = 01; Mode control is "UP MODE" (counts up to TACCR0)
    TA0CCR0 = capt_comp; //set the number of counts for the timer to interrupt
    TA0CTL |= 0x0004;    //TACLR = 1 We do this to let it know we want to change TIMERA setting and get started
    TA0CCTL0 |= 0x0010;  // CCIE = 1 ; enable the interrupt
}

#pragma vector = TIMER0_A0_VECTOR
interrupt void TIMERA0_interrupt(void)
{

    if(timer_count == claw_duty)
    {
        servo0_flag.clear = 1;
    }

    if(timer_count == arm_duty)
    {
        servo1_flag.clear = 1;
    }

    if(timer_count == cam_tilt_duty)
    {
        servo2_flag.clear = 1;
    }

    if(timer_count == cam_pan_duty)
    {
        servo3_flag.clear = 1;
    }

    if(timer_count >= 637)
    {
        timer_count = 0;
        servo0_flag.set = 1;
        servo1_flag.set = 1;
        servo2_flag.set = 1;
        servo3_flag.set = 1;
    }

    timer_count++;
    __bic_SR_register_on_exit(LPM0_bits); //clears the bits in the passed variable

}
