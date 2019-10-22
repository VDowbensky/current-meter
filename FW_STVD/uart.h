#ifndef _UART_H_
#define _UART_H_

#include "global.h"

#define RXBUFFERSIZE 128
#define TXBUFFERSIZE 128

#define BDIVIDER (WORD)(FCPU/BAUDRATE)

extern void initUART(void);
extern void putpack (BYTE c);
//extern void copypacket(BYTE cnt);
extern void clearRxBuff(void);


extern BYTE data RxBuffer[RXBUFFERSIZE];
extern BYTE data TxBuffer[TXBUFFERSIZE];
extern BYTE Comm_Len;
extern BYTE RxStatus;
extern BYTE RxCount;
extern BYTE plen;
extern BYTE data TxCount;
extern BYTE data TxPointer;
extern BYTE data RxPointer;
extern bit TxBusy;

#endif
