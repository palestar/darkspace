// DarkSpaceClient.h : main header file for the CLIENT application
//

#if !defined(AFX_CLIENT_H__0888EF29_D307_11D3_BA92_00C0DF22DE85__INCLUDED_)
#define AFX_CLIENT_H__0888EF29_D307_11D3_BA92_00C0DF22DE85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "Gui3d/InterfaceContext.h"

/////////////////////////////////////////////////////////////////////////////
// CClientApp:
// See Client.cpp for the implementation of this class
//

class CClientApp : public CWinApp
{
public:
	CClientApp();

	void			RunGame( InterfaceContext * pInterface );
	int				StartGame();

	CharString		m_sAddress;
	int				m_nPort;
	unsigned int	m_SessionId;
	//bool			m_RunLobbyOnExit;
	
	bool			m_Error;
	CString			m_ErrorMessage;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CClientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENT_H__0888EF29_D307_11D3_BA92_00C0DF22DE85__INCLUDED_)
