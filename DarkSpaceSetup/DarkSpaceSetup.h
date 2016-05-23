// ClientSetup.h : main header file for the CLIENTSETUP application
//

#if !defined(AFX_CLIENTSETUP_H__D47D6C65_266C_11D4_BA92_00C0DF22DE85__INCLUDED_)
#define AFX_CLIENTSETUP_H__D47D6C65_266C_11D4_BA92_00C0DF22DE85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CClientSetupApp:
// See ClientSetup.cpp for the implementation of this class
//

class CClientSetupApp : public CWinApp
{
public:
	CClientSetupApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientSetupApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CClientSetupApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSETUP_H__D47D6C65_266C_11D4_BA92_00C0DF22DE85__INCLUDED_)
