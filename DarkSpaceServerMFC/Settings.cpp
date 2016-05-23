// Settings.cpp : implementation file
//

#include "stdafx.h"
#include "DarkSpaceServerMFC.h"
#include "Settings.h"

#include "DarkSpace/GameContext.h"
#include "Standard/MD5.h"
#include "Standard/Settings.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettings dialog


CSettings::CSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CSettings::IDD, pParent)
	, m_nMaxPlayers(0)
	, m_bEnableTeamAI(TRUE)
{
	//{{AFX_DATA_INIT(CSettings)
	m_Description = _T("");
	m_Port = 0;
	m_MaxClients = 0;
	m_Balanced = FALSE;
	m_Trace = FALSE;
	m_Type = -1;
	m_Name = _T("");
	m_DetectAddress = FALSE;
	m_Address = _T("");
	m_Clan = FALSE;
	m_MaxRank = 14;
	m_Trusted = FALSE;
	m_User = _T("");
	m_Password = _T("");
	m_EnableLog = FALSE;
	m_ServerDescription = _T("");
	m_bPublish = FALSE;
	m_ServerFlags = 0;
	m_ClientFlags = 0;
	m_ModFlags = 0;
	m_bTutorial = FALSE;
	m_bPersistant = FALSE;
	m_sSaveFile = _T("");
	//}}AFX_DATA_INIT
}


void CSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettings)
	DDX_Control(pDX, IDC_COMBO3, m_cMaxRank);
	DDX_Control(pDX, IDC_CHECK11, m_cTutorial);
	DDX_Control(pDX, IDC_CHECK5, m_cPersistant);
	DDX_Control(pDX, IDC_CHECK8, m_cPublish);
	DDX_Control(pDX, IDC_COMBO2, m_cType);
	DDX_Control(pDX, IDC_EDIT12, m_cSaveFile);
	DDX_Control(pDX, IDC_EDIT8, m_PasswordControl);
	DDX_Control(pDX, IDC_EDIT6, m_UserControl);
	DDX_Control(pDX, IDC_CHECK6, m_ClanControl);
	DDX_Control(pDX, IDC_CHECK1, m_BalancedControl);
	DDX_Control(pDX, IDC_EDIT1, m_AddressControl);
	DDX_Control(pDX, IDC_COMBO1, m_Context);
	DDX_Text(pDX, IDC_EDIT2, m_Description);
	DDX_Text(pDX, IDC_EDIT5, m_Port);
	DDX_Text(pDX, IDC_EDIT7, m_MaxClients);
	DDX_Check(pDX, IDC_CHECK1, m_Balanced);
	DDX_Check(pDX, IDC_CHECK4, m_Trace);
	DDX_CBIndex(pDX, IDC_COMBO2, m_Type);
	DDX_Text(pDX, IDC_EDIT4, m_Name);
	DDX_Check(pDX, IDC_CHECK2, m_DetectAddress);
	DDX_Text(pDX, IDC_EDIT1, m_Address);
	DDX_Check(pDX, IDC_CHECK6, m_Clan);
	DDX_CBIndex(pDX, IDC_COMBO3, m_MaxRank);
	DDX_Check(pDX, IDC_CHECK3, m_Trusted);
	DDX_Text(pDX, IDC_EDIT6, m_User);
	DDX_Text(pDX, IDC_EDIT8, m_Password);
	DDX_Check(pDX, IDC_CHECK9, m_EnableLog);
	DDX_Text(pDX, IDC_EDIT3, m_ServerDescription);
	DDX_Check(pDX, IDC_CHECK8, m_bPublish);
	DDX_Text(pDX, IDC_EDIT10, m_ServerFlags);
	DDX_Text(pDX, IDC_EDIT11, m_ClientFlags);
	DDX_Text(pDX, IDC_EDIT14, m_ModFlags);
	DDX_Check(pDX, IDC_CHECK11, m_bTutorial);
	DDX_Check(pDX, IDC_CHECK5, m_bPersistant);
	DDX_Text(pDX, IDC_EDIT12, m_sSaveFile);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT17, m_nMaxPlayers);
	DDX_Check(pDX, IDC_CHECK7, m_bEnableTeamAI);
}


BEGIN_MESSAGE_MAP(CSettings, CDialog)
	//{{AFX_MSG_MAP(CSettings)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnUpdateDescription)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnUpdateContext)
	ON_BN_CLICKED(IDC_CHECK2, OnDetectAddress)
	ON_BN_CLICKED(IDC_CHECK3, OnTrusted)
	ON_BN_CLICKED(IDC_CHECK5, OnPersistant)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettings message handlers

BOOL CSettings::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// read the settings from the registry
	CServerApp * pApp = (CServerApp *)AfxGetApp();
	ASSERT( pApp );

	Settings settings( CharString( pApp->m_ServerKey ) );

	// read server settings from registry
	m_Type = settings.get( "type", (dword)0 );
	m_Name = settings.get( "name", "" );
	m_bPublish = settings.get( "publish", 1 ) != 0;
	m_ServerDescription = settings.get( "description", "" );
	m_Address = settings.get( "address", "" );
	m_DetectAddress = m_Address.GetLength() == 0;
	m_Port = settings.get( "port", 9001 );
	m_MaxClients = settings.get( "maxClients", 32 );
	m_Balanced = settings.get( "Balanced", 1 );
	m_Clan = settings.get( "clan", (dword)0 );
	m_bTutorial = settings.get( "tutorial", (dword)0 ) != 0;
	m_Trace = settings.get( "Trace", (dword)0 );
	m_EnableLog = settings.get( "enableLog", (dword)0 );
	m_nMaxPlayers = settings.get( "maxPlayers", 32 );
	m_MaxRank = settings.get( "maxRank", CHIEF_MARSHAL );
	m_Trusted = settings.get( "trusted", (dword)0 );
	m_User = settings.get( "user", "" );
	m_Password = settings.get( "password", "" );

	m_ServerFlags = settings.get( "serverFlags", 2 );
	m_ClientFlags = settings.get( "clientFlags", (dword)0 );
	m_ModFlags = settings.get( "modFlags", 4 );
	
	m_bPersistant = settings.get( "persistant", (dword)0 ) != 0;
	m_sSaveFile = settings.get( "saveFile", ".\\Universe.dat" );
	m_bEnableTeamAI = settings.get( "enableTeamSpawners", (dword)1) != 0;

	UpdateData( false );

	m_AddressControl.EnableWindow( !m_DetectAddress );
	m_UserControl.EnableWindow( m_Trusted );
	m_PasswordControl.EnableWindow( m_Trusted );
	m_cSaveFile.EnableWindow( m_bPersistant );

	// enumerate all the available GameContext objects
	if (! loadResource( "Campaign.prt", m_Collection ) )
	{
		MessageBox( _T("Failed to load scenerio data!") );
		return FALSE;
	}

	// get the maps exported by the editor as well
	if ( loadResource( "Exported.prt", m_Exported ) )
		m_Collection->push( m_Exported.pointer() );	// compbine the 2 collections

	OnUpdateContext();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSettings::OnOK() 
{
	if ( UpdateData( true ) )
	{
		// write the settings to the registry
		CServerApp * pApp = (CServerApp *)AfxGetApp();
		ASSERT( pApp );

		Settings settings( CharString( pApp->m_ServerKey ) );

		int comboItem = m_Context.GetCurSel();
		if ( comboItem < 0 )
		{
			MessageBox(_T("No valid GameContext has been selected!"),_T("Invalid GameContext") );
			return;
		}
		if ( m_nMaxPlayers > m_MaxClients )
		{
			MessageBox(_T("Max players must be less than or equal to max clients!"), _T("Invalid Value") );
			return;
		}

		GameContext * pContext = (GameContext *)m_Context.GetItemDataPtr( comboItem );
		ASSERT( pContext );

		settings.put( "context", pContext->key().string() );
			
		settings.put( "type", m_Type );
		settings.put( "address", (CharString)m_Address );
		settings.put( "name", (CharString)m_Name );
		settings.put( "description", (CharString)m_ServerDescription );
		settings.put( "port", m_Port );
		settings.put( "maxClients", m_MaxClients );
		settings.put( "Balanced", m_Balanced );
		settings.put( "clan", m_Clan );
		settings.put( "tutorial", m_bTutorial );
		settings.put( "Trace", m_Trace );
		settings.put( "enableLog", m_EnableLog );
		settings.put( "maxPlayers", m_nMaxPlayers );
		settings.put( "maxRank", m_MaxRank );
		settings.put( "trusted", m_Trusted );
		settings.put( "user", (CharString)m_User );
		settings.put( "password", (CharString)m_Password );

		settings.put( "serverFlags", m_ServerFlags );
		settings.put( "clientFlags", m_ClientFlags );
		settings.put( "modFlags", m_ModFlags );
	
		settings.put( "persistant", m_bPersistant );
		settings.put( "saveFile", (CharString)m_sSaveFile );
		settings.put( "enableTeamSpawners", m_bEnableTeamAI );

		CDialog::OnOK();
	}
}

void CSettings::OnUpdateDescription() 
{
	if ( UpdateData( true ) )
	{
		int comboItem = m_Context.GetCurSel();
		if ( comboItem < 0 )
		{
			m_Description = _T("No Context Selected");
			UpdateData( false );
			return;
		}

		GameContext * pContext = 
			(GameContext *)m_Context.GetItemDataPtr( comboItem );

		m_Description = pContext->description();
		UpdateData( false );
	}
}

void CSettings::OnUpdateContext() 
{
	if ( UpdateData( true ) )
	{
		CServerApp * pApp = (CServerApp *)AfxGetApp();
		ASSERT( pApp );

		switch( m_Type )
		{
		case GameContext::SCENERIO:
			m_BalancedControl.EnableWindow( true );
			m_ClanControl.EnableWindow( true );
			break;
		case GameContext::COMBAT:
			m_BalancedControl.EnableWindow( true );
			m_ClanControl.EnableWindow( true );
			break;
		case GameContext::CLAN:
			m_BalancedControl.EnableWindow( true );
			m_ClanControl.EnableWindow( false );
			m_Clan = true;
			break;
		case GameContext::METAVERSE:
			m_ClanControl.EnableWindow( true );
			m_BalancedControl.EnableWindow( false );
			m_Balanced = false;
			break;
		case GameContext::TUTORIAL:
			m_BalancedControl.EnableWindow( false );
			m_Balanced = false;
			m_ClanControl.EnableWindow( false );
			m_Clan = false;
			break;
		}

		UpdateData( false );

		CString context = Settings( CharString( pApp->m_ServerKey ) ).get( "Context", "" );

		m_Context.ResetContent();
		m_Collection->load();

		for(int i=0;i<m_Collection->resourceCount();i++)
		{
			GameContext::Link pContext = m_Collection->resource(i);
			if ( pContext.valid() && pContext->type() == m_Type )
			{
				int comboItem = m_Context.AddString( CString( pContext->name() ) );
				m_Context.SetItemDataPtr( comboItem, pContext );

				if ( CString( pContext->key().string() ) == context )
				{
					m_Type = pContext->type();
					m_Context.SetCurSel( comboItem );
				}
			}
		}

		OnUpdateDescription();
	}
}

void CSettings::OnDetectAddress() 
{
	if ( UpdateData() )
	{
		if ( m_DetectAddress )
		{
			m_Address = "";
			m_AddressControl.EnableWindow( false );
			UpdateData( false );
		}
		else
			m_AddressControl.EnableWindow( true );
	}
}

void CSettings::OnTrusted() 
{
	UpdateData();	
	m_UserControl.EnableWindow( m_Trusted );
	m_PasswordControl.EnableWindow( m_Trusted );
}


void CSettings::OnPersistant() 
{
	UpdateData();	
	m_cSaveFile.EnableWindow( m_bPersistant );
}
