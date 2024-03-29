//Main file for the MSP430G2553 Servo controller
//DCO is 16MHz which is used for MCLK and SMCLK
//Frequency of signal is 50Hz
//Min Poswidth for servo is 1.0 mS which will be a count of 32 in this program
//Max Poswidth for servo is 2.0 mS which will be a count of 65 in this program

//Servo controller will receive characters through the UART peripheral
//UART is running at 115200 baud rate

#include <msp430.h> 
#include <msp430_gpio.h>
#include <stdint.h>
#include "main.h"
#include "msp430_timera.h"
#include "msp430_uart.h"
#include "msp430_servo.h"

volatile struct flags led0_flag, servo0_flag, servo1_flag, servo2_flag, servo3_flag;


//servo0_duty will change the duty cycle of servo signal on pin 2.0
//arm_duty will change the duty cycle of servo signal on pin 2.1
//cam_tilt_duty will change the duty cycle of servo signal on pin 2.2
//cam_pan_duty will change the duty cycle of servo signal on pin 2.3
uint16_t claw_duty = CLAW_MIN_DUTY;         //Servo0
uint16_t arm_duty = ARM_MAX_DUTY;           //Servo1
uint16_t cam_tilt_duty = TILT_NEUTRAL_DUTY; //Servo2
uint16_t cam_pan_duty = PAN_NEUTRAL_DUTY;   //Servo3

int main(void)
{
	uint8_t received_byte = 0xff;    
    WDTCTL  = WDTPW | WDTHOLD;      //stop watchdog timer
    BCSCTL1 =   CALBC1_16MHZ;       //set internal clock range to 16MHz
    DCOCTL  =   CALDCO_16MHZ;       //set DCO step | modulation
    BCSCTL2 &= 0xF0;                //Ensure that DCO resistor is internal | Divider for SMCLK /1
    p3_init();
    uart_init();
    servo_init();
    led_init();
    timera_cc_init(COUNTER_COMPARE);

    while(1)
    {
        led0_toggle();
        //After each timer interrupt, CPU wakes up and runs once before going back to sleep
        //check if any servo flag has been updated and clear/set its signal accordingly
        if(servo0_flag.set)
         {
             servo0_flag.set = 0;
             servo0_set();
         }

         if(servo1_flag.set)
         {
             servo1_flag.set = 0;
             servo1_set();
         }

         if(servo2_flag.set)
          {
              servo2_flag.set = 0;
              servo2_set();
          }

         if(servo3_flag.set)
          {
              servo3_flag.set = 0;
              servo3_set();
          }

         if(servo0_flag.clear)
          {
              servo0_flag.clear = 0;
              servo0_clear();
          }

          if(servo1_flag.clear)
          {
              servo1_flag.clear = 0;
              servo1_clear();
          }

          if(servo2_flag.clear)
           {
               servo2_flag.clear = 0;
               servo2_clear();
           }

          if(servo3_flag.clear)
           {
               servo3_flag.clear = 0;
               servo3_clear();
           }

          //after each timer interrupt and updating the servo signals, check if a new command has arrived through uart
          //and update the duty_cycle variable accordingly
        received_byte = uart_rd_char();
        switch(received_byte)
        {
        case (0xC8)    :                 //close the claw
            if(claw_duty < CLAW_MAX_DUTY)
                claw_duty++;
            break;

        case (0xC7)    :                //open the claw
            if(claw_duty > CLAW_MIN_DUTY)
                claw_duty--;
            break;

        case (0xC6)    :                   //raise the arm
            if(arm_duty < ARM_MAX_DUTY)
                arm_duty++;
            break;

        case (0xC5)    :                   //lower the arm
            if(arm_duty > ARM_MIN_DUTY)
                arm_duty--;
            break;

        case (0xC3)    :                   //tilt down the camera
            if(cam_tilt_duty < TILT_MAX_DUTY)
                cam_tilt_duty++;
            break;

        case (0xC4)    :                   //tilt up the camera
            if(cam_tilt_duty > TILT_MIN_DUTY)
                cam_tilt_duty--;
            break;

        case (0xC2)    :                   //pan left the camera
            if(cam_pan_duty < PAN_MAX_DUTY)
                cam_pan_duty++;
            break;

        case (0xC1)    :                   //pan right the camera
            if(cam_pan_duty > PAN_MIN_DUTY)
                cam_pan_duty--;
            break;

        default    :                   //if 0xC0 or 0x00 is received, don't  move the servos
            claw_duty = claw_duty;
            arm_duty = arm_duty;
            cam_tilt_duty = cam_tilt_duty;
            cam_pan_duty = cam_pan_duty;
            break;
        }

        //Enable the global interrupt and go to sleep to wait to be waken up by the TimerA interrupt
        __bis_SR_register(LPM0_bits | GIE);
    }
}
