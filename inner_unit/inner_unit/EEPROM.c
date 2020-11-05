/*
 * EEPROM.c
 *
 * Created: 9/26/2019 7:02:45 PM
 *  Author: RI-5RQ7TW2
 */ 

#include "External_EEPROM.h"

void EEPROM_Init(void){
	TWI_Init(); // just intialize the TWI(I2C) module inside the MC
}

unsigned char EEPROM_Write_Byte(unsigned short u16addr, unsigned char u8data)
{
    TWI_Start();
    if (TWI_Get_Status() != TW_START)
        return ERROR;
	/*chip contains 2048 bytes of EEPROM memory in order to 
	address all bytes 11 bits addressing is used	
    24Cxx chips have four high bit fixed ID which his 0b1010 lower three
    bits are used for addressing chip memory
	write the device address and so we need to get A8 A9 A10 address 
	bits and R/W=0 (write)*/
	TWI_Write(0xA0 | (unsigned char)((u16addr & 0x0700)>>7));// 1010 1110 
    if (TWI_Get_Status() != TW_MT_SLA_W_ACK)
        return ERROR; 
		 
    //send the required location address
    TWI_Write((unsigned char)(u16addr));// word address
    if (TWI_Get_Status() != TW_MT_DATA_ACK)
        return ERROR;
		
    //write byte to eeprom
    TWI_Write(u8data);
    if (TWI_Get_Status() != TW_MT_DATA_ACK)
        return ERROR;
		
    TWI_Stop();
	
    return SUCCESS;
}

unsigned char EEPROM_Read_Byte(unsigned short u16addr, unsigned char *u8data)
{
    TWI_Start();
    if (TWI_Get_Status() != TW_START)
        return ERROR;
		
    //write the device address and so we need to get A8 A9 A10 address bits and R/W=0 (write)
    TWI_Write((0xA0) | ((unsigned char)((u16addr & 0x0700)>>7)));
    if (TWI_Get_Status() != TW_MT_SLA_W_ACK)
        return ERROR;
		
    //send the required location address
    TWI_Write((unsigned char)(u16addr));// word address 
    if (TWI_Get_Status() != TW_MT_DATA_ACK)
        return ERROR;
		
    //send a repeated start
    TWI_Start();
    if (TWI_Get_Status() != TW_REP_START)
        return ERROR;
		
    //write the device address and so we need to get A8 A9 A10 address bits and R/W=1 (Read)
    TWI_Write((0xA0) | ((unsigned char)((u16addr & 0x0700)>>7))|1);
    if (TWI_Get_Status() != TW_MT_SLA_R_ACK)
        return ERROR;
		
    *u8data = TWI_Read_With_NACK();
    if (TWI_Get_Status() != TW_MR_DATA_NACK)
        return ERROR;
		
    TWI_Stop();
    return SUCCESS;
}
