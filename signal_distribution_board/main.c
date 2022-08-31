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


extern volatile uint8_t *rx_str_interrupt;

uint8_t text1[] = "Hello, Welcome!";
uint8_t instructions1[] = "Use left joystick to move left wheel";
uint8_t instructions2[] = "Use right joystick to move right wheel";
uint8_t instructions3[] = "Press 'q' and 'e' to turn light beam off and on";
uint8_t wii_classic_packet[] = "hello!!";

struct uart_package
{
    uint8_t lx_joystick;
    uint8_t ly_joystick;
    uint8_t rx_joystick;
    uint8_t ry_joystick;
    uint8_t d_pad;
    uint8_t action_buttons;
};

struct ctrl_buttons
{
    uint8_t a;
    uint8_t b;
    uint8_t x;
    uint8_t y;
};

struct uart_package classic_ctrl;
struct ctrl_buttons act_buttons;


void main()
{
    uint8_t dummy_spi_tx;
    rx_str_interrupt = &wii_classic_packet; //copy the address of wii_classic_packet[] to rx_str_interrupt 
    IPEN=0; //disable priority levels on interrupts (pic16cxxx compatibility mode)
    INTCON=0b00000000;      //disables all interrupts
    gpio_init();            //initialize GPIOs, set leds and relay controller as output. Turn off all the lights
    spi_master_init();      //initialize SPI in master mode
    uart_init();            //initialize both UART1 and UART2 module
    //timer1_init(60000,8);    //initialize timer1 to every 2000 timer1 cycles. Asynchrounous, source clk is fosc/4. Pre-scaler is 1/8
    INTCONbits.GIE = 1; //enables all unmasked interrupts
    INTCONbits.PEIE = 1; //enables all unmasked peripherals interrupts  
    
    dummy_spi_tx=spi_data(3,0x6F); //send an 'o' to the motor controller board to turn off the motors
    uart_wr_str(1, text1);
    tx1('\n'); //new line
    tx1('\r'); //return to the beginning of the same line
    uart_wr_str(1, instructions1);
    tx1('\n');
    tx1('\r');
    uart_wr_str(1, instructions2);
    tx1('\n');
    tx1('\r');
    uart_wr_str(1, instructions3);
    tx1('\n');
    tx1('\r');

    while(1)
    {
        //remove the offset given by the HID keyboard controller and save the values to a struct
        classic_ctrl.lx_joystick = wii_classic_packet[1] - 33;
        classic_ctrl.ly_joystick = wii_classic_packet[2] - 33;
        classic_ctrl.rx_joystick = wii_classic_packet[3] - 33;
        classic_ctrl.ry_joystick = wii_classic_packet[4] - 33;
        classic_ctrl.d_pad       = wii_classic_packet[5] - 33;
        classic_ctrl.action_buttons = wii_classic_packet[6] - 33;
        wii_classic_packet[7] = '\0';

        //dummy_spi_tx = spi_data(3,'o'); //send 'o' to SPI device 3 @FOSC/64
        tx2(classic_ctrl.d_pad);         //send directional pad info to servo controller
        uart_wr_str(1,wii_classic_packet); //sending the whole string received on uart1_rx back to uart1_tx for debugging purposes
        tx1('\r');

        //sending set of data to motor controller board through spi, starting with 'z' and ending with 'y' to set start and end of package
        dummy_spi_tx = spi_data(3,'z');
        dummy_spi_tx = spi_data(3,classic_ctrl.lx_joystick);
        dummy_spi_tx = spi_data(3,classic_ctrl.ly_joystick);
        dummy_spi_tx = spi_data(3,classic_ctrl.rx_joystick);
        dummy_spi_tx = spi_data(3,classic_ctrl.ry_joystick);
        dummy_spi_tx = spi_data(3,'y');
        
        //read the status of each of the action buttons
        act_buttons.a = ((classic_ctrl.action_buttons & 0b00001000) >> 3);
        act_buttons.b = ((classic_ctrl.action_buttons & 0b00000100) >> 2);
        act_buttons.x = ((classic_ctrl.action_buttons & 0b00000010) >> 1);
        act_buttons.y = ((classic_ctrl.action_buttons & 0b00000001) >> 0);

        //Interpret the commands and turn on the LEDs in a pattern depending on 
        //the data received
        RED_LED = act_buttons.a;
        GREEN_LED = act_buttons.b;
        if((act_buttons.x) & (!act_buttons.y))
        {
            high_beams_on();
        }
        else if((!act_buttons.x) & (act_buttons.y))
        {
            high_beams_off();
        }
        else
        {
            
        }
        

      rx1_overrun_detect_reset();
        
       //load_timer1();
       // __asm("sleep");
        
    }
}
