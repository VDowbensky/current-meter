#ifndef _DAC_H_
#define _DAC_H_

#include "global.h"

#define VREF 3110

extern void initDAC(void);
extern void updateDAC (WORD v);

#endif