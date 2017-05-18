#if defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__PIC24H__)
#include <p24hxxxx.h>
#endif

#include "IC.h"

void InitIC()
{
	IC1CONbits.ICM=0b000; // Disable Input Capture 1 module
	IC1CONbits.ICTMR= 1;
	IC2CONbits.ICM=0b000; // Disable Input Capture 1 module
	IC2CONbits.ICTMR= 1;
	IC3CONbits.ICM=0b000; 
	IC3CONbits.ICTMR= 1;
	IC4CONbits.ICM=0b000; // Disable Input Capture 1 module
	IC4CONbits.ICTMR= 1;
	IC5CONbits.ICM=0b000; // Disable Input Capture 1 module
	IC5CONbits.ICTMR= 1;
	IC6CONbits.ICM=0b000; // Disable Input Capture 1 module
	IC6CONbits.ICTMR= 1;
	IC7CONbits.ICM=0b000; // Disable Input Capture 1 module
	IC7CONbits.ICTMR= 1;
	IC8CONbits.ICM=0b000; // Disable Input Capture 1 module
	IC8CONbits.ICTMR= 1;
}

void StartIC()
{
	unsigned int temp;
	while(IC1CONbits.ICBNE == 1)
	{
		temp = IC1BUF;
	}
	while(IC2CONbits.ICBNE == 1)
	{
		temp = IC2BUF;
	}
	while(IC3CONbits.ICBNE == 1)
	{
		temp = IC3BUF;
	}
	while(IC4CONbits.ICBNE == 1)
	{
		temp = IC4BUF;
	}
	while(IC5CONbits.ICBNE == 1)
	{
		temp = IC5BUF;
	}
	while(IC6CONbits.ICBNE == 1)
	{
		temp = IC6BUF;
	}
	while(IC7CONbits.ICBNE == 1)
	{
		temp = IC7BUF;
	}
	while(IC8CONbits.ICBNE == 1)
	{
		temp = IC8BUF;
	}
	IC1CONbits.ICM=0b100; // Every 4th edge
	IC2CONbits.ICM=0b100;
	IC3CONbits.ICM=0b100;
	IC4CONbits.ICM=0b100;
	IC5CONbits.ICM=0b100;
	IC6CONbits.ICM=0b100;
	IC7CONbits.ICM=0b100;
	IC8CONbits.ICM=0b100;
}

void StopIC()
{
	IC1CONbits.ICM=0b000;
	IC2CONbits.ICM=0b000; 
	IC3CONbits.ICM=0b000;
	IC4CONbits.ICM=0b000;
	IC5CONbits.ICM=0b000;
	IC6CONbits.ICM=0b000;
	IC7CONbits.ICM=0b000;
	IC8CONbits.ICM=0b000;
}

unsigned int GetPeriod(int Channel)
{
	unsigned int IC1,IC2,IC3;
	unsigned int PR1,PR2;
	switch(Channel)
	{
		case 1:
			IC1 = IC1BUF;
			IC2 = IC1BUF;
			IC3 = IC1BUF;
			break;
		
		case 2:
			IC1 = IC2BUF;
			IC2 = IC2BUF;
			IC3 = IC2BUF;
			break;
		
		case 3:
			IC1 = IC3BUF;
			IC2 = IC3BUF;
			IC3 = IC3BUF;
			break;
			
		case 4:
			IC1 = IC4BUF;
			IC2 = IC4BUF;
			IC3 = IC4BUF;
			break;
		
		case 5:
			IC1 = IC5BUF;
			IC2 = IC5BUF;
			IC3 = IC5BUF;
			break;
		
		case 6:
			IC1 = IC6BUF;
			IC2 = IC6BUF;
			IC3 = IC6BUF;
			break;
		
		case 7:
			IC1 = IC7BUF;
			IC2 = IC7BUF;
			IC3 = IC7BUF;
			break;
		
		case 8:
			IC1 = IC8BUF;
			IC2 = IC8BUF;
			IC3 = IC8BUF;
			break;	
		default:
			IC1 = 0;
			IC2 = 0;
			IC3 = 0;
	}
	if(IC2<IC1)
	{
		PR1 = 65535 - IC1;
		PR1 = PR1 + IC2;			
	}
	else
	{
		PR1 = IC2 - IC1;
	}
	
	if(IC3<IC2)
	{
		PR2 = 65535 - IC2;
		PR2 = PR1 + IC3;			
	}
	else
	{
		PR2 = IC3 - IC2;
	}
	return (PR1+PR2)>>1;
}
