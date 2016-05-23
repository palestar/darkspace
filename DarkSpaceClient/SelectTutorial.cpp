// SelectTutorial.cpp : implementation file
//

#include "stdafx.h"
#include "DarkSpaceClient.h"
#include "SelectTutorial.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectTutorial dialog


CSelectTutorial::CSelectTutorial(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectTutorial::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectTutorial)
	m_Description = _T("");
	//}}AFX_DATA_INIT
}


void CSelectTutorial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectTutorial)
	DDX_Control(pDX, IDC_COMBO1, m_Context);
	DDX_Text(pDX, IDC_EDIT1, m_Description);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectTutorial, CDialog)
	//{{AFX_MSG_MAP(CSelectTutorial)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnUpdateDescription)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectTutorial message handlers

BOOL CSelectTutorial::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if ( loadResource( "Campaign.prt", m_Campaign ) )
	{
		Collection::Link exported;
		if ( loadResource( "Exported.prt", exported ) )
			m_Campaign->push( exported.pointer() );
		
		m_Campaign->load();

		m_Context.ResetContent();
		for(int i=0;m_Campaign.valid() && i<m_Campaign->resourceCount();i++)
		{
			GameContext * pContext = WidgetCast<GameContext>( m_Campaign->resource(i) );
			if ( pContext != NULL && pContext->type() == GameContext::TUTORIAL )
			{
				int comboItem = m_Context.AddString( CString( pContext->name() ) );
				m_Context.SetItemDataPtr( comboItem, pContext );
			}
		}

		m_Context.SetCurSel( 0 );
		OnUpdateDescription();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectTutorial::OnOK() 
{
	int comboItem = m_Context.GetCurSel();
	if ( comboItem < 0 )
	{
		MessageBox(_T("No valid tutorial has been selected!"),_T("Invalid Tutorial") );
		return;
	}

	GameContext * pContext = (GameContext *)
		m_Context.GetItemDataPtr( comboItem );
	m_Selected = pContext->key();

	CDialog::OnOK();
}

void CSelectTutorial::OnUpdateDescription() 
{
	int comboItem = m_Context.GetCurSel();
	if ( comboItem < 0 )
		return;

	GameContext * pContext = (GameContext *)m_Context.GetItemDataPtr( comboItem );
	m_Description = pContext->description();
	UpdateData( false );
}
