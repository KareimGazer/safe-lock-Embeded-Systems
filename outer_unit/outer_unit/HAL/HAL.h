/*
 * HAL.h
 *
 * Created: 10/4/2019 6:01:00 PM
 *  Author: RI-5RQ7TW2
 */ 


#ifndef HAL_H_
#define HAL_H_
#include "../MCAL/MCAL.h"
//LCD
#define RS  PB1
#define RW  PB2
#define EN  PB3
void LCD_INIT(void);
void Gen_Pulse();
void writeCmd(unsigned char cmd);
void writeData(unsigned char Data);
void WriteString(unsigned char * str);
#define _8BITSMODE   0
void ToggleLamp(void);
void KeyPadInit(void);
unsigned char Get_Key();





#endif /* HAL_H_ */