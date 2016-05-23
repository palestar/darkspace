#if !defined(AFX_CLIENTUPDATE_H__D9E4E7E1_269F_11D4_B451_00C04F6FF8BD__INCLUDED_)
#define AFX_CLIENTUPDATE_H__D9E4E7E1_269F_11D4_B451_00C04F6FF8BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientUpdate.h : header file
//

#include "Network/MirrorClient.h"
#include "Resource.h"



/////////////////////////////////////////////////////////////////////////////
// CClientUpdate dialog

class CClientUpdate : public CDialog, public MirrorClient
{
// Construction
public:
	CClientUpdate( const wchar * mirrorDirectory, 
		const wchar * mirrorServer, int port, CWnd* pParent = NULL);  
	virtual ~CClientUpdate();

	CString				m_MirrorDirectory;
	CString				m_MirrorServer;
	int					m_MirrorPort;

	volatile bool		m_Exit;

	// MirrorClient interface
	void				onStatus( const wchar * status );
	void				onError( const wchar * error );
	void				onDone();					

	// Helpers
	CString				formatBytes( dword bytes );

	class UpdateThread : public Thread	
	{
	public:
		// Construction
		UpdateThread( CClientUpdate * pClient ) : m_pClient( pClient )
		{}
		// Thread interface
		int run()
		{
			m_pClient->doUpdate();
			return 0;
		}
	private:
		CClientUpdate *	m_pClient;
	};

	void				doUpdate();
	UpdateThread *		m_pUpdateThread;


// Dialog Data
	//{{AFX_DATA(CClientUpdate)
	enum { IDD = IDD_CLIENT_UPDATE };
	CProgressCtrl	m_FileProgress;
	CProgressCtrl	m_Progress;
	CString	m_FileStatus;
	CString	m_Status;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientUpdate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CClientUpdate)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTUPDATE_H__D9E4E7E1_269F_11D4_B451_00C04F6FF8BD__INCLUDED_)
