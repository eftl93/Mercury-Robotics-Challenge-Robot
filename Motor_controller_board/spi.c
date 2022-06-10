/*
 * File:   spi.c
 * Author: eder0
 *
 * Created on June 10, 2022, 3:56 PM
 */
#include <xc.h>
#include "spi.h"

void spi_slave_init()
{
    SSP1IE=1;
    PEIE=1;
    GIE=1;
    SSPSTAT=0x00;
    SSPCON1=0b00100100;
    SSP1CON3=0b00010000;
    ADCON0=0x00;
    ADCON1=0x00;
    SS1_DIR =1;
    SCLK1_DIR=1;
    SDO1_DIR=0;
    SDI1_DIR=1;
}

void spi_data(unsigned char tx_data)
{
    SSP1BUF=tx_data;
}

void __interrupt() SPI()
{
    if(PIR1bits.SSP1IF&&PIE1bits.SSP1IE)
    {
        //while(!SSPSTATbits.BF);
        spi_read_data=SSP1BUF;
        SSP1IF=0;
    }
}

/***********************************************************************
End of definition of functions
***********************************************************************/
