/*
 * msp430_led.h
 *
 *  Created on: May 27, 2022
 *      Author: eder0
 */

#ifndef MSP430_GPIO_H_
#define MSP430_GPIO_H_

void led_init(void);
void led0_high(void);
void led0_low(void);
void led0_toggle(void);
void p3_init(void);
#endif /* MSP430_GPIO_H_ */
