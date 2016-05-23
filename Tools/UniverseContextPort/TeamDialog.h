#if !defined(AFX_TEAMDIALOG_H__B9FB5B41_89FF_11D3_B451_00C04F6FF8BD__INCLUDED_)
#define AFX_TEAMDIALOG_H__B9FB5B41_89FF_11D3_B451_00C04F6FF8BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TeamDialog.h : header file
//

#include "Resource.h"
#include "colorbtn.h"
#include "Port.h"

/////////////////////////////////////////////////////////////////////////////
// CTeamDialog dialog

class CTeamDialog : public CDialog
{
// Construction
public:
	CTeamDialog( UniverseContextPort * pDoc, CWnd* pParent = NULL);   // standard constructor

	CColorBtn 			m_Color;

	UniverseContextPort *
			m_pDoc;
	UniverseContextPort::Team
			m_Team;

	void				updateShips();

// Dialog Data
	//{{AFX_DATA(CTeamDialog)
	enum { IDD = IDD_DIALOG1 };
	CComboBox	m_FactionControl;
	CString	m_Logo;
	CString	m_Name;
	CString	m_Description;
	int		m_Faction;
	CString	m_Ships;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeamDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTeamDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnOpenLogo();
	afx_msg void OnBrowseLogo();
	virtual void OnOK();
	afx_msg void OnOpenShip();
	afx_msg void OnBrowseShip();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bNoJoin;
	BOOL m_bNoSpawn;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEAMDIALOG_H__B9FB5B41_89FF_11D3_B451_00C04F6FF8BD__INCLUDED_)
