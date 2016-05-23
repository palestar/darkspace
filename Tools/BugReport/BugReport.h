// BugReport.h : main header file for the BUGREPORT application
//

#if !defined(AFX_BUGREPORT_H__B93A9605_84F7_11D4_A297_00C04F6FF8BD__INCLUDED_)
#define AFX_BUGREPORT_H__B93A9605_84F7_11D4_A297_00C04F6FF8BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBugReportApp:
// See BugReport.cpp for the implementation of this class
//

class CBugReportApp : public CWinApp
{
public:
	CBugReportApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBugReportApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CBugReportApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUGREPORT_H__B93A9605_84F7_11D4_A297_00C04F6FF8BD__INCLUDED_)
