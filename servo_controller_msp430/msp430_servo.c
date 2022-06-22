/*
 * msp430_servo.c
 *
 *  Created on: May 21, 2022
 *      Author: eder0
 */
#include <msp430.h>
#include <stdint.h>
#include "msp430_servo.h"
#include "main.h"
#include "msp430_timera.h"

#ifdef N0_OPTOCOUPLERS_USE
void servo_init(void)
{
	P2DIR |= 0x0F; //P2.0, P2.1, P2.2, P2.3 are set as output
	P2OUT |= 0xFF; //Default values of outputs are high
}

void servo0_set(void)
{
	P2OUT |= 0x01;
}

void servo0_clear(void)
{
	P2OUT &= 0xFE;
}

void servo0_toggle(void)
{
    P2OUT ^= 0x01;
}

void servo1_set(void)
{
	P2OUT |= 0x02;
}

void servo1_clear(void)
{
	P2OUT &= 0xFD;
}

void servo1_toggle(void)
{
    P2OUT ^= 0x02;
}

void servo2_set(void)
{
	P2OUT |= 0x04;
}

void servo2_clear(void)
{
    P2OUT &= 0xFB;
}

void servo2_toggle(void)
{
    P2OUT ^= 0x04;
}

void servo3_set(void)
{
    P2OUT |= 0x08;
}

void servo3_clear(void)
{
    P2OUT &= 0xF7;
}

void servo3_toggle(void)
{
    P2OUT ^= 0x08;
}
#endif

//the setting and clearing of the GPIOs will be the opposite since the optocouplers are inverting the signal.
#ifdef OPTOCOUPLERS_USE
void servo_init(void)
{
    P2DIR |= 0x0F; //P2.0, P2.1, P2.2, P2.3 are set as output
    P2OUT &= ~0xFF; //Default values of outputs are high
}

void servo0_set(void)
{
    P2OUT &= ~0x01;
}

void servo0_clear(void)
{
    P2OUT |= ~0xFE;
}

void servo0_toggle(void)
{
    P2OUT ^= 0x01;
}

void servo1_set(void)
{
    P2OUT &= ~0x02;
}

void servo1_clear(void)
{
    P2OUT |= ~0xFD;
}

void servo1_toggle(void)
{
    P2OUT ^= 0x02;
}

void servo2_set(void)
{
    P2OUT &= ~0x04;
}

void servo2_clear(void)
{
    P2OUT |= ~0xFB;
}

void servo2_toggle(void)
{
    P2OUT ^= 0x04;
}

void servo3_set(void)
{
    P2OUT &= ~0x08;
}

void servo3_clear(void)
{
    P2OUT |= ~0xF7;
}

void servo3_toggle(void)
{
    P2OUT ^= 0x08;
}
#endif
