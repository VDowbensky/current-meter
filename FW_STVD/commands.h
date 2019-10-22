#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include "global.h"
//RS485 commands

//_________________________________________________________
//"Senao" protocol
//#define TXHEAD_HIGH   0x6B
//#define TXHEAD_LOW	  0x23
//#define RXHEAD_HIGH   0x6C
//#define RXHEAD_LOW	  0xC9

//specific commands
#define DATA_READ		0xC0 //read counts/current value
#define CAL_READ    0xC2 //read calibration parameters
#define CAL_SET			0xC3 //set calibration parameters
#define VT_READ			0xC4 //read voltage and measuring time
#define VT_SET			0xC5 //set voltage and measuring time
//"Omnicomm" protocol

#define TXPREFIX				0x3E
#define RXPREFIX				0x31

#define CRCPOLY					0x18
#define RESP_OK					0
#define RESP_ERROR			1

//read current condition
//set output voltage
//set pulse duration - not implemented now
//set date-time - not implemented now
//set count duration
//no-load calibration
//load calibration

//extern void compCRC (BYTE crcCount);
//extern void comp_crc (BYTE b);
extern void checkpacket (BYTE packlen);
extern void handlecommand(void);
//extern void sendpacket(BYTE txcount);
//extern void sendOKresponse(void);
//extern void sendERRresponse(void);

extern BYTE CRC;
extern BYTE TxCRC;
extern BYTE RxCRC;
extern bit F_Comm;
extern BYTE Command;
extern BYTE NetAddr;



extern code const BYTE COMMANDLIST[];

#endif

