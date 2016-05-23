; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSelectTutorial
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Client.h"

ClassCount=3
Class1=CClientApp
Class2=CSelectTutorial

ResourceCount=8
Resource1=IDD_CLIENT_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_LOGIN
Class3=CProgressDlg
Resource4=IDD_PASSWORD
Resource5=IDD_SETTINGS
Resource6=IDD_SELECT_TUTORIAL
Resource7=IDD_CLIENT_UPDATE
Resource8=CG_IDD_PROGRESS

[CLS:CClientApp]
Type=0
HeaderFile=Client.h
ImplementationFile=Client.cpp
Filter=N
LastObject=CClientApp
BaseClass=CWinApp
VirtualFilter=AC

[DLG:IDD_CLIENT_DIALOG]
Type=1
Class=?
ControlCount=4
Control1=IDC_BUTTON2,button,1342242817
Control2=IDC_BUTTON5,button,1342242816
Control3=IDC_BUTTON3,button,1342242816
Control4=IDCANCEL,button,1342242816

[DLG:IDD_LOGIN]
Type=1
Class=?
ControlCount=8
Control1=IDC_EDIT1,edit,1350631552
Control2=IDC_EDIT2,edit,1350631584
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816
Control5=IDC_CHECK1,button,1342242819
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342177288

[DLG:IDD_SETTINGS]
Type=1
Class=?
ControlCount=28
Control1=IDC_CHECK1,button,1342242819
Control2=IDC_COMBO1,combobox,1344339971
Control3=IDC_COMBO2,combobox,1344339971
Control4=IDC_COMBO6,combobox,1344339971
Control5=IDC_COMBO3,combobox,1344339971
Control6=IDC_COMBO4,combobox,1344339971
Control7=IDC_COMBO5,combobox,1344339971
Control8=IDC_SLIDER2,msctls_trackbar32,1342242840
Control9=IDC_SLIDER3,msctls_trackbar32,1342242840
Control10=IDOK,button,1342242817
Control11=IDCANCEL,button,1342242816
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_SFXVOLUME,static,1342308352
Control17=IDC_MUSICVOLUME,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342177288
Control20=IDC_STATIC,static,1342177288
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC,static,1342308352
Control23=IDC_STATIC,static,1342177288
Control24=IDC_STATIC,static,1342308352
Control25=IDC_STATIC,static,1342308352
Control26=IDC_EDIT1,edit,1350631552
Control27=IDC_STATIC,static,1342308352
Control28=IDC_EDIT2,edit,1350631552

[CLS:CProgressDlg]
Type=0
HeaderFile=ProgDlg.h
ImplementationFile=ProgDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CG_IDC_PROGDLG_STATUS

[DLG:IDD_PASSWORD]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT1,edit,1350631584
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816

[DLG:IDD_CLIENT_UPDATE]
Type=1
Class=?
ControlCount=5
Control1=IDCANCEL,button,1342242816
Control2=IDC_PROGRESS1,msctls_progress32,1350565888
Control3=IDC_PROGRESS2,msctls_progress32,1350565888
Control4=IDC_FILESTATUS,static,1342308352
Control5=4003,static,1342308352

[DLG:CG_IDD_PROGRESS]
Type=1
Class=?
ControlCount=1
Control1=CG_IDC_PROGDLG_STATUS,static,1342308353

[DLG:IDD_SELECT_TUTORIAL]
Type=1
Class=CSelectTutorial
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_COMBO1,combobox,1344340227
Control5=IDC_EDIT1,edit,1352730692

[CLS:CSelectTutorial]
Type=0
HeaderFile=SelectTutorial.h
ImplementationFile=SelectTutorial.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CSelectTutorial

