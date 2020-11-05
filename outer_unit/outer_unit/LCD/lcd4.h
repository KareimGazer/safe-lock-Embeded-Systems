/*
 * lcd4.h
 *
 * Created: 10/27/2019 7:02:56 PM
 *  Author: kareim
 */ 


#ifndef LCD4_H_
#define LCD4_H_

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



#endif /* LCD4_H_ */