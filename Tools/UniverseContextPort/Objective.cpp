// Objective.cpp : implementation file
//

#include "stdafx.h"
#include "Objective.h"

#include "Tools/ScenePort/SelectNode.h"

/////////////////////////////////////////////////////////////////////////////
// CObjective dialog


CObjective::CObjective(UniverseContextPort * pDoc, CWnd* pParent /*=NULL*/)
	: CDialog(CObjective::IDD, pParent), m_pDoc( pDoc )
{
	//{{AFX_DATA_INIT(CObjective)
	m_Type = -1;
	m_Noun = _T("");
	m_Description = _T("");
	m_Faction = -1;
	//}}AFX_DATA_INIT

	m_Objective.description = "Objective Description";
	m_Objective.noun = "NULL";
	m_Objective.type = GameContext::CONTROL;
	m_Objective.faction = 0;
}


void CObjective::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjective)
	DDX_Control(pDX, IDC_COMBO2, m_FactionControl);
	DDX_CBIndex(pDX, IDC_COMBO1, m_Type);
	DDX_Text(pDX, IDC_EDIT1, m_Noun);
	DDX_Text(pDX, IDC_EDIT2, m_Description);
	DDX_CBIndex(pDX, IDC_COMBO2, m_Faction);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CObjective, CDialog)
	//{{AFX_MSG_MAP(CObjective)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjective message handlers

BOOL CObjective::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// initialzie the faction control
	m_FactionControl.ResetContent();
	for(int i=0;i<UniverseContextPort::factionCount();i++)
		m_FactionControl.AddString( (CString)UniverseContextPort::factionName( i ) );

	m_Description = m_Objective.description;
	m_Type = m_Objective.type;
	m_Faction = m_Objective.faction;
	m_Noun = m_Objective.noun;
	
	UpdateData( false );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CObjective::OnOK() 
{
	if ( UpdateData( true ) )
	{
		m_Objective.description = m_Description;
		m_Objective.type = (GameContext::ObjectiveType)m_Type;
		m_Objective.faction = m_Faction;
		m_Objective.noun = m_Noun;

		CDialog::OnOK();
	}
}

