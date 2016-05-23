// Server.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DarkSpaceServerMFC.h"
#include "ServerDlg.h"

#include "Debug/Assert.h"
#include "Standard/Library.h"
#include "File/FileDisk.h"
#include "Factory/BrokerFolder.h"
#include "Standard/Process.h"
#include "Standard/CharString.h"
#include "Standard/Types.h"
#include "Standard/Settings.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE 
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerApp

BEGIN_MESSAGE_MAP(CServerApp, CWinApp)
	//{{AFX_MSG_MAP(CServerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerApp construction

CServerApp::CServerApp() : m_IsSlave( false ), m_ServerId( 0 )
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CServerApp object

CServerApp theApp;

//----------------------------------------------------------------------------

bool CServerApp::safeDoModal( CServerDlg * pDialog )
{
#ifndef _DEBUG
	__try {
#endif
		// run the server dialog
		if ( pDialog->DoModal() != IDOK )
			return false;
		else
			return true;

#ifndef _DEBUG
		}
		__except( ProcessException( GetExceptionInformation() ) )
		{}
#endif

	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CServerApp initialization

BOOL CServerApp::InitInstance()
{
#ifdef _DEBUG
	// this stops the massive memory dumps on program exit..
	_CrtSetReportMode( _CRT_WARN, 0 );
#endif

#ifdef _DEBUG
	//new DebugReactor();
#endif

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	SetRegistryKey( _T("Palestar") );
	m_ServerKey = _T("Server");

	Event serverUpdate( "ServerUpdate" );
	if ( _tcslen( m_lpCmdLine ) > 0 )
	{
		m_IsSlave = true;
		m_ServerId = _tcstol( m_lpCmdLine, NULL, 10 );

		if ( m_ServerId > 0 )
			m_ServerKey.Format(_T("Server%d"), m_ServerId );
	}

	CharString sKey = m_ServerKey;
	Settings config( sKey );

	bool bRunServer = true;
	while( bRunServer )
	{
		// get the current directory
		m_ServerDirectory = FileDisk::currentDirectory();

		// open the data file
		BrokerFolder data;
		if (! data.open( config.get( "Data", ".\\data\\" ), false, false ) )
		{
			MessageBox( NULL, _T("Failed to open data source; please reinstall the server!"), _T("Failure"), MB_OK );
			return FALSE;
		}

		// display and run the server dialog
		CServerDlg dialog;
		bRunServer = safeDoModal( &dialog );

		if ( serverUpdate.signaled() && m_IsSlave )
			bRunServer = false;

		serverUpdate.clear();

	} // end while

	return FALSE;
}

int CServerApp::ExitInstance() 
{
	return CWinApp::ExitInstance();
}
