/*
 * File:   uart.c
 * Author: Eder Torres
 *
 * 
 */
#include <xc.h>
#include "uart.h"

volatile unsigned char y;
volatile unsigned int z;
volatile unsigned char a;
volatile unsigned char b;

void uart_init()
{
    ANSELA=0;               //disable analog
    ANSELB=0;               //disable analog
    ANSELC=0;               //disable analog
    ANSELD=0;               //disable analog
    ANSELE=0;               //disable analog
    TXSTA1bits.BRGH=1;      //for ASYNC: Highs baud rate selected
    BAUDCON1bits.BRG16=1;   //16 bit baud rate generator is used (because of the HS clock)
    TXSTA2bits.BRGH=1;      //for ASYNC: Highs baud rate selected
    BAUDCON2bits.BRG16=1;   //16 bit baud rate generator is used (because of the HS clock)
    SPBRG1=0x0A;            //Set the baud rate to 2400 (spbrgh1:spbrg1 = 6666)
    SPBRGH1=0x1A;           //Set the baud rate to 2400 (spbrgh1:spbrg1 = 6666)
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
    IPEN=0;
    RC1IE=1;
    INTCON|=0b11000000;
    RCSTA1bits.CREN=1;      //Receiver enabled
    RCSTA2bits.CREN=1;      //Receiver enabled
    
    
}

void tx1(char data1)
{
    TXREG1=data1;
}

void tx2(char data2)
{
    TXREG2=data2;
}

//This function is being taken care by the ISR
//char rx1()
//{
//char x;
//while(~RC1IF);
//x=RCREG1;
//return x;
//}

//This ISR takes care of RX1, this signal comes from the SBC (Beaglebone Black in this case)
void __interrupt() UART_ISR(void)
{
    if(RC1IF)
    {
        y=RCREG1;
        z=0;
        if(y==a)
        {
            b=0;
        }
        else
        {
            a=y;
            b=1;
        }

    }
    RC1IF=0;
}
