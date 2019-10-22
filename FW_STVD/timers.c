#include "timers.h"
#include "dac.h"
#include "global.h"
#include "gpio.h"

void initTIM2(void);
void updateTim2(void);
void initTIM3(void);
void updateTIM3(void);
void sendpulse (void);

bit Tim2Flag;
WORD Tim2Counter;
bit Tim3Phase = 0;
bit Tim3Flag = 0;



void initTIM2(void)
{
			Tim2Counter = RXTIMEOUT;
			//compute values here according to FCPU. Tick must be equal 1 millisecond
			TIM2_PSCR = 7;//2 MHz
			TIM2_CNTRH = 0x9C;
			TIM2_CNTRL = 0x40;//50 Hz
			//TIM2_CNTRH = 0x0;
			//TIM2_CNTRL = 0x14;//100 kHz
			TIM2_ARRH = 0x9C;//0x0;
			TIM2_ARRL = 0x40;
			
			TIM2_IER = 0x01; //update interrupt enabled
			TIM2_CR1 = 0b10010001;//counter enable, countdown
		
}

void TIM2OVF_ISR() interrupt 19
{
		TIM2_SR1 &= 0xFE;
		Tim2Counter--;
		if (!Tim2Counter)
		{
		Tim2Flag = 1;
		Tim2Counter = RXTIMEOUT;
		}
}

void updateTim2(void)
{
	
}

void initTIM3(void)

{
	//prepare for one-shot mode; pulse duration - from 10 us to 1 ms (???)
			TIM3_PSCR = 0;//2 MHz
			//TIM3_CNTRH = 0x00;
			//TIM3_CNTRL = 0x64;//100kHz
			TIM3_CNTRH = HI(TIM3CNT);
			TIM3_CNTRL = LO(TIM3CNT);
			//TIM3_ARRH = 0x00;//0x0;
			//TIM3_ARRL = 0x64;
			TIM3_ARRH = HI(TIM3CNT);//0x0;
			TIM3_ARRL = LO(TIM3CNT);
			TIM3_IER = 0x01; //update interrupt enabled
			//TIM3_CR1 = 0b10010000;//counter disable, countdown
			//TIM3_CR1 = 0x90;//counter disable, countdown
			//TIM3_CR1 |= 0x09; //counter enable, one shot
			TIM3_CR1 = 0b10010001;//counter enable, countdown
			
			//Tim3Flag = 0;

	

}

void updateTIM3(void)
{
			//TIM3_CCR1H = Vtune;
			//TIM3_CCR1L = 0;
			//TIM3_CCR1H = 0;
			//TIM3_CCR1L = Vtune;
}

void TIM3OVF_ISR() interrupt 21
{
	 //pulse sent, increment counter
	 TIM3_SR1 &= ~0x01;//clear flag
	 if (Tim3Phase == 0)
	 {
		
	 if (!CheckCmp()) 
		{
			Pulse_Set(1);
			Pulse = 1;
		}
		else
		{
			Pulse_Set(0);
			Pulse = 0;
		}
		Tim3Phase = 1;
	}
	
	else 
		{
			Pulse_Set(0);
			Pulse = 0;
			Tim3Phase = 0;
		}
}

void sendpulse (void)
{
	//d - pulse duration in us
	
		//Pulse_Set(1);
		//TIM3_CR1 |= 0x09; //counter enable, one shot
		//Pulse = 0;
	
		
}
