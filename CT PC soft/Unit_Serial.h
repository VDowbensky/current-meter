//---------------------------------------------------------------------------

#ifndef Unit_Serial_H
#define Unit_Serial_H
#include <Classes.hpp>


#define APP_VERSION 				0x0100






#define RX_TIMEOUT 	  500
#define TX_TIMEOUT 	  1000


//common settings
#define MAX_PACKET_LEN 150

#define ERR_OK			0
#define ERR_COMMAND		1
#define ERR_PARAM       2
#define ERR_CRC			3
#define ERR_PACKET_LEN	4
#define ERR_WRONG_ANS	5
#define ERR_NO_ANS		6
#define ERR_TRANSMITT	7
#define ERR_COMPORT		8

#define PORT_OPENED	    0
#define PORT_CLOSED		1
#define PORT_ERROR      2

#define MSG_RX		    0
#define MSG_TX          1



typedef struct{
DWORD beg;						  //end stack pointer
DWORD end;                        //begin stack pointer
DWORD len[16];					  //message len
DWORD rxtx[16];					  //0=rx,1=tx
DWORD stat[16];
byte buff[16][MAX_PACKET_LEN];	  //struct for message LParam
}TLstruct;

extern DWORD			comm_txbuff[];
extern DWORD			comm_rxbuff[];


int serial_init(DWORD COM,DWORD BAUD,HWND__ * hwnd);
int serial_uninit(void);
extern DWORD processcommand(DWORD cmd,DWORD addr,DWORD tbytes, DWORD rbytes);


//---------------------------------------------------------------------------

extern CRITICAL_SECTION   critsect;

#endif


