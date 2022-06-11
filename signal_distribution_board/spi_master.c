/*
 * File:   spi_master.c
 * Author: Eder Torres
 *
 * The functions in this file initialize SSP1 for SPI communication as Master
 * it also controls Slave Select pins to select to communicate up to 4 devices 
 * within this function.
 * function can both be received and sent. For the robot, the receive function is not
 * used and it discards that information
 */


#include <xc.h>
#include "spi.h"

void spi_master_init(void)
{
    SSP1STAT=0x00;      //TX occurs on transition from idle to active clock
    SSP1CON1=0b00100010;//Enables the corresponding pin to SPI function & SPI master mode, clock = FOSC/64
    ADCON0=0x00;        //disables ADC
    SS0_DIR=0;          //slave_select0 is output
    SS1_DIR=0;          //slave_select1 is output
    SS2_DIR=0;          //slave_select2 is output
    SS3_DIR=0;          //slave_select3 is output
    SCK1_DIR=0;         //SPI CLK is output
    SDI1_DIR=1;         //SDI is input
    SDO1_DIR=0;         //SDO is output
    SS0=1;              //set slave_select0 pin (SS on slave is active low)
    SS1=1;              //set slave_select1 pin (SS on slave is active low)
    SS2=1;              //set slave_select2 pin (SS on slave is active low)
    SS3=1;              //set slave_select3 pin (SS on slave is active low)
}

//Argument "device" tells the function to which device the communication will 
//happen. "tx_data" is the actual 8 bits to be sent.
//The corresponding select line will be brought down to ground and the the data
//transfer will start and it will wait for the "Buffer Full" bit to be emptied,
//then, the slave select line will be pulled back up to Vdd to deselect the slave.
//the data received will be read from the buffer and returned.
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
