/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TIMER1_H
#define	TIMER1_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdio.h>
#include "main.h"

void timer1_init(uint16_t cnts_to_overflow, uint8_t prescaler);


#endif	

