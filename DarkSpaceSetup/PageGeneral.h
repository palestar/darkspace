#if !defined(AFX_PAGEGENERAL_H__C41F0E43_F6C7_477F_AAD9_076980D8A4D0__INCLUDED_)
#define AFX_PAGEGENERAL_H__C41F0E43_F6C7_477F_AAD9_076980D8A4D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageGeneral.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageGeneral dialog

class CPageGeneral : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageGeneral)

// Construction
public:
	CPageGeneral();
	~CPageGeneral();

// Dialog Data
	//{{AFX_DATA(CPageGeneral)
	enum { IDD = IDD_PAGE_GENERAL };
	CComboBox	m_BPS;
	CString	m_BPSText;
	BOOL	m_bYawControl;
	BOOL	m_bShowShieldStatus;
	BOOL	m_bShowTurnStatus;
	BOOL	m_bShowContacts;
	CString	m_FPSLimit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageGeneral)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageGeneral)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetBPS();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL m_bEnableUDP;
	BOOL m_bInvertButtons;
	BOOL m_bDisableRightActions;
	BOOL m_bForceMultCore;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEGENERAL_H__C41F0E43_F6C7_477F_AAD9_076980D8A4D0__INCLUDED_)
