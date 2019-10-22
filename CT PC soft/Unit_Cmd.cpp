// ---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#include <stdio.h>
#include <io.h>         //הכÿ נאבמעû ס פאיכאלט
#include <fcntl.h>      //הכÿ נאבמעû ס פאיכאלט
#include <fstream.h>
#include <sys\stat.h>   //הכÿ נאבמעû ס פאיכאלט
#include <locale.h>
#include <mmsystem.h>

#pragma hdrstop

#include "Unit_Cmd.h"
#include "Unit_Serial.h"
#include "Unit_Main.h"
//#include "Unit_Tare.h"

#pragma package(smart_init)
// ---------------------------------------------------------------------------

CRITICAL_SECTION thr_cmd_critsect;
DWORD thr_cmd_job;
//DWORD thr_cmd_ku;
HANDLE thr_cmd_event;
int thr_setup_comport;
int thr_setup_baud;
int thr_setup_addr = 0;

int TimerID;

		//measuring data
DWORD PulseCount;
DWORD Current;

	//device settings
DWORD Voltage;
DWORD Mtime;

	//device calibration data
DWORD PulseNL;
DWORD PulseNH;

TCmdThread *CmdThread;

__fastcall TCmdThread::TCmdThread(bool CreateSuspended) : TThread
(CreateSuspended) {
}

// ------------------------- TimerProc -------------------------------------
void CALLBACK TimerProc(unsigned int uID, unsigned int uMsg, DWORD dwUser,
	DWORD dw1, DWORD dw2) {
	EnterCriticalSection(&thr_cmd_critsect);
	thr_cmd_job = dwUser;
	LeaveCriticalSection(&thr_cmd_critsect);
	SetEvent(thr_cmd_event);
	return;
}
// ---------------------------------------------------------------------------

// ------------------------- set_thr_cmd -------------------------------------
void TCmdThread::SetThrCmd(DWORD cmd) {
	EnterCriticalSection(&thr_cmd_critsect);
	timeKillEvent(TimerID);
	thr_cmd_job = cmd;
	LeaveCriticalSection(&thr_cmd_critsect);
	SetEvent(thr_cmd_event);
}
// ---------------------------------------------------------------------------

// ------------------------- SetThrTimer -------------------------------------
void TCmdThread::SetThrTimer(DWORD Job, DWORD Period) {
	EnterCriticalSection(&thr_cmd_critsect);
	if (thr_cmd_job == THR_CMD_NOP) {
		timeKillEvent(TimerID);
		TimerID = timeSetEvent(Period, TIMER_ACCURACY, TimerProc, Job,
			TIME_ONESHOT | TIME_KILL_SYNCHRONOUS);
	}
	LeaveCriticalSection(&thr_cmd_critsect);
}
// ---------------------------------------------------------------------------

// --------------------- StatusBarPortUpdate ---------------------------------
void __fastcall TCmdThread::StatusBarPortUpdate() {
	//Form1->StatusBar1->Panels->Items[0]->Text = sta_port[port_stat];
	return;
}
// ---------------------------------------------------------------------------

// -------------------- StatusBarDeviceUpdate ---------------------------------
void __fastcall TCmdThread::StatusBarDeviceUpdate() {
	//Form1->StatusBar1->Panels->Items[1]->Text = sta_device[cmd_stat];
	return;
}
// ---------------------------------------------------------------------------

// ----------------------- ActivateMainPage ----------------------------------
void __fastcall TCmdThread::ActivateMainPage() {


	Form1->btnClosePort->Enabled = true;
	Form1->btnOpenPort->Enabled = false;
	Form1->CmbCom->Enabled = false;
	Form1->CmbBaud->Enabled = false;
	Form1->btnStop->Enabled = true;
	Form1->btnStart->Enabled = false;
	Form1->spTimeSet->Enabled = false;
	Form1->spVoltSet->Enabled = false;
	Form1->edtTime->Enabled = false;
	Form1->edtVoltage->Enabled = false;

	Form1->btnSetCal->Enabled = false;
	Form1->btnCalLoad->Enabled = false;
	Form1->btnCalNoLoad->Enabled = false;
	Form1->btnEnterCal->Enabled = true;

	Form1->btnStartLog->Enabled = true;
	Form1->btnStopLog->Enabled = false;

	//SetThrCmd(THR_CMD_GETDATA); //for automatic requests
	//SetThrCmd(THR_CMD_GETCALIBR);
	return;
}
// ---------------------------------------------------------------------------

// ---------------------- DeactivateMainPage ---------------------------------
void __fastcall TCmdThread::DeactivateMainPage() {

	Form1->btnClosePort->Enabled = false;
	Form1->btnOpenPort->Enabled = true;
	Form1->CmbCom->Enabled = true;
	Form1->CmbBaud->Enabled = true;
	Form1->btnStop->Enabled = false;
	Form1->btnStart->Enabled = false;
	Form1->btnReadVoltage->Enabled = false;
	Form1->btnSetVoltage->Enabled = false;
	Form1->spTimeSet->Enabled = false;
	Form1->spVoltSet->Enabled = false;
	Form1->edtTime->Enabled = false;
	Form1->edtVoltage->Enabled = false;
	Form1->btnSetCal->Enabled = false;
	Form1->btnCalLoad->Enabled = false;
	Form1->btnCalNoLoad->Enabled = false;
	Form1->btnEnterCal->Enabled = false;

	Form1->btnStartLog->Enabled = false;
	Form1->btnStopLog->Enabled = false;
	SetThrCmd(THR_CMD_NOTHING);

	return;
}
// ---------------------------------------------------------------------------

// ----------------------- SetCursorDefault ----------------------------------
void __fastcall TCmdThread::SetCursorDefault() {
	Screen->Cursor = crDefault;
	return;
}
// ---------------------------------------------------------------------------

// ---------------------- SetCursorHourglass ---------------------------------
void __fastcall TCmdThread::SetCursorHourglass() {
	Screen->Cursor = crHourGlass;
	return;
}
// ---------------------------------------------------------------------------

// ----------------------- WriteComSettings ----------------------------------
void __fastcall TCmdThread::WriteComSettings() {
	Form1->write_comport_settings();
	Form1->CmbBaud->ItemIndex = Form1->CmbBaud->Items->IndexOf(thr_setup_baud);
   //	Form1->EditAddr->Text = thr_setup_addr;
	return;
}
// ---------------------------------------------------------------------------

// ############################ PROGRAM ######################################

//Open
void TCmdThread::JobOpen() {
	DWORD ans1;

	if (serial_init(thr_setup_comport, thr_setup_baud, NULL)) {
		MessageDlg(msg_error_open_port + thr_setup_comport, mtError,
			TMsgDlgButtons() << mbOK, 0);
		port_stat = PORT_ERROR;
		Synchronize(&StatusBarPortUpdate);
	}
	else {
		//processcommand(DATA_READ, thr_setup_addr, 0, 4);
		Sleep(200);
		port_stat = PORT_OPENED;
			//send command for voltage and time reading
		processcommand(VT_READ , thr_setup_addr, 0, 4);
		//fill settings edt's
		Voltage = 256*comm_rxbuff[1] + comm_rxbuff[0];
		Mtime = 256*comm_rxbuff[3] + comm_rxbuff[2];
		Form1->edtVoltage->Text = Voltage;
		Form1->edtTime->Text = Mtime;
		Synchronize(&StatusBarDeviceUpdate);

		Synchronize(&StatusBarPortUpdate);
		Synchronize(&ActivateMainPage);

		SetThrCmd(THR_CMD_STARTMEAS);

	}

	return;
}

// Close
void TCmdThread::JobClose() {

	Synchronize(&DeactivateMainPage);
	serial_uninit();

	return;
}

//GetData
bool TCmdThread::JobGetData(DWORD JobOK, DWORD JobFail) {

	cmd_stat = processcommand(DATA_READ, thr_setup_addr, 0, 8);
	Synchronize(&StatusBarDeviceUpdate);
	if (cmd_stat == ERR_NO_ANS) {
		reload_flag = 1;
		return true;
	}
	else if (cmd_stat != ERR_OK) {
		return true;
	}
	else if (reload_flag) {
		reload_flag = 0;
        SetThrCmd(JobFail);
		return false;
		}
	Synchronize(&JobGetDataUpdate);

return true;
}
//
void __fastcall TCmdThread::JobGetDataUpdate() {
	TFormatSettings FS;
	String S;
	PulseCount = 16777216*comm_rxbuff[3] + 65536*comm_rxbuff[2] + 256*comm_rxbuff[1] + comm_rxbuff[0];
	Current =  16777216*comm_rxbuff[7] + 65536*comm_rxbuff[6] + 256*comm_rxbuff[5] + comm_rxbuff[4];
	Form1->edtCounts->Text = PulseCount;
	//Form1->edtCurrent->Text = Current;
	Form1->edtCurrent->Text = ((float)PulseCount)/2;
	//add line to log file
	if(handle)
	{
	  GetLocaleFormatSettings(LC_ALL,FS);
	  FS.ShortDateFormat="dd.MM.yyyy";
	  S = TimeToStr(Now(),FS);
	  strcpy(FileUpdate, AnsiString(S).c_str());
	  //strcpy(FileUpdate, "I=");
	  strcat(FileUpdate, ", ");
	  strcat(FileUpdate, AnsiString(Form1->edtCurrent->Text).c_str());
	  strcat(FileUpdate, " uA\r\n");
	  write(handle, FileUpdate, strlen(FileUpdate));
	}

	return;
}

// GetCalibr
bool TCmdThread::JobGetCalibr(DWORD JobOK, DWORD JobFail)
{

  cmd_stat = processcommand(CAL_READ , thr_setup_addr, 0, 8);
  Synchronize(&StatusBarDeviceUpdate);

	if (cmd_stat != ERR_OK)
	{
		SetThrCmd(JobFail);
		return false;
	}

  Synchronize(&JobGetCalibrUpdate);
return true;
}

void __fastcall TCmdThread::JobGetCalibrUpdate()
{
	PulseNL =  16777216*comm_rxbuff[3] + 65536*comm_rxbuff[2] + 256*comm_rxbuff[1] + comm_rxbuff[0];
	PulseNH =  16777216*comm_rxbuff[7] + 65536*comm_rxbuff[6] + 256*comm_rxbuff[5] + comm_rxbuff[4];
	Form1->edtNoload->Text = PulseNL;
	Form1->edtLoad->Text = PulseNH;
	return;
}

bool TCmdThread::JobSetCalibr(DWORD JobOK, DWORD JobFail)
{
	cmd_stat = processcommand(CAL_SET, thr_setup_addr, 8, 1);
	Synchronize(&StatusBarDeviceUpdate);

	if (cmd_stat != ERR_OK)
	{
		SetThrCmd(JobFail);
		return false;
	}

  Synchronize(&JobSetCalibrUpdate);
  return true;
}

void __fastcall TCmdThread::JobSetCalibrUpdate()
{

}



//start measuring

bool TCmdThread::JobStartMeas(DWORD JobOK, DWORD JobFail){

		Synchronize(&JobStartMeasUpdate);
		SetThrCmd(THR_CMD_GETDATA);
		return true;

}

void __fastcall TCmdThread::JobStartMeasUpdate(){

		Form1->btnSetVoltage->Enabled = false;
		Form1->btnReadVoltage->Enabled = false;
		Form1->edtVoltage->Enabled = false;
		Form1->edtTime->Enabled = false;
		Form1->btnStart->Enabled = false;
		Form1->btnStop->Enabled = true;
		Form1->spTimeSet->Enabled = false;
		Form1->spVoltSet->Enabled = false;

		//Form1->btnSetCal->Enabled = true;
		//Form1->btnCalLoad->Enabled = true;
		//Form1->btnCalNoLoad->Enabled = true;
		Form1->btnEnterCal->Enabled = true;


}

//stop measuring

bool TCmdThread::JobStopMeas (DWORD JobOK, DWORD JobFail){

		//send command for voltage and time reading
	cmd_stat = processcommand(VT_READ , thr_setup_addr, 0, 4);
	Synchronize(&StatusBarDeviceUpdate);
	if (cmd_stat != ERR_OK) {
		SetThrCmd(JobFail);
		return false;
	}

	Synchronize(&JobStopMeasUpdate);
	SetThrCmd(JobOK);

return true;

}

void __fastcall TCmdThread::JobStopMeasUpdate() {

		Form1->btnSetVoltage->Enabled = true;
		Form1->btnReadVoltage->Enabled = true;
		Form1->edtVoltage->Enabled = true;
		Form1->edtTime->Enabled = true;
		Form1->btnStart->Enabled = true;
		Form1->btnStop->Enabled = false;
		Form1->spTimeSet->Enabled = true;
		Form1->spVoltSet->Enabled = true;
		Form1->btnEnterCal->Enabled = false;
		//fill settings edt's
		Voltage = 256*comm_rxbuff[1] + comm_rxbuff[0];
		Mtime = 256*comm_rxbuff[3] + comm_rxbuff[2];
		Form1->edtVoltage->Text = Voltage;
		Form1->edtTime->Text = Mtime;

}

bool TCmdThread::JobSetVoltage(DWORD JobOK, DWORD JobFail)
{
	 cmd_stat = processcommand(VT_SET , thr_setup_addr, 4, 1);
	Synchronize(&StatusBarDeviceUpdate);
	if (cmd_stat != ERR_OK) {
		SetThrCmd(JobFail);
		return false;
	}
	//cmd_stat = processcommand(VT_READ , thr_setup_addr, 0, 4);
	//Synchronize(&StatusBarDeviceUpdate);
	//if (cmd_stat != ERR_OK) {
	//	SetThrCmd(JobFail);
	//	return false;
	//}
	SetThrCmd(JobOK);
	return true;
}
void __fastcall TCmdThread::JobSetVoltUpdate()
{
	  Synchronize(&JobStopMeasUpdate);
}

bool TCmdThread::JobGetVT(DWORD JobOK, DWORD JobFail)
{
	cmd_stat = processcommand(VT_READ , thr_setup_addr, 0, 4);
	Synchronize(&StatusBarDeviceUpdate);
	if (cmd_stat != ERR_OK) {
		SetThrCmd(JobFail);
		return false;
	}

	Synchronize(&JobGetVTUpdate);
	SetThrCmd(JobOK);
	return true;
}

void __fastcall TCmdThread::JobGetVTUpdate()
{
      //fill settings edt's
		Voltage = 256*comm_rxbuff[1] + comm_rxbuff[0];
		Mtime = 256*comm_rxbuff[3] + comm_rxbuff[2];
		Form1->edtVoltage->Text = Voltage;
		Form1->edtTime->Text = Mtime;
}



// ------------------------- Execute -----------------------------------------
void __fastcall TCmdThread::Execute() {
	DWORD job;

	while (!Terminated) {
		// wait event
		if (WaitForSingleObject(thr_cmd_event, 10) != WAIT_OBJECT_0)
			continue;
		EnterCriticalSection(&thr_cmd_critsect);
		job = thr_cmd_job;
		thr_cmd_job = THR_CMD_NOP;
		LeaveCriticalSection(&thr_cmd_critsect);

		switch(job) {
		case THR_CMD_OPEN:
			JobOpen();
			break;
		case THR_CMD_CLOSE:
			JobClose();
			break;

		//periodical data requests
		case THR_CMD_GETDATA:
			if (JobGetData(THR_CMD_GETDATA, THR_CMD_GETDATA))
			{
				SetThrTimer(THR_CMD_GETDATA, TIMER_CMD_DATA_PERIOD);
			}
			break;
		//one request
		case THR_CMD_GETCALIBR:
			JobGetCalibr(THR_CMD_NOTHING, THR_CMD_NOTHING);
			break;

			case THR_CMD_GETVT: //get parameters then send periodical requests
			JobGetVT (THR_CMD_NOTHING, THR_CMD_NOTHING);
			break;

			case THR_CMD_SETVOLT: //set parameters, then get parameters then send periodical requests
			JobSetVoltage(THR_CMD_NOTHING, THR_CMD_NOTHING);
			break;

			case THR_CMD_SETTIME:
			break;

			case THR_CMD_SETCALIBR: //set parameters, then get parameters then send periodical requests
			JobSetCalibr(THR_CMD_NOTHING, THR_CMD_NOTHING);
			break;

			case THR_CMD_STARTMEAS: //send periodical requests
			JobStartMeas (THR_CMD_GETDATA, THR_CMD_GETDATA);
			break;

			case THR_CMD_STOPMEAS:  //stop sending periodical requests
				JobStopMeas (THR_CMD_NOTHING, THR_CMD_GETDATA);
			break;

			case THR_CMD_STARTLOG:   //open log file and start writing
			break;

			case THR_CMD_STOPLOG:    //stop writing, close log file
			break;

			default:
			break;   //nothing to do
		} // switch (job)
	} // while (1)

	timeKillEvent(TimerID);
	Sleep(1);
	return;
}


// ---------------------------------------------------------------------------


