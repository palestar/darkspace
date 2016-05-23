// BugReportDlg.h : header file
//

#if !defined(AFX_BUGREPORTDLG_H__B93A9607_84F7_11D4_A297_00C04F6FF8BD__INCLUDED_)
#define AFX_BUGREPORTDLG_H__B93A9607_84F7_11D4_A297_00C04F6FF8BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CBugReportDlg dialog

class CBugReportDlg : public CDialog
{
// Construction
public:
	CBugReportDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CBugReportDlg)
	enum { IDD = IDD_BUGREPORT_DIALOG };
	CListCtrl	m_Attachments;
	CString	m_Description;
	CString m_sEmail;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBugReportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CBugReportDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnAddAttachment();
	afx_msg void OnRemoveAttachment();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUGREPORTDLG_H__B93A9607_84F7_11D4_A297_00C04F6FF8BD__INCLUDED_)
