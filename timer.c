#include "timer.h"
#if defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__PIC24H__)
#include <p24hxxxx.h>
#endif


void InitTimer6()
{
	T6CONbits.TON = 0; // Stop timer
	T6CONbits.TCKPS = 0; // Prescaler is 1:1
	T6CONbits.TCS = 0; // Timer mode
	PR6 = 19999; // 40MHz osc, 1ms interrupt
	TMR6 = 0;
	IPC11 |= 0x7000; // Highest interrupt priority
	IFS2bits.T6IF = 0;
	IEC2bits.T6IE = 1;	
}

void StartTimer6()
{
	TMR6 = 0;
	T6CONbits.TON = 1;
}

void StopTimer6()
{
	T6CONbits.TON = 0;
}

void InitTimer2()
{
	T2CONbits.TON = 0; // Stop timer
	TMR2 = 0;
	PR2 = 0xfffe;
	T2CONbits.TCKPS = 1; // Prescaler is 1:8
	T2CONbits.TCS = 0; // Using internal clock
	T2CONbits.TGATE = 0;
}

void StartTimer2()
{
	TMR2 = 0;
	T2CONbits.TCKPS = 1; // Prescaler is 1:8
	T2CONbits.TON = 1;
}

void StopTimer2()
{
	T2CONbits.TON = 0;
}
