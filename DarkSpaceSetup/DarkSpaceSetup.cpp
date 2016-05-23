// ClientSetup.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DarkSpaceSetup.h"
//#include "Settings.h"
#include "SetupSheet.h"
#include "Standard/Library.h"
#include "System/LocalizedString.h"



#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
Library		LIB_DISPLAYD3D( "DisplayD3DD.dll" );
#else
Library		LIB_DISPLAYD3D( "DisplayD3D.dll" );
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientSetupApp

BEGIN_MESSAGE_MAP(CClientSetupApp, CWinApp)
	//{{AFX_MSG_MAP(CClientSetupApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientSetupApp construction

CClientSetupApp::CClientSetupApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CClientSetupApp object

CClientSetupApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CClientSetupApp initialization

BOOL CClientSetupApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
#if 0
    // Verify that DirectX 8.0 is installed
    HINSTANCE hD3D8DLL = LoadLibrary( _T("D3D8.DLL") );
    if( hD3D8DLL == NULL )
    {
		MessageBox( NULL, _T("DirectX 8.0 is required to play DarkSpace!"), _T("DX8 not installed"), MB_OK );
        return 0;
    }
	// DX8 is installed
    FreeLibrary( hD3D8DLL );
#endif

	CSetupSheet().DoModal();
	//CSettings().DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
