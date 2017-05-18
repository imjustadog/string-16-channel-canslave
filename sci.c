#if defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__PIC24H__)
#include <p24hxxxx.h>
#endif

#include "sci.h"

void InitSCI()
{
	TRISFbits.TRISF4 = 1; // RX
	TRISFbits.TRISF5 = 0; // TX
	IFS1bits.U2RXIF = 0;
	U2BRG = 128;	// BAUD Rate Setting for 9600	
	U2MODEbits.STSEL = 0; // 1 Stop bit
	U2MODEbits.PDSEL = 0; // No Parity, 8 data bits
	U2MODEbits.ABAUD = 0; // Auto-Baud Disabled
	U2MODEbits.UEN = 0;
	U2STAbits.UTXISEL0 = 0; // Interrupt after one TX character is transmitted
	U2STAbits.UTXISEL1 = 0;
	U2STAbits.URXISEL = 0; // Interrupt after one RX character is received
	U2MODEbits.UARTEN = 1; // Enable UART
	U2STAbits.UTXEN = 1; // Enable UART TX
	//IPC2 |= 0x7000; // Highest priority
	IEC1bits.U2RXIE = 1; // Enable UART2 RX interrupt
	IEC1bits.U2TXIE = 0;
	IFS1bits.U2TXIF = 0;
	IEC1bits.U2TXIE = 0;
	IFS1bits.U2RXIF = 0;

}
