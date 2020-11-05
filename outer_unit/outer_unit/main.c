/*
 * outer_unit.c
 *
 * Created: 10/27/2019 6:32:21 PM
 * Author : kareim
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "HAL/HAL.h"

#define LCD_PORT PORTA
#define LCD_DDR DDRA
#define LCD_PIN PINA

#define LCD_RS 0
#define LCD_RW 1
#define LCD_EN 2

void init4();
void send4Command(unsigned char command);
void sendData(unsigned char data);
void pulse();//generates a pulse to make the LCD able to latch the command
void waitForNorCommand();//waits for normal commands to be done by the LCD
void waitForExepCommand();//waits for expectational commands to be done by the LCD like clear and return home
void print(char* word);

void UART_INIT();
void UART_SEND(unsigned char data);
void UART_RECIEVE(unsigned char *data);

unsigned char keyinput();

int main(void)
{	
	init4();
	UART_INIT();
	KeyPadInit();
	unsigned char pass[3];
	unsigned char flag=0;
	unsigned char choise=0;
	unsigned char reset=0;
	
	/* Replace with your application code */
	while (1)
	{	
		flag=0;
		send4Command(0x01);//clear LCD
		print("enter password");
		send4Command(0xC0);//new line
		
		pass[0]=keyinput();
		sendData(pass[0]+48);
		pass[1]=keyinput();
		sendData(pass[1]+48);
		pass[2]=keyinput();
		sendData(pass[2]+48);
		
		UART_SEND(pass[0]);
		UART_SEND(pass[1]);
		UART_SEND(pass[2]);
		
		UART_RECIEVE(&flag);
		
		if(flag==1)
		{
			send4Command(0x01);//clear LCD
			print("1.open	");
			print("2.close");
			send4Command(0XC0);
			print("3.reset password");
			
			choise=keyinput();
			UART_SEND(choise);
			
			switch(choise)
			{
				case(1):
					send4Command(0x01);//clear LCD
					print("open door");
					break;
				case(2):
					send4Command(0x01);//clear LCD
					print("close door");
					break;
				case(3):
					send4Command(0X01);//clear LCD
					print("enter new pass");
					send4Command(0xC0);//new line
					reset=keyinput();
					sendData(reset+48);
					UART_SEND(reset);
					reset=keyinput();
					sendData(reset+48);
					UART_SEND(reset);
					reset=keyinput();
					sendData(reset+48);
					UART_SEND(reset);
					break;
					
				default:
					send4Command(0X01);//clear LCD
					print("wrong entry");
					break;	
				
			}
			
		}
		
		else
		{
			send4Command(0x01);//clear LCD
			print("wrong password");
		}
		
	}
}

void UART_INIT()
{
	UCSRA= 0x00;// Clear the UASRT status register
	UCSRB|=(1<<TXEN);// Enable  Transmitter
	UCSRB|= (1<<RXEN);// Enable Receiver
	UCSRC= (1<<UCSZ1) | (1<<UCSZ0) | (1<<URSEL);
	UBRRL=3;
}

void UART_SEND(unsigned char data)
{
	while(!((UCSRA)&(1<<UDRE)));
	UDR=data;
}

void UART_RECIEVE(unsigned char *data)
{
	while(!(UCSRA&(1<<RXC)));
	*data=UDR;
}

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

unsigned char keyinput()
{	
	unsigned char keyin=0;
	while(1)
	{
		keyin=Get_Key();
		if(keyin!=0)
		return keyin-48;
	}
	
}