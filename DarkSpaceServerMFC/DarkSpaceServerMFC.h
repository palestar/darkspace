// Server.h : main header file for the SERVER application
//

#if !defined(AFX_SERVER_H__392ADB63_8AB5_11D3_B451_00C04F6FF8BD__INCLUDED_)
#define AFX_SERVER_H__392ADB63_8AB5_11D3_B451_00C04F6FF8BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "Standard/Event.h"



// forward declare
class CServerDlg;

/////////////////////////////////////////////////////////////////////////////
// CServerApp:
// See Server.cpp for the implementation of this class
//

class CServerApp : public CWinApp
{
public:
	CServerApp();

	CString		m_ServerDirectory;
	CString		m_ServerKey;
	dword		m_ServerId;
	bool		m_IsSlave;

	bool		safeDoModal( CServerDlg * pDialog );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CServerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVER_H__392ADB63_8AB5_11D3_B451_00C04F6FF8BD__INCLUDED_)
