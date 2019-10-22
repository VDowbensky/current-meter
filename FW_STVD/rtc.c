#include "global.h"
#include "rtc.h"

void RTC_unlock(void);
void RTC_lock(void);

void RTC_init (void);
void RTC_setDateTime(BYTE y, BYTE mo, BYTE w, BYTE d, BYTE h, BYTE m, BYTE s);
void RTC_readDateTime(void);
//void RTC_setAlarm(BYTE y, BYTE mo, BYTE w, BYTE d, BYTE h, BYTE m, BYTE s);
//void RTC_readAlarm(BYTE y, BYTE mo, BYTE w, BYTE d, BYTE h, BYTE m, BYTE s)
//void RTC_enableAlarm(BYTE s); //0 - off, !0 - on


bit RTC_Flag;


void RTC_init (void)
{
	//RTC_unlock();
	RTC_WPR = 0xCA;
	RTC_WPR = 0x53;
	RTC_ISR1 = 0x80;
	while (!(RTC_ISR1 & 0x40));//check init mode
	RTC_WPR = 0xCA;
	RTC_WPR = 0x53;
	RTC_APRER = 0x7F; //128
	RTC_WPR = 0xCA;
	RTC_WPR = 0x53;
	//RTC_SPRERH = 0;
	//RTC_SPRERL = 0xFF; //1 s at 32768 Hz
	RTC_SPRERH = 0x01;
	RTC_WPR = 0xCA;
	RTC_WPR = 0x53;
	RTC_SPRERL = 0x28; //1 s at 38000 Hz
	//set initial values (BCD format)
	RTC_WPR = 0xCA;
	RTC_WPR = 0x53;
	RTC_TR1 = 0; //seconds
	RTC_WPR = 0xCA;
	RTC_WPR = 0x53;
	RTC_TR2 = 0; //minutes
	RTC_WPR = 0xCA;
	RTC_WPR = 0x53;
	RTC_TR3 = 0x12; //hours
	RTC_WPR = 0xCA;
	RTC_WPR = 0x53;
	RTC_DR1 = 0x03;//date
	RTC_WPR = 0xCA;
	RTC_WPR = 0x53;
	RTC_DR2 = 0x60 | 0x10; //Wednesday, October
	RTC_WPR = 0xCA;
	RTC_WPR = 0x53;
	RTC_DR3 = 0x18;//2018
	RTC_WPR = 0xCA;
	RTC_WPR = 0x53;
	RTC_ISR1 |= 0x04; //enable WUT changing
	RTC_WPR = 0xCA;
	RTC_WPR = 0x53;
	RTC_WUTRH = 0;
	RTC_WPR = 0xCA;
	RTC_WPR = 0x53;
	RTC_WUTRL = 0;
	RTC_WPR = 0xCA;
	RTC_WPR = 0x53;
	RTC_CR1 = 0x04;//wake-up 1 Hz
	RTC_WPR = 0xCA;
	RTC_WPR = 0x53;
	RTC_CR2 = 0x44;//wake-up timer on, interrupt enable
	//exit init mode
	RTC_WPR = 0xCA;
	RTC_WPR = 0x53;
	RTC_ISR1 &= ~0x80;
	//RTC_lock();
}

void RTC_setDateTime(BYTE y, BYTE mo, BYTE w, BYTE d, BYTE h, BYTE m, BYTE s)
{
	RTC_unlock();
	RTC_ISR1 = 0x80;
	while(RTC_ISR1 & 0x40);//check init mode
	RTC_TR1 = s; //seconds
	RTC_TR2 = m; //minutes
	RTC_TR3 = h; //hours
	RTC_DR1 = d;//date
	RTC_DR2 = w<<5 | mo; //Wednesday, October
	RTC_DR3 = y;//2018
	//exit init mode
	RTC_ISR1 &= ~0x80;
	RTC_lock();
}

void RTC_readDateTime(void)
{
	
}

void RTC_irq() interrupt RTC_INTERRUPT
{
	//set RTC flag
	if (RTC_ISR2 & 0x04) RTC_ISR2 &= ~0x04; //clear flag
	RTC_Flag = 1;
	
}

void RTC_unlock(void)
{
	RTC_WPR = 0xCA;
	RTC_WPR = 0x53;
}
void RTC_lock(void)
{
	RTC_WPR = 0xFF;
	RTC_WPR = 0xFF;
}