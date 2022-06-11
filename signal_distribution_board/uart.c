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
    ANSELA=0;
    ANSELB=0;
    ANSELC=0;
    ANSELD=0;
    ANSELE=0;
    TXSTA1bits.BRGH=1;
    BAUDCON1bits.BRG16=1;
    TXSTA2bits.BRGH=1;
    BAUDCON2bits.BRG16=1;
    SPBRG1=0x0A;
    SPBRGH1=0x1A;
    //SPBRG1=0x82;
    //SPBRGH1=0x06;

    SPBRG2=0x82;
    SPBRGH2=0x06;
    RX1_DIR=1;
    TX1_DIR=1;
    RX2_DIR=1;
    TX2_DIR=1;
    TXSTA1bits.SYNC=0;
    TXSTA2bits.SYNC=0;
    RCSTA1bits.SPEN=1;
    RCSTA2bits.SPEN=1;
    TXSTA1bits.TXEN=1;
    TXSTA2bits.TXEN=1;
    IPEN=0;
    RC1IE=1;
    INTCON|=0b11000000;
    RCSTA1bits.CREN=1;
    RCSTA2bits.CREN=1;
}

void tx1(char data1)
{
    TXREG1=data1;
}

void tx2(char data2)
{
    TXREG2=data2;
}

//char rx1()
//{
//char x;
//while(~RC1IF);
//x=RCREG1;
//return x;
//}

void __interrupt() UART_ISR(void)
{
    if(RC1IF)
    {
        y=RCREG1;
        z=0;
        if(y==a)
        {
            tx1(0);
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
