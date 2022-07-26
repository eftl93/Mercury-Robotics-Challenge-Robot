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
unsigned int glitch_watchdog_counter = 0 ;
extern volatile unsigned char previous_command;
extern volatile uint16_t tick_counter;
extern volatile uint16_t ticks_per_frame;
extern volatile uint8_t new_frame;

uint8_t text1[] = "Hello, Welcome!";
uint8_t instructions1[] = "Press 'w' and 's' to move robot forward and backwards";
uint8_t instructions2[] = "Press 'a' and 'd' to spin robot left and right";
uint8_t instructions3[] = "Press 'q' and 'e' to turn light beam off and on";



void main()
{
    uint8_t dummy_spi_tx;
    uint8_t forwarded_command;
    IPEN=0;
    INTCON=0b00000000;
    gpio_init();            //initialize GPIOs, set leds and relay controller as output. Turn off all the lights
    spi_master_init();      //initialize SPI in master mode
    uart_init();            //initialize both UART1 and UART2 module
    timer1_init(60000,8);    //initialize timer1 to every 2000 timer1 cycles. Asynchrounous, source clk is fosc/4. Pre-scaler is 1/8
    dummy_spi_tx=spi_data(3,0x6F); //send an 'o' to the motor controller board to turn off the motors
    uart_wr_str(1, text1);
    uart_wr_str(1, instructions1);
    uart_wr_str(1, instructions2);
    uart_wr_str(1, instructions3);
    current_command = 0x00;
    previous_command = 0x00;
    forwarded_command = 'o';
    
    
    while(1)
    {
        while(new_frame)
        {
            current_command = rx1();                //read the character on uart1
            if(current_command != 0xFF)
            {
                if(current_command == previous_command) //Check if the received command is the same as the previous received command
                {
                    glitch_watchdog_counter++;          //if it is, start counting how many times
                    if(glitch_watchdog_counter >= 200)  //once the received command is the same as the previous command a certain number of times, send a 'o' character to the servo controller and motor controller
                    {
                        forwarded_command = 'o';
                        high_beams_on();
                    }
                    else
                    {
                        forwarded_command = current_command; //if not, send the received character to both servo controller and motor controller
                    }
                }

                else if(current_command != previous_command) //if the received command is different than the previous command, just send the received command to the servo and motor controllers
                {
                    forwarded_command = current_command;
                    glitch_watchdog_counter = 0;
                    high_beams_off();
                }

                previous_command = current_command; //record the received command in order to do comparisons.     
            }
            
            else
            {
                forwarded_command = 'o';
            }

            //forward data received @2400Baud on RX1 to SPI_device_3 @ FOSC/64
            //and to TX2 @ 9600Baud  
            dummy_spi_tx=spi_data(3,forwarded_command); 
            tx2(forwarded_command);

            //Interpret the commands and turn on the LEDs in a pattern depending on 
            //the data received
            switch(forwarded_command)
            {
                case('a') :
                    debug_leds_off();
                    RED_LED = 1;
                    break;
                case('d'):
                    debug_leds_off();
                    GREEN_LED = 1;
                    break;
                case('w'):
                    debug_leds_off();
                    YELLOW_LED = 1;
                    break;
                case('o'):
                    debug_leds_off();
                    break;
                case('q'):
                    high_beams_off();
                    break;
                case('e'):
                    high_beams_on();
                    break;
                default:
                    debug_leds_on();
                    break;
            }
           //new_frame = 0;
        }
       //load_timer1();
       // __asm("sleep");
       __delay_ms(2); 
    }
}
