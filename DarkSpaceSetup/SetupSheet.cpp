// SetupSheet.cpp : implementation file
//

#include "stdafx.h"
#include "DarkSpaceSetup.h"
#include "SetupSheet.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetupSheet

IMPLEMENT_DYNAMIC(CSetupSheet, CPropertySheet)

CSetupSheet::CSetupSheet() : CPropertySheet( _T("DarkSpace Setup"), NULL )
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psh.dwSize = sizeof( m_psh );
	m_psh.dwFlags |= PSP_USEHICON;
	m_psh.hIcon = m_hIcon;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;    // Lose the Apply Now button
	m_psh.dwFlags &= ~PSH_HASHELP;  // Lose the Help button

	AddPage( &m_Video );
	AddPage( &m_Audio );
	AddPage( &m_General );
}

CSetupSheet::~CSetupSheet()
{
}


BEGIN_MESSAGE_MAP(CSetupSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CSetupSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

