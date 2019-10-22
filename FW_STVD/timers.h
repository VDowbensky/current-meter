#ifndef _TIM_H_
#define _TIM_H_

#include "global.h"

#define RXTIMEOUT 2 //tick = 20 ms

//#define PULSEDURATION 20 //5 us
#define PULSEFREQ 47500UL
#define TIM3CNT FCPU/PULSEFREQ 

extern void initTIM2(void);
extern void updateTim2(void);
extern void initTIM3(void);
extern void updateTIM3(void);
extern void sendpulse (void);

extern bit Tim2Flag;
extern bit Tim3Flag;
extern WORD Tim2Counter;

#endif
