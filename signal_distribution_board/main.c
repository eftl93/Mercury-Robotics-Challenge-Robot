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

unsigned char x;

void main()
{
    CM1CON0bits.C1ON=0;
    CM2CON0bits.C2ON=0;
    spi_master_init();
    uart_init();
    TRISA=0;
    TRISD=0;
    PORTA=0;
    PORTD=0b00000001;
    __delay_ms(10);
    __delay_ms(5);
    PORTD=0;
    x=spi_data(3,0x6F);
    //a=y;
    while(1)
    {
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

        x=spi_data(3,y);
        tx2(y);

        if(y==0x61) //a
        {
            PORTA=0b00000001;
        }

        else if(y==0x64)//d
        {
            PORTA=0b00000010;
        }

        else if(y==0x77) //w
        {
            PORTA=0b00000100;
        }

        else if(y==0x71) //q
        {
            PORTD=0b00000001;
            __delay_ms(10);
            __delay_ms(5);
            PORTD=0;
        }

        else if(y==0x65) //e
        {
            PORTD=0b00000010;
            __delay_ms(10);
            __delay_ms(5);
            PORTD=0;
        }

        else if(y==0x6F) //o
        {
            PORTA=0b00000111;
        }

        z++;
        if(z==35530)
        {
            y=0x6F; //o
            z=0;
        }

    }
}