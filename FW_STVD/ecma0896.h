#ifndef _ECMA_0896_
#define _ECMA_0896_

#include "global.h"
#include "itoa.h"

#define CONTRAST 6

extern void initLCD(void);
//extern void writebyte (char  ch);
//extern void commandLCD(char comLCD);
extern void lcdprint(char symbol);
extern void lcdprintstr(char  LCDSTRING[], BYTE row);
extern void cls (void);
extern void updateLCD(void);

extern void updTime (WORD t);
extern void updVolt (WORD t);
extern void updCnt (DWORD t);
extern void updCurr (WORD t);

extern char strbuff[];

#endif