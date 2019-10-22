/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */
#include "global.h"
#include "uart.h"
#include "commands.h"
#include "timers.h"
#include "eeprom.h"
#include "gpio.h"
#include "dac.h"
#include "rtc.h"

#ifdef _LCD
#include "ecma0896.h"
#endif

void delay(WORD msec);
void setWKUP(void);

WORD Voltage;
WORD Mtime;
DWORD PulseNL;
DWORD PulseNH;
bit Pulse;

DWORD PulseCount = 0;
DWORD PulseCounter = 0;
DWORD Imeas = 0;

void main (void)
{	
	CLK_CKDIVR = 0;
	initIO();
	
	//CLK_ICKCR |= 0x05; //HSI and LSI on
	CLK_PCKENR1 = 0b10100011;//switch clock to DAC, USART, timer 2 & timer 3
	CLK_PCKENR2 = 0x04;
	//choose and enable RTC clock
	CLK_CRTCR |= 0x04; //LSI for RTC
	//check RTC initialisation
	if (!(RTC_ISR1 & 0x10)) RTC_init();
	//RTC_init();
	
	readparameters();
	
	setWKUP();
	initTIM2();
	initTIM3();
	#ifdef _LCD
	initLCD();
	#endif
	delay(50);
	LED_set(1);
	_rim_ ();
	#ifdef _LCD
	cls();
	#endif
	initDAC();
	initUART();
	updateDAC(Voltage);
	PulseCount = 0;
	//update LCD
		#ifdef _LCD
	updVolt(Voltage);
	updTime(Mtime);
	updCnt(PulseCount);
	updCurr(0);
		#endif
	//set WKUP period


		
	while (1)
		{
			
			
			if (RxStatus == 4) checkpacket (Comm_Len);
			if (F_Comm) handlecommand();
			if (Pulse)
			{
				PulseCounter++;
				Pulse = 0;
				//Tim3Flag = 0;
				//if (!CheckCmp()) Pulse = 1;
				//else Pulse = 0;
			}
			//if (Pulse) sendpulse();
			if(Tim2Flag)
			{
				Tim2Flag = 0;
				
			}
			if (RTC_Flag) 
				{		
					RTC_Flag = 0;
					PulseCount = PulseCounter;
					PulseCounter = 0;
					updCnt(PulseCount);
					
				}		
			//add keyboard handler here - ???
		}
}



void delay(WORD msec)	
{
  WORD del = 0;
  while(msec)									// delay till msec is 0
  {
    for(del = 0; del < 1000; del++);		// 1 msec delay, m.b. corrected according to FCPU!
    msec--;										// decrement msec
  }
}
void setWKUP(void)
{
	WORD t;
	t = Mtime;
	if (t == 0) t=1; //protect
	t = t-1;
	RTC_WPR = 0xCA;
	RTC_WPR = 0x53;
	RTC_ISR1 |= 0x04; //enable WUT changing
	RTC_WPR = 0xCA;
	RTC_WPR = 0x53;
	RTC_CR2 = 0x00;//wake-up timer off, interrupt disable
	RTC_WPR = 0xCA;
	RTC_WPR = 0x53;
	RTC_WUTRH = HI(t);
	RTC_WPR = 0xCA;
	RTC_WPR = 0x53;
	RTC_WUTRL = LO(t);
	RTC_WPR = 0xCA;
	RTC_WPR = 0x53;
	RTC_CR2 = 0x44;//wake-up timer on, interrupt enable
	RTC_WPR = 0xCA;
	RTC_WPR = 0x53;
	RTC_ISR1 &= ~0x84;
	RTC_lock();
	
}
