//---------------------------------------------------------------------------
#pragma hdrstop

#include "Unit_Main.h"
#include "Unit_Serial.h"
#include "Unit_Cmd.h"
#include <stdio.h>


//---------------------------------------------------------------------------

#pragma package(smart_init)

HANDLE           comport;          //comport handle
DCB              dcb;              //DCB
COMMTIMEOUTS     ct;               //timeouts
CRITICAL_SECTION critsect;
HANDLE           thr_hwnd;
DWORD            thr_id;
DWORD            thr_exit;     //if 1 thread exit
HANDLE	 	     thr_tx_event;
HANDLE	 	     thr_rx_event;


HWND__ *	    hwnd_msg;
TLstruct 	    Lstruct;

DWORD           thr_rx_cmd;
DWORD           thr_rx_addr;
DWORD           txcount;
DWORD			rxcount;
DWORD			thr_txbuff[MAX_PACKET_LEN];
DWORD			thr_rxbuff[MAX_PACKET_LEN];
DWORD			comm_txbuff[MAX_PACKET_LEN];
DWORD			comm_rxbuff[MAX_PACKET_LEN];

DWORD 			tmp_txbuff[MAX_PACKET_LEN];

DWORD 	        thr_rx_stat;
DWORD           thr_rx_timeout;

DWORD           thr_tx_cmd;
DWORD           thr_tx_addr;

DWORD tx_len;
DWORD bc;
DWORD x;


DWORD 	        thr_tx_stat;
DWORD 	        thr_tx_delay;
DWORD           thr_tx_timeout;


byte          rx_buf[MAX_PACKET_LEN];
byte          tx_buf[MAX_PACKET_LEN];
byte          CRC;                //here calculated CRC sum


DWORD WINAPI serial_thread(LPVOID);
int serial_uninit(void);
DWORD processcommand(DWORD cmd,DWORD addr,DWORD tbytes, DWORD rbytes);
//----------------------------- uart_init -------------------------------------

int serial_init(DWORD COM,DWORD BAUD,HWND__ * hwnd){
char init_buff[25];

serial_uninit();

//Init Critical section
InitializeCriticalSection(&critsect);


//OpenComPort
	if (comport)
	CloseHandle(comport);
  sprintf(init_buff,"//./COM%d",COM);
  comport=CreateFile(init_buff,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,NULL,NULL);
	if (comport==INVALID_HANDLE_VALUE){
	comport=0;
	return 1;
	}

//InitComport
  dcb.DCBlength     =sizeof(dcb);
  dcb.fBinary       =1;
  dcb.fParity       =1;    /* Enable parity checking          */
  dcb.fOutxCtsFlow  =0;    /* CTS handshaking on output       */
  dcb.fOutxDsrFlow  =0;    /* DSR handshaking on output       */
  dcb.fDtrControl   =0;    /* DTR Flow control                */
  dcb.fDsrSensitivity  =0; /* DSR Sensitivity                 */
  dcb.fTXContinueOnXoff=0; /* Continue TX when Xoff sent      */
  dcb.fOutX         = 0;   /* Enable output X-ON/X-OFF        */
  dcb.fInX          = 0;   /* Enable input X-ON/X-OFF         */
  dcb.fErrorChar    = 0;   /* Enable Err Replacement          */
  dcb.fNull         = 0;   /* Enable Null stripping           */
  dcb.fRtsControl   = 0;   /* Rts Flow control                */
  dcb.fAbortOnError = 0;   /* Abort all reads and writes on Error */

  dcb.BaudRate  = BAUD;
  dcb.XonLim    = 0;
  dcb.XoffLim   = 0;
  dcb.ByteSize  = 8;
  dcb.Parity    = 0;
  dcb.StopBits  = 0;
  dcb.XonChar   = 0;            // Tx and Rx XON character
  dcb.XoffChar  = 0;            // Tx and Rx XOFF character
  dcb.ErrorChar = 0;            // error replacement character
  dcb.EofChar   = 0;            // end of input character
  dcb.EvtChar   = 0;            // received event character


  SetCommState(comport,&dcb);
  SetCommMask(comport,EV_RXCHAR);

	ct.ReadIntervalTimeout         = 0xFFFFFFFF;
	ct.ReadTotalTimeoutMultiplier  = 0xFFFFFFFF;
	switch (BAUD) {
	case 2400:
	  ct.ReadTotalTimeoutConstant = 15+15;
	  break;
	case 4800:
	  ct.ReadTotalTimeoutConstant = 8+15;
	  break;
	case 9600:
	  ct.ReadTotalTimeoutConstant = 4+15;
	  break;
	case 14400:
	  ct.ReadTotalTimeoutConstant = 3+15;
	  break;
	default:
	  ct.ReadTotalTimeoutConstant = 2+15;
	  break;
	}
  ct.WriteTotalTimeoutMultiplier = 0;
  ct.WriteTotalTimeoutConstant   = 0;
  SetCommTimeouts(comport,&ct);
  thr_rx_timeout=RX_TIMEOUT+115200UL*2UL/BAUD+1500000/BAUD;
  thr_tx_timeout=TX_TIMEOUT+115200UL*2UL/BAUD+1500000/BAUD;

  PurgeComm(comport,PURGE_RXCLEAR);

//Create Thread
  thr_exit=0;
  thr_hwnd=CreateThread(0,0,&serial_thread,0,0/*CREATE_SUSPENDED*/,&thr_id);
//ResumeThread(thread_hwnd);
	if (!thr_hwnd)
	return 1;

  thr_rx_event=CreateEvent(NULL,1,0,NULL);
  thr_tx_event=CreateEvent(NULL,1,0,NULL);
  hwnd_msg=hwnd;
return 0;
}

//---------------------------- uart_uninit ------------------------------------
int serial_uninit(void)
{
DWORD x;

//Terminate thread
  thr_exit=1;
  WaitForSingleObject(thr_hwnd,2000);
  GetExitCodeThread(thr_hwnd,&x);
	if (x==STILL_ACTIVE)
	TerminateThread(&thr_hwnd,0);
  thr_hwnd=0;
  thr_id=0;

  CloseHandle(thr_rx_event);
  CloseHandle(thr_tx_event);
  thr_rx_event=0;
  thr_tx_event=0;

//Delete Critical Section
  DeleteCriticalSection(&critsect);

  CloseHandle(comport);
  comport=0;

return 0;
}

//--------------------- uart_CRC -------------------------
void uart_CRC(byte data)
{
byte x;

	for (x=0;x<8;x++){
	  if ((data^CRC)&0x01){
	  CRC = (CRC^0x18)>>1;
	  CRC |= 0x80;
	  }
	  else CRC >>= 1;
	data >>= 1;
	}

return;
}

//-------------------- calc_crc ----------------------------
byte serial_calc_crc(byte len, byte * pointer)
{
WORD x;

  CRC=0;
	for(x=0;x<len;x++)
	uart_CRC(*(pointer+x));

return CRC;
}

//------------------ rx_proc_set_event -------------------------------
void rx_proc_set_event(DWORD rx_stat,DWORD rx_cnt)
{
DWORD x;

  EnterCriticalSection(&critsect);
  thr_rx_stat=rx_stat;
  thr_tx_delay=rx_stat;

  Lstruct.len[Lstruct.end]=rx_cnt;
  Lstruct.rxtx[Lstruct.end]=MSG_RX;
  Lstruct.stat[Lstruct.end]=rx_stat;

	for (x = 0; x <= rx_cnt; x++) {
	Lstruct.buff[Lstruct.end][x]=rx_buf[x];
	}
  Lstruct.end++;
  Lstruct.end&=0x0F;
  LeaveCriticalSection(&critsect);

  PostMessage(hwnd_msg,WM_USER+1,0,(long)&Lstruct);
  SetEvent(thr_rx_event);

return;
}

//----------------------- rx_proc ------------------------------------
void rx_proc(void)
{
DWORD x;
DWORD bc;
DWORD rx_cnt;
DWORD rx_stat;
int rf_result;

	rx_cnt=0;
	rx_stat=ERR_NO_ANS;
	  while(1){//read packet while loop
	  rf_result=ReadFile(comport,&rx_buf[rx_cnt],1,&bc,NULL);
		if ((!rf_result)||(!bc)){
		  if (rx_stat==ERR_PACKET_LEN){
		  rx_proc_set_event(ERR_PACKET_LEN,rx_cnt);
		  }
		  else{
		  EnterCriticalSection(&critsect);
		  thr_tx_delay=rx_stat;					//for TX delay
		  LeaveCriticalSection(&critsect);
		  }
		return;		//on read timeout we are exit while read loop
		}

	  rx_stat=ERR_PACKET_LEN;
	  rx_cnt++;

		if (rx_cnt==MAX_PACKET_LEN) {
		rx_proc_set_event(ERR_PACKET_LEN,rx_cnt);//check for rx overflow
		return;
		}

	  ////here we are recieved next byte

	  if (rx_cnt<4){                       		 //check for CMD min lenght
	  continue;
	  }

	  else if (rx_buf[0]!=OMN_ACK){//TODO add Omnicomm
	  rx_proc_set_event(ERR_WRONG_ANS,rx_cnt);	//check for SYN byte
	  return;
	  }

		  if (rx_cnt<(rxcount+4))               			//check for lenght
		  continue;
		  if (serial_calc_crc(rx_cnt-1,rx_buf)!=rx_buf[rx_cnt-1]){//check for crc
		  rx_stat=ERR_CRC;
		  rx_proc_set_event(rx_stat,rx_cnt);
		  return;
		  }
		//serial_ans(rxcount);  //

        EnterCriticalSection(&critsect);
		thr_rx_addr=rx_buf[1];
		thr_rx_cmd =rx_buf[2];
		for(DWORD i = 0; i < rxcount; i++) thr_rxbuff[i] = rx_buf[i+3];
		LeaveCriticalSection(&critsect);

		if(rx_cnt == 4) rx_stat=rx_buf[3]; else rx_stat = ERR_OK;
		rx_proc_set_event(rx_stat,rx_cnt);
		return;





	}//read packet while loop

}


//------------------ tx_proc_set_event -------------------------------
void tx_proc_set_event(DWORD tx_stat,DWORD tx_cnt)
{
DWORD x;

  EnterCriticalSection(&critsect);
  thr_tx_stat=tx_stat;
  Lstruct.len[Lstruct.end]=tx_cnt;
  Lstruct.rxtx[Lstruct.end]=MSG_TX;
  Lstruct.stat[Lstruct.end]=tx_stat;
	for (x = 0; x <= tx_cnt; x++) {
	Lstruct.buff[Lstruct.end][x]=tx_buf[x];
	}
  Lstruct.end++;
  Lstruct.end&=0x0F;
  LeaveCriticalSection(&critsect);

  PostMessage(hwnd_msg,WM_USER+1,0,(long)&Lstruct);
  SetEvent(thr_tx_event);

return;
}


//----------------------- tx_proc ------------------------------------
void tx_proc(void)
{
DWORD tmp_tx_delay;
DWORD tmp_tx_cmd;
DWORD tmp_tx_addr;
DWORD tmp_tx_stat;
DWORD i;


int wf_result;

	EnterCriticalSection(&critsect);
	  tmp_tx_delay=thr_tx_delay;
	  tmp_tx_cmd =thr_tx_cmd;
	  tmp_tx_addr=thr_tx_addr;
	  //copy buffer
	  for(i=0; i < txcount; i++) tmp_txbuff[i] = thr_txbuff[i];

	LeaveCriticalSection(&critsect);

	  if (!tmp_tx_cmd) {					//wait for tx command
	  return;
	  }

	  if ( (tmp_tx_delay!=ERR_NO_ANS)&&(tmp_tx_delay!=ERR_WRONG_ANS) ) {		//wait for rx timeout
	  return;
	  }

	EnterCriticalSection(&critsect);
	  thr_tx_cmd=0;
	LeaveCriticalSection(&critsect);

		tx_buf[0]=OMN_SYN;
		tx_buf[1]=tmp_tx_addr;
		tx_buf[2]=tmp_tx_cmd;
		//if(txcount != 0)
		//{
		//for(i=0; i < txcount; i++) tx_buf[i+3] = tmp_txbuff[i]; //!!!
		//tx_buf[txcount+3]=serial_calc_crc(txcount+3,tx_buf);
		//}
		//else tx_buf[3] = serial_calc_crc(3,tx_buf);
		for(i=0; i < txcount; i++) tx_buf[i+3] = tmp_txbuff[i];
		tx_buf[i+3] = serial_calc_crc(txcount+3,tx_buf);
		tx_len = txcount+4;  //+4???


  tmp_tx_stat=ERR_OK;
  PurgeComm(comport,PURGE_RXCLEAR);		//clear rx if tx OK
  wf_result=WriteFile(comport,&tx_buf,tx_len,&bc,NULL);


	if ((!wf_result)||(bc!=tx_len)){
	tmp_tx_stat=ERR_TRANSMITT;
	}

  tx_proc_set_event(tmp_tx_stat,tx_len);

return;
}

//-------------------------- serial_thread ---------------------------------------
DWORD WINAPI serial_thread(LPVOID){

DWORD tmp_exit;

	while (1){//thread while loop
	EnterCriticalSection(&critsect);
	tmp_exit=thr_exit;
	LeaveCriticalSection(&critsect);
	  if (tmp_exit){
	  return 1;
	  }

	tx_proc();
	rx_proc();

  }//thread while loop
}

//------------------- uart_before_transmitt --------------------------
DWORD serial_before_transmit(void){

   if (!comport){
   Lstruct.len[Lstruct.end]=0;
   Lstruct.rxtx[Lstruct.end]=MSG_TX;
   Lstruct.stat[Lstruct.end]=ERR_COMPORT;
   Lstruct.end++;
   Lstruct.end&=0x0F;

   PostMessage(hwnd_msg,WM_USER+1,0,(long)&Lstruct);
   return ERR_COMPORT;						 //check comport opened
   }

  ResetEvent(thr_rx_event);					 //reset events
  ResetEvent(thr_tx_event);

return ERR_OK;
}

//------------------- uart_after_transmitt --------------------------
DWORD serial_after_transmit(void){
DWORD stat;

	if (WaitForSingleObject(thr_tx_event,thr_tx_timeout)!=WAIT_OBJECT_0){//wait tx complete
	EnterCriticalSection(&critsect);
	Lstruct.len[Lstruct.end]=0;
	Lstruct.rxtx[Lstruct.end]=MSG_TX;
	Lstruct.stat[Lstruct.end]=ERR_TRANSMITT;
	Lstruct.end++;
	Lstruct.end&=0x0F;
	LeaveCriticalSection(&critsect);
	PostMessage(hwnd_msg,WM_USER+1,0,(long)&Lstruct);
	return ERR_TRANSMITT;									   //if timeout return error code
	}

  EnterCriticalSection(&critsect);
  stat=thr_tx_stat;							 //check tx_state
  LeaveCriticalSection(&critsect);

	if (stat) {
	return stat;							 //if error return error code
	}

	if( WaitForSingleObject(thr_rx_event,thr_rx_timeout)!=WAIT_OBJECT_0){	//wait rx complete
	EnterCriticalSection(&critsect);
	Lstruct.len[Lstruct.end]=0;
	Lstruct.rxtx[Lstruct.end]=MSG_RX;
	Lstruct.stat[Lstruct.end]=ERR_NO_ANS;
	Lstruct.end++;
	Lstruct.end&=0x0F;
	LeaveCriticalSection(&critsect);
	PostMessage(hwnd_msg,WM_USER+1,0,(long)&Lstruct);
	return ERR_NO_ANS;										//if timeout return error code
	}

return ERR_OK;
}

//serial_transfer (processcommand)
DWORD processcommand(DWORD cmd,DWORD addr,DWORD tbytes, DWORD rbytes){
DWORD rx_cmd;
DWORD rx_addr;
DWORD ugen;
DWORD i;
DWORD stat;

	txcount = tbytes;
	rxcount = rbytes;

	ugen=serial_before_transmit();				 //prepare for transmitt command
	if (ugen)
	return ugen;

	EnterCriticalSection(&critsect);
	//thr_txbuff[0]=OMN_SYN;
	thr_tx_addr = addr;
	thr_tx_cmd = cmd;							//start_tx
	//thr_txbuff[1]=addr;
	//thr_txbuff[2]=cmd;
	for (i = 0; i < txcount; i++) thr_txbuff[i] = comm_txbuff[i];
	LeaveCriticalSection(&critsect);

	ugen=serial_after_transmit();				//transmitt and recieve answer
	if (ugen)
	return ugen;

	EnterCriticalSection(&critsect);
	rx_cmd = thr_rx_cmd;
	rx_addr= thr_rx_addr;
	  //fill working rx buffer
  //..........................
	for (i=0; i < rxcount; i++) comm_rxbuff[i] = thr_rxbuff[i];
	stat   = thr_rx_stat;
	thr_rx_cmd=0;
	LeaveCriticalSection(&critsect);

	if ( ((rx_addr!=addr)&&(addr!=255))||(rx_cmd!=cmd) ){
	return ERR_WRONG_ANS;					//check correct answer
	}

return stat;

}

void serial_ans(DWORD c) {
DWORD i;

	EnterCriticalSection(&critsect);
	thr_rx_addr=rx_buf[1];
	thr_rx_cmd =rx_buf[2];
	for(i = 0; i < c; i++) thr_rxbuff[i] = rx_buf[i+3];
	LeaveCriticalSection(&critsect);
}






