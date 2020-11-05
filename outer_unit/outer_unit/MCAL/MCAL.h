/*
 * MCAL.h
 *
 * Created: 9/27/2019 7:12:24 PM
 *  Author: RI-5RQ7TW2
 */ 


#ifndef MCAL_H_
#define MCAL_H_
#include <avr/io.h>
#define  F_CPU 8000000UL
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>
#define SETBIT(REG,BITNO) REG|=(1<<BITNO)
unsigned char * FloatToAscii(float num);

#define CLRBIT(REG,BITNO) REG&=~(1<<BITNO)
#define TOGBIT(REG,BITNO) REG^=(1<<BITNO)
unsigned char ReadBit(unsigned char REG,unsigned char BITNO);
#define PULLUP(REG1,REG2,BITNO) CLRBIT(REG1,BITNO);SETBIT(REG2,BITNO)
//ISR PROTOTYPES 
void INT0_INIT(void);

//ADC PROTOTYPES
void ADC_INIT();
unsigned short ADC_Read();
unsigned char* Read_analoge();






#endif /* MCAL_H_ */