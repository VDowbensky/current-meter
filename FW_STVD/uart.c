#include "uart.h"
#include "uart_reg.h"
#include "commands.h"
#include "gpio.h"
#include "global.h"

BYTE temp;

void initUART(void);
void putstr (const char *Addr);
void putpack (BYTE c);
void clearRxBuff(void);
void rec_pref(void);
void rec_addr(void);
void rec_command(void);
void rec_param(void);
void nocommand(void);

BYTE data RxBuffer[RXBUFFERSIZE];
BYTE data TxBuffer[TXBUFFERSIZE];
BYTE Comm_Len;
BYTE RxStatus;
BYTE RxCount;
BYTE plen;
BYTE data TxCount;
BYTE data TxPointer;
BYTE data RxPointer;
bit TxBusy;


void initUART(void)
{	
	SYSCFG_RMPCR1 = 0x2C; //remap UART lines to PC5, PC6
	USART1_BRR2 = (HI(BDIVIDER) & 0xF0) | (LO(BDIVIDER) & 0x0F);
	USART1_BRR1 = ((HI(BDIVIDER) & 0x0F)<<4) | ((LO(BDIVIDER) & 0xF0)>>4);
	USART1_CR1 = 0;
	USART1_CR2 = 0b01101100;
	USART1_CR3 = 0b00000000;
	USART1_CR4 = 0;
	USART1_CR5 = 0;
	USART1_CR4 = 0;
	USART1_CR5 = 0;
	clearRxBuff();
	TxPointer=0;
}


void putstr (const char *Addr)
{
	BYTE i=0;
	while (*Addr !=0)
		{
		TxBuffer[i]=*Addr;
		i++;
		Addr++;
		}
					
	//i++;
	TxBuffer[i]=0x0D;
	i++;
	TxBuffer[i]=0x0A;
	putpack(i);
}


void putpack (BYTE c)
{	while (TxBusy);
	TxPointer = 0;
	TxCount = c;
	//DE_set(1); //switch driver to transmit
	USART1_DR = TxBuffer[0];
	TxBusy = 1;
}



void clearRxBuff(void)

{
	RxPointer=0;
	RxBuffer[0]=0;
	F_Comm=0;
	RxStatus = 0;
}

//USART interrupts
void USART1_TX_Interrupt() interrupt 27		

	{
	 if (TC)
	 {TC=0;
	 if (TxPointer < TxCount)  	// it's end?
 		{
		TxPointer++;			// next byte
		USART1_DR = TxBuffer[TxPointer];	// put next byte from buffer
		}
	 else 
	 {
		TxCount=0;
		TxPointer=0;
		//DE_set(0);
		TxBusy=0;//transmitter free
	 }
 }

	}
void USART1_RX_Interrupt() interrupt 28	
{
		//static BYTE plen;
		
		if (RXNE)
		{
		
		RXNE=0;
		temp = USART1_DR;
		switch (RxStatus)
		{
			case 0://nothing received
			rec_pref();
			break;
			
			case 1://prefix received
			rec_addr();
			break;
			
			case 2://address received
			rec_command();
			break;
			
			case 3://command received
			rec_param();
			break;
			
			case 4://command not executed, nothing to do
			break;
			
			default:
			nocommand();
			break;
					
		}
		}
}		

void rec_pref(void)
	{
		
		
			
				if (temp == RXPREFIX)
				{
					RxBuffer[RxPointer] = temp;
					RxPointer++;
					RxStatus = 1;
				}
				
				else nocommand();
				
			
	}
			


void rec_addr(void)
{
	if ((temp == 0xFF) || (temp == NetAddr))
	{
	RxBuffer[RxPointer] = temp;
	RxPointer++;
	RxStatus = 2;
	}
	else 
			{//check prefix
				if (temp == RXPREFIX)
				{
					RxPointer = 0;
					RxBuffer[RxPointer] = temp;
					RxPointer++;
					RxStatus = 1;
				}
				
				else nocommand();
			}
	
}


void rec_command(void)
{
	
	
		BYTE i = 0;
		while ((COMMANDLIST[i] != 0))
				{
				 if (temp == COMMANDLIST[i])
				 {
				 	plen = COMMANDLIST[i+1];
					RxCount = plen-3;
					RxBuffer[RxPointer] = temp;
					RxPointer++;
					RxStatus = 3;
					break;
				 }
				 else i += 2;
				 
				}
			if 	(RxStatus != 3) 
			{
			//plen = 0;
			RxCount = 0;
			clearRxBuff(); //command not found
			}
			
	
}



void rec_param(void)
{
			RxBuffer[RxPointer] = temp;
			RxPointer++;
			RxCount--;
			if (RxCount ==0)
			{			
			RxStatus = 4; 
			Comm_Len = plen-1;
			}
			
}

void nocommand(void)
{
	F_Comm = 0;
	RxCount = 0;
	RxStatus = 0;
	clearRxBuff();
}


