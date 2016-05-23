; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPortView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "universecontextport.h"
LastPage=0

ClassCount=11
Class1=CChildFrame
Class2=CColorBtn
Class3=CColorBtnDlg
Class4=CCondition
Class5=CMask
Class6=CObjective
Class7=CPortView
Class8=CServerView
Class9=CShip
Class10=CTeamDialog
Class11=CTeamMask

ResourceCount=9
Resource1=IDD_CONDITION
Resource2=IDD_OBJECTIVE
Resource3=IDD_TEAMMASK
Resource4=IDD_SHIP
Resource5=IDD_FORMVIEW
Resource6=IDD_DIALOG1
Resource7=IDD_FORMVIEW1
Resource8=IDR_PORT
Resource9=IDD_MASK

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=ChildFrame.h
ImplementationFile=ChildFrame.cpp

[CLS:CColorBtn]
Type=0
BaseClass=CButton
HeaderFile=colorbtn.h
ImplementationFile=colorbtn.cpp

[CLS:CColorBtnDlg]
Type=0
BaseClass=CDialog
HeaderFile=colorbtn.h
ImplementationFile=colorbtn.cpp

[CLS:CCondition]
Type=0
BaseClass=CDialog
HeaderFile=Condition.h
ImplementationFile=Condition.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_LIST1

[CLS:CMask]
Type=0
BaseClass=CDialog
HeaderFile=Mask.h
ImplementationFile=Mask.cpp

[CLS:CObjective]
Type=0
BaseClass=CDialog
HeaderFile=Objective.h
ImplementationFile=Objective.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_COMBO2

[CLS:CPortView]
Type=0
BaseClass=CFormView
HeaderFile=PortView.h
ImplementationFile=PortView.cpp
Filter=D
VirtualFilter=VWC
LastObject=CPortView

[CLS:CServerView]
Type=0
BaseClass=CFormView
HeaderFile=ServerView.h
ImplementationFile=ServerView.cpp

[CLS:CShip]
Type=0
BaseClass=CDialog
HeaderFile=Ship.h
ImplementationFile=Ship.cpp

[CLS:CTeamDialog]
Type=0
BaseClass=CDialog
HeaderFile=TeamDialog.h
ImplementationFile=TeamDialog.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_BUTTON4

[CLS:CTeamMask]
Type=0
BaseClass=CDialog
HeaderFile=TeamMask.h
ImplementationFile=TeamMask.cpp

[DLG:IDD_COLORBTN]
Type=1
Class=CColorBtnDlg

[DLG:IDD_CONDITION]
Type=1
Class=CCondition
ControlCount=13
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_LIST1,SysListView32,1350633493
Control5=IDC_BUTTON1,button,1342242816
Control6=IDC_BUTTON2,button,1342242816
Control7=IDC_BUTTON3,button,1342242816
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT1,edit,1352728644
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDIT3,edit,1350633600
Control12=IDC_BUTTON4,button,1342242816
Control13=IDC_BUTTON5,button,1342242816

[DLG:IDD_MASK]
Type=1
Class=CMask
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST2,SysListView32,1350633499

[DLG:IDD_OBJECTIVE]
Type=1
Class=CObjective
ControlCount=11
Control1=IDC_EDIT2,edit,1350631552
Control2=IDC_COMBO1,combobox,1344339971
Control3=IDC_COMBO2,combobox,1344339971
Control4=IDC_BUTTON1,button,1342242816
Control5=IDOK,button,1342242817
Control6=IDCANCEL,button,1342242816
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT1,edit,1350633600
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352

[DLG:IDD_FORMVIEW]
Type=1
Class=CPortView
ControlCount=36
Control1=IDC_BUTTON13,button,1342242816
Control2=IDC_EDIT1,edit,1350631552
Control3=IDC_EDIT2,edit,1352728644
Control4=IDC_EDIT8,edit,1484849280
Control5=IDC_BUTTON17,button,1342242816
Control6=IDC_BUTTON18,button,1342242816
Control7=IDC_EDIT3,edit,1484849280
Control8=IDC_BUTTON1,button,1342242816
Control9=IDC_BUTTON3,button,1342242816
Control10=IDC_EDIT7,edit,1484849280
Control11=IDC_BUTTON11,button,1342242816
Control12=IDC_BUTTON12,button,1342242816
Control13=IDC_LIST1,SysListView32,1350633493
Control14=IDC_BUTTON5,button,1342242816
Control15=IDC_BUTTON6,button,1342242816
Control16=IDC_BUTTON7,button,1342242816
Control17=IDC_LIST3,SysListView32,1350633493
Control18=IDC_BUTTON14,button,1342242816
Control19=IDC_BUTTON15,button,1342242816
Control20=IDC_BUTTON16,button,1342242816
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC,static,1342308352
Control23=IDC_STATIC,static,1342308352
Control24=IDC_STATIC,static,1342308352
Control25=IDC_STATIC,static,1342308352
Control26=IDC_STATIC,static,1342308352
Control27=IDC_STATIC,static,1342308352
Control28=IDC_STATIC,static,1342308352
Control29=IDC_COMBO1,combobox,1344339971
Control30=IDC_STATIC,static,1342308352
Control31=IDC_EDIT5,edit,1350631552
Control32=IDC_EDIT6,edit,1350631552
Control33=IDC_EDIT9,edit,1350631552
Control34=IDC_STATIC,static,1342308352
Control35=IDC_EDIT10,edit,1350631552
Control36=IDC_STATIC,static,1342308352

[DLG:IDD_FORMVIEW1]
Type=1
Class=CServerView
ControlCount=1
Control1=IDC_STATUS,static,1342308865

[DLG:IDD_SHIP]
Type=1
Class=CShip
ControlCount=9
Control1=IDC_BUTTON1,button,1342242816
Control2=IDC_BUTTON2,button,1342242816
Control3=IDC_BUTTON3,button,1342242816
Control4=IDOK,button,1342242817
Control5=IDCANCEL,button,1342242816
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT1,edit,1350633600
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT3,edit,1350633600

[DLG:IDD_DIALOG1]
Type=1
Class=CTeamDialog
ControlCount=18
Control1=IDC_BUTTON1,button,1342242816
Control2=IDC_BUTTON2,button,1342242816
Control3=IDC_COLOR,button,1342242827
Control4=IDC_EDIT3,edit,1350631552
Control5=IDC_EDIT4,edit,1352732740
Control6=IDOK,button,1342242817
Control7=IDCANCEL,button,1342242816
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT2,edit,1484849280
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_COMBO1,combobox,1344339971
Control15=IDC_STATIC,static,1342308352
Control16=IDC_EDIT1,edit,1350633600
Control17=IDC_BUTTON3,button,1342242816
Control18=IDC_BUTTON4,button,1342242816

[DLG:IDD_TEAMMASK]
Type=1
Class=CTeamMask
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST2,SysListView32,1350633499

[MNU:IDR_PORT]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_SEND_MAIL
Command10=ID_FILE_MRU_FILE1
Command11=ID_APP_EXIT
Command12=ID_EDIT_UNDO
Command13=ID_EDIT_CUT
Command14=ID_EDIT_COPY
Command15=ID_EDIT_PASTE
Command16=ID_EDIT_RENAME
Command17=ID_VIEW_TOOLBAR
Command18=ID_VIEW_STATUS_BAR
Command19=ID_VIEW_OPTIONS
Command20=ID_VIEW_REFRESH
Command21=ID_VIEW_CLASSHEIRARCHY
Command22=ID_WINDOW_NEW
Command23=ID_WINDOW_CASCADE
Command24=ID_WINDOW_TILE_HORZ
Command25=ID_WINDOW_ARRANGE
Command26=ID_APP_ABOUT
CommandCount=26

