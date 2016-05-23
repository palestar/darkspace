#if !defined(AFX_SETTINGS_H__392ADB6F_8AB5_11D3_B451_00C04F6FF8BD__INCLUDED_)
#define AFX_SETTINGS_H__392ADB6F_8AB5_11D3_B451_00C04F6FF8BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Settings.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSettings dialog

#include "Resource/Collection.h"


class CSettings : public CDialog
{
// Construction
public:
	CSettings(CWnd* pParent = NULL);   // standard constructor

	Collection::Link		m_Collection; 
	Collection::Link		m_Exported; 

// Dialog Data
	//{{AFX_DATA(CSettings)
	enum { IDD = IDD_DIALOG1 };
	CComboBox	m_cMaxRank;
	CButton	m_cTutorial;
	CButton	m_cPersistant;
	CButton	m_cPublish;
	CComboBox	m_cType;
	CEdit	m_cSaveFile;
	CEdit	m_PasswordControl;
	CEdit	m_UserControl;
	CButton	m_ClanControl;
	CButton	m_BalancedControl;
	CEdit	m_AddressControl;
	CComboBox	m_Context;
	CString	m_Description;
	int		m_Port;
	int		m_MaxClients;
	BOOL	m_Balanced;
	BOOL	m_Trace;
	int		m_Type;
	CString	m_Name;
	BOOL	m_DetectAddress;
	CString	m_Address;
	BOOL	m_Clan;
	int		m_MaxRank;
	BOOL	m_Trusted;
	CString	m_User;
	CString	m_Password;
	BOOL	m_EnableLog;
	CString	m_ServerDescription;
	BOOL	m_bPublish;
	DWORD	m_ServerFlags;
	DWORD	m_ClientFlags;
	DWORD	m_ModFlags;
	CString	m_sMasterAddress;
	int		m_nMasterPort;
	BOOL	m_bTutorial;
	BOOL	m_bPersistant;
	CString	m_sSaveFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSettings)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnUpdateDescription();
	afx_msg void OnUpdateContext();
	afx_msg void OnDetectAddress();
	afx_msg void OnTrusted();
	afx_msg void OnPersistant();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int m_nMaxPlayers;
	BOOL m_bEnableTeamAI;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGS_H__392ADB6F_8AB5_11D3_B451_00C04F6FF8BD__INCLUDED_)
