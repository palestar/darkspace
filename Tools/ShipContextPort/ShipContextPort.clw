; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPortView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "shipcontextport.h"
LastPage=0

ClassCount=3
Class1=CChildFrame
Class2=CMask
Class3=CPortView

ResourceCount=3
Resource1=IDD_FORMVIEW
Resource2=IDR_PORT
Resource3=IDD_MASK

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=ChildFrame.h
ImplementationFile=ChildFrame.cpp

[CLS:CMask]
Type=0
BaseClass=CDialog
HeaderFile=Mask.h
ImplementationFile=Mask.cpp

[CLS:CPortView]
Type=0
BaseClass=CFormView
HeaderFile=PortView.h
ImplementationFile=PortView.cpp
Filter=D
VirtualFilter=VWC
LastObject=IDC_EDIT3

[DLG:IDD_MASK]
Type=1
Class=CMask
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST2,SysListView32,1350633499

[DLG:IDD_FORMVIEW]
Type=1
Class=CPortView
ControlCount=9
Control1=IDC_EDIT3,edit,1350633600
Control2=IDC_BUTTON2,button,1342242816
Control3=IDC_COMBO1,combobox,1344339971
Control4=IDC_EDIT4,edit,1484849280
Control5=IDC_BUTTON1,button,1342242816
Control6=IDC_BUTTON3,button,1342242816
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352

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

