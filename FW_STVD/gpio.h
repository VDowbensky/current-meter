#ifndef _GPIO_H_
#define _GPIO_H_

#include "global.h"



extern void initIO(void);
extern void blink (BYTE b);
extern void LED_set (BYTE s);
extern void DE_set (BYTE s);
extern void Pulse_Set (BYTE s);
extern bit CheckCmp(void);


#endif

