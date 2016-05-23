#if !defined(AFX_PORTVIEW_H__08CBB341_88B2_11D3_B451_00C04F6FF8BD__INCLUDED_)
#define AFX_PORTVIEW_H__08CBB341_88B2_11D3_B451_00C04F6FF8BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PortView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPortView form view

#include "Resource.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "afxwin.h"
#include "afxcmn.h"

class CPortView : public CFormView
{
protected:
	CPortView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPortView)

// Form Data
public:
	//{{AFX_DATA(CPortView)
	enum { IDD = IDD_FORMVIEW };
	CListCtrl	m_ConditionList;
	CListCtrl	m_TeamList;
	CString	m_Name;
	CString	m_Description;
	CString	m_Scenery;
	CString	m_Script;
	int		m_Type;
	float	m_TimeLimit;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPortView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPortView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPortView)
	afx_msg void OnUpdateContext();
	afx_msg void OnAddTeam();
	afx_msg void OnEditTeam();
	afx_msg void OnRemoveTeam();
	afx_msg void OnOpenScenery();
	afx_msg void OnBrowseScenery();
	afx_msg void OnRun();
	afx_msg void OnAddCondition();
	afx_msg void OnEditCondition();
	afx_msg void OnRemoveCondition();
	afx_msg void OnEditTeamDC(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditConditionDC(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOpenScript();
	afx_msg void OnBrowseScript();
	//}}AFX_MSG
	afx_msg void OnStopSever();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()
public:
	CString m_sServerStatus;

	void *		m_pServerProcess;
	time_t		m_nServerStartTime;

	CButton m_cStopButton;
	CButton m_cStartButton;
	CListCtrl m_ZoneList;
	afx_msg void OnAddZone();
	afx_msg void OnEditZone();
	afx_msg void OnRemoveZone();
	afx_msg void OnEditZoneDC(NMHDR *pNMHDR, LRESULT *pResult);
	float m_fCenterX;
	float m_fCenterY;
	float m_fCenterZ;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORTVIEW_H__08CBB341_88B2_11D3_B451_00C04F6FF8BD__INCLUDED_)
