#include "gpio.h"

void initIO(void);
void blink (BYTE b);
void LED_set (BYTE s);
bit CheckCmp(void);
void DE_set (BYTE s);

hbit LED = PD_ODR^6;
hbit DE = PD_ODR^7;
hbit PULSE_PIN = PD_ODR^7;//temporary
hbit CMP = PB_IDR^0; //comparator input

hbit CALIBR = PC_ODR^4;

void initIO(void)
{	//PA0, PA1 - inputs input w/pullup - SWIM
	//PA2, PA3 - ??? - oscillator
	//PA4...PA6 - push-pull
	PA_DDR = 0b01110000;
	PA_CR1 = 0b11110011;
	PA_CR2 = 0b00000000;
	//PA_ODR = 0;
		//configure PB6 as input without pullup
		PB_DDR = 0b00000000; //
		PB_CR1 = 0b10111111;
		PB_CR2 = 0b00000000; //
	//CCR|=0b00101000;
	
	//PB_ODR = 0;
	//PC0, PC1 - open drain for I2C
	//PC2 - input w/pullup - RxD
	//PC3 - push-pull - TxD
	//PC4 - push-pull - maybe DE
	//PC5 - push-pull - maybe TxD1
	//PC6 - input w/pullup w/interrupt - maybe RxD1
	PC_DDR = 0b00111011;
	PC_CR1 = 0b11111100;
	PC_CR2 = 0b00110000;
	
	PC_ODR |= 0b00100000;
	
	//PC_ODR = 0;
	//PD0...PD3 - inputs w/pullup
	//PD4///PD7 - push-pull
	PD_DDR = 0b11110000;
	PD_CR1 = 0b11111111;
	PD_CR2 = 0b11110000; //10 MHz
	//PD_ODR = 0;
}



void blink (BYTE b)
{
		BYTE i;
		for (i=0; i<b; i++)
			{
				LED_set(0);
				delay(100);
				LED_set(1);
				delay(100);
			}
	
}

void LED_set (BYTE s)
{
	if (s == 0)LED = 0;
	else LED = 1;
}


void DE_set (BYTE s)
{
	if (s == 0)DE = 0;
	else DE = 1;
}


void Pulse_Set (BYTE s)
{
	if (s == 0) PULSE_PIN = 0;
	else PULSE_PIN = 1;
}

bit CheckCmp(void)
{
	if (CMP) return 1;
	else return 0;
}

