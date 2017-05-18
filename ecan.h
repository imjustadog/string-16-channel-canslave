/******************************************************************************
*                                                                             
*    Filename:          ecan.h                                                      
*    Date:              11/01/07                                                      
*    File Version:      Ver 1.0                                                      
*                                                                             
*    Author:            Jatinder Gharoo                                                      
*    Company:           Microchip Inc.                                                       
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
/*****************************************************************************/

#ifndef __ecan_H
#define __ecan_H

/* CAN Baud Rate Configuration 		*/
#define FCAN  	      	20000000 
#define BITRATE 		500000  
#define NTQ 			20		// 20 Time Quanta in a Bit Time
#define BRP_VAL			((FCAN/(2*NTQ*BITRATE))-1)

/* defines used in ecan.c */
/* ECAN message buffer length */
#define ECAN1_MSG_BUF_LENGTH 	4

/* ECAN message type identifiers */
#define CAN_MSG_DATA	0x01
#define CAN_MSG_RTR		0x02
#define CAN_FRAME_EXT	0x03
#define CAN_FRAME_STD	0x04
#define CAN_BUF_FULL	0x05
#define CAN_BUF_EMPTY	0x06

/* filter and mask defines */
/* Macro used to write filter/mask ID to Register CiRXMxSID and 
CiRXFxSID. For example to setup the filter to accept a value of 
0x123, the macro when called as CAN_FILTERMASK2REG_SID(0x123) will 
write the register space to accept message with ID ox123 
USE FOR STANDARD MESSAGES ONLY */
#define CAN_FILTERMASK2REG_SID(x) ((x & 0x07FF)<< 5)
/* the Macro will set the "MIDE" bit in CiRXMxSID */
#define CAN_SETMIDE(sid) (sid | 0x0008)
/* the macro will set the EXIDE bit in the CiRXFxSID to 
accept extended messages only */
#define CAN_FILTERXTD(sid) (sid | 0x0008)
/* the macro will clear the EXIDE bit in the CiRXFxSID to 
accept standard messages only */
#define CAN_FILTERSTD(sid) (sid & 0xFFF7)
/* Macro used to write filter/mask ID to Register CiRXMxSID, CiRXMxEID and 
CiRXFxSID, CiRXFxEID. For example to setup the filter to accept a value of 
0x123, the macro when called as CAN_FILTERMASK2REG_SID(0x123) will 
write the register space to accept message with ID 0x123 
USE FOR EXTENDED MESSAGES ONLY */
#define CAN_FILTERMASK2REG_EID0(x) (x & 0xFFFF)
#define CAN_FILTERMASK2REG_EID1(x) (((x & 0x1FFC)<< 3)|(x & 0x3))

/* DMA CAN Message Buffer Configuration */
typedef unsigned int ECAN1MSGBUF [ECAN1_MSG_BUF_LENGTH][8];
extern ECAN1MSGBUF  ecan1msgBuf __attribute__((space(dma)));

/* message structure in RAM */
typedef struct{
	/* keep track of the buffer status */
	unsigned char buffer_status;
	/* RTR message or data message */
	unsigned char message_type;
	/* frame type extended or standard */
	unsigned char frame_type;
	/* buffer being used to send and receive messages */
	unsigned char buffer;
	/* 29 bit id max of 0x1FFF FFFF 
	*  11 bit id max of 0x7FF */
	unsigned long id; 
	unsigned char data[8];	
	unsigned char data_length;
}mID;

/* function prototypes as defined in can.c */
void initECAN (void);
void sendECAN(mID *message);
void initDMAECAN(void);
void rxECAN(mID *message);
void clearRxFlags(unsigned char buffer_number);
	
#endif
