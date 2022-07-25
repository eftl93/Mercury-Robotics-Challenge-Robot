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
#include "timer1.h"
#include "gpio.h"

#define _XTAL_FREQ 64000000


extern volatile unsigned char current_command;
extern volatile unsigned int glitch_watchdog_counter;
extern volatile unsigned char previous_command;
extern volatile unsigned char nglitch_flag;


void main()
{
    uint8_t dummy_spi_tx;
    gpio_init();            //initialize GPIOs, set leds and relay controller as output. Turn off all the lights
    spi_master_init();      //initialize SPI in master mode
    uart_init();            //initialize both UART1 and UART2 module, only interrupt on RX1;
    timer1_init(2000,8);    //initialize timer1 to every 2000 timer1 cycles. Asynchrounous, source clk is fosc/4. Pre-scaler is 1/8
    dummy_spi_tx=spi_data(3,0x6F); //send an 'o' to the motor controller board to turn off the motors

    
    while(1)
    {

        //if same character has been received back to back, forward custom signal
        //to both TX2 and SPI3
        //glitch detection loop
        while(nglitch_flag==0) //if the command received is the same as the last one, enter the while loop
        {
            glitch_watchdog_counter++;    //and increase the global z counter
            if(glitch_watchdog_counter==65530) //after many cycles, assume glitch and turn off the motors and servos
            {
                current_command=0x6F;       //this is the command to turn off motors and servos
                glitch_watchdog_counter=0;  //reset counter
                nglitch_flag=1;             //exit loop
                previous_command=0;         //clear previous received command
            }
        }
        
        //forward data received @2400Baud on RX1 to SPI_device_3 @ FOSC/64
        //and to TX2 @ 9600Baud  
        dummy_spi_tx=spi_data(3,current_command); 
        tx2(current_command);
        
        //Interpret the commands and turn on the LEDs in a pattern depending on 
        //the data received
        if(current_command==0x61) //'a'
        {
            LATA=0b00000001; //red_led
        }

        else if(current_command==0x64)//'d'
        {
            LATA=0b00000010; //green_led
        }

        else if(current_command==0x77) //'w'
        {
            LATA=0b00000100; //yellow_led
        }
        
        else if(current_command==0x6F) //'o'
        {
            LATA=0b00000111; //all leds
        }
        
        //'e' and 'q' are to set or reset a "2 coil latching relay"
        //'e' will energize the "set" coil
        //'q' will energize the "reset" coil
        //therefore, these two characters will turn on and off the head beams. 
        else if(current_command==0x71) //'q'
        {
            LATD=0b00000001; //reset relay to turn off the lights
            __delay_ms(10);
            __delay_ms(5);
            LATD=0;
        }

        else if(current_command==0x65) //'e'
        {

        }
        
        //This loop is to check lack of communication between Beaglebone and
        //signal_distribution board
        //z gets cleared inside the interrupt, if no interrupt happens,
        //it means that there is no UART1 reception and the motor controller
        //and servo controller must stop the motors and servos
        //z will be incremented every cycle inside the while(1) loop.
        glitch_watchdog_counter++;
        if(glitch_watchdog_counter==35530)
        {
            current_command=0x6F; //'o'
            glitch_watchdog_counter=0;
        }

    }
}
