#include "DS1302.h"
#if defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__PIC24H__)
#include <p24hxxxx.h>
#endif

unsigned char ds1302_address[7]={0x80,0x82,0x84,0x86,0x88,0x8a,0x8C}; //秒，分，时，日，月，星期，年;写入地址

void write8bit(unsigned char input)
{
	unsigned char temp,i;
	TRISGbits.TRISG12 = 0;
	
	for(temp=8;temp!=0;temp--)
	{
 		SDA=(input&0x01);
 		input=input>>1;
		for(i=0;i<5;i++)
		{asm("NOP");}
 		SCK=1;
		for(i=0;i<60;i++)
		{asm("NOP");}
 		SCK=0;
		for(i=0;i<50;i++)
		{asm("NOP");}
 	}
}

void ds1302_write_byte(unsigned char address, unsigned char tmp)
{
	nRST = 1;
	address = address & 0xFE;
	write8bit(address);
	write8bit(tmp);
	nRST = 0;
}

unsigned char read8bit()
{
	unsigned char temp,i,j;
	TRISGbits.TRISG12 = 1;
	for(i=0;i<10;i++)
	{asm("NOP");}
	for(i=8;i!=0;i--)
	{
 		temp=temp>>1;
 		if(PORTGbits.RG12)
			temp=temp|0x80;
		else 
			temp=temp & 0x7F;

 		SCK=1;
		for(j=0;j<60;j++)
		{asm("NOP");}
 		SCK=0;
		for(j=0;j<60;j++)
		{asm("NOP");}
 	}
	return temp;
}

unsigned char ds1302_read_byte(unsigned char address)
{
	unsigned char temp;
 	nRST = 1;
 	address = address | 0x01;
 	write8bit(address);
 	temp=read8bit();
 	nRST = 0;
 	return temp;
}

void ds1302_write_time(unsigned char shift_address, unsigned char time_data)
{
	unsigned char *p;
	p=ds1302_address;
	ds1302_write_byte(ds1302_control_add, 0x00);  // Clear write protect
	ds1302_write_byte(*(p+shift_address), time_data);;
	ds1302_write_byte(ds1302_control_add,0x80); // Set write protect
}

unsigned char ds1302_bcd2asc(unsigned char temp) //将bcd码转化为十进制
{
	unsigned tmp_buf;
	tmp_buf=temp/16;
	temp=temp%16;
	temp=temp+tmp_buf * 10;
	return temp;
}

unsigned char ds1302_read_time(unsigned char shift_address)
{
//	unsigned char i;
	unsigned time;
	time = ds1302_read_byte(ds1302_address[shift_address]);	
	//time = ds1302_bcd2asc(time);
	return time;
}

void ds1302_init(void)
{
    
    unsigned char sec,i;
	nRST=0;
    SCK=0;
	for(i=0;i<127;i++)
	{asm("NOP");}
	sec=ds1302_read_time(0); // read sec
	ds1302_write_time(0,sec&0x7f);    /*start timer*/  
	//ds1302_write_time(0,0x00);           
}
