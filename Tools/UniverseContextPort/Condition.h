#if !defined(AFX_CONDITION_H__807F91C1_71CF_11D4_BA92_00C0DF22DE85__INCLUDED_)
#define AFX_CONDITION_H__807F91C1_71CF_11D4_BA92_00C0DF22DE85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Condition.h : header file
//

#include "Resource.h"
#include "Port.h"

/////////////////////////////////////////////////////////////////////////////
// CCondition dialog

class CCondition : public CDialog
{
// Construction
public:
	CCondition(UniverseContextPort * pDoc, CWnd* pParent = NULL);   // standard constructor

	UniverseContextPort *
			m_pDoc;
	UniverseContextPort::Condition	
			m_Condition;

	void	updateObjectives();

// Dialog Data
	//{{AFX_DATA(CCondition)
	enum { IDD = IDD_CONDITION };
	CListCtrl	m_Objectives;
	CString	m_Message;
	CString	m_NextContext;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCondition)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCondition)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnAddObjective();
	afx_msg void OnRemoveObjective();
	afx_msg void OnEditObjective();
	afx_msg void OnOpenContext();
	afx_msg void OnBrowseContext();
	afx_msg void OnEditObjectiveDC(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONDITION_H__807F91C1_71CF_11D4_BA92_00C0DF22DE85__INCLUDED_)
