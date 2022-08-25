/*
 * File:   timer1.c
 * Author: eder0
 *
 * Created on July 23, 2022, 9:30 AM
 * Timer1 will be used to wake up the microcontroller to  read UART1. 
 * While the controller is awake, it will process the command and forward it
 * to UART2 and the corresponding SPI channel. 
 * 
 * 
 */


#include <xc.h>
#include <stdio.h>
#include "main.h"
#include "timer1.h"

volatile uint16_t tick_counter = 0;
volatile uint16_t ticks_per_frame = 1000;
volatile uint8_t new_frame = 1;
volatile uint16_t timer1_reg = 0;
volatile uint8_t timer1_reg_h = 0;
volatile uint8_t timer1_reg_l = 0;
void timer1_init(uint16_t cnts_to_overflow, uint8_t prescaler) 
{
    timer1_reg = 65535 - cnts_to_overflow; //calculate the number to load into the TMR1H:TMR1L register pair
    timer1_reg_h = ((timer1_reg & 0xFF00) >> 8); //store upper byte of calculated timer into a variable
    timer1_reg_l = (timer1_reg & 0x00FF);  //store lower byte of calculated timer into a variable
    T1CONbits.TMR1ON = 0;                  //Stop timer1
    PIE1bits.TMR1IE = 0;                   //Disable timer1 interrupt in order to avoid glitches while in asynchronous mode
    T1CONbits.TMR1CS = 0b00;               //Clock source is the instruction clock (FOSC/4)
    T1CONbits.T1SYNC = 1;                  //external clock is async
    switch(prescaler)                      //select pre-scaler, if invalid it will default to 1:1 
    {
        case(1) :
            T1CONbits.T1CKPS = 0b00;
            break;
        case(2):
            T1CONbits.T1CKPS = 0b01;
            break;
        case(4):
            T1CONbits.T1CKPS = 0b10;
            break;
        case(8):
            T1CONbits.T1CKPS = 0b11;
            break;
        default:
            T1CONbits.T1CKPS = 0b00;
    }
}

void load_timer1(void)
{
    T1CONbits.TMR1ON = 0;                  //Stop timer1
    PIE1bits.TMR1IE = 0;                   //Disable timer1 interrupt in order to avoid glitches while in asynchronous mode
    TMR1H = timer1_reg_h;               //load timer variable (upper byte)
    TMR1L = timer1_reg_l;               //load timer variable (lower byte)
    T1CONbits.TMR1ON = 1;               //turn timer1 on
    //timer1 with async clock source generates random interrupts when writing
    //to the timer registers TMR1H:TMR1L or change modes, this only has a window
    //of occurance two T1CKI periods, so the timer interrupt must be off until then
    //and as per the errata sheet, wait two additional instruction cycles.
    //before enabling the timer's interrupt
    while(TMR1L < (timer1_reg_l + 0x02));                
    __asm("nop");
    __asm("nop");
    PIR1bits.TMR1IF = 0; //clear timer1 int. flag in case it was set
    PIE1bits.TMR1IE = 1; //enable interrupt vectoring for timer 1
}

#ifdef UART1_TIMER1_INTERRUPT
void __interrupt() TIMER1_ISR(void)
{
    PIE1bits.TMR1IE = 0; //disable interrupt vectoring for timer 1
    PIR1bits.TMR1IF = 0; //clear timer1 int. flag in case it was set
    if(tick_counter == ticks_per_frame)
    {
        tick_counter = 0;
        new_frame = 1;
    }
    YELLOW_LED ^= 1;
   tick_counter++;              
}
#endif