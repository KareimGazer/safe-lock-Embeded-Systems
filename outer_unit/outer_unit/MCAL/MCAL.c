
/*
 * MCAL.c
 *
 * Created: 9/27/2019 7:11:51 PM
 *  Author: RI-5RQ7TW2
 */ 
#include "MCAL.h"

unsigned char ReadBit(unsigned char REG,unsigned char BIT)
{
	if(REG&(1<<BIT))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

unsigned char * FloatToAscii(float num)
{

	static unsigned char str[10]={0};
	char strTemp[10]={0};
	int IntPart=(int)num;
	float FltPart=num-IntPart;
	int DecimalCounter=0;
	float temp=FltPart;
	while(temp<1.0 && temp!=0.0 )
	{
		temp*=10;
		DecimalCounter++;
	}
	temp*=10;
	if(DecimalCounter!=0)
	{
		memset(strTemp,'0',DecimalCounter-1);
	}

	sprintf((char*)str,"%d.%s%d",IntPart,strTemp,(int)temp);
	return str;

}


void ADC_INIT()
{
	//make direction of DDA0 is input
	SETBIT(DDRA,DDA0);
	//internal voltage 2.56 and with external cap at vref
	SETBIT(ADMUX,REFS0);
	CLRBIT(ADMUX,REFS1);
	//Enable ADC
	SETBIT(ADCSRA,ADEN);
	SETBIT(ADMUX,MUX0);
	//Enable Global interrupt flag
	SETBIT(SREG,7);
	//Enable interrupt
	SETBIT(ADCSRA,ADIE);
	//use prescaler 64 as ADC use freq from 50khz to 200 khz
	SETBIT(ADCSRA,ADPS1);
	SETBIT(ADCSRA,ADPS2);
	_delay_ms(10);

}

unsigned short ADC_Read()
{
	//Start Conversion
	SETBIT(ADCSRA,ADSC);
	//wait until conversion complete
	while(ReadBit(ADCSRA,ADIF)==0);
	return ADC;

}

unsigned char* Read_analoge()
{
	unsigned short digitaldata= ADC_Read();
	float  AnalogueData=0.0;
	AnalogueData=(float)(digitaldata*5)/1023;
	return (FloatToAscii(AnalogueData));

}




//ISR 

void INT0_INIT()
{
	//set PD2 as input to accept sig from interrupt 
	CLRBIT(DDRD,DDD2);
	//any logical change
	SETBIT(MCUCR,ISC00);
	CLRBIT(MCUCR,ISC01);
	//enable Global iNT
	SETBIT(SREG,7);
	//enable INT0
	SETBIT(GICR,INT0);
}