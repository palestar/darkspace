#if !defined(AFX_MASK_H__C8EB75A2_776A_11D4_BA92_00C0DF22DE85__INCLUDED_)
#define AFX_MASK_H__C8EB75A2_776A_11D4_BA92_00C0DF22DE85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Mask.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMask dialog

#include "Resource.h"

class CMask : public CDialog
{
// Construction
public:
	CMask(CWnd* pParent = NULL);   // standard constructor

	Array< CharString >
				m_Items;			// items in order of bit position
	dword		m_Mask;				// result of dialog

// Dialog Data
	//{{AFX_DATA(CMask)
	enum { IDD = IDD_MASK };
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMask)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMask)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MASK_H__C8EB75A2_776A_11D4_BA92_00C0DF22DE85__INCLUDED_)
