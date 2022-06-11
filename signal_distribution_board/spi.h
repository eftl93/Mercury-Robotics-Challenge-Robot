#ifndef SPI_H
#define SPI_H

#include <stdio.h>
#include <string.h>
#include <xc.h>

#define SS0 LATBbits.LATB2
#define SS1 LATBbits.LATB3
#define SS2 LATBbits.LATB4
#define SS3 LATBbits.LATB5
#define SCK1 PORTCbits.RC3
#define SDI1 PORTCbits.RC4
#define SDO1 PORTCbits.RC5

#define SS0_DIR TRISBbits.RB2
#define SS1_DIR TRISBbits.RB3
#define SS2_DIR TRISBbits.RB4
#define SS3_DIR TRISBbits.RB5
#define SCK1_DIR TRISCbits.RC3
#define SDI1_DIR TRISCbits.RC4
#define SDO1_DIR TRISCbits.RC5

void spi_master_init(void);
unsigned char spi_data(unsigned char device, unsigned char tx_data);



#endif