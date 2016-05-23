#include "afxwin.h"
#if !defined(AFX_PAGEVIDEO_H__5FD8D824_D1C8_4685_A508_1E2E5E3AB7D9__INCLUDED_)
#define AFX_PAGEVIDEO_H__5FD8D824_D1C8_4685_A508_1E2E5E3AB7D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageVideo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageVideo dialog

class CPageVideo : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageVideo)

// Construction
public:
	CPageVideo();
	~CPageVideo();

// Dialog Data
	//{{AFX_DATA(CPageVideo)
	enum { IDD = IDD_PAGE_VIDEO };
	CComboBox	m_GraphicsDetail;
	CComboBox	m_ShadowDetail;
	CComboBox	m_TextureDetail;
	CComboBox	m_DisplayModes;
	int		m_AutoConfigure;
	CString	m_MissilePercent;
	CString	m_ParticlePercent;
	CString	m_ShipPercent;
	int		m_ShipTrailLength;
	int		m_MissileTrailLength;
	int		m_ParticleDetail;
	BOOL	m_EnableSpecular;
	BOOL	m_EnableLightmaps;
	BOOL	m_WaitVB;
	BOOL	m_AlphaFonts;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageVideo)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageVideo)
	virtual BOOL OnInitDialog();
	afx_msg void OnAutoConfigure();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bWindowed;
	BOOL m_bHDR;
	int m_nBloomScale;
	CString m_sBloomScale;
	CComboBox m_cFSAA;
	CComboBox m_cAF;
	BOOL m_bSoftwareCursor;
	BOOL m_bEnableShadows;
	BOOL m_bUseFixedFunction;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEVIDEO_H__5FD8D824_D1C8_4685_A508_1E2E5E3AB7D9__INCLUDED_)
