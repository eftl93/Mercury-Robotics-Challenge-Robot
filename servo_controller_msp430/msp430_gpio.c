/*
 * msp430_led.c
 *
 *  Created on: May 27, 2022
 *      Author: eder0
 */
#include <msp430.h>
#include <msp430_gpio.h>
#include <stdint.h>
#include "main.h"

void led_init(void)
{
    P1DIR |= 0x01; //P1.0 are set as output
    P1OUT &= 0xFE;
}

void led0_high(void)
{
    P1OUT |= 0x01;
}

void led0_low(void)
{
    P1OUT |= 0xFE;
}

void led0_toggle(void)
{
    P1OUT ^= 0x01;
}

void p3_init(void)
{
    P3DIR &= 0x00;                  //P3 are set as input
    P3REN |= 0xFF;                  //enable pull up/down resistor
    P3OUT &= 0x00;                  //pull down resistor are used
}
