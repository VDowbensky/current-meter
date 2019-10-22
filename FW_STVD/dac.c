#include "dac.h"
#ifdef _LCD
#include "ecma0896.h"
#endif
#include "timers.h"


void initDAC(void);
void updateDAC (WORD v);

void initDAC(void)
{
		//enable DAC clock
		//CLK_PCKENR1 |=0x80; //PCKEN17
		//enable comparator clock (FUCK!!! It's needed for RI registers access!!!)
		CLK_PCKENR2 |=0x20;
		//map DAC to PB6
		RI_ASCR1 |= 0b00010000;
		RI_IOSR1 |= 0b00010000;
		//RI_IOSR2 |= 0b00010000;
		//RI_IOSR3 |= 0b00010000;
		//CLK_PCKENR1 |=0x80; //PCKEN17
		//CLK_PCKENR2 |=0x20;
		//DAC_CR1 = 0x03;//enable DAC, output buffer off
		DAC_CR1 = 0x01;//enable DAC, output buffer on
		//DACvalue = 1365; //1 V at 3.00 V reference
		//DAC_RDHR = 0;
		
				
		
}

void updateDAC (WORD v)
{
		
		//char sb[16];
		DWORD  dv, mv; //DAC value
		WORD tmp;
		dv = ((DWORD)v*4096)/VREF;
		if (dv > 4095)
		{		
			dv = 4095;
			mv = (dv*VREF)/4096;
		}
		else mv = v;
		tmp = (WORD)dv;
		_sim_(); //for atomary operation
		DAC_RDHRH = HI(tmp);
		DAC_RDHRL = LO(tmp);
		_rim_();
		
		//DAC_RDHR = (WORD)mv;
		#ifdef _LCD
		updVolt(mv);
		//lcdprintstr ("            ",1);
		//itoa(mv, sb);
		//strcpy (strbuff, "Uset: ");
		//strcat (strbuff,sb);
		//strcat (strbuff,"mV");
		//lcdprintstr (strbuff, 1);

		#endif
}



