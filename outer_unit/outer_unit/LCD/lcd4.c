/*
 * lcd4.c
 *
 * Created: 10/27/2019 7:03:57 PM
 *  Author: kareim
 */ 

#include "lcd4.h"

void init4()
{
	LCD_DDR=0XFF;
	//LCD_COMMAND_PORT &=~(1<<LCD_EN);
	send4Command(0x33);//send for init
	send4Command(0x32);//send for init
	send4Command(0x28);//4 bit mode
	send4Command(0x0E);//display on, cursor on
	send4Command(0x01);//clear LCD
	waitForExepCommand();
}


void send4Command(unsigned char command)
{
	//enable writing
	LCD_PORT &=~(1<<LCD_RS);//RS pin 0 for commands
	LCD_PORT &=~(1<<LCD_RW);//R/W pin 0 for writing
	
	//first command half
	LCD_PORT=(LCD_PORT&0X0F)|(command&0xF0);
	pulse();
	waitForExepCommand();
	
	//second command half
	LCD_PORT=(LCD_PORT&0X0F)|(command<<4);
	pulse();
	waitForExepCommand();
}


void sendData(unsigned char data)
{
	//enable writing
	LCD_PORT |=(1<<LCD_RS);//RS pin 0 for commands
	LCD_PORT &=~(1<<LCD_RW);//R/W pin 0 for writing
	
	//first command half
	LCD_PORT=(LCD_PORT&0X0F)|(data&0xF0);
	pulse();
	waitForExepCommand();
	
	//second command half
	LCD_PORT=(LCD_PORT&0X0F)|(data<<4);
	pulse();
	waitForExepCommand();
}


void pulse()
{
	LCD_PORT |=(1<<LCD_EN);
	//_delay_us(1);
	LCD_PORT &=~(1<<LCD_EN);
	_delay_us(1);
}


void waitForNorCommand()
{
	_delay_us(100);
}


void waitForExepCommand()
{
	_delay_ms(2);
}


void print(char* word)
{
	unsigned char i=0;
	
	while(word[i]!=0)
	{
		sendData(word[i]);
		i++;
	}
}