//---------------------------------------------------------------------------

#ifndef Unit_CmdH
#define Unit_CmdH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Unit_Serial.h"


#define THR_CMD_NOP					0xFFFFFFFF
#define THR_CMD_NOTHING 			0
#define THR_CMD_OPEN    			1
#define THR_CMD_CLOSE   			2

#define THR_CMD_GETDATA  			3
#define THR_CMD_GETVT	 			4
#define THR_CMD_SETVOLT	 			5
#define THR_CMD_SETTIME	 			6
#define THR_CMD_GETCALIBR 			7
#define THR_CMD_SETCALIBR 			8
#define THR_CMD_STARTMEAS 			9
#define THR_CMD_STOPMEAS 			10
#define THR_CMD_STARTLOG			11
#define THR_CMD_STOPLOG				12

#define TIMER_CMD_DATA_PERIOD	    1000
#define TIMER_CMD_CALIBR_PERIOD	    250
#define TIMER_CMD_MEASURE_PERIOD	250


#define TIMER_ACCURACY				10

#define MEASURE_CNT					8


//"Senao" protocol
#define TXHEAD_HIGH   0x6B
#define TXHEAD_LOW	  0x23
#define RXHEAD_HIGH   0x6C
#define RXHEAD_LOW	  0xC9

//specific commands
#define DATA_READ		0xC0
#define CAL_READ        0xC2
#define CAL_SET			0xC3
#define VT_READ			0xC4
#define VT_SET			0xC5
//"Omnicomm" protocol
#define OMN_SYN		  0x31
#define OMN_ACK		  0x3E

//---------------------------------------------------------------------------
class TCmdThread : public TThread
{
private:

	void __fastcall ActivateMainPage();
	void __fastcall DeactivateMainPage();
	void __fastcall SetCursorDefault();
	void __fastcall SetCursorHourglass();
	void __fastcall WriteComSettings();

	void __fastcall JobGetDataUpdate();
	void __fastcall JobGetCalibrUpdate();


	void __fastcall StatusBarPortUpdate();
	void __fastcall StatusBarDeviceUpdate();
	void __fastcall JobSetCalParUpdate();
	void __fastcall JobStartMeasUpdate();
	void __fastcall JobStopMeasUpdate();
	void __fastcall JobSetVoltUpdate();
	void __fastcall JobGetVTUpdate();
	void __fastcall JobSetCalibrUpdate();



	void JobOpen();
	void JobClose();

	bool JobGetData(DWORD JobOK,DWORD JobFail);

	bool JobGetCalibr(DWORD JobOK,DWORD JobFail);
	bool JobSetCalibr(DWORD JobOK, DWORD JobFail);
	bool JobStartMeas(DWORD JobOK, DWORD JobFail);
	bool JobStopMeas (DWORD JobOK, DWORD JobFail);
	bool JobSetVoltage(DWORD JobOK, DWORD JobFail);
	bool JobGetVT(DWORD JobOK, DWORD JobFail);



	void  SetThrCmd(DWORD cmd);
	void  SetThrTimer(DWORD Job,DWORD Period);

	//operative procedures

	DWORD cmd_stat;
	DWORD port_stat;
	bool  reload_flag;




protected:
	void __fastcall Execute();
public:
	__fastcall TCmdThread(bool CreateSuspended);



};
//---------------------------------------------------------------------------
extern CRITICAL_SECTION  thr_cmd_critsect;
extern HANDLE	 		 thr_cmd_event;
extern DWORD             thr_cmd_job;
extern int 			     thr_setup_comport;
extern int 			     thr_setup_baud;
extern int 			     thr_setup_addr;
extern int 			   	 TimerID;
extern TCmdThread		*CmdThread;

		//measuring data
extern 	DWORD PulseCount;
extern 	DWORD Current;

	//device settings
extern 	DWORD Voltage;
extern 	DWORD Mtime;

	//device calibration data
extern 	DWORD PulseNL;
extern 	DWORD PulseNH;
//---------------------------------------------------------------------------
#endif
