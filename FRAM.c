#if defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__PIC24H__)
#include <p24hxxxx.h>
#endif

#include "FRAM.h"

void InitSPI()
{
	TRISGbits.TRISG6 = 0;
	TRISGbits.TRISG7 = 1;
	TRISGbits.TRISG8 = 0;
	TRISGbits.TRISG9 = 0;
	IFS2bits.SPI2IF = 0; // Clear the Interrupt Flag
	IEC2bits.SPI2IE = 0; // Disable the Interrupt
	// SPI1CON1 Register Settings
	SPI2CON1bits.PPRE = 2; // 4:1 primary prescale
	SPI2CON1bits.SPRE = 7; // 1:1 secondary prescale
	SPI2CON1bits.DISSCK = 0; // Internal Serial Clock is Enabled
	SPI2CON1bits.DISSDO = 0; // SDOx pin is controlled by the module
	SPI2CON1bits.MODE16 = 0; // Communication is byte-wide (8 bits)
	SPI2CON1bits.SMP = 0; // 
	SPI2CON1bits.CKE = 1; // 
	SPI2CON1bits.CKP = 0; // Idle state for clock is a low level; 
	// active state is a high level
	SPI2CON1bits.MSTEN = 1; // Master mode Enabled
	SPI2STATbits.SPIEN = 1; // Enable SPI module
	//SPI2BUF = 0x0000; // Write data to be transmitted 
	// Interrupt Controller Settings
	nSS = 1;
	WREN();
	WRSR(0x40);
}

void WREN()
{
	unsigned char temp;
	nSS = 0;
	asm("nop");
	while(SPI2STATbits.SPITBF){asm("nop");}
	SPI2BUF = 0x06;
	while(!SPI2STATbits.SPIRBF){asm("nop");}
	temp = SPI2BUF;
	asm("nop");
	nSS = 1;
}

unsigned char RDSR()
{
	unsigned char stat;
	nSS = 0;
	asm("nop");
	while(SPI2STATbits.SPITBF){asm("nop");}
	SPI2BUF = 0x05;
	while(!SPI2STATbits.SPIRBF){asm("nop");}
	stat = SPI2BUF;
	while(SPI2STATbits.SPITBF){asm("nop");}
	SPI2BUF = 0x00;
	while(!SPI2STATbits.SPIRBF){asm("nop");}
	stat = SPI2BUF;
	asm("nop");
	nSS = 1;
	return stat;
}

void WRSR(unsigned char value)
{
	unsigned char temp;
	nSS = 0;
	asm("nop");
	while(SPI2STATbits.SPITBF){asm("nop");}
	SPI2BUF = 0x01;
	while(!SPI2STATbits.SPIRBF){asm("nop");}
	temp = SPI2BUF;
	while(SPI2STATbits.SPITBF){asm("nop");}
	SPI2BUF = value;
	while(!SPI2STATbits.SPIRBF){asm("nop");}
	temp = SPI2BUF;
	asm("nop");
	nSS = 1;
}

void RDID()
{
	unsigned char stat;
	nSS = 0;
	asm("nop");
	while(SPI2STATbits.SPITBF){asm("nop");}
	SPI2BUF = 0x9f;
	while(!SPI2STATbits.SPIRBF){asm("nop");}
	stat = SPI2BUF;
	while(SPI2STATbits.SPITBF){asm("nop");}
	SPI2BUF = 0x00;
	while(!SPI2STATbits.SPIRBF){asm("nop");}
	stat = SPI2BUF;
	SPI2BUF = 0x00;
	while(!SPI2STATbits.SPIRBF){asm("nop");}
	stat = SPI2BUF;
	SPI2BUF = 0x00;
	while(!SPI2STATbits.SPIRBF){asm("nop");}
	stat = SPI2BUF;
	asm("nop");
	nSS = 1;
}

void WRITE(unsigned char addr1, unsigned char addr2, unsigned char addr3, unsigned char data)
{
	unsigned char temp;
	WREN();
	
	nSS = 0;
	

	while(SPI2STATbits.SPITBF){asm("nop");}
	SPI2BUF = 0x02;
	while(!SPI2STATbits.SPIRBF){asm("nop");}
	temp = SPI2BUF;
	while(SPI2STATbits.SPITBF){asm("nop");}
	SPI2BUF = addr1;
	while(!SPI2STATbits.SPIRBF){asm("nop");}
	temp = SPI2BUF;
	while(SPI2STATbits.SPITBF){asm("nop");}
	SPI2BUF = addr2;
	while(!SPI2STATbits.SPIRBF){asm("nop");}
	temp = SPI2BUF;
	while(SPI2STATbits.SPITBF){asm("nop");}
	SPI2BUF = addr3;
	while(!SPI2STATbits.SPIRBF){asm("nop");}
	temp = SPI2BUF;
	while(SPI2STATbits.SPITBF){asm("nop");}
	SPI2BUF = data;
	while(!SPI2STATbits.SPIRBF){asm("nop");}
	temp = SPI2BUF;
	asm("nop");
	asm("nop");
	nSS = 1;
}

unsigned char READ(unsigned char addr1, unsigned char addr2, unsigned char addr3)
{
	unsigned char temp;
	nSS = 0;
	asm("nop");
	while(SPI2STATbits.SPITBF){asm("nop");}
	SPI2BUF = 0x03;
	while(!SPI2STATbits.SPIRBF){asm("nop");}
	temp = SPI2BUF;
	while(SPI2STATbits.SPITBF){asm("nop");}
	SPI2BUF = addr1;
	while(!SPI2STATbits.SPIRBF){asm("nop");}
	temp = SPI2BUF;
	while(SPI2STATbits.SPITBF){asm("nop");}
	SPI2BUF = addr2;
	while(!SPI2STATbits.SPIRBF){asm("nop");}
	temp = SPI2BUF;
	while(SPI2STATbits.SPITBF){asm("nop");}
	SPI2BUF = addr3;
	while(!SPI2STATbits.SPIRBF){asm("nop");}
	temp = SPI2BUF;
	while(SPI2STATbits.SPITBF){asm("nop");}
	SPI2BUF = 0x00;
	while(!SPI2STATbits.SPIRBF){asm("nop");}
	temp = SPI2BUF;
	asm("nop");
	nSS = 1;
	
	return temp;

}
