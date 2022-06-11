/*
 * File:   main.c
 * Author: Eder Torres
 *
 * This code is for a PIC18F46k22 that controls the signal distribution board
 * this board, receives character commands through UART1 from a Beaglebone Black
 * and (if not a repeated character or a disconnect from BBB and this board will
 * forward the received character
 * 
 * This board also controls a "2 Coil Latch relay" to turn on and off beam lights
 * the relay was chosen in order to save power and not overheat the relay coil
 * 
 * This board also has 3 on-board led lights to debug and diagnose the board 
 * in order to see if the board is received the right character and be able to 
 * trace mis-communications
 */

#include <xc.h>
#include <stdio.h>
#include <string.h>
#include "spi.h"
#include "uart.h"
#include "main.h"

#define _XTAL_FREQ 64000000


extern volatile unsigned char y;
extern volatile unsigned int z;
extern volatile unsigned char a;
extern volatile unsigned char b;


void main()
{
    unsigned char dummy_spi_tx;
    
    CM1CON0bits.C1ON=0; //disable comparator1 module
    CM2CON0bits.C2ON=0; //diasble comparator2 module
    spi_master_init();  //initialize spi in master mode
    uart_init();        //initialize both uart1 and uart2 module, only interrupt on rx1;
    TRISA=0;  //output for onboard LED indicators and debugging
    TRISD=0;  //output to control the relay for the beam lights
    LATA=0;   //Turn off the onboard diagnostic LEDs 
    
    LATD = 0b00000001; //reset the relay to turn on off the lights
    __delay_ms(10);
    __delay_ms(5);
    LATD=0; 
    
    dummy_spi_tx=spi_data(3,0x6F); //send an 'o' to the motor controller board to turn off the motors

    
    while(1)
    {

        //if same character has been received back to back, forward custom signal
        //to both TX2 and SPI3
        while(b==0)
        {
            z++;
            if(z==65530)
            {
                y=0x6F;
                z=0;
                b=1;
                a=0;
            }
        }
        
        //forward data received @2400Baud on RX1 to SPI device 3
        //and to TX2 at 9600Baud  
        dummy_spi_tx=spi_data(3,y); 
        tx2(y);
        
        //Interpret the commands and turn on the LEDs in a pattern depending on 
        //the data received
        if(y==0x61) //'a'
        {
            LATA=0b00000001;
        }

        else if(y==0x64)//'d'
        {
            LATA=0b00000010;
        }

        else if(y==0x77) //'w'
        {
            LATA=0b00000100;
        }
        
        else if(y==0x6F) //'o'
        {
            LATA=0b00000111;
        }
        
        //'q' and 'e' are to set or reset a "2 coil latching relay"
        //'e' will energize the "set" coil
        //'q' will energize the "reset" coil
        //therefore, these two characters will turn on and off the head beams. 
        else if(y==0x71) //'q'
        {
            LATD=0b00000001; //reset relay to turn off the lights
            __delay_ms(10);
            __delay_ms(5);
            LATD=0;
        }

        else if(y==0x65) //'e'
        {
            LATD=0b00000010; //set relay to turn on the lights
            __delay_ms(10);
            __delay_ms(5);
            LATD=0;
        }
        
        //This loop is to check lack of communication between Beaglebone and
        //signal_distribution board
        //z gets cleared inside the interrupt, if no interrupt happens,
        //it means that there is no UART1 reception and the motor controller
        //and servo controller must stop the motors and servos
        //z will be incremented every cycle inside the while(1) loop.
        z++;
        if(z==35530)
        {
            y=0x6F; //'o'
            z=0;
        }

    }
}