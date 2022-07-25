/*
 * File:   gpio.c
 * Author: eder0
 *
 * Created on July 23, 2022, 9:02 PM
 */
#include <xc.h>
#include <stdio.h>
#include "gpio.h"
#include "main.h"

void gpio_init(void)
{
    ANSELA=0;           //disable analog
    ANSELB=0;           //disable analog
    ANSELC=0;           //disable analog
    ANSELD=0;           //disable analog
    ANSELE=0;           //disable analog
   CM1CON0bits.C1ON=0; //disable comparator1 module
   CM2CON0bits.C2ON=0; //disable comparator2 module
   RED_LED_DIR = 0;     //set output for debugging led
   GREEN_LED_DIR = 0;   //set output for debugging led
   YELLOW_LED_DIR = 0;  //set output for debugging led
   TRISD    |= 0x03;  //output to control the relay for the beam lights
   debug_leds_off();
   high_beams_off();
}
//'e' and 'q' are to set or reset a "2 coil latching relay"
//'e' will energize the "set" coil
//'q' will energize the "reset" coil
//therefore, these two characters will turn on and off the head beams. 
void high_beams_on(void)
{
    LATD=0b00000010; //set relay to turn on the lights
    __delay_ms(10);
    __delay_ms(5);
    LATD=0; 
}
void high_beams_off(void)
{
    LATD = 0b00000001; //reset the relay to turn on off the lights
    __delay_ms(10);
    __delay_ms(5);
    LATD=0;
}

void debug_leds_on(void)
{
    LATA |= 0x07;    
}

void debug_leds_off(void)
{
   LATA &= 0xF8;   //Turn off the onboard diagnostic LEDs  
}
