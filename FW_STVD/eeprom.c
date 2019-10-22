#include "eeprom.h"
#include "commands.h"
#include "dac.h"
#include "global.h"


 //voltage - 2 bytes
 //meas. time - 2 bytes
 //no-load calibration point - 2 bytes
 //loaded calibration point - 2 bytes
 
BYTE eeprom  PARAMS[] = {0xB8,0x0B, 0x05,0, 0x10,0,0,0, 0x00,0x00,0x00, 0x01};


BYTE EEbuffer[EEBUFFERSIZE];

void ee_read(void);
void ee_pgm(void);
void readparameters(void);

//EEPROM routines
//unlock eeprom
void ee_read(void)
{
		BYTE j;
		for (j=0; j<EEBUFFERSIZE; j++) EEbuffer[j] = PARAMS[j];
				
}

void ee_pgm(void)
{		
		BYTE j;
		
		//unlock eeprom
		FLASH_DUKR = 0xAE;
		FLASH_DUKR = 0x56;
		//check DUL in FLASH_IAPSR here
		while (!(FLASH_IAPSR & 0x08));
		//erase eeprom parameters area
		for (j=0; j<EEBUFFERSIZE; j++) 
		{
			while (!(FLASH_IAPSR & 0x40)); //wait for HVOFF
			PARAMS[j] = 0;
		}
		//write parameters here
		for (j=0; j<EEBUFFERSIZE; j++) 
		{
			while (!(FLASH_IAPSR & 0x40)); //wait for HVOFF
			PARAMS[j] = EEbuffer[j];
		}		
		
		//lock eeprom - clear DUL
		FLASH_IAPSR &= ~0x08;
		
		
}

void readparameters(void)
{
Voltage = ((WORD)PARAMS[1]<<8)+PARAMS[0];
Mtime =   ((WORD)PARAMS[3]<<8)+PARAMS[2];
PulseNL = ((DWORD)PARAMS[7]<<24)+((DWORD)PARAMS[6]<<16)+((DWORD)PARAMS[5]<<8)+PARAMS[4];
PulseNH = ((DWORD)PARAMS[11]<<24)+((DWORD)PARAMS[10]<<16)+((DWORD)PARAMS[9]<<8)+PARAMS[8];

}
