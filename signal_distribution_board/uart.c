/*
 * File:   uart.c
 * Author: Eder Torres
 *
 * 
 */
#include <xc.h>
#include "uart.h"
#include "main.h"
#include <stdio.h>

volatile unsigned char current_command;
volatile unsigned char previous_command;

void uart_init()
{
    TXSTA1bits.BRGH=1;      //for ASYNC: Highs baud rate selected
    BAUDCON1bits.BRG16=1;   //16 bit baud rate generator is used (because of the HS clock)
    TXSTA2bits.BRGH=1;      //for ASYNC: Highs baud rate selected
    BAUDCON2bits.BRG16=1;   //16 bit baud rate generator is used (because of the HS clock)
    SPBRG1=0x8A;            //Set the baud rate to 115200 (spbrgh1:spbrg1 = 138)
    SPBRGH1=0x00;           //Set the baud rate to 115200 (spbrgh1:spbrg1 = 138)
    SPBRG2=0x82;            //Set the baud rate to 9600 (spbrgh1:spbrg1 = 1666)
    SPBRGH2=0x06;           //Set the baud rate to 9600 (spbrgh1:spbrg1 = 1666)
    RX1_DIR=1;              //RX must be set as an input for uart
    TX1_DIR=1;              //TX must be set as an input for uart
    RX2_DIR=1;              //RX must be set as an input for uart
    TX2_DIR=1;              //TX must be set as an input for uart
    TXSTA1bits.SYNC=0;      //Async mode
    TXSTA2bits.SYNC=0;      //Async mode
    RCSTA1bits.SPEN=1;      //Serial port enabled
    RCSTA2bits.SPEN=1;      //Serial port enabled
    TXSTA1bits.TXEN=1;      //Transmit enabled
    TXSTA2bits.TXEN=1;      //Transmit enabled
    
#ifndef UART1_INTERRUPT     //if interrupt macro is not defined, disable uart1_rx interrupt
    PIE1bits.RC1IE=0;
#endif
#ifdef UART1_INTERRUPT      //if interrupt macro is defined, enable uart1_rx interrupt
    PIE1bits.RC1IE=1;
#endif

    RCSTA1bits.CREN=1;      //Receiver enabled
    RCSTA2bits.CREN=1;      //Receiver enabled
}

//function to transmit to Beaglebone, it was not used for SR. Design 2
//but it was to be used to send sensor information back to the driver of
//the robot
void tx1(char data1)
{
    while(!PIR1bits.TX1IF); //keep checking until the txbuffer is empty
    TXREG1=data1;
}

//function used to transmit commands to the servo controller
void tx2(char data2)
{
    while(!PIR3bits.TX2IF); //wait until the tx is not full
    TXREG2=data2;
}

void uart_wr_str(uint8_t port, uint8_t *str)
{
    switch(port)
    {
        case(1):
            while(*str != '\0')
            {
                tx1(*str++);
            }
            tx1('\0');
            tx1('\n');
            tx1('\r');
            break;
        case(2):
            while(*str != '\0')
            {
                tx2(*str++);
            }
            tx2('\0');
            tx2('\n');
            tx2('\r');
            break;
        default:
            while(*str != '\0')
            {
                tx1(*str++);
            }
            tx1('\0');
            tx1('\n');
            tx1('\r');
            break;
    }
            
                
}
void rx1_overrun_detect_reset(void)
{
          if(RCSTA1bits.OERR)
          {
              RCSTA1bits.CREN = 0;
              __delay_us(4);
              RCSTA1bits.CREN = 1;
          }
}

#ifndef UART1_INTERRUPT
//This is the polling method, it will run if UART1_INTERRUPT is not defined
uint8_t rx1()
{
    uint8_t x;
    if(PIR1bits.RC1IF) //keep if rcbuffer is full
    {
        x=RCREG1;
        PIR1bits.RC1IF = 0;
    }
    else
    {
        x = 0xFF;
    }
    return x;
}
#endif

//This ISR takes care of RX1, this signal comes from the SBC (Beaglebone Black in this case)
#ifdef UART1_INTERRUPT
void __interrupt() UART_ISR(void)
{
    if(RC1IF)
    {
        current_command=RCREG1; //save content into global variable
    }
    RC1IF=0;
}
#endif