////////////////////////////////////////////////////////////////////////////////
///%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%///
///%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%///
///%%%%%%%%%%%%%%%%LM629-8 controller with PIC18F46K22%%%%%%%%%%%%%%%%%%%%%%%///
///%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%///
///%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%///
////////////////////////////////////////////////////////////////////////////////
//This program is to control the LM629. Main goal is to be able to control four
//controllers.

//Name: Eder Torres
//Project: TI Innovation Challenge

/*******************************************************************************
********************************************************************************
*******************************Hardware Information*****************************
********************************************************************************
********************************************************************************
MCU: PIC18F46k22 @ 64MHz
Motor Control IC: LM629-8 @ 4MHz

Motors: Pololu 12V, 29:1 Gear Motor w/Encoder
Gear ratio: 29:1
Free-run speed @ 12V: 350rpm
Stall torque @ 12V: 110 oz.in (8 kg-cm)
Free-run current @ 12V: 300mA
Stall current @ 12V: 5A
Quadrature encoder: 64 counts per revolution of motor shaft
6mm diameter D-shaped shaft
1856 counts per revolution
Weight 7.5oz

Encoder colors;
Black>>>>>>motor power
Red>>>>>>>>motor power
Blue>>>>>>>Hall Sensor Vcc (3.5-20V)
Green>>>>>>Hall Sensor GND
Yellow>>>>>Hall Sensor A Output
White>>>>>>Hall Sensor B Output
/*******************************************************************************
********************************************************************************
************************End of Hardware Information*****************************
********************************************************************************
*******************************************************************************/




//REV. 5. October 15, 2014: //Board Modifications were made, two more LM629 chips 
//were added. 
//pinouts were modified as follows
//definition of DATABUS was replaced by DATABUS() function
//definition of DATABUS_DIR was replaced by DATABUS_DIR() function

//LM629_0 CS>>>>RD3
//LM629_1 CS>>>>RB2
//LM629_2 CS>>>>RB3
//LM629_3 CS>>>>RB4
//Other pin definitions are as follows:
//WR>>>>>RD1
//RD>>>>>RD2
//RST>>>>RB5
//PS>>>>>RC2

//D0>>>>>RC6
//D1>>>>>RC7
//D2>>>>>RD4
//D3>>>>>RD5
//D4>>>>>RD6
//D5>>>>>RD7
//D6>>>>>RB0
//D7>>>>>RB1

//SPI Connections
//~SS>>>>RA5
//SDI>>>>RC4
//SDO>>>>RC5
//CLCK>>>RC3

//REV. 4. September 14, 2014: "chip" argument removed from functions.
//chip_select(chip) function created, to be used before functions
//LM629_init() initializes both LM629_0 and LM629_1
//and spelling revision on Revisions' section
//Clock change in Rev. 3 was undone since it was unecessary.
//All function and variable declarations and definition were moved to lm629.h
//Calibrate PID
//Small code to run motor was written
//simple absolute position was written (needs debug: convert long variable into 
//4 char variable

//REV. 3. September 13, 2014: Code modified to adjust the variable definitions
//to the new testing board built, which now includes two LM629
//New Pin definition are as follows:
//LM629_0 CS>>>>RC0
//LM629_0 HI>>>>RB0
//LM629_1 HI>>>>RB1
//LM629_1 CS>>>>RB3
//Other pin definitions are as follows:
//WR>>>>>RC6
//RD>>>>>RC1
//RST>>>>RC7
//PS>>>>>RC2
//Also, on the testing board, 4 pins were taken out for SPI module 1
//Timing were modified since the clock for the LM629 is now 4MHz
//This code now brings motor back to initial condition.
//Header File started

//REV. 2. September 6, 2014: Set SFRs, Wrote main code for functions (CS option
//needs debugging)Timings are meant for LM629_clock to be 6MHz and MCU to be
//64MHz

//REV. 1. September 5,2014: Defined Name for bits and name of functions. Set up 
//Clock correctly. Formated C main file. 


#include <htc.h>                  //For use in Hi-Tech, change to <xc.h> for xc8
#include "lm629.h"
#include "spi.h"
#include <p18f46k22.h>            //For compability with PICC18?
#define _XTAL_FREQ 40000000       //Use 64MHz as FOSC (16MHz Crystal with 4X PLL)



////////////////////////////////////////////////////////////////////////////////
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
//%///////////////////////////Start Main Loop////////////////////////////////%//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
////////////////////////////////////////////////////////////////////////////////
void main(void)
{
IPEN=0;
INTCON=0;
ANSELA=0;
ANSELB=0;
ANSELC=0;
ANSELD=0;
ANSELE=0;
SSP1CON1=0x00;
SSP2CON1=0x00;
CM1CON0=0b00000000;
CM2CON0=0b00000000;
PORTB=0x00;
TRISA=0x00;
TRISD=0x00;
TRISB=0x00;
TRISC=0x00;
CTMUCONH=0x00;
SRCON0=0x00;
VREFCON0=0x00;
VREFCON1=0x00;
HLVDCON=0x00;
LM629_init();
spi_slave_init();
unsigned char received_data;
unsigned char temp_clear;








while(1)
{
temp_clear=SSP1BUF;
spi_data(0b01010100);
received_data=spi_read_data;

if(received_data==0x77)
{
all_off();
forward();
}

else if(received_data==0x73)
{
all_off();
reverse();
}

else if(received_data==0x61)
{
all_off();
left();
}

else if(received_data==0x64)
{
all_off();
right();
}

else if(received_data==0x6F)
{
all_off();
}

else if(received_data==0x62)
{
all_break();
}

else if(received_data==0x71)
{
}


}

}
////////////////////////////////////////////////////////////////////////////////
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
//%/////////////////////////////End Main Loop////////////////////////////////%//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
////////////////////////////////////////////////////////////////////////////////

