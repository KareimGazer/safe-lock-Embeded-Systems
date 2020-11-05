
/*
 * HAL.c
 *
 * Created: 10/4/2019 5:59:53 PM
 *  Author: RI-5RQ7TW2
 */ 
/*This function is an implem to LCD */

#include "HAL.h"


void LCD_INIT(void)
{
	
	//set 3 bits as o/p
	SETBIT(DDRB,RS);
	SETBIT(DDRB,RW);
	SETBIT(DDRB,EN);
	CLRBIT(PORTB,RW);
	writeCmd(0x02);
	#if _8BITSMODE
	DDRA=0xFF;
	writeCmd(0x38);
	writeCmd(0x01);
	writeCmd(0x0C);
	_delay_ms(20);
	#else
	//4 bits mode
	for(int i=PA4;i<=PA7;i++)
	{
		SETBIT(DDRA,i);
		CLRBIT(PORTA,i);
	}
	writeCmd(0x28);
	writeCmd(0x01);
	writeCmd(0x0C);
	_delay_ms(20);
	#endif
	
}
void Gen_Pulse()
{
	SETBIT(PORTB,EN);
	_delay_ms(1);
	CLRBIT(PORTB,EN);
	_delay_ms(1);
}
//  1001 1111 & 
//  1111 0000
//  genpulse()
//  1001 1111
// &0000 1111
//  1111


void writeCmd(unsigned char cmd)
{
	CLRBIT(PORTB,RS);
	#if _8BITSMODE
	PORTA=cmd;
	Gen_Pulse();
	#else
	for(int i=PA4;i<=PA7;i++)
	{
		CLRBIT(PORTA,i); //PORTA&=0x0f;
	}
	PORTA|=(cmd&0xf0);
	Gen_Pulse();
	
	for(int i=PA4;i<=PA7;i++)
	{
		CLRBIT(PORTA,i);
	}
	PORTA|=((cmd&0x0f)<<4);
	Gen_Pulse();
	#endif
}

void writeData(unsigned char Data)
{
	SETBIT(PORTB,RS);
	#if _8BITSMODE
	PORTA=Data;
	Gen_Pulse();
	#else
	for(int i=PA4;i<=PA7;i++)
	{
		CLRBIT(PORTA,i);
	}
	PORTA|=(Data&0xf0);
	Gen_Pulse();
	
	for(int i=PA4;i<=PA7;i++)
	{
		CLRBIT(PORTA,i);
	}
	PORTA|=((Data&0x0f)<<4);
	Gen_Pulse();
	#endif
}

void WriteString(unsigned char * str)
{
	for(int i=0;i<strlen((const char*)str);i++)
	{
		writeData(str[i]);
	}
}

void ToggleLamp(void)
{
	SETBIT(DDRB,DDB4);
	TOGBIT(PORTB,PB4);
	_delay_ms(1000);
}

void KeyPadInit(void)
{
	SETBIT(DDRC,DDC3);
	SETBIT(DDRC,DDC4);
	SETBIT(DDRC,DDC5); // DDRC|=0x38; //0b 0011 1000
	
    PULLUP(DDRC,PORTC,PC0);
	PULLUP(DDRC,PORTC,PC1);
	PULLUP(DDRC,PORTC,PC2);
}

unsigned char Get_Key()

{
	while(1)
	{
		int i=PC3;
		SETBIT(PORTC,PC3);
		SETBIT(PORTC,PC4);
		SETBIT(PORTC,PC5);//PORTC|=0x38;
		for(i=PC3;i<PC6;i++)
		{
			CLRBIT(PORTC,i);
			unsigned char state;
			state =(PORTC&0x38);
			switch (state)
			{
				case 0x30:
				if(ReadBit(PINC,PINC0)==0)
				{
					while(ReadBit(PINC,PINC0)==0);
					return '1';
				}
				else if(ReadBit(PINC,PINC1)==0)
				{
					while(ReadBit(PINC,PINC1)==0);
					return '4';
				}
				else if(ReadBit(PINC,PINC2)==0)
				{
					while(ReadBit(PINC,PINC2)==0);
					return '7';

				}

				break;
				case 0x28:

				if(ReadBit(PINC,PINC0)==0)
				{
					while(ReadBit(PINC,PINC0)==0);
					return '2';
				}
				else if(ReadBit(PINC,PINC1)==0)
				{
					while(ReadBit(PINC,PINC1)==0);
					return '5';
				}
				else if(ReadBit(PINC,PINC2)==0)
				{
					while(ReadBit(PINC,PINC2)==0);
					return '8';

				}
				break;

				case 0x18:
				// 0001 1000
				//0x 1 8

				if(ReadBit(PINC,PINC0)==0)
				{
					while(ReadBit(PINC,PINC0)==0);
					return '3';
				}
				else if(ReadBit(PINC,PINC1)==0)
				{
					while(ReadBit(PINC,PINC1)==0);
					return '6';
				}
				else if(ReadBit(PINC,PINC2)==0)
				{
					while(ReadBit(PINC,PINC2)==0);
					return '9';

				}

				break;

			}
			SETBIT(PORTC,i);
		}
	}
	return 0;
}


