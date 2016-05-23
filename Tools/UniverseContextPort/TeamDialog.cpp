// TeamDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TeamDialog.h"

#include "Tools/ImagePort/Port.h"
#include "Tools/CollectionPort/Port.h"
#include "Tools/ResourcerDoc/SelectResource.h"

/////////////////////////////////////////////////////////////////////////////
// CTeamDialog dialog


CTeamDialog::CTeamDialog( UniverseContextPort * pDoc, CWnd* pParent /*=NULL*/)
	: CDialog(CTeamDialog::IDD, pParent), m_pDoc( pDoc )
	, m_bNoJoin(FALSE)
	, m_bNoSpawn(FALSE)
{
	//{{AFX_DATA_INIT(CTeamDialog)
	m_Logo = _T("");
	m_Name = _T("");
	m_Description = _T("");
	m_Faction = -1;
	m_Ships = _T("");
	//}}AFX_DATA_INIT

	m_Team.name = "Team Name";
	m_Team.description = "Team Description";
	m_Team.faction = 0;
}


void CTeamDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTeamDialog)
	DDX_Control(pDX, IDC_COMBO1, m_FactionControl);
	DDX_Text(pDX, IDC_EDIT2, m_Logo);
	DDX_Text(pDX, IDC_EDIT3, m_Name);
	DDX_Text(pDX, IDC_EDIT4, m_Description);
	DDX_CBIndex(pDX, IDC_COMBO1, m_Faction);
	DDX_Text(pDX, IDC_EDIT1, m_Ships);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK1, m_bNoJoin);
	DDX_Check(pDX, IDC_CHECK2, m_bNoSpawn);
}


BEGIN_MESSAGE_MAP(CTeamDialog, CDialog)
	//{{AFX_MSG_MAP(CTeamDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnOpenLogo)
	ON_BN_CLICKED(IDC_BUTTON2, OnBrowseLogo)
	ON_BN_CLICKED(IDC_BUTTON3, OnOpenShip)
	ON_BN_CLICKED(IDC_BUTTON4, OnBrowseShip)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeamDialog message handlers

BOOL CTeamDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_Color.SubclassDlgItem(IDC_COLOR,this); 	

	// initialzie the faction control
	m_FactionControl.ResetContent();
	for(int i=0;i<UniverseContextPort::factionCount();i++)
		m_FactionControl.AddString( (CString)UniverseContextPort::factionName( i ) );

	m_Faction = m_Team.faction;
	m_Name = m_Team.name;
	m_Ships = m_Team.ships;
	m_Description = m_Team.description;
	m_Color.currentcolor = m_Team.color.RGB();
	m_bNoJoin = (m_Team.flags & WorldContext::TF_NOJOIN) != 0;
	m_bNoSpawn = (m_Team.flags & WorldContext::TF_NOSPAWN) != 0;

	UpdateData( false );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTeamDialog::OnOpenLogo() 
{
	AfxGetApp()->OpenDocumentFile( m_Logo );
}

void CTeamDialog::OnBrowseLogo() 
{
	UpdateData( true );

	CSelectResource dialog( CLASS_KEY( ImagePort ) );
	if ( dialog.DoModal() == IDOK)
	{
		m_Logo = dialog.m_Selected;
		UpdateData( false );
	}
}

void CTeamDialog::OnOK() 
{
	if ( UpdateData( true ) )
	{
		m_Team.faction = m_Faction;
		m_Team.name = m_Name;
		m_Team.ships = m_Ships;
		m_Team.description = m_Description;
		m_Team.color = m_Color.currentcolor;

		m_Team.flags = 0;
		if ( m_bNoJoin )
			m_Team.flags |= WorldContext::TF_NOJOIN;
		if ( m_bNoSpawn )
			m_Team.flags |= WorldContext::TF_NOSPAWN;

		CDialog::OnOK();
	}
}

void CTeamDialog::OnOpenShip() 
{
	AfxGetApp()->OpenDocumentFile( m_Ships );
}

void CTeamDialog::OnBrowseShip() 
{
	UpdateData( true );

	CSelectResource dialog( CLASS_KEY( CollectionPort ) );
	if ( dialog.DoModal() == IDOK)
	{
		m_Ships = dialog.m_Selected;
		UpdateData( false );
	}
}
