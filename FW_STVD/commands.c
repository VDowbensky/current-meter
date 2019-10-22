#include "commands.h"
#include "uart.h"
#include "timers.h"
#include "eeprom.h"
#include "gpio.h"
#include "dac.h"
#include "global.h"
#include "ecma0896.h"


void compCRC (BYTE crcCount);
void comp_crc (BYTE b);
void checkpacket (BYTE packlen);
void handlecommand(void);

void sendpacket(BYTE txcount);
void sendOKresponse(void);
void sendERRresponse(void);

BYTE CRC;
BYTE TxCRC;
BYTE RxCRC;
bit F_Comm;
BYTE Command;
BYTE NetAddr = 1;

bit TuneMode;
bit F_Comm;
bit Lister;



//code const BYTE COMMANDLIST[] = {C_READONCE,9, C_READCONT,9, TC_ENTRY,4, 0,0};
//code const BYTE TUNELIST[] = {TC_RD_SN,4, TC_RD_CONFIG,4, TC_WR_CONFIG,5, TC_RD_OFFSET,4, 
//TC_WR_OFFSET,6, TC_RD_VTUNE,4, TC_WR_VTUNE,5, TC_WRDAC12,6, TC_WRDAC8,5, TC_RD_GAIN,4, TC_WR_GAIN,5, TC_WR_DEFAULT,
//4, TC_WR_SN,10, TC_RESET_SN,8, TC_EXIT,4, 0, 0};

code const BYTE COMMANDLIST[] = {DATA_READ,4, CAL_READ,4, CAL_SET,12, VT_READ,4, VT_SET,8, 0,0};


void compCRC (BYTE crcCount)
{  	
	BYTE i;
	CRC=0;
	for (i=0; i<(crcCount); i++)
	{
		comp_crc(TxBuffer[i]);
		TxCRC=CRC;
	}

}//CompCRC


void comp_crc (BYTE b)
{
	BYTE j;
	for (j=8; j>0; j--)
	{
		if ( (b ^ CRC) & 0x01) 
		{ 
      	CRC = ( (CRC ^ CRCPOLY) >> 1 ) | 0x80; 
    	} 
		else 
		{ 
      	CRC >>= 1; 
    	} 
    	b >>= 1;
	}
}//comp_crc

void checkpacket (BYTE packlen)
{
 	BYTE i;
	//RxCRC = 0;
	CRC=0;
	for (i=0; i<packlen; i++)	
	{
	comp_crc (RxBuffer[i]);
	RxCRC = CRC;
	}
	if (RxCRC == RxBuffer[packlen]) 
	{
	F_Comm = 1;
	Command = RxBuffer[2];
	}
	else clearRxBuff();
	
} 

void handlecommand(void)
{	 	
		F_Comm = 0;
		RxStatus = 0;
		//Command=RxBuffer[2];
		switch (Command)
		{
		case DATA_READ:
		TxBuffer[3] = LOWL(PulseCount);
		TxBuffer[4] = LOWH(PulseCount);
		TxBuffer[5] = HIGL(PulseCount);
		TxBuffer[6] = HIGH(PulseCount);
		TxBuffer[7] = LOWL(Imeas);
		TxBuffer[8] = LOWH(Imeas);
		TxBuffer[9] = HIGL(Imeas);
		TxBuffer[10] = HIGH(Imeas);
		sendpacket(11);
		break;
		
		case VT_READ:
		TxBuffer[3] = LO(Voltage);
		TxBuffer[4] = HI(Voltage);
		TxBuffer[5] = LO(Mtime);
		TxBuffer[6] = HI(Mtime);
		sendpacket(7);
		break;
		
		case VT_SET: //store and accept readed parameters
		{
		DWORD t1, t2, t3;
		ee_read();
		//EEbuffer[0] = RxBuffer[3];
		//EEbuffer[1] = RxBuffer[4];
		t1 = (RxBuffer[4]<<8)+RxBuffer[3];
		t3 = t1;
		t2 = (t1*4096)/VREF;
		if (t2 > 4095)
		{		
		t2 = 4095; //check for DAC overflow
		t1 = (t2*VREF)/4096;
		}
		else t1 = t3; //no change
		EEbuffer[0] = LO(t1);
		EEbuffer[1] = HI(t1);
		EEbuffer[2] = RxBuffer[5];
		EEbuffer[3] = RxBuffer[6];
		ee_pgm();
		readparameters();
		setWKUP();
		updateDAC(Voltage);
		//update time on LCD
		updTime(Mtime);
		sendOKresponse();
		break; 
		}
		
		case CAL_READ:
		TxBuffer[3] = LOWL(PulseNL);
		TxBuffer[4] = LOWH(PulseNL);
		TxBuffer[5] = HIGL(PulseNL);
		TxBuffer[6] = HIGH(PulseNL);
		TxBuffer[7] = LOWL(PulseNH);
		TxBuffer[8] = LOWH(PulseNH);
		TxBuffer[9] = HIGL(PulseNH);
		TxBuffer[10] = HIGH(PulseNH);
		sendpacket(11);
		break;
		
		case CAL_SET:
		//store and accept readed parameters
		ee_read();
		EEbuffer[4] = RxBuffer[3];
		EEbuffer[5] = RxBuffer[4];
		EEbuffer[6] = RxBuffer[5];
		EEbuffer[7] = RxBuffer[6];
		EEbuffer[8] = RxBuffer[7];
		EEbuffer[9] = RxBuffer[8];
		EEbuffer[10] = RxBuffer[9];
		EEbuffer[11] = RxBuffer[10];
		ee_pgm();
		readparameters();
		//temporary
		//DACvalue = Voltage;
		//updateDAC();
		sendOKresponse();
		break;
	
		
		default:
		break;	//nothing to do

		}//switch (Command)
		clearRxBuff();
		//else
} //void EDEhandle(void)
	

void sendpacket(BYTE txcount)
{
		TxBuffer[0]=TXPREFIX;
		TxBuffer[1]=NetAddr;//Net Addtess
		TxBuffer[2]=Command;
		//other fields are filled in command handler
		compCRC(txcount);
		TxBuffer[txcount]=TxCRC;
		RxPointer=0;
		putpack(txcount);
		clearRxBuff();//RxFlag=0;//command executed

}
void sendOKresponse(void)
{		TxBuffer[3]=0;
		sendpacket(4);
		//blink(2);
}

void sendERRresponse(void)
{		TxBuffer[3]=1;
		sendpacket(4);
		//blink(3);
}

