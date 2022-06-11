/*
 * File:   spi_master.c
 * Author: eder0
 *
 * Created on June 10, 2022, 11:14 PM
 */


#include <xc.h>
#include "spi.h"

void spi_master_init(void)
{
SSP1STAT=0x00;
SSP1CON1=0b00100010;
ADCON0=0x00;
ADCON1=0x0F;
SS0_DIR=0;
SS1_DIR=0;
SS2_DIR=0;
SS3_DIR=0;
SCK1_DIR=0;
SDI1_DIR=1;
SDO1_DIR=0;
SS0=1;
SS1=1;
SS2=1;
SS3=1;
}

unsigned char spi_data(unsigned char device, unsigned char tx_data)
{
char data_read;
if(device==0)
{
SS0=0;
SSP1BUF=tx_data;
while(!SSP1STATbits.BF);
SS0=1;
data_read=SSP1BUF;
}

else if(device==1)
{
SS1=0;
SSP1BUF=tx_data;
while(!SSP1STATbits.BF);
SS1=1;
data_read=SSP1BUF;
}

else if(device==2)
{
SS2=0;
SSP1BUF=tx_data;
while(!SSP1STATbits.BF);
SS2=1;
data_read=SSP1BUF;
}

else if(device==3)
{
SS3=0;
SSP1BUF=tx_data;
while(!SSP1STATbits.BF);
SS3=1;
data_read=SSP1BUF;
}

return data_read;
}
