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
extern volatile uint16_t tick_counter;
extern volatile uint16_t ticks_per_frame;
extern volatile uint8_t *rx_str_interrupt;

uint8_t text1[] = "Hello, Welcome!";
uint8_t instructions1[] = "Use left joystick to move left wheel";
uint8_t instructions2[] = "Use right joystick to move right wheel";
uint8_t instructions3[] = "Press 'q' and 'e' to turn light beam off and on";
uint8_t success[] = "success";
uint8_t fail[] = "fail";
uint8_t wii_classic_packet[] = "hello!!";



void main()
{
    rx_str_interrupt = &wii_classic_packet; //copy the address of wii_classic_packet[] to rx_str_interrupt
    uint8_t dummy_spi_tx;
    uint8_t forwarded_command;
    IPEN=0; //disable priority levels on interrupts (pic16cxxx compatibility mode)
    INTCON=0b00000000;
    gpio_init();            //initialize GPIOs, set leds and relay controller as output. Turn off all the lights
    spi_master_init();      //initialize SPI in master mode
    uart_init();            //initialize both UART1 and UART2 module
    //timer1_init(60000,8);    //initialize timer1 to every 2000 timer1 cycles. Asynchrounous, source clk is fosc/4. Pre-scaler is 1/8
    INTCONbits.GIE = 1; //enables all unmasked interrupts
    INTCONbits.PEIE = 1; //enables all unmasked peripherals interrupts  
    
    dummy_spi_tx=spi_data(3,0x6F); //send an 'o' to the motor controller board to turn off the motors
    uart_wr_str(1, text1);
    uart_wr_str(1, instructions1);
    uart_wr_str(1, instructions2);
    uart_wr_str(1, instructions3);
    forwarded_command = 'o';

    while(1)
    {
            forwarded_command = wii_classic_packet[1];
            dummy_spi_tx = spi_data(3,'o'); //send 'o' to SPI device 3 @FOSC/64
            tx2('o');                       //send 'o' to uart2 TX
            wii_classic_packet[7] = '\0';
            uart_wr_str(1,wii_classic_packet);
            
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
                case('1'):
                    high_beams_off();
                    break;
                case('@'):
                    high_beams_on();
                    break;
                default:
                    YELLOW_LED = 1;
                    break;
            }
         __delay_ms(1);
          rx1_overrun_detect_reset();
        
       //load_timer1();
       // __asm("sleep");
        
    }
}
