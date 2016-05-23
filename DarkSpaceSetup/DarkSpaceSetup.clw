; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPageGeneral
LastTemplate=CPropertyPage
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "clientsetup.h"
LastPage=0

ClassCount=6
Class1=CClientSetupApp
Class2=CSettings

ResourceCount=3
Class3=CSetupSheet
Resource1=IDD_PAGE_AUDIO
Resource2=IDD_PAGE_VIDEO
Class4=CPageVideo
Class5=CPageAudio
Class6=CPageGeneral
Resource3=IDD_PAGE_GENERAL

[CLS:CClientSetupApp]
Type=0
BaseClass=CWinApp
HeaderFile=ClientSetup.h
ImplementationFile=ClientSetup.cpp

[CLS:CSettings]
Type=0
BaseClass=CDialog
HeaderFile=Settings.h
ImplementationFile=Settings.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_CHECK1

[CLS:CSetupSheet]
Type=0
HeaderFile=SetupSheet.h
ImplementationFile=SetupSheet.cpp
BaseClass=CPropertySheet
Filter=W
LastObject=CSetupSheet
VirtualFilter=hWC

[DLG:IDD_PAGE_VIDEO]
Type=1
Class=CPageVideo
ControlCount=19
Control1=IDC_COMBO3,combobox,1344339971
Control2=IDC_COMBO2,combobox,1344339971
Control3=IDC_COMBO6,combobox,1344339971
Control4=IDC_CHECK2,button,1342242819
Control5=IDC_CHECK3,button,1342242819
Control6=IDC_CHECK4,button,1342242819
Control7=IDC_CHECK5,button,1342242819
Control8=IDC_SLIDER1,msctls_trackbar32,1342242840
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_SLIDER4,msctls_trackbar32,1342242840
Control16=IDC_SLIDER5,msctls_trackbar32,1342242840
Control17=IDC_SHIPTRAIL_PERCENT,static,1342308352
Control18=IDC_MISSILE_PERCENT,static,1342308352
Control19=IDC_PARTICLE_PERCENT,static,1342308352

[DLG:IDD_PAGE_AUDIO]
Type=1
Class=CPageAudio
ControlCount=15
Control1=IDC_COMBO4,combobox,1344339971
Control2=IDC_COMBO5,combobox,1344339971
Control3=IDC_COMBO7,combobox,1344339971
Control4=IDC_SLIDER2,msctls_trackbar32,1342242840
Control5=IDC_SLIDER3,msctls_trackbar32,1342242840
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_SFXVOLUME,static,1342308352
Control11=IDC_MUSICVOLUME,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_EDIT1,edit,1350633600
Control15=IDC_BUTTON1,button,1342242816

[DLG:IDD_PAGE_GENERAL]
Type=1
Class=CPageGeneral
ControlCount=9
Control1=IDC_STATIC,static,1342308352
Control2=IDC_COMBO1,combobox,1344339971
Control3=IDC_BPS_TEXT,static,1342308354
Control4=IDC_CHECK1,button,1342242819
Control5=IDC_CHECK2,button,1342242819
Control6=IDC_CHECK3,button,1342242819
Control7=IDC_CHECK4,button,1342242819
Control8=IDC_COMBO2,combobox,1344339971
Control9=IDC_STATIC,static,1342308352

[CLS:CPageVideo]
Type=0
HeaderFile=PageVideo.h
ImplementationFile=PageVideo.cpp
BaseClass=CPropertyPage
Filter=D
VirtualFilter=idWC
LastObject=CPageVideo

[CLS:CPageAudio]
Type=0
HeaderFile=PageAudio.h
ImplementationFile=PageAudio.cpp
BaseClass=CPropertyPage
Filter=D
VirtualFilter=idWC
LastObject=IDC_BUTTON1

[CLS:CPageGeneral]
Type=0
HeaderFile=PageGeneral.h
ImplementationFile=PageGeneral.cpp
BaseClass=CPropertyPage
Filter=D
VirtualFilter=idWC
LastObject=IDC_COMBO2

