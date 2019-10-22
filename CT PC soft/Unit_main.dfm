object Form1: TForm1
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'uCurrent meter v1.0.0.0'
  ClientHeight = 300
  ClientWidth = 645
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object gbComport: TGroupBox
    Left = 8
    Top = 8
    Width = 187
    Height = 129
    Caption = 'COM port '
    TabOrder = 0
    object btnOpenPort: TButton
      Left = 16
      Top = 88
      Width = 75
      Height = 25
      Caption = 'Open'
      TabOrder = 0
      OnClick = btnOpenPortClick
    end
    object btnClosePort: TButton
      Left = 97
      Top = 88
      Width = 75
      Height = 25
      Caption = 'Close'
      Enabled = False
      TabOrder = 1
      OnClick = btnClosePortClick
    end
    object CmbCom: TComboBox
      Left = 16
      Top = 32
      Width = 75
      Height = 21
      TabOrder = 2
      OnChange = CmbComChange
    end
    object CmbBaud: TComboBox
      Left = 97
      Top = 32
      Width = 75
      Height = 21
      ItemIndex = 6
      TabOrder = 3
      Text = '115200'
      OnChange = CmbBaudChange
      Items.Strings = (
        '2400'
        '4800'
        '9600'
        '19200'
        '38400'
        '57600'
        '115200')
    end
  end
  object gbCalibration: TGroupBox
    Left = 443
    Top = 8
    Width = 182
    Height = 147
    Caption = 'Calibration'
    TabOrder = 1
    object lblNoLoad: TLabel
      Left = 16
      Top = 16
      Width = 71
      Height = 13
      Caption = 'No load counts'
    end
    object lblLoad: TLabel
      Left = 16
      Top = 66
      Width = 70
      Height = 13
      Caption = 'Loaded counts'
    end
    object edtNoload: TEdit
      Left = 16
      Top = 32
      Width = 74
      Height = 21
      ReadOnly = True
      TabOrder = 0
      Text = '0'
    end
    object btnCalNoLoad: TButton
      Left = 96
      Top = 30
      Width = 75
      Height = 25
      Caption = 'Set current'
      Enabled = False
      TabOrder = 1
      OnClick = btnCalNoLoadClick
    end
    object edtLoad: TEdit
      Left = 16
      Top = 85
      Width = 74
      Height = 21
      ReadOnly = True
      TabOrder = 2
      Text = '0'
    end
    object btnCalLoad: TButton
      Left = 96
      Top = 83
      Width = 75
      Height = 25
      Caption = 'Set current'
      Enabled = False
      TabOrder = 3
      OnClick = btnCalLoadClick
    end
    object btnSetCal: TButton
      Left = 96
      Top = 112
      Width = 75
      Height = 25
      Caption = 'Save settings'
      Enabled = False
      TabOrder = 4
      OnClick = btnSetCalClick
    end
    object btnEnterCal: TButton
      Left = 15
      Top = 112
      Width = 75
      Height = 25
      Caption = 'Enter Calibr.'
      Enabled = False
      TabOrder = 5
      OnClick = btnEnterCalClick
    end
  end
  object gbSettings: TGroupBox
    Left = 217
    Top = 8
    Width = 209
    Height = 129
    Caption = 'Settings'
    TabOrder = 2
    object lblMeasTime: TLabel
      Left = 18
      Top = 66
      Width = 84
      Height = 13
      Caption = 'Measuring time, s'
    end
    object lblVoltage: TLabel
      Left = 18
      Top = 16
      Width = 57
      Height = 13
      Caption = 'Voltage, mV'
    end
    object edtTime: TEdit
      Left = 18
      Top = 87
      Width = 58
      Height = 21
      Enabled = False
      TabOrder = 0
      Text = '0'
    end
    object edtVoltage: TEdit
      Left = 19
      Top = 32
      Width = 57
      Height = 21
      Enabled = False
      TabOrder = 1
      Text = '0'
    end
    object btnSetVoltage: TButton
      Left = 108
      Top = 85
      Width = 77
      Height = 25
      Caption = 'Set'
      Enabled = False
      TabOrder = 2
      OnClick = btnSetVoltageClick
    end
    object spVoltSet: TSpinButton
      Left = 81
      Top = 28
      Width = 20
      Height = 25
      DownGlyph.Data = {
        0E010000424D0E01000000000000360000002800000009000000060000000100
        200000000000D800000000000000000000000000000000000000008080000080
        8000008080000080800000808000008080000080800000808000008080000080
        8000008080000080800000808000000000000080800000808000008080000080
        8000008080000080800000808000000000000000000000000000008080000080
        8000008080000080800000808000000000000000000000000000000000000000
        0000008080000080800000808000000000000000000000000000000000000000
        0000000000000000000000808000008080000080800000808000008080000080
        800000808000008080000080800000808000}
      Enabled = False
      TabOrder = 3
      UpGlyph.Data = {
        0E010000424D0E01000000000000360000002800000009000000060000000100
        200000000000D800000000000000000000000000000000000000008080000080
        8000008080000080800000808000008080000080800000808000008080000080
        8000000000000000000000000000000000000000000000000000000000000080
        8000008080000080800000000000000000000000000000000000000000000080
        8000008080000080800000808000008080000000000000000000000000000080
        8000008080000080800000808000008080000080800000808000000000000080
        8000008080000080800000808000008080000080800000808000008080000080
        800000808000008080000080800000808000}
    end
    object spTimeSet: TSpinButton
      Left = 82
      Top = 85
      Width = 20
      Height = 25
      DownGlyph.Data = {
        0E010000424D0E01000000000000360000002800000009000000060000000100
        200000000000D800000000000000000000000000000000000000008080000080
        8000008080000080800000808000008080000080800000808000008080000080
        8000008080000080800000808000000000000080800000808000008080000080
        8000008080000080800000808000000000000000000000000000008080000080
        8000008080000080800000808000000000000000000000000000000000000000
        0000008080000080800000808000000000000000000000000000000000000000
        0000000000000000000000808000008080000080800000808000008080000080
        800000808000008080000080800000808000}
      Enabled = False
      TabOrder = 4
      UpGlyph.Data = {
        0E010000424D0E01000000000000360000002800000009000000060000000100
        200000000000D800000000000000000000000000000000000000008080000080
        8000008080000080800000808000008080000080800000808000008080000080
        8000000000000000000000000000000000000000000000000000000000000080
        8000008080000080800000000000000000000000000000000000000000000080
        8000008080000080800000808000008080000000000000000000000000000080
        8000008080000080800000808000008080000080800000808000000000000080
        8000008080000080800000808000008080000080800000808000008080000080
        800000808000008080000080800000808000}
    end
    object btnReadVoltage: TButton
      Left = 107
      Top = 30
      Width = 77
      Height = 25
      Caption = 'Read'
      TabOrder = 5
      OnClick = btnReadVoltageClick
    end
  end
  object gbMeasuring: TGroupBox
    Left = 8
    Top = 161
    Width = 418
    Height = 105
    Caption = 'Measuring'
    TabOrder = 3
    object lblCounts: TLabel
      Left = 3
      Top = 66
      Width = 49
      Height = 19
      Caption = 'Counts'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object lblCurrent: TLabel
      Left = 169
      Top = 68
      Width = 36
      Height = 19
      Caption = 'I, uA'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object btnStart: TButton
      Left = 50
      Top = 24
      Width = 120
      Height = 25
      Caption = 'Start measuring'
      Enabled = False
      TabOrder = 0
      OnClick = btnStartClick
    end
    object edtCounts: TEdit
      Left = 58
      Top = 64
      Width = 105
      Height = 27
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      ReadOnly = True
      TabOrder = 1
      Text = '0'
    end
    object btnStop: TButton
      Left = 211
      Top = 24
      Width = 121
      Height = 25
      Caption = 'Stop measuring'
      Enabled = False
      TabOrder = 2
      OnClick = btnStopClick
    end
    object edtCurrent: TEdit
      Left = 227
      Top = 64
      Width = 84
      Height = 27
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      ReadOnly = True
      TabOrder = 3
      Text = '0'
    end
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 281
    Width = 645
    Height = 19
    Panels = <
      item
        Width = 50
      end
      item
        Width = 50
      end
      item
        Width = 50
      end>
  end
  object gbLogging: TGroupBox
    Left = 443
    Top = 161
    Width = 182
    Height = 105
    Caption = 'Logging'
    TabOrder = 5
    object btnStopLog: TButton
      Left = 96
      Top = 24
      Width = 75
      Height = 25
      Caption = 'Stop Log'
      Enabled = False
      TabOrder = 0
      OnClick = btnStopLogClick
    end
    object btnStartLog: TButton
      Left = 15
      Top = 24
      Width = 75
      Height = 25
      Caption = 'Start Log'
      Enabled = False
      TabOrder = 1
      OnClick = btnStartLogClick
    end
  end
  object opdLog: TOpenDialog
    FileName = 'ctester.log'
    Filter = 'log files|*.log|all files|*.*'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
    Left = 520
    Top = 232
  end
end
