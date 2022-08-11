/*
 * File:   lm629.c
 * Author: eder0
 *
 * Created on June 10, 2022, 3:37 PM
 */


#include <xc.h>
#include "lm629.h"
/***********************************************************************
Definition of Functions
***********************************************************************/

void check_busy()
{
	unsigned char x;
	x=read_status();
	while (x & busy_bit)
		{
			x=read_status();
		}
}
	
unsigned char read_status()
{
	unsigned char status;
	DATABUS_DIR(0xFF);
	LM629_PS=0;
	__delay_us(1);
	LM629_RD=0;
	__delay_us(1);
	status=DATABUS(0xFF,0);
	LM629_RD=1;
	__delay_us(5);
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
    LM629_RD_DIR = 0;
    LM629_PS_DIR = 0;
    LM629_WR_DIR = 0;
    LM629_RST_DIR= 0;
    LM629_0_CS_DIR = 0;
    LM629_1_CS_DIR = 0;
    LM629_2_CS_DIR = 0;
    LM629_3_CS_DIR = 0;
	LM629_PS=1;
	LM629_RD=1;
	LM629_WR=1;
	LM629_RST=1;
	chip_select(0);
beginning:
	__delay_us(10);
	LM629_RST=0;
	__delay_us(12);
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
	__delay_us(12);
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

    	chip_select(2);
beginning2:
	__delay_us(10);
	LM629_RST=0;
	__delay_us(12);
	LM629_RST=1;
	__delay_ms(2);
	x=read_status();
	if(!(x==0xC4 || x==0x84))
		{
		goto beginning2;
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
				goto beginning2;
				}
		}
    
	chip_select(3);
beginning3:
	__delay_us(10);
	LM629_RST=0;
	__delay_us(12);
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

//Since the same bus is used for both reading and writing
//the direction of the bus must be changed to input or output
//before writing or reading data to the bus
//if arg is 0x00, the direction will be OUT
//if arg is 0xFF, the direction will be IN
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

//either read or write data to the pins connected to the DATABUS in the correct order (connected to the lm629)
//if dirl is 0x00, byte0 must be passed to the DATABUS in the correct order
//if dirl is 0xFF, The values in the DATABUS must be returned in the correct order
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
         
    x |= D7;
    x = (x<<1) | D6;
    x = (x<<1) | D5;
    x = (x<<1) | D4;
    x = (x<<1) | D3;
    x = (x<<1) | D2;
    x = (x<<1) | D1;
    x = (x<<1) | D0; 
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

void simple_absolute_position()
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
    check_busy();
    write_data(0x00,0x2B); //All parameter will be loaded Vel & Acc
    check_busy();
    write_data(0x00,0x00); //Acc. High
    check_busy();
    write_data(0x00,0x40); //Acc. Low
    check_busy();
    write_data(0x00,0x05); //Vel. High
    check_busy();
    write_data(0x75,0x3F); //Vel. Low
    check_busy();
    write_data(0x00,0x01); //Pos. High (0xff,0xFE)
    check_busy();
    write_data(0x00,0x40); //Pos. Low
    check_busy();
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

void forward_left()
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

    write_data(0x00,0x00);
    check_busy();
    write_data(0xFD,0x68);
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

    write_data(0x00,0x00);
    check_busy();
    write_data(0xFD,0x68);
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

    void forward_right()
    {
    chip_select(0);
    check_busy();
    motor_off();
    write_command(LTRJ);
    check_busy();
    write_data(0x18,0x28);  //right wheel
    check_busy();

    write_data(0x00,0x00);
    check_busy();
    write_data(0x00,0x48);
    check_busy();

    write_data(0x00,0x00);
    check_busy();
    write_data(0xFD,0x68);
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
    write_data(0x18,0x28); //right wheel
    check_busy();

    write_data(0x00,0x00);
    check_busy();
    write_data(0x00,0x48);
    check_busy();

    write_data(0x00,0x00);
    check_busy();
    write_data(0xFD,0x68);
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

    void reverse_left()
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

    write_data(0x00,0x00);
    check_busy();
    write_data(0xFD,0x68);
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

    write_data(0x00,0x00);
    check_busy();
    write_data(0xFD,0x68);
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

void reverse_right()
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

    write_data(0x00,0x00);
    check_busy();
    write_data(0xFD,0x68);
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

    write_data(0x00,0x00);
    check_busy();
    write_data(0xFD,0x68);
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