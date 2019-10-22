//---------------------------------------------------------------------------

#ifndef Unit_mainH
#define Unit_mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "Unit_Serial.h"
#include "cspin.h"
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>

#include <Menus.hpp>
#include "Spin.hpp"


#define HI(x) ((x)>>8)
#define LO(x) ((x)& 0xFF)

#define HIGH(x) (x>>24)
#define HIGL(x) ((x>>16) & 0xFF)
#define LOWH(x) ((x>>8) & 0xFF)
#define LOWL(x) ((x) & 0xFF)

//---------------------------------------------------------------------------

#define LNG_FILENAME	"Ctester.lng"
#define INI_FILENAME	"Ctester.ini"
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *gbComport;
	TButton *btnOpenPort;
	TButton *btnClosePort;
	TComboBox *CmbCom;
	TComboBox *CmbBaud;
	TGroupBox *gbCalibration;
	TLabel *lblNoLoad;
	TEdit *edtNoload;
	TButton *btnCalNoLoad;
	TLabel *lblLoad;
	TEdit *edtLoad;
	TButton *btnCalLoad;
	TGroupBox *gbSettings;
	TEdit *edtTime;
	TEdit *edtVoltage;
	TButton *btnSetVoltage;
	TLabel *lblMeasTime;
	TLabel *lblVoltage;
	TSpinButton *spVoltSet;
	TSpinButton *spTimeSet;
	TGroupBox *gbMeasuring;
	TButton *btnStart;
	TEdit *edtCounts;
	TLabel *lblCounts;
	TButton *btnStop;
	TEdit *edtCurrent;
	TLabel *lblCurrent;
	TStatusBar *StatusBar1;
	TGroupBox *gbLogging;
	TButton *btnStopLog;
	TButton *btnStartLog;
	TButton *btnSetCal;
	TButton *btnEnterCal;
	TButton *btnReadVoltage;
	TOpenDialog *opdLog;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall btnOpenPortClick(TObject *Sender);
	void __fastcall btnStartClick(TObject *Sender);
	void __fastcall btnClosePortClick(TObject *Sender);
	void __fastcall CmbComChange(TObject *Sender);
	void __fastcall CmbBaudChange(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnStopClick(TObject *Sender);
	void __fastcall btnEnterCalClick(TObject *Sender);
	void __fastcall btnSetVoltageClick(TObject *Sender);
	void __fastcall btnReadVoltageClick(TObject *Sender);
	void __fastcall btnCalNoLoadClick(TObject *Sender);
	void __fastcall btnCalLoadClick(TObject *Sender);
	void __fastcall btnSetCalClick(TObject *Sender);
	void __fastcall btnStartLogClick(TObject *Sender);
	void __fastcall btnStopLogClick(TObject *Sender);
private:	// User declarations

	void fill_comports(void);
	void set_thr_cmd(DWORD cmd);

public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
	int read_comport_settings(void);
	int write_comport_settings(void);
	int read_program_settings(void);
	int write_program_settings(void);

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;

extern String sta_device[];
extern String sta_port[];
extern String msg_error_open_port;
extern String msg_device_wrong;
extern String msg_update_program;
extern String msg_firmware_version_wrong;
extern String msg_Serial_Get_OK;
extern String msg_Serial_Set_OK;
extern String msg_Serial_Reset_OK;
extern String msg_hub_data_version_wrong;
extern String msg_hub_data_crc_wrong;
extern String msg_hub_data_len_wrong;
extern String msg_wrong_tank_volume;
extern String msg_tank_volume_undefinded;
extern String msg_liters_bigger_than_tank;
extern String msg_measure;
extern String msg_cbl_mode_off;
extern String msg_C1Cap_zero;
extern String msg_C1Code_zero;
extern String msg_C2Cap_zero;
extern String msg_C2Code_zero;
extern String msg_C1C2Cap_identical;
extern String msg_C1C2Code_identical;
extern String msg_RefCap_calc_wrong;

extern int handle;
extern char* FileUpdate;
//---------------------------------------------------------------------------
#endif
