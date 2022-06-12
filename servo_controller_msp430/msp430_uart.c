/*
 * msp430_uart.c
 *
 *  Created on: May 21, 2022
 *      Author: eder0
 */
#include <msp430.h>
#include <stdint.h>
#include "msp430_uart.h"
#include "main.h"

void uart_init(void)
{
    P1SEL    |=  0x06; //set function in pin 1.1 and 1.2 from usci which here it will be rx
    P1SEL2   |=  0x06; //set function in pin 1.1 and 1.2 from usci which here it will be rx
    P1DIR    |=  0x04; //set pin 1.2 as an output
    UCA0CTL1 |=  0x81; //hold USCI in reset state and select SMCLK as clock source
    UCA0CTL0 |=  0x00; //parity disabled, lsb, 8b, 1-stop bit, UART Mode, Asynchronous
    UCA0BR0  =  138;  //104 set baud rate 9600 when source 1MHz //138 for 115200 at 16MHz
    UCA0BR1  =  0;     //0 set baud rate 9600 when source 1MHz //0 for 115200 at 16MHz
    UCA0MCTL |= 0x0E; //Bits [3:1]  UCBRS set to '1' as per the table for 9600 @ 1MHz // UCBRS = 7 for 115200 at 16MHz
    UCA0CTL1 &= 0xFE; //enable uart
    //IE2     |= UCA0TXIE; //enable tx interrupt
    //IE2     |= UCA0RXIE; //enable rx interrupt


}

void uart_wr_char(unsigned char byte)
{
    while(!(IFG2 & UCA0TXIFG));
    UCA0TXBUF = byte;

}

//function to send arbitrary sequence of chars with arbitrary length
void uart_wr_chars(unsigned char *chars, unsigned char length)
{
  while(length--)
  {
     uart_wr_char(*chars++) ;
  }
}


//function to send strings (array of char terminated by nul)
void uart_wr_str(unsigned char *str)
{
  while(*str!= '\0')
  {
     uart_wr_char(*str++) ;
  }
  uart_wr_char('\0');
}


unsigned char uart_rd_char(void)
{
    volatile unsigned char x;

    if(IFG2 & UCA0RXIFG)
    {
        IFG2 &= ~UCA0RXIFG;
        x = UCA0RXBUF;
        return x;
    }
    else
    {
        return 0xFF;
    }
}

