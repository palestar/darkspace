// PageGeneral.cpp : implementation file
//

#include "stdafx.h"
#include "DarkSpaceSetup.h"
#include "PageGeneral.h"

#include "Standard/Settings.h"
#include "System/LocalizedString.h"




#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageGeneral property page

IMPLEMENT_DYNCREATE(CPageGeneral, CPropertyPage)

CPageGeneral::CPageGeneral() : CPropertyPage(CPageGeneral::IDD)
, m_bEnableUDP(FALSE)
, m_bInvertButtons(FALSE)
, m_bDisableRightActions(FALSE)
, m_bForceMultCore(FALSE)
{
	//{{AFX_DATA_INIT(CPageGeneral)
	m_BPSText = _T("");
	m_bYawControl = FALSE;
	m_bShowShieldStatus = FALSE;
	m_bShowTurnStatus = FALSE;
	m_bShowContacts = FALSE;
	m_FPSLimit = _T("");
	//}}AFX_DATA_INIT

	m_psp.dwFlags &= ~PSP_HASHELP;
}

CPageGeneral::~CPageGeneral()
{
}

void CPageGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageGeneral)
	DDX_Control(pDX, IDC_COMBO1, m_BPS);
	DDX_Text(pDX, IDC_BPS_TEXT, m_BPSText);
	DDX_Check(pDX, IDC_CHECK1, m_bYawControl);
	DDX_Check(pDX, IDC_CHECK2, m_bShowShieldStatus);
	DDX_Check(pDX, IDC_CHECK3, m_bShowTurnStatus);
	DDX_Check(pDX, IDC_CHECK4, m_bShowContacts);
	DDX_CBString(pDX, IDC_COMBO2, m_FPSLimit);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK5, m_bEnableUDP);
	DDX_Check(pDX, IDC_CHECK6, m_bInvertButtons);
	DDX_Check(pDX, IDC_CHECK7, m_bDisableRightActions);
	DDX_Check(pDX, IDC_CHECK8, m_bForceMultCore);
}


BEGIN_MESSAGE_MAP(CPageGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(CPageGeneral)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSetBPS)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageGeneral message handlers

BOOL CPageGeneral::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	

#ifdef _DEBUG
	Settings settings( "ClientD" );
#else
	Settings settings( "Client" );
#endif

	m_bEnableUDP = settings.get("EnableUDP", 1 );
	m_bYawControl = settings.get("YawControl", 1 );
	m_bShowShieldStatus = settings.get("ShowShieldStatus", 1 );
	m_bShowTurnStatus = settings.get("ShowTurnStatus", 1 );
	m_bShowContacts = settings.get("ShowContacts", 1 );
	m_bInvertButtons = settings.get( "invertButtons", (dword)0 );
	m_bDisableRightActions = settings.get( "disableRightActions", (dword)0 );
	m_bForceMultCore = settings.get( "ForceMultiCore", (dword)0 );

	dword nFPSLimit = settings.get("FPSLimit", (dword)0 );
	if ( nFPSLimit > 0 )
		m_FPSLimit = CharString().format("%d", nFPSLimit );
	else
		m_FPSLimit = _T("None");

	UpdateData( false );

	int bps = settings.get( "bps", (dword)0 );
	
	static const TCHAR * CONNECTION_TEXT[] = 
	{
		_T("Auto Detect"),
		_T("Modem 28.8k"),
		_T("Modem 56k"),
		_T("ISDN/DSL"),
		_T("Cable Modem/T1"),
		NULL
	};
	static int CONNECTION_BPS[] = 
	{
		0,
		3 * 1024,
		5 * 1024,
		16 * 1024,
		32 * 1024,
	};

	int i = 0;
	while( CONNECTION_TEXT[ i ] != NULL )
	{
		int item = m_BPS.AddString( CONNECTION_TEXT[ i ] );
		m_BPS.SetItemData( item, CONNECTION_BPS[ i ] );

		if ( CONNECTION_BPS[ i ] == bps )
			m_BPS.SetCurSel( item );

		i++;
	}


	OnSetBPS();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageGeneral::OnOK() 
{
	if ( UpdateData( true ) )
	{
#ifdef _DEBUG
		Settings settings( "ClientD" );
#else
		Settings settings( "Client" );
#endif
	
		settings.put( "EnableUDP", m_bEnableUDP );
		settings.put( "YawControl", m_bYawControl );
		settings.put( "ShowShieldStatus", m_bShowShieldStatus );
		settings.put( "ShowTurnStatus", m_bShowTurnStatus );
		settings.put( "ShowContacts", m_bShowContacts );
		settings.put( "bps", m_BPS.GetItemData( m_BPS.GetCurSel() ) );
		settings.put( "FPSLimit", _tcstol( m_FPSLimit, NULL, 10 ) );
		settings.put( "invertButtons", m_bInvertButtons );
		settings.put( "disableRightActions", m_bDisableRightActions );
		settings.put( "ForceMultiCore", m_bForceMultCore );

		CPropertyPage::OnOK();
	}
}

void CPageGeneral::OnSetBPS() 
{
	if ( UpdateData() )
	{
		int bps = m_BPS.GetItemData( m_BPS.GetCurSel() );

		if ( bps > 0 )
			m_BPSText.Format( _T("%u Bytes / Second"), bps );
		else
			m_BPSText = _T("Auto Detect");

		UpdateData( false );
	}
}
