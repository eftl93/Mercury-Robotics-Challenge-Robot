/**********************************************************************
University of Texas - Pan American
Eder Torres
Sept. 13, 2014
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




#ifndef LM629_H
#define LM629_H
#include <htc.h>
#include <p18f46k22.h>
#define _XTAL_FREQ 64000000       //Use 64MHz as FOSC (16MHz Crystal with 4X PLL)
/***********************************************************************
Declaration
***********************************************************************/
//LM629 Command Addresses
#define LM629_RESET    0x00 // Reset LM629
#define DFH      0x02 // Define Home
#define SIP      0x03 // Set Index Position
#define LPEI     0x1B // Interrupt on Error
#define LPES     0x1A // Stop on Error
#define SBPA     0x20 // Set Breakpoint, Absolute
#define SBPR     0x21 // Set Breakpoint, Relative
#define MSKI     0x1C // Mask Interrupts
#define RSTI     0x1D // Reset Interrupts
#define LFIL     0x1E // Load Filter Parameters
#define UDF      0x04 // Update Filter
#define LTRJ     0x1F // Load Trajectory
#define STT      0x01 // Start Motion
#define RDSIGS   0x0C // Read Signals Register
#define RDIP     0x09 // Read Index Position
#define RDDP     0x08 // Read Desired Position
#define RDRP     0x0A // Real Real Position
#define RDDV     0x07 // Read Desired Velocity
#define RDRV     0x0B // Read Real Velocity
#define RDSUM    0x0D // Read Integration Sum
#define INPUT    0xFF // To Set a port as input
#define OUTPUT   0x00 // To Set a port as output

//Pins Declaration
#define LM629_0_CS  PORTDbits.RD3 //Chip Select 0
#define LM629_1_CS  PORTBbits.RB2 //Chip Select 1
#define LM629_2_CS  PORTAbits.RA0
#define LM629_3_CS  PORTBbits.RB4

#define LM629_RD    PORTDbits.RD2
#define LM629_PS    PORTCbits.RC2
#define LM629_WR    PORTDbits.RD1
#define LM629_RST   PORTBbits.RB5

#define D0          PORTCbits.RC6
#define D1          PORTCbits.RC7
#define D2          PORTDbits.RD4
#define D3          PORTDbits.RD5
#define D4          PORTDbits.RD6
#define D5          PORTDbits.RD7
#define D6          PORTBbits.RB0
#define D7          PORTBbits.RB1



//Procedure Declaration
void LM629_init();
void DATABUS_DIR(unsigned char dir);     //Added in REV 5 for new pin assignment
unsigned char DATABUS(unsigned char dir1, unsigned char byte0);// Added in Rev 5 for new pin assigment
void LM629_velocity_start(const unsigned char chip, unsigned long vel);
void LM629_position_start(const unsigned char chip, const unsigned long vel, const long pos);
void check_busy();
unsigned char read_status();
unsigned int read_data();
void write_data(unsigned char byte1, unsigned char byte2);
void write_command(unsigned char command);
void chip_select(unsigned char chip);
bit busy_bit;
void motor_break();
void all_break();
void motor_off();
void all_off();
void filter_module();
void simple_absolute_position();
void simple_relative_position();
void velocity_mode_breakpoints();
void forward();
void reverse();
void left();
void right();
/***********************************************************************
End of Declaration
***********************************************************************/


/***********************************************************************
Definition of Functions
***********************************************************************/


void check_busy()
{
	unsigned char x;
	x=read_status();
	busy_bit=x;
	while (busy_bit==1)
		{
			x=read_status();
			busy_bit=x;
		}
}
	


unsigned char read_status()
{
	unsigned char status;
	DATABUS_DIR(0xFF);
	LM629_PS=0;
	_delay(3);
	LM629_RD=0;
	_delay(12);
	status=DATABUS(0xFF,0);
	LM629_RD=1;
	_delay(5);
	LM629_PS=1;

return status;
}

unsigned int read_data()
{
	unsigned char left_value;
	unsigned char right_value;
	unsigned int data;
	DATABUS_DIR(0xFF);
	LM629_PS=1;
	_delay(3);
	LM629_RD=0;
	_delay(12);
	left_value=DATABUS(0xFF,0);
	LM629_RD=1;
	_delay(12);
	LM629_RD=0;
	_delay(12);
	right_value=DATABUS(0xFF,0);
	LM629_RD=1;
	_delay(3);
	LM629_PS=1;
	data=left_value;
	data=data<<8;
	data=data+right_value;	
	
	return data;
}

void write_data( unsigned char byte1, unsigned char byte2)
{
	__delay_us(1);
	DATABUS_DIR(0);
	LM629_PS=1;
	_delay(3);
	LM629_WR=0;
	_delay(8);
	DATABUS(0,byte1);
	_delay(5);
	LM629_WR=1;
	_delay(10);
	LM629_WR=0;
	_delay(5);
	DATABUS(0,byte2);
	_delay(5);
	LM629_WR=1;
	_delay(3);
	LM629_PS=1;

}

void write_command(unsigned char command)
{
	DATABUS_DIR(0);
	LM629_PS=0;
	_delay(3);
	LM629_WR=0;
	_delay(5);
	DATABUS(0,command);
	_delay(5);
	LM629_WR=1;
	_delay(3);
	LM629_PS=1;
	_delay(11);
		
}

void LM629_init()
{
	unsigned char x;
	x=0;
	TRISB&=0b11000011;
	TRISD&=0b11110001;
	TRISC&=0b11111011;
	LM629_PS=1;
	LM629_RD=1;
	LM629_WR=1;
	LM629_RST=1;
	chip_select(0);
beginning:
	__delay_us(10);
	LM629_RST=0;
	__delay_us(2);
	LM629_RST=1;
	__delay_ms(2);
	x=read_status();
	if(!(x==0xC4 || x==0x84))
		{
		goto beginning;
		}
	else
		{
			check_busy();
			write_command(RSTI);
        	check_busy();
			write_data(0x00,0x00);
			check_busy();
			x=read_status();
            if(!(x==0xC0 || x==0x80))
				{	
				goto beginning;
				}
		}

	chip_select(1);
beginning1:
	__delay_us(10);
	LM629_RST=0;
	__delay_us(2);
	LM629_RST=1;
	__delay_ms(2);
	x=read_status();
	if(!(x==0xC4 || x==0x84))
		{
		goto beginning1;
		}
	else
		{
			check_busy();
			write_command(0x1D);
        	check_busy();
			write_data(0x00,0x00);
			check_busy();
			x=read_status();
            if(!(x==0xC0 || x==0x80))
				{	
				goto beginning1;
				}
		}

	chip_select(3);
beginning3:
	__delay_us(10);
	LM629_RST=0;
	__delay_us(2);
	LM629_RST=1;
	__delay_ms(2);
	x=read_status();
	if(!(x==0xC4 || x==0x84))
		{
		goto beginning3;
		}
	else
		{
			check_busy();
			write_command(0x1D);
        	check_busy();
			write_data(0x00,0x00);
			check_busy();
			x=read_status();
            if(!(x==0xC0 || x==0x80))
				{	
				goto beginning3;
				}
		}
chip_select(0);
filter_module();
chip_select(1);
filter_module();
chip_select(2);
filter_module();
chip_select(3);
filter_module();


}

void DATABUS_DIR(unsigned char dir)
{
if(dir==0)
{
TRISC&=0b00111111;
TRISD&=0b00001111;
TRISB&=0b11111100;
}
else if(dir==0xFF)
{
TRISC|=0b11000000;
TRISD|=0b11110000;
TRISB|=0b00000011;
}
}

unsigned char DATABUS(unsigned char dir1, unsigned char byte0)
{
unsigned char x;
if(dir1==0)
{
D0 = ((byte0 & 0x01) >> 0);
D1 = ((byte0 & 0x02) >> 1);
D2 = ((byte0 & 0x04) >> 2);
D3 = ((byte0 & 0x08) >> 3);
D4 = ((byte0 & 0x10) >> 4);
D5 = ((byte0 & 0x20) >> 5);
D6 = ((byte0 & 0x40) >> 6);
D7 = ((byte0 & 0x80) >> 7);
x=0;
}

else if (dir1==0xFF)
{
x=D7;
x=(x<<1)+D6;
x=(x<<1)+D5;
x=(x<<1)+D4;
x=(x<<1)+D3;
x=(x<<1)+D2;
x=(x<<1)+D1;
x=(x<<1)+D0; 
}
return x;
}

void chip_select(unsigned char chip)
{
	if(chip==0)
	{	
		LM629_0_CS=0;
		LM629_1_CS=1;
		LM629_2_CS=1;
		LM629_3_CS=1;
	}
	else if(chip==1)
	{
		LM629_0_CS=1;
		LM629_1_CS=0;
		LM629_2_CS=1;
		LM629_3_CS=1;
	}
	else if(chip==2)
	{
		LM629_0_CS=1;
		LM629_1_CS=1;
		LM629_2_CS=0;
		LM629_3_CS=1;
	}
	else if(chip==3)
	{
		LM629_0_CS=1;
		LM629_1_CS=1;
		LM629_2_CS=1;
		LM629_3_CS=0;
	}
	else 
	{
		LM629_0_CS=1;
		LM629_1_CS=1;
		LM629_2_CS=1;
		LM629_3_CS=1;	
	}		

	__delay_us(10);
}

void motor_off()
{
write_command(LTRJ);
check_busy();
write_data(0x01,0x00);
check_busy();
write_command(STT);
check_busy();
}

void motor_break()
{
write_command(LTRJ);
check_busy();
write_data(0x02,0x00);
check_busy();
write_command(STT);
check_busy();
}

void all_break()
{
chip_select(0);
motor_break();
chip_select(1);
motor_break();
chip_select(2);
motor_break();
chip_select(3);
motor_break();
}

void all_off()
{
chip_select(0);
motor_off();
chip_select(1);
motor_off();
chip_select(2);
motor_off();
chip_select(3);
motor_off();
}

void filter_module()
{
write_command(LFIL);
check_busy();
write_data(0x00,0x0F); //set all coefficients kp, ki, kd, il
check_busy();
write_data(0x01,0x0F); //kp
check_busy();
write_data(0x00,0x04); //ki
check_busy();
write_data(0x7F,0xFF); //kd
check_busy();
write_data(0x00,0xFF); //il
check_busy();
write_command(UDF);
check_busy();
}

void simple_absolute_postion()
{
write_command(LTRJ);
check_busy();
write_data(0x00,0x24);
check_busy();
write_data(0x00,0x00);
check_busy();
write_data(0x00,0x02);
check_busy();
write_data(0x00,0x00);
check_busy();
write_data(0x34,0x6E);
check_busy();
write_data(0x00,0x00);
check_busy();
write_data(0x1F,0x40);
check_busy();
write_command(STT);
}

void simple_relative_position()
{
write_command(LTRJ);
check_busy;
write_data(0x00,0x2B); //All parameter will be loaded Vel & Acc
check_busy;
write_data(0x00,0x00); //Acc. High
check_busy;
write_data(0x00,0x11); //Acc. Low
check_busy;
write_data(0x00,0x02); //Vel. High
check_busy;
write_data(0x75,0x3F); //Vel. Low
check_busy;
write_data(0xFF,0xFE); //Pos. High
check_busy;
write_data(0x2B,0x40); //Pos. Low
check_busy;
write_command(STT);
}

void velocity_mode_breakpoints()
{

}

void forward()
{
chip_select(0);
check_busy();
motor_off();
write_command(LTRJ);
check_busy();
write_data(0x18,0x28);
check_busy();

write_data(0x00,0x00);
check_busy();
write_data(0x00,0x48);
check_busy();

write_data(0x00,0x01);
check_busy();
write_data(0xFA,0xD0);
check_busy();


chip_select(1);
check_busy();
motor_off();
write_command(LTRJ);
check_busy();
write_data(0x08,0x28);
check_busy();

write_data(0x00,0x00);
check_busy();
write_data(0x00,0x48);
check_busy();

write_data(0x00,0x01);
check_busy();
write_data(0xFA,0xD0);
check_busy();


chip_select(2);
check_busy();
motor_off();
write_command(LTRJ);
check_busy();
write_data(0x08,0x28);
check_busy();

write_data(0x00,0x00);
check_busy();
write_data(0x00,0x48);
check_busy();

write_data(0x00,0x01);
check_busy();
write_data(0xFA,0xD0);
check_busy();


chip_select(3);
check_busy();
motor_off();
write_command(LTRJ);
check_busy();
write_data(0x18,0x28);
check_busy();

write_data(0x00,0x00);
check_busy();
write_data(0x00,0x48);
check_busy();

write_data(0x00,0x01);
check_busy();
write_data(0xFA,0xD0);
check_busy();


chip_select(0);
write_command(STT);
check_busy();

chip_select(2);
write_command(STT);
check_busy();

chip_select(1);
write_command(STT);
check_busy();

chip_select(3);
write_command(STT);
check_busy();
}


void reverse()
{
chip_select(1);
check_busy();
motor_off();
write_command(LTRJ);
check_busy();
write_data(0x18,0x28);
check_busy();

write_data(0x00,0x00);
check_busy();
write_data(0x00,0x48);
check_busy();

write_data(0x00,0x01);
check_busy();
write_data(0xFA,0xD0);
check_busy();


chip_select(0);
check_busy();
motor_off();
write_command(LTRJ);
check_busy();
write_data(0x08,0x28);
check_busy();

write_data(0x00,0x00);
check_busy();
write_data(0x00,0x48);
check_busy();

write_data(0x00,0x01);
check_busy();
write_data(0xFA,0xD0);
check_busy();



chip_select(3);
check_busy();
motor_off();
write_command(LTRJ);
check_busy();
write_data(0x08,0x28);
check_busy();

write_data(0x00,0x00);
check_busy();
write_data(0x00,0x48);
check_busy();

write_data(0x00,0x01);
check_busy();
write_data(0xFA,0xD0);
check_busy();

chip_select(2);
check_busy();
motor_off();
write_command(LTRJ);
check_busy();
write_data(0x18,0x28);
check_busy();

write_data(0x00,0x00);
check_busy();
write_data(0x00,0x48);
check_busy();

write_data(0x00,0x01);
check_busy();
write_data(0xFA,0xD0);
check_busy();


chip_select(0);
write_command(STT);
check_busy();

chip_select(2);
write_command(STT);
check_busy();

chip_select(1);
write_command(STT);
check_busy();

chip_select(3);
write_command(STT);
check_busy();
}

void right()
{
chip_select(0);
check_busy();
motor_off();
write_command(LTRJ);
check_busy();
write_data(0x08,0x28);
check_busy();

write_data(0x00,0x00);
check_busy();
write_data(0x00,0x48);
check_busy();

write_data(0x00,0x01);
check_busy();
write_data(0xFA,0xD0);
check_busy();


chip_select(1);
check_busy();
motor_off();
write_command(LTRJ);
check_busy();
write_data(0x08,0x28);
check_busy();

write_data(0x00,0x00);
check_busy();
write_data(0x00,0x48);
check_busy();

write_data(0x00,0x01);
check_busy();
write_data(0xFA,0xD0);
check_busy();


chip_select(2);
check_busy();
motor_off();
write_command(LTRJ);
check_busy();
write_data(0x08,0x28);
check_busy();

write_data(0x00,0x00);
check_busy();
write_data(0x00,0x48);
check_busy();

write_data(0x00,0x01);
check_busy();
write_data(0xFA,0xD0);
check_busy();


chip_select(3);
check_busy();
motor_off();
write_command(LTRJ);
check_busy();
write_data(0x08,0x28);
check_busy();

write_data(0x00,0x00);
check_busy();
write_data(0x00,0x48);
check_busy();

write_data(0x00,0x01);
check_busy();
write_data(0xFA,0xD0);
check_busy();


chip_select(0);
write_command(STT);
check_busy();

chip_select(2);
write_command(STT);
check_busy();

chip_select(1);
write_command(STT);
check_busy();

chip_select(3);
write_command(STT);
check_busy();
}

void left()
{
chip_select(0);
check_busy();
motor_off();
write_command(LTRJ);
check_busy();
write_data(0x18,0x28);
check_busy();

write_data(0x00,0x00);
check_busy();
write_data(0x00,0x48);
check_busy();

write_data(0x00,0x01);
check_busy();
write_data(0xFA,0xD0);
check_busy();

chip_select(1);
check_busy();
motor_off();
write_command(LTRJ);
check_busy();
write_data(0x18,0x28);
check_busy();

write_data(0x00,0x00);
check_busy();
write_data(0x00,0x48);
check_busy();

write_data(0x00,0x01);
check_busy();
write_data(0xFA,0xD0);
check_busy();


chip_select(2);
check_busy();
motor_off();
write_command(LTRJ);
check_busy();
write_data(0x18,0x28);
check_busy();

write_data(0x00,0x00);
check_busy();
write_data(0x00,0x48);
check_busy();

write_data(0x00,0x01);
check_busy();
write_data(0xFA,0xD0);
check_busy();


chip_select(3);
check_busy();
motor_off();
write_command(LTRJ);
check_busy();
write_data(0x18,0x28);
check_busy();

write_data(0x00,0x00);
check_busy();
write_data(0x00,0x48);
check_busy();

write_data(0x00,0x01);
check_busy();
write_data(0xFA,0xD0);
check_busy();


chip_select(0);
write_command(STT);
check_busy();

chip_select(2);
write_command(STT);
check_busy();

chip_select(1);
write_command(STT);
check_busy();

chip_select(3);
write_command(STT);
check_busy();
}

/***********************************************************************
End of definition of functions
***********************************************************************/
#endif 