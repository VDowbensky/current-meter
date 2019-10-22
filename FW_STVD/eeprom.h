#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "global.h"

extern BYTE eeprom  PARAMS[];
code const BYTE PASSWORD[];
#define EEBUFFERSIZE 32
extern BYTE EEbuffer[];

extern void ee_read(void);
extern void ee_pgm(void);
extern void readparameters(void);

#endif