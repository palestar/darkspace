// Condition.cpp : implementation file
//

#include "stdafx.h"
#include "Condition.h"
#include "Objective.h"

#include "Tools/ResourcerDoc/SelectResource.h"

/////////////////////////////////////////////////////////////////////////////
// CCondition dialog


CCondition::CCondition(UniverseContextPort * pDoc, CWnd* pParent /*=NULL*/)
	: CDialog(CCondition::IDD, pParent), m_pDoc( pDoc )
{
	//{{AFX_DATA_INIT(CCondition)
	m_Message = _T("");
	m_NextContext = _T("");
	//}}AFX_DATA_INIT

	m_Condition.message = "New Condition";
}


void CCondition::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCondition)
	DDX_Control(pDX, IDC_LIST1, m_Objectives);
	DDX_Text(pDX, IDC_EDIT1, m_Message);
	DDX_Text(pDX, IDC_EDIT3, m_NextContext);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCondition, CDialog)
	//{{AFX_MSG_MAP(CCondition)
	ON_BN_CLICKED(IDC_BUTTON1, OnAddObjective)
	ON_BN_CLICKED(IDC_BUTTON2, OnRemoveObjective)
	ON_BN_CLICKED(IDC_BUTTON3, OnEditObjective)
	ON_BN_CLICKED(IDC_BUTTON4, OnOpenContext)
	ON_BN_CLICKED(IDC_BUTTON5, OnBrowseContext)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnEditObjectiveDC)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCondition message handlers

BOOL CCondition::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_Message = m_Condition.message;
	m_NextContext = m_Condition.next;

	UpdateData( false );

	// initialize the list control
	RECT rect;
	m_Objectives.GetWindowRect(&rect);
	
	// setup the report columns
	int columnWidth = (rect.right - rect.left) / 3;
	m_Objectives.InsertColumn(0,_T("Description"), LVCFMT_LEFT, columnWidth );
	m_Objectives.InsertColumn(1,_T("Faction"), LVCFMT_LEFT, columnWidth );
	m_Objectives.InsertColumn(2,_T("Objective"), LVCFMT_LEFT, columnWidth );

	// add list items
	updateObjectives();

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCondition::OnOK() 
{
	if ( UpdateData( true ) )
	{
		m_Condition.message = m_Message;
		m_Condition.next = m_NextContext;
		
		CDialog::OnOK();
	}
}

void CCondition::OnAddObjective() 
{
	CObjective dialog( m_pDoc );
	if ( dialog.DoModal() == IDOK )
	{
		m_Condition.objectives.push( dialog.m_Objective );
		updateObjectives();
	}
}

void CCondition::OnRemoveObjective() 
{
	int selectedItem = m_Objectives.GetNextItem( -1, LVNI_SELECTED );
	if ( selectedItem < 0 )
		return;
	
	m_Condition.objectives.remove( m_Objectives.GetItemData( selectedItem ) );
	updateObjectives();
}

void CCondition::OnEditObjective() 
{
	int selectedItem = m_Objectives.GetNextItem( -1, LVNI_SELECTED );
	if ( selectedItem < 0 )
		return;
	
	int index = m_Objectives.GetItemData( selectedItem );

	CObjective dialog( m_pDoc );
	dialog.m_Objective = m_Condition.objectives[ m_Objectives.GetItemData( selectedItem ) ];

	if ( dialog.DoModal() == IDOK )
	{
		m_Condition.objectives[ m_Objectives.GetItemData( selectedItem ) ] = dialog.m_Objective;
		updateObjectives();
	}
}

void CCondition::OnEditObjectiveDC(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnEditObjective();
	*pResult = 0;
}

void CCondition::OnOpenContext() 
{
	AfxGetApp()->OpenDocumentFile( m_NextContext );
}

void CCondition::OnBrowseContext() 
{
	UpdateData( true );

	CSelectResource dialog( CLASS_KEY( UniverseContextPort ) );
	if ( dialog.DoModal() == IDOK)
	{
		m_NextContext = dialog.m_Selected;
		UpdateData( false );
	}
}

//----------------------------------------------------------------------------

void CCondition::updateObjectives()
{
	m_Objectives.DeleteAllItems();
	for(int i=0;i<m_Condition.objectives.size();i++)
	{
		int item = m_Objectives.InsertItem( 0, (CString)m_Condition.objectives[i].description );

		m_Objectives.SetItemText( item, 1, 
			(CString)UniverseContextPort::factionName( m_Condition.objectives[i].faction ) );

		CString objective( GameContext::objectiveText( m_Condition.objectives[i].type ) );
		objective += " ";
		objective += (CString)m_Condition.objectives[i].noun;

		m_Objectives.SetItemText( item, 2, objective );
		m_Objectives.SetItemData( item, i );
	}
}

//----------------------------------------------------------------------------
// EOF

