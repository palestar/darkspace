#if !defined(AFX_SETUPSHEET_H__630DBB03_EDEE_40C7_9ECA_7A1E8F0D1BF7__INCLUDED_)
#define AFX_SETUPSHEET_H__630DBB03_EDEE_40C7_9ECA_7A1E8F0D1BF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetupSheet.h : header file
//

#include "PageVideo.h"
#include "PageAudio.h"
#include "PageGeneral.h"

/////////////////////////////////////////////////////////////////////////////
// CSetupSheet

class CSetupSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CSetupSheet)

// Construction
public:
	CSetupSheet();

	HICON			m_hIcon;
	CPageVideo		m_Video;
	CPageAudio		m_Audio;
	CPageGeneral	m_General;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetupSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSetupSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSetupSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETUPSHEET_H__630DBB03_EDEE_40C7_9ECA_7A1E8F0D1BF7__INCLUDED_)
