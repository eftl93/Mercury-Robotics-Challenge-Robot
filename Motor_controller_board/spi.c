/*
 * File:   spi.c
 * Author: Eder Torres
 *
 * These functions are used to initialize the SPI module as a slave device with interrupts enabled
 * spi_data() can load the buffer with a value. this function is not needed in this project
 * The ISR checks the serial peripheral interrupt flag to check if the interrupt was trigger by the spi module and loads the buffer with a dummy value
 */
#include <xc.h>
#include "spi.h"

void spi_slave_init()
{
    SSP1STATbits.SMP = 0;  //must be cleared for slave mode
    SSP1STATbits.CKE = 1; //Trasnmit occurs on transition from active to idle clock state
    SSP1CON1bits.CKP = 0;
    SSP1CON1bits.SSPM = 0x04; //SPI Slave mode, csk pin, and ss pin are enabled
    SSP1CON3 = 0b00010000;  //SSP1BUF update every time that a new data byte is shifted in ignoring the BF bit
    ADCON0 = 0x00;          //disable adc0 circuitry
    ADCON1 = 0x00;          //disable adc1 circuitry
    SS1_DIR = 1;            //set pin for~ss1 as an input 
    SCLK1_DIR = 1;          //set pin for sclk as an input
    SDO1_DIR = 0;           //set pin for sdo as an output
    SDI1_DIR = 1;           //set pin for sdi as an input
    SSP1IE=1;               //enable ssp interrupt
    PEIE=1;                 //enable peripheral interrupt
    GIE=1;                  //enable global interrupt
    SSP1CON1bits.SSPEN = 1; //SPI enabled
}

void spi_data(unsigned char tx_data)
{
    SSP1BUF=tx_data;
}

void __interrupt() SPI_ISR(void)
{
    if(SSPIF)
    {
        spi_read_data=SSP1BUF;  //
        SSP1IF=0;
        SSP1BUF = 0x55;         //fill the buffer with dummy data
    }
}

/***********************************************************************
End of definition of functions
***********************************************************************/
