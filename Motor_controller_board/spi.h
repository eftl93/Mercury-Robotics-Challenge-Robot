/**********************************************************************
University of Texas - Pan American
Eder Torres
Oct. 18, 2014
This header file will have data to facilitate the lm629 programming
**********************************************************************/


/**********************************************************************
Revisions
***********************************************************************
REV. 1: September 13, 2014: File created to have a cleaner main file
Command addresses were defined by their name. 
Pin Declaration has been started
************************************************************************
End of Revisions
***********************************************************************/
#ifndef SPI_H
#define SPI_H
#include <htc.h>
#include <p18f46k22.h>
#define _XTAL_FREQ 40000000

#define SS1 PORTAbits.RA5
#define SCLK1 PORTCbits.RC3
#define SDI1 PORTCbits.RC4
#define SDO1 PORTCbits.RC5

#define SS1_DIR TRISAbits.RA5
#define SCLK1_DIR TRISCbits.RC3
#define SDI1_DIR TRISCbits.RC4
#define SDO1_DIR TRISCbits.RC5


void spi_slave_init();
void spi_data(unsigned char tx_data);
unsigned char spi_read_data;

void interrupt SPI()
{
if(PIR1bits.SSP1IF&&PIE1bits.SSP1IE)
{
//while(!SSPSTATbits.BF);
spi_read_data=SSP1BUF;
SSP1IF=0;
}
}

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

/***********************************************************************
End of definition of functions
***********************************************************************/
#endif 

