

#define _LCD

#define HI(x) ((x)>>8)
#define LO(x) ((x)& 0xFF)

#define FCPU 22118400
//#define FCPU 7372800
//#define FCPU 8000000
#define REQUESTPERIOD 400 //tick = 20 ms
#define MAXERRCOUNT 30
#define IAPSTART 0xF000



#define BAUDRATE0 19200//115200
#define BAUDRATE1 115200
#define RXBUFFERSIZE 32
//#define Rx2Buffersize 128
#define TXBUFFERSIZE 128
//#define Tx2Buffersize 32
//#define RFbufferSize 16
#define EEBUFFERSIZE 32

void initIO(void) ;
void initUART(void);
//void switch_to485(void);
//void switch_to232(void);
void putstr (const char *Addr);
void putpack (BYTE c);
void clearRxBuff(void);
void rec_pref(void);
void checktuneprefix (void);
void rec_addr(void);
void rec_ident(void);
void rec_command(void);
void checktunetable(void);
void rec_param(void);
void nocommand(void);


//void clearRx2Buff(void);
void delay(WORD msec);
void initLCD(void);
void writebyte (char  ch);
void commandLCD(char comLCD);
void lcdprint(char symbol);
void lcdprintstr(char  LCDSTRING[], BYTE row);
void cls (void);
void updateLCD(void);

void initDAC(void);
void updateDAC (void);
//void compDACvalue(WORD d);

void handlecommand(void);
void checkpacket (BYTE packlen);
//void compRxCRC(void);
void comp_crc (BYTE b);
void compCRC (BYTE crcCount);
void sendpacket(BYTE txcount);
void sendOKresponse(void);
void sendERRresponse(void);
void sendrequest(void);

void ee_read(void);
void ee_pgm(void);
void readparameters(void);
bit checkEmptyID(void);
bit checkpassword(void);

void blink (BYTE b);

void initTIM2(void);
void initTIM3(void);
void updateTIM3(void);
BYTE data RxBuffer[RXBUFFERSIZE];
BYTE data TxBuffer[TXBUFFERSIZE];
BYTE tmp1;
BYTE RxStatus;
BYTE RxCount;
BYTE plen;
BYTE temp;
BYTE eeprom  PARAMS[] = {0,0,0,0,0,0,01,0x20,0x92,0,0}; //place correct structure here!!!

code const BYTE COMMANDLIST[] = {0x06,9, 0x07,9, 0x80,4, 0,0};
code const BYTE TUNELIST[] = {0x81,4, 0x82,4, 0x83,5, 0x84,4, 
0x85,6, 0x86,4, 0x87,5, 0x88,6, 0x89,5, 0x8A,4, 0x8B,5, 0x8C,
4, 0x8D,10, 0x8E,8, 0x8F,4, 0, 0};


BYTE EEbuffer[EEBUFFERSIZE];

bit TuneMode;
bit F_Comm;
bit CommError;
bit Lister;

BYTE data TxCount;
BYTE data TxPointer;
BYTE data RxPointer;

BYTE CRC;
BYTE TxCRC;
BYTE RxCRC;
BYTE Command;

bit TxBusy;
bit Tim2Flag;
WORD Tim2Counter;
BYTE ErrCounter;

WORD DACvalue;
BYTE Offset12;
BYTE Gain;
BYTE Vtune;

hbit LED = PD_ODR^6;

//LCD

hbit DE = PD_ODR^7;

hbit ERF = PB_IDR^4;

code const BYTE PASSWORD[] ="eg20";