#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <STM8\STM8L151K6.h>
#include <intrins.h>
#include <string.h>

typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;

#define FCPU 16000000UL
#define BAUDRATE 115200UL

#define _LCD

#define HI(x) ((x)>>8)
#define LO(x) ((x)& 0xFF)

#define HIGH(x) (x>>24)
#define HIGL(x) ((x>>16) & 0xFF)
#define LOWH(x) ((x>>8) & 0xFF)
#define LOWL(x) ((x) & 0xFF)

extern void delay(WORD msec);
extern void setWKUP(void);

extern WORD Voltage;
extern WORD Mtime;
extern DWORD PulseNL;
extern DWORD PulseNH;

extern DWORD PulseCount; //pulses
extern DWORD Imeas;

extern bit Pulse;

#endif

