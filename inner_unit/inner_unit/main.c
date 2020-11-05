/*
 * inner_unit.c
 *
 * Created: 10/27/2019 6:57:32 PM
 * Author : kareim
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "External_EEPROM.h"

#define LCD_PORT PORTA
#define LCD_DDR DDRA
#define LCD_PIN PINA

#define LCD_RS 0
#define LCD_RW 1
#define LCD_EN 2

#define SERVO_PORT PORTD
#define SERVO_DDR DDRD
#define SERVO_PIN PD5

#define SET_BIT(REG,NUM) REG|=(1<<NUM)
#define CLR_BIT(REG,NUM) REG&=~(1<<NUM)

void init4();
void send4Command(unsigned char command);
void sendData(unsigned char data);
void pulse();//generates a pulse to make the LCD able to latch the command
void waitForNorCommand();//waits for normal commands to be done by the LCD
void waitForExepCommand();//waits for expectational commands to be done by the LCD like clear and return home
void print(char* word);

int getAngle(int angle);//determine the angle the servo motor should take to move
void openServo();//turns the servo the desired angle, the angle is the get angle return angle
void closeServo();
void redLedOn();
void greenLedOn();
void redLedOff();
void greenLedOff();

void UART_INIT();
void UART_SEND(unsigned char data);
void UART_RECIEVE(unsigned char *data);

int main(void)
{	
	unsigned char password[3];//password array
	unsigned char value=0;
	unsigned char checker=0;
	unsigned char operation=0;
	unsigned char num=1;
	unsigned char memory=0;
	unsigned char edit=1;
	
	init4();//initialize lcd in 4-bit mode
	UART_INIT();
	
	//enable write;
 	DDRC|=(1<<PC2);
 	//PORTC|=(1<<PC2);
 	EEPROM_Init();
 	EEPROM_Write_Byte(0X0311,num);
 	EEPROM_Write_Byte(0X0312,num);
 	EEPROM_Write_Byte(0X0313,num);

    /* Replace with your application code */

	while(1)
	{
		checker=0;
		//load password in the beging of the cycle
		EEPROM_Read_Byte(0X0311,&password[0]);
		password[0]=memory+1;
		EEPROM_Read_Byte(0X0312,&password[1]);
		password[1]=memory+1;
		EEPROM_Read_Byte(0X313,&password[2]);
		password[2]=memory+1;
		value=0;
		
    while (1) 
    {	
		checker=0;
		value=0;
 		 if(edit==0)
 		 {
			//load password in the beging of the cycle
			EEPROM_Read_Byte(0X0311,&password[0]);
			password[0]=memory;
			EEPROM_Read_Byte(0X0312,&password[1]);
			password[1]=memory;
			EEPROM_Read_Byte(0X313,&password[2]);
			password[2]=memory;
 		 }
		
		sendData(password[0]+48);
		sendData(password[1]+48);
		sendData(password[2]+48);
		
 		int i;
 		for(i=0;i<3;i++)
 		{
 			UART_RECIEVE(&value);
			 if(value==password[i])
			 checker++;
 		}
		 
		 if(checker==3)
		 {
			 
			 UART_SEND(1);//right password flag
			 UART_RECIEVE(&operation);
			 
			 switch(operation)
			 {
				case (1):
					openServo();
					break;
				case (2):
					closeServo();
					break;
				case(3):
					UART_RECIEVE(&memory);
					 EEPROM_Write_Byte(0X0311,memory);
					 UART_RECIEVE(&memory);
					 EEPROM_Write_Byte(0X0312,memory);
					 UART_RECIEVE(&memory);
					 EEPROM_Write_Byte(0X0313,memory);
					 edit=0;
					break;
				default:
					break;
			 }
		 }
		 
		 else
		 {	
			 UART_SEND(0);//wrong password flag
			 closeServo();
		 }
		
		
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

int getAngle(int angle)
{
	return ((angle+90)/180)+1;
}


void openServo()
{
	//intialization
	SET_BIT(SERVO_DDR,SERVO_PIN);
	
	//green led on
	greenLedOn();
	
	redLedOff();
	
	//giving pulse
	SET_BIT(SERVO_PORT,SERVO_PIN);
	_delay_ms(2);
	CLR_BIT(SERVO_PORT,SERVO_PIN);
	_delay_ms(20);
	
	
}


void closeServo()
{

	//intialization
	SET_BIT(SERVO_DDR,SERVO_PIN);
	
	//red led on
	redLedOn();
	
	greenLedOff();
	
	//giving pulse
	SET_BIT(SERVO_PORT,SERVO_PIN);
	_delay_ms(0);
	CLR_BIT(SERVO_PORT,SERVO_PIN);
	_delay_ms(20);
	
	
}

void redLedOn()
{

	DDRD|=(1<<PD3);
	PORTD|=(1<<PD3);
}


void greenLedOn()
{
	DDRD|=(1<<PD4);
	PORTD|=(1<<PD4);
}

void redLedOff()
{

	DDRD|=(1<<PD3);
	PORTD&=~(1<<PD3);
}


void greenLedOff()
{
	DDRD|=(1<<PD4);
	PORTD&=~(1<<PD4);
}