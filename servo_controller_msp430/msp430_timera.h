/*
 * msp430_timera.h
 *
 *  Created on: May 21, 2022
 *      Author: eder0
 */

#include <msp430.h>
#include <stdint.h>

#ifndef TIMERA_H_
#define TIMERA_H_

void timera_cc_init(uint16_t capt_comp);

#endif /* MSP430_TIMERA_H_ */
