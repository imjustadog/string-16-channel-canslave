/**********************************************************************
* ?2007 Microchip Technology Inc.
*
* FileName:        main.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       dsPIC33Fxxxx
* Compiler:        MPLAB?C30 v3.00 or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all ownership and 
* intellectual property rights in the code accompanying this message and in all 
* derivatives hereto.  You may use this code, and any derivatives created by 
* any person or entity by or on your behalf, exclusively with Microchip,s 
* proprietary products.  Your acceptance and/or use of this code constitutes 
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A 
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP,S 
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER 
* IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), 
* STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, 
* PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF 
* ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN 
* ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
* ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO 
* THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO 
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify, test, 
* certify, or support the code.
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author          	Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Jatinder Gharoo 	10/30/08  First release of source file
* 
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*************************************************************************************************/
/* Include the appropriate header (.h) file, depending on device used */

#if defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__PIC24H__)
#include <p24hxxxx.h>
#endif
#include "ecan.h"


//unsigned char ID=0X03;
extern unsigned char BOARD_ID ;// Board ID
/************* START OF GLOBAL DEFINITIONS **********/


/************** END OF GLOBAL DEFINITIONS ***********/


/******************************************************************************
*                                                                             
*    Function:			sendECAN
*    Description:       sends the message on a CAN bus  
*                                                                             
*    Arguments:			*message: a pointer to the message structure
*						containing all the information about the message 
*	 Author:            Jatinder Gharoo                                                      
*	                                                                 
*                                                                              
******************************************************************************/
void sendECAN(mID *message)
{
	unsigned long word0=0;
	unsigned long word1=0;
	unsigned long word2=0;
	
	/*
	Message Format: 
	Word0 : 0bUUUx xxxx xxxx xxxx
			     |____________|||
 					SID10:0   SRR IDE(bit 0)     
	Word1 : 0bUUUU xxxx xxxx xxxx
			   	   |____________|
						EID17:6
	Word2 : 0bxxxx xxx0 UUU0 xxxx
			  |_____||	     |__|
			  EID5:0 RTR   	  DLC
	
	Remote Transmission Request Bit for standard frames 
	SRR->	"0"	 Normal Message 
			"1"  Message will request remote transmission
	Substitute Remote Request Bit for extended frames 
	SRR->	should always be set to "1" as per CAN specification
	
	Extended  Identifier Bit			
	IDE-> 	"0"  Message will transmit standard identifier
	   		"1"  Message will transmit extended identifier
	
	Remote Transmission Request Bit for extended frames 
	RTR-> 	"0"  Message transmitted is a normal message
			"1"  Message transmitted is a remote message
	Don't care for standard frames 
	*/
		
	/* check to see if the message has an extended ID */
	if(message->frame_type==CAN_FRAME_EXT)
	{
		/* get the extended message id EID28..18*/		
		word0=(message->id & 0x1FFC0000) >> 16;			
		/* set the SRR and IDE bit */
		word0=word0+0x0003;
		/* the the value of EID17..6 */
		word1=(message->id & 0x0003FFC0) >> 6;
		/* get the value of EID5..0 for word 2 */
		word2=(message->id & 0x0000003F) << 10;			
	}	
	else
	{
		/* get the SID */
		word0=((message->id & 0x000007FF) << 2);	
	}
	/* check to see if the message is an RTR message */
	if(message->message_type==CAN_MSG_RTR)
	{		
		if(message->frame_type==CAN_FRAME_EXT)
			word2=word2 | 0x0200;
		else
			word0=word0 | 0x0002;	
								
		ecan1msgBuf[message->buffer][0]=word0;
		ecan1msgBuf[message->buffer][1]=word1;
		ecan1msgBuf[message->buffer][2]=word2;
	}
	else
	{
		word2=word2+(message->data_length & 0x0F);
		ecan1msgBuf[message->buffer][0]=word0;
		ecan1msgBuf[message->buffer][1]=word1;
		ecan1msgBuf[message->buffer][2]=word2;
		/* fill the data */
		ecan1msgBuf[message->buffer][3]=((message->data[1] << 8) + message->data[0]);
		ecan1msgBuf[message->buffer][4]=((message->data[3] << 8) + message->data[2]);
		ecan1msgBuf[message->buffer][5]=((message->data[5] << 8) + message->data[4]);
		ecan1msgBuf[message->buffer][6]=((message->data[7] << 8) + message->data[6]);
	}
	/* set the message for transmission */
	C1TR01CONbits.TXREQ0=1;
}

/******************************************************************************
*                                                                             
*    Function:			rxECAN
*    Description:       moves the message from the DMA memory to RAM
*                                                                             
*    Arguments:			*message: a pointer to the message structure in RAM 
*						that will store the message. 
*	 Author:            Jatinder Gharoo                                                      
*	                                                                 
*                                                                              
******************************************************************************/
void rxECAN(mID *message)
{
	unsigned int ide=0;
	unsigned int rtr=0;
	unsigned long id=0;
			
	/*
	Standard Message Format: 
	Word0 : 0bUUUx xxxx xxxx xxxx
			     |____________|||
 					SID10:0   SRR IDE(bit 0)     
	Word1 : 0bUUUU xxxx xxxx xxxx
			   	   |____________|
						EID17:6
	Word2 : 0bxxxx xxx0 UUU0 xxxx
			  |_____||	     |__|
			  EID5:0 RTR   	  DLC
	word3-word6: data bytes
	word7: filter hit code bits
	
	Remote Transmission Request Bit for standard frames 
	SRR->	"0"	 Normal Message 
			"1"  Message will request remote transmission
	Substitute Remote Request Bit for extended frames 
	SRR->	should always be set to "1" as per CAN specification
	
	Extended  Identifier Bit			
	IDE-> 	"0"  Message will transmit standard identifier
	   		"1"  Message will transmit extended identifier
	
	Remote Transmission Request Bit for extended frames 
	RTR-> 	"0"  Message transmitted is a normal message
			"1"  Message transmitted is a remote message
	Don't care for standard frames 
	*/
		
	/* read word 0 to see the message type */
	ide=ecan1msgBuf[message->buffer][0] & 0x0001;			
	
	/* check to see what type of message it is */
	/* message is standard identifier */
	if(ide==0)
	{
		message->id=(ecan1msgBuf[message->buffer][0] & 0x1FFC) >> 2;		
		message->frame_type=CAN_FRAME_STD;
		rtr=ecan1msgBuf[message->buffer][0] & 0x0002;
	}
	/* mesage is extended identifier */
	else
	{
		id=ecan1msgBuf[message->buffer][0] & 0x1FFC;		
		message->id=id << 16;
		id=ecan1msgBuf[message->buffer][1] & 0x0FFF;
		message->id=message->id+(id << 6);
		id=(ecan1msgBuf[message->buffer][2] & 0xFC00) >> 10;
		message->id=message->id+id;		
		message->frame_type=CAN_FRAME_EXT;
		rtr=ecan1msgBuf[message->buffer][2] & 0x0200;
	}
	/* check to see what type of message it is */
	/* RTR message */
	if(rtr==1)
	{
		message->message_type=CAN_MSG_RTR;	
	}
	/* normal message */
	else
	{
		message->message_type=CAN_MSG_DATA;
		message->data[0]=(unsigned char)ecan1msgBuf[message->buffer][3];
		message->data[1]=(unsigned char)((ecan1msgBuf[message->buffer][3] & 0xFF00) >> 8);
		message->data[2]=(unsigned char)ecan1msgBuf[message->buffer][4];
		message->data[3]=(unsigned char)((ecan1msgBuf[message->buffer][4] & 0xFF00) >> 8);
		message->data[4]=(unsigned char)ecan1msgBuf[message->buffer][5];
		message->data[5]=(unsigned char)((ecan1msgBuf[message->buffer][5] & 0xFF00) >> 8);
		message->data[6]=(unsigned char)ecan1msgBuf[message->buffer][6];
		message->data[7]=(unsigned char)((ecan1msgBuf[message->buffer][6] & 0xFF00) >> 8);
		message->data_length=(unsigned char)(ecan1msgBuf[message->buffer][2] & 0x000F);
	}
	clearRxFlags(message->buffer);	
}

/******************************************************************************
*                                                                             
*    Function:			clearRxFlags
*    Description:       clears the rxfull flag after the message is read
*                                                                             
*    Arguments:			buffer number to clear 
*	 Author:            Jatinder Gharoo                                                      
*	                                                                 
*                                                                              
******************************************************************************/
void clearRxFlags(unsigned char buffer_number)
{
	if((C1RXFUL1bits.RXFUL1) && (buffer_number==1))
		/* clear flag */
		C1RXFUL1bits.RXFUL1=0;		
	/* check to see if buffer 2 is full */
	else if((C1RXFUL1bits.RXFUL2) && (buffer_number==2))
		/* clear flag */
		C1RXFUL1bits.RXFUL2=0;				
	/* check to see if buffer 3 is full */
	else if((C1RXFUL1bits.RXFUL3) && (buffer_number==3))
		/* clear flag */
		C1RXFUL1bits.RXFUL3=0;				
	else;

}

/******************************************************************************
*                                                                             
*    Function:			initCAN
*    Description:       Initialises the ECAN module                                                        
*                                                                             
*    Arguments:			none 
*	 Author:            Jatinder Gharoo                                                      
*	                                                                 
*                                                                              
******************************************************************************/
void initECAN (void)
{
//	unsigned long temp;
//	unsigned int tempint;
	
	/* put the module in configuration mode */
	C1CTRL1bits.REQOP=4;
	while(C1CTRL1bits.OPMODE != 4);
			
	/* FCAN is selected to be FCY
    FCAN = FCY = 20MHz */
	C1CTRL1bits.CANCKS = BRP_VAL;

	/*
	Bit Time = (Sync Segment + Propagation Delay + Phase Segment 1 + Phase Segment 2)=20*TQ
	Phase Segment 1 = 8TQ
	Phase Segment 2 = 6Tq
	Propagation Delay = 5Tq
	Sync Segment = 1TQ
	CiCFG1<BRP> =(FCAN /(2 ×N×FBAUD))?1
	BIT RATE OF 1Mbps
	*/	
	C1CFG1bits.BRP = BRP_VAL;
	/* Synchronization Jump Width set to 4 TQ */
	C1CFG1bits.SJW = 0x3;
	/* Phase Segment 1 time is 8 TQ */
	C1CFG2bits.SEG1PH=0x7;
	/* Phase Segment 2 time is set to be programmable */
	C1CFG2bits.SEG2PHTS = 0x1;
	/* Phase Segment 2 time is 6 TQ */
	C1CFG2bits.SEG2PH = 0x5;
	/* Propagation Segment time is 5 TQ */
	C1CFG2bits.PRSEG = 0x4;
	/* Bus line is sampled three times at the sample point */
	C1CFG2bits.SAM = 0x1;
	
	/* 4 CAN Messages to be buffered in DMA RAM */	
	C1FCTRLbits.DMABS=0b000;
	
	/* Filter configuration */
	/* enable window to access the filter configuration registers */
	C1CTRL1bits.WIN=0b1;
	/* select acceptance mask 0 filter 0 buffer 1 */
	C1FMSKSEL1bits.F0MSK=0;
	/* configure accpetence mask 0 - match the id in filter 0 
	setup the mask to check mid bits of the standard message, 
	the macro when called as CAN_FILTERMASK2REG_SID(0x7F0) will 
	write the register C1RXM0SID to include mid bits in filter comparison 
	*/ 	




/*
01 02  03  04
12  22  32  42
EC  DC  CC  BC
13  23  33  43

*/
    switch(BOARD_ID)
{
	case 1:
	C1RXM0SID=CAN_FILTERMASK2REG_SID(0x7FF);  //   EC  DC    CC  BC
    C1RXF0SID=CAN_FILTERMASK2REG_SID(0x012);   //    13  23    33  43 
     break;
	case 2:
	C1RXM0SID=CAN_FILTERMASK2REG_SID(0x7FF);  //   EC  DC    CC  BC
    C1RXF0SID=CAN_FILTERMASK2REG_SID(0x022);
     break; 
	case 3:
	C1RXM0SID=CAN_FILTERMASK2REG_SID(0x7FF);  //   EC  DC    CC  BC
    C1RXF0SID=CAN_FILTERMASK2REG_SID(0x032);
     break;
	case 4:
    C1RXM0SID=CAN_FILTERMASK2REG_SID(0x7FF);  //   EC  DC    CC  BC
    C1RXF0SID=CAN_FILTERMASK2REG_SID(0x042);
      break;

}
	/* configure accpetence filter 0 
	setup the filter to accept a standard id of 0x123, 
	the macro when called as CAN_FILTERMASK2REG_SID(0x100) will 
	write the register C1RXF0SID to accept only standard id of 0x10x 	
	*/ 	


	//C1RXF0SID=CAN_FILTERMASK2REG_SID(0x033);
	/* set filter to check for standard ID and accept standard id only */
	C1RXM0SID=CAN_SETMIDE(C1RXM0SID);
	C1RXF0SID=CAN_FILTERSTD(C1RXF0SID);	
	/* acceptance filter to use buffer 1 for incoming messages */
	C1BUFPNT1bits.F0BP=0b0001;
	/* enable filter 0 */
	C1FEN1bits.FLTEN0=1;
	
	/* select acceptance mask 1 filter 1 and buffer 2 */
	C1FMSKSEL1bits.F1MSK=0b01;
	C1RXM1SID=CAN_FILTERMASK2REG_SID(0x7FF);
	C1RXF1SID=CAN_FILTERMASK2REG_SID(0x001);
	C1RXM1SID=CAN_SETMIDE(C1RXM1SID);
	C1RXF1SID=CAN_FILTERSTD(C1RXF1SID);
	/* acceptance filter to use buffer 2 for incoming messages */
	C1BUFPNT1bits.F1BP=0b0010;
	/* enable filter 1 */
	C1FEN1bits.FLTEN1=1;
	
	         
	/* clear window bit to access ECAN control registers */
	C1CTRL1bits.WIN=0;
		
	/* put the module in normal mode */
	C1CTRL1bits.REQOP=0;
	while(C1CTRL1bits.OPMODE != 0);	
	
	/* clear the buffer and overflow flags */
	C1RXFUL1=C1RXFUL2=C1RXOVF1=C1RXOVF2=0x0000;
	/* ECAN1, Buffer 0 is a Transmit Buffer */
	C1TR01CONbits.TXEN0=1;			
	/* ECAN1, Buffer 1 is a Receive Buffer */
	C1TR01CONbits.TXEN1=0;	
	/* ECAN1, Buffer 2 is a Receive Buffer */
	C1TR23CONbits.TXEN2=0;	
	/* ECAN1, Buffer 3 is a Receive Buffer */
	C1TR23CONbits.TXEN3=0;	
	/* Message Buffer 0 Priority Level */
	C1TR01CONbits.TX0PRI=0b11; 		
		
	/* configure the device to interrupt on the receive buffer full flag */
	/* clear the buffer full flags */
	C1RXFUL1=0;
	C1INTFbits.RBIF=0;
}

/******************************************************************************
*                                                                             
*    Function:			initDMAECAN
*    Description:       Initialises the DMA to be used with ECAN module                                                        
*                       Channel 0 of the DMA is configured to Tx ECAN messages
* 						of ECAN module 1. 
*						Channel 2 is uconfigured to Rx ECAN messages of module 1.                                                      
*    Arguments:			
*	 Author:            Jatinder Gharoo                                                      
*	                                                                 
*                                                                              
******************************************************************************/
void initDMAECAN(void)
{
	/* initialise the DMA channel 0 for ECAN Tx */
	/* clear the collission flags */
	DMACS0=0;	
    /* setup channel 0 for peripheral indirect addressing mode 
    normal operation, word operation and select as Tx to peripheral */
    DMA0CON=0x2020; 
    /* setup the address of the peripheral ECAN1 (C1TXD) */ 
	DMA0PAD=0x0442;
	/* Set the data block transfer size of 8 */
 	DMA0CNT=7;
 	/* automatic DMA Tx initiation by DMA request */
	DMA0REQ=0x0046;	
	/* DPSRAM atart adddress offset value */ 
	DMA0STA=__builtin_dmaoffset(&ecan1msgBuf);	
	/* enable the channel */
	DMA0CONbits.CHEN=1;
	
	/* initialise the DMA channel 2 for ECAN Rx */
	/* clear the collission flags */
	DMACS0=0;
    /* setup channel 2 for peripheral indirect addressing mode 
    normal operation, word operation and select as Rx to peripheral */
    DMA2CON=0x0020;
    /* setup the address of the peripheral ECAN1 (C1RXD) */ 
	DMA2PAD=0x0440;	
 	/* Set the data block transfer size of 8 */
 	DMA2CNT=7;
 	/* automatic DMA Rx initiation by DMA request */
	DMA2REQ=0x0022;	
	/* DPSRAM atart adddress offset value */ 
	DMA2STA=__builtin_dmaoffset(&ecan1msgBuf);	
	/* enable the channel */
	DMA2CONbits.CHEN=1;
}	 
