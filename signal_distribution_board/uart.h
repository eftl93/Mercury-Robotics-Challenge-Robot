#ifndef UART_H
#define UART_H


#include <xc.h>
#include <stdio.h>
#include <string.h>

#define _XTAL_FREQ 64000000
#define RX1 PORTCbits.RC6
#define TX1 PORTCbits.RC7
#define RX2 PORTDbits.RD6
#define TX2 PORTDbits.RD7
#define RX1_DIR TRISCbits.RC6
#define TX1_DIR TRISCbits.RC7
#define RX2_DIR TRISDbits.RD6
#define TX2_DIR TRISDbits.RD7



void uart_init(void);
void tx1(char data1);
void tx2(char data2);
//unsigned char rx1(void); This function is taken care by the ISR





#endif