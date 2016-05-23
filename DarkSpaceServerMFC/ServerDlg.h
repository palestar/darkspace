// ServerDlg.h : header file
//

#if !defined(AFX_SERVERDLG_H__392ADB65_8AB5_11D3_B451_00C04F6FF8BD__INCLUDED_)
#define AFX_SERVERDLG_H__392ADB65_8AB5_11D3_B451_00C04F6FF8BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DarkSpace/GameServer.h"

/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog

class FileReactor;

class CServerDlg : public CDialog
{
// Construction
public:
	CServerDlg(CWnd* pParent = NULL);	// standard constructor

	class MyGameServer : public GameServer, public Logging::Reactor
	{
	public:
		// Construction
		MyGameServer()
		{}

		//! Logging::Reactor interface
		virtual void onReport( const Logging::Report & a_Report )
		{
			if ( a_Report.m_nLevel >= LL_STATUS  )
				m_Log.push( a_Report.m_pMessage );
		}

		Queue< CharString >	m_Log;
	};

	MyGameServer 			m_Server;
	FileReactor *			m_pFileReactor;
	int						m_pLastChatMessage;
	int						m_nShutdownTime;			// how long do we warn the players before shutting down
	int						m_nRestartTime;
	bool					m_bEnableRestart;

	void					RestartServer();


// Dialog Data
	//{{AFX_DATA(CServerDlg)
	enum { IDD = IDD_SERVER_DIALOG };
	CListCtrl	m_Players;
	CListCtrl	m_Chat;
	CString	m_Clients;
	CString	m_MaxClients;
	CString	m_Received;
	CString	m_Sent;
	CString	m_Status;
	CString	m_Time;
	CString	m_Message;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnStart();
	afx_msg void OnStop();
	afx_msg void OnSettings();
	afx_msg void OnDestroy();
	afx_msg void OnSend();
	afx_msg void OnDisconnect();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERDLG_H__392ADB65_8AB5_11D3_B451_00C04F6FF8BD__INCLUDED_)
