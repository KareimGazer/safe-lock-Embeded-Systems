/*
 * External_EEPROM.h
 *
 * Created: 9/26/2019 7:05:51 PM
 *  Author: RI-5RQ7TW2
 */ 


#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_

#define ERROR 0
#define SUCCESS 1

#include "TWI.h"

void EEPROM_Init(void);
unsigned char EEPROM_Write_Byte(unsigned short u16addr, unsigned char u8data);
unsigned char EEPROM_Read_Byte(unsigned short u16addr, unsigned char *u8data);

#endif /* EXTERNAL_EEPROM_H_ */