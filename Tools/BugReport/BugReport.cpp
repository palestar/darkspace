// BugReport.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "BugReport.h"
#include "BugReportDlg.h"

#include "Standard/String.h"

/////////////////////////////////////////////////////////////////////////////
// CBugReportApp

BEGIN_MESSAGE_MAP(CBugReportApp, CWinApp)
	//{{AFX_MSG_MAP(CBugReportApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBugReportApp construction

CBugReportApp::CBugReportApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CBugReportApp object

CBugReportApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CBugReportApp initialization

BOOL CBugReportApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	// set the company name
	SetRegistryKey( _T("Palestar") );
	// display the dialog
	CBugReportDlg().DoModal();

	return FALSE;
}
