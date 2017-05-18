#include "adc.h"
#if defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__PIC24H__)
#include <p24hxxxx.h>
#endif
//北京市海淀区中关村南大街5号院
//山西省运城市临猗县乐善村
void InitADC(void)
{
	AD1CON1bits.FORM = 0; // Data Output Format: Unsigend int
	AD1CON2bits.VCFG = 0; // Convert voltage reference set to VDD and VSS
	AD1CON1bits.SSRC = 7; // Internal Counter (SAMC) ends sampling and starts conversion
	AD1CON1bits.ASAM = 0; // ADC Sample Control: Sample begins after sample bit is set
	AD1CON1bits.AD12B = 1; // 12-bit ADC operation
	AD1CON1bits.SIMSAM = 0; // Sequential sampling of channels
	//AD1CON2bits.CHPS = 1; // In 12-bit, this bit is unimplemented
	
	AD1CON3bits.ADRC = 0; // ADC Clock is derived from Systems Clock
	AD1CON3bits.SAMC = 3; // Auto Sample Time = 4 * TAD, required by the chip
	AD1CON3bits.ADCS = 2; // TAD = 3 * Tcy = 150 ns
	
	//AD1CON3 = 0x8F00;
	//AD1CON4 = 0x0000;
	//AD1CON1bits.ADDMABM = 1; // DMA buffers are built in conversion order mode
	AD1CON2bits.CSCNA = 0; // Scan inputs
	AD1CON2bits.SMPI = 0; // Interrupt generated after 5th conversion
						//AD1CHS0/AD1CHS123: A/D Input Select Register
	AD1CHS0bits.CH0SA = 9; // MUXA +ve input selection (AIN15) for CH0
	AD1CHS0bits.CH0NA = 0; // MUXA -ve input selection (VREF-) for CH0
	

/*****************
选择则产生一个AD DE 
		
              1	AN16	RC1	2	AN13 RB13	3 AN3 RB3		4	AN1 RB1
              5	AN9	    RB9	6	AN0	 RB0   7 AN10	RB10	8   AN11  RB11;
           

         
*****************/
	
	AD1CSSH=0B0000000000000001; // Select AN16
	AD1CSSL=0B0010111000001011; // Select AN13,11,10,9,3,1,0 
	
    AD1PCFGH=0B1111111111111110;
    AD1PCFGL=0B1101000111110100;
   //   D1CSSL=0x0010111000001011
	TRISCbits.TRISC1 = 1;
	TRISBbits.TRISB13 = 1;
	TRISBbits.TRISB3 = 1;
	TRISBbits.TRISB1 = 1;
	TRISBbits.TRISB9 = 1;
	TRISBbits.TRISB0 = 1;
	TRISBbits.TRISB10 = 1;
	TRISBbits.TRISB11 = 1;

	//AD1PCFGL = 0xFFFF;
	//AD1PCFGH = 0xFFFF;
	//AD1PCFGLbits.PCFG0 = 0; // AN0 as Analog Input
	IFS0bits.AD1IF = 0; // Clear the A/D interrupt flag bit
	IEC0bits.AD1IE = 0; // Do Not Enable A/D interrupt
	//AD1CON1bits.ADON = 1; // Turn on the A/D converter
}

