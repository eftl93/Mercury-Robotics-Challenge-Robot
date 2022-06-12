/*
 * msp430_uart.h
 *
 *  Created on: May 21, 2022
 *      Author: eder0
 */

#ifndef MSP430_UART_H_
#define MSP430_UART_H_

void uart_init(void);
void uart_wr_char(unsigned char byte);
void uart_wr_chars(unsigned char *chars, unsigned char length);
void uart_wr_str(unsigned char *str);
unsigned char uart_rd_char(void);

//unsigned char uart_rd_char(void);

#endif /* MSP430_UART_H_ */
