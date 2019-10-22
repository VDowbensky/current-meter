//---------------------------------------------------------------------------

#include <vcl.h>
#include <vcl\inifiles.hpp>
#include <Registry.hpp>
#include <math.h>
#include <stdio.h>
#include <io.h>         //для работы с файлами
#include <fcntl.h>      //для работы с файлами
#include <fstream.h>
#include <sys\stat.h>   //для работы с файлами
#include <locale.h>
#include <mmsystem.h>

#pragma hdrstop

#include "Unit_main.h"
#include "Unit_Cmd.h"
#include "Unit_Serial.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma link "Spin"
#pragma resource "*.dfm"
TForm1 *Form1;

String msg_change_language;
String msg_error_open_port;
String msg_firmware_version_wrong;
String msg_device_wrong;
String msg_update_program;

String msg_error_open_file;
String msg_error_read_file;
String msg_error_write_file;

String msg_Serial_Get_OK;
String msg_Serial_Set_OK;
String msg_Serial_Reset_OK;

String msg_Date_empty;
String msg_Date_wrong;

String msg_Sn_empty;
String msg_Sn_wrong;

String msg_Model_empty;
String msg_Model_wrong;

String msg_Temp_empty;
String msg_Temp_wrong;

String msg_hub_data_version_wrong;
String msg_hub_data_crc_wrong;
String msg_hub_data_len_wrong;
String msg_wrong_tank_volume;
String msg_tank_volume_undefinded;
String msg_liters_bigger_than_tank;

String msg_measure;
String msg_cbl_mode_off;
String msg_C1Cap_zero;
String msg_C1Code_zero;
String msg_C2Cap_zero;
String msg_C2Code_zero;
String msg_C1C2Cap_identical;
String msg_C1C2Code_identical;
String msg_RefCap_calc_wrong;

String msg_wrong_value;

String sta_device[256];
String sta_port[3];

int handle;             	//дескриптор для работы с файлом с помощью библиотеки <io.h>
AnsiString LogFileName;
char* FileUpdate;

std::ofstream out;



//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

//######################### COMPORT #########################################

//--------------------- read_comport_settings -------------------------------
int TForm1::read_comport_settings(void){
TIniFile  *IniFile;
  IniFile = new TIniFile(ExtractFilePath(Application->ExeName)+INI_FILENAME);
  EnterCriticalSection(&thr_cmd_critsect);
  thr_setup_comport     =IniFile->ReadInteger ("PORT","NUM",1);
  thr_setup_baud        =IniFile->ReadInteger ("PORT","BAUD",19200);
  thr_setup_addr        =IniFile->ReadInteger ("PORT","ADDR",1);
  LeaveCriticalSection(&thr_cmd_critsect);
  IniFile->Free();

return 0;
}

//--------------------- read_program_settings -------------------------------
int TForm1::read_program_settings(void){
TIniFile  *IniFile;
  IniFile = new TIniFile(ExtractFilePath(Application->ExeName)+INI_FILENAME);
  EnterCriticalSection(&thr_cmd_critsect);
//  Cap1     = IniFile->ReadFloat   ("Cap","C1"  ,C1_DEFAULT);
//  Cap2     = IniFile->ReadFloat   ("Cap","C2"  ,C2_DEFAULT);
  //Form1->PageControl1->Pages[PAGE_TARE]->TabVisible =
	//		 IniFile->ReadInteger   ("Program","PageTareVisible",false);
  LeaveCriticalSection(&thr_cmd_critsect);
  IniFile->Free();
return 0;
}

//--------------------- write_comport_settings ------------------------------
int TForm1::write_comport_settings(void){
TIniFile  *IniFile;
  IniFile = new TIniFile(ExtractFilePath(Application->ExeName)+INI_FILENAME);
  EnterCriticalSection(&thr_cmd_critsect);
  IniFile->WriteInteger("PORT","NUM" ,thr_setup_comport);
  IniFile->WriteInteger("PORT","BAUD",thr_setup_baud);
  IniFile->WriteInteger("PORT","ADDR",thr_setup_addr);
  LeaveCriticalSection(&thr_cmd_critsect);
  IniFile->Free();

return 0;
}
//---------------------------------------------------------------------------

//--------------------- write_program_settings ------------------------------
int TForm1::write_program_settings(void){
TIniFile  *IniFile;
  IniFile = new TIniFile(ExtractFilePath(Application->ExeName)+INI_FILENAME);
  //IniFile->WriteInteger("Program","GValue",StrToInt(EditGValue->Text));
  IniFile->Free();

return 0;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

//--------------------- fill_comports ---------------------------------------
void TForm1::fill_comports(void){

TRegistry   * Reg;
TStringList * List;
int x;
String comx;

  CmbCom->Clear();
  Reg  = new TRegistry;
  List = new TStringList;
  Reg->RootKey=HKEY_LOCAL_MACHINE;
  Reg->OpenKeyReadOnly("HARDWARE\\DEVICEMAP\\SERIALCOMM");
  Reg->GetValueNames(List);
	for (x=0;x<List->Count;x++){
	  if (Reg->ReadString(List->Strings[x]).SetLength(3)=="COM")
	  CmbCom->Items->Add(Reg->ReadString(List->Strings[x]));
	}

  List->Free();
  Reg->CloseKey();
  Reg->Free();

return;
}

//---------------------------------------------------------------------------



//------------------------- set_thr_cmd -------------------------------------
void TForm1::set_thr_cmd(DWORD cmd)
{
  EnterCriticalSection(&thr_cmd_critsect);
  timeKillEvent(TimerID);
  thr_cmd_job=cmd;
  LeaveCriticalSection(&thr_cmd_critsect);
  SetEvent(thr_cmd_event);
}
//---------------------------------------------------------
void __fastcall TForm1::FormActivate(TObject *Sender)
{


return;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormDestroy(TObject *Sender)
{
		 timeKillEvent(TimerID);
  Sleep(1);
  CmdThread->Terminate();
  CloseHandle(thr_cmd_event);
  DeleteCriticalSection(&thr_cmd_critsect);
  serial_uninit();
  //close log file if it's open
     if (handle != 0)
   {
        FileUpdate = "Log closed.\r\n";
		write(handle, FileUpdate, strlen(FileUpdate));
		close(handle);
		handle = 0;
   }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::btnOpenPortClick(TObject *Sender)
{
	set_thr_cmd (THR_CMD_OPEN);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::btnStartClick(TObject *Sender)
{
	set_thr_cmd(THR_CMD_STARTMEAS);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnClosePortClick(TObject *Sender)
{
	set_thr_cmd(THR_CMD_CLOSE );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CmbComChange(TObject *Sender)
{
  EnterCriticalSection(&thr_cmd_critsect);
	if (CmbCom->ItemIndex==-1) {
	thr_setup_comport=0;
	return;
	}

  thr_setup_comport=StrToInt(CmbCom->Text.Delete(1,3));
  LeaveCriticalSection(&thr_cmd_critsect);
  write_comport_settings();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CmbBaudChange(TObject *Sender)
{
  EnterCriticalSection(&thr_cmd_critsect);
  thr_setup_baud=StrToInt(CmbBaud->Items->Strings[CmbBaud->ItemIndex]);
  LeaveCriticalSection(&thr_cmd_critsect);
  write_comport_settings();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormShow(TObject *Sender)
{
      	     InitializeCriticalSection(&thr_cmd_critsect);
  thr_cmd_event=CreateEvent(NULL,false,false,NULL);
  CmdThread = new TCmdThread(false);

  setlocale(LC_ALL, "");

  //fill_language();

  fill_comports();

  read_comport_settings();

  read_program_settings();

  StatusBar1->Panels->Items[0]->Text=sta_port[PORT_CLOSED];
  StatusBar1->Panels->Items[1]->Text=sta_device[ERR_OK];


  CmbCom->ItemIndex = CmbCom->Items->IndexOf("COM"+IntToStr((int)thr_setup_comport));
  CmbBaud   -> ItemIndex = CmbBaud-> Items->IndexOf(thr_setup_baud);
  //EditAddr  -> Text      = thr_setup_addr;

  //EditC1Cap->Text=FloatToStrF(Cap1,ffFixed,10,3);
  //EditC2Cap->Text=FloatToStrF(Cap2,ffFixed,10,3);

  //DpDate->Date=Now();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnStopClick(TObject *Sender)
{
	set_thr_cmd (THR_CMD_STOPMEAS);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::btnEnterCalClick(TObject *Sender)
{
	if(Form1->btnEnterCal->Caption == "Enter Calibr.")
	{
		Form1->btnEnterCal->Caption = "Exit Calibr.";
		Form1->btnClosePort->Enabled = false;
		Form1->btnStop->Enabled = false;
		Form1->btnSetCal->Enabled = true;
		Form1->btnCalLoad->Enabled = true;
		Form1->btnCalNoLoad->Enabled = true;
		set_thr_cmd(THR_CMD_GETCALIBR);
	}

	else
    {
		Form1->btnEnterCal->Caption = "Enter Calibr.";
		Form1->btnClosePort->Enabled = true;
		Form1->btnStop->Enabled = true;
		Form1->btnSetCal->Enabled = false;
		Form1->btnCalLoad->Enabled = false;
		Form1->btnCalNoLoad->Enabled = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnSetVoltageClick(TObject *Sender)
{
	//fill comm tx buffer
	Voltage = StrToInt(Form1->edtVoltage->Text);
	comm_txbuff[0] = LOWL(Voltage);
	comm_txbuff[1] = LOWH(Voltage);
	Mtime = StrToInt(Form1->edtTime->Text);
	comm_txbuff[2] = LOWL(Mtime);
	comm_txbuff[3] = LOWH(Mtime);
	set_thr_cmd(THR_CMD_SETVOLT);
}
//---------------------------------------------------------------------------



void __fastcall TForm1::btnReadVoltageClick(TObject *Sender)
{
	set_thr_cmd(THR_CMD_GETVT);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnCalNoLoadClick(TObject *Sender)
{
	Form1->edtNoload->Text =  PulseCount;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnCalLoadClick(TObject *Sender)
{
	Form1->edtLoad->Text = PulseCount;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnSetCalClick(TObject *Sender)
{
	PulseNL = StrToInt(Form1->edtNoload->Text);
	PulseNH = StrToInt(Form1->edtLoad->Text);
	comm_txbuff[0] = LOWL(PulseNL);
	comm_txbuff[1] = LOWH(PulseNL);
	comm_txbuff[2] = HIGL(PulseNL);
	comm_txbuff[3] = HIGH(PulseNL);
	comm_txbuff[4] = LOWL(PulseNH);
	comm_txbuff[5] = LOWH(PulseNH);
	comm_txbuff[6] = HIGL(PulseNH);
	comm_txbuff[7] = HIGH(PulseNH);

	set_thr_cmd(THR_CMD_SETCALIBR);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::btnStartLogClick(TObject *Sender)
{
	if(Form1->opdLog->Execute())
	{
	LogFileName = Form1->opdLog->FileName;

	handle = open(LogFileName.c_str(), O_CREAT | O_APPEND | O_BINARY | O_WRONLY, S_IREAD | S_IWRITE);
	if(handle==-1)		//если произошла ошибка открытия файла
   {
	MessageDlg("File opening error", mtError, TMsgDlgButtons() << mbOK, 0);
	return;
   }

	 Form1->btnStartLog->Enabled = false;
	 Form1->btnStopLog->Enabled = true;
	 FileUpdate =  "";
	 write(handle, FileUpdate, strlen(FileUpdate));



	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnStopLogClick(TObject *Sender)
{
   if (handle != 0)

   {
		//FileUpdate = "Log stopped.\r\n";
		//write(handle, FileUpdate, strlen(FileUpdate));
		close(handle);
		handle = 0;
   }

   Form1->btnStartLog->Enabled = true;
   Form1->btnStopLog->Enabled = false;
}
//---------------------------------------------------------------------------

