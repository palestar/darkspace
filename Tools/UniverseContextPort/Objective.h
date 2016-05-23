#if !defined(AFX_OBJECTIVE_H__807F91C2_71CF_11D4_BA92_00C0DF22DE85__INCLUDED_)
#define AFX_OBJECTIVE_H__807F91C2_71CF_11D4_BA92_00C0DF22DE85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Objective.h : header file
//

#include "Resource.h"
#include "Port.h"

/////////////////////////////////////////////////////////////////////////////
// CObjective dialog

class CObjective : public CDialog
{
// Construction
public:
	CObjective(UniverseContextPort * pDoc, CWnd* pParent = NULL);   // standard constructor

	UniverseContextPort *
			m_pDoc;
	UniverseContextPort::Objective
			m_Objective;

// Dialog Data
	//{{AFX_DATA(CObjective)
	enum { IDD = IDD_OBJECTIVE };
	CComboBox	m_FactionControl;
	int		m_Type;
	CString	m_Noun;
	CString	m_Description;
	int		m_Faction;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjective)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CObjective)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJECTIVE_H__807F91C2_71CF_11D4_BA92_00C0DF22DE85__INCLUDED_)
