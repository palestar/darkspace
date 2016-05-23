#if !defined(AFX_SELECTTUTORIAL_H__2ED29595_4F12_43EA_A708_C3C337FE74AF__INCLUDED_)
#define AFX_SELECTTUTORIAL_H__2ED29595_4F12_43EA_A708_C3C337FE74AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectTutorial.h : header file
//

#include "DarkSpace/GameContext.h"



/////////////////////////////////////////////////////////////////////////////
// CSelectTutorial dialog

class CSelectTutorial : public CDialog
{
// Construction
public:
	CSelectTutorial(CWnd* pParent = NULL);   // standard constructor

	Collection::Link				m_Campaign;
	WidgetKey						m_Selected;

// Dialog Data
	//{{AFX_DATA(CSelectTutorial)
	enum { IDD = IDD_SELECT_TUTORIAL };
	CComboBox	m_Context;
	CString	m_Description;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectTutorial)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectTutorial)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnUpdateDescription();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTTUTORIAL_H__2ED29595_4F12_43EA_A708_C3C337FE74AF__INCLUDED_)
