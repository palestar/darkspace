// PortView.cpp : implementation file
//

#include "stdafx.h"
#include "PortView.h"
#include "Port.h"
#include "Mask.h"

#include "Tools/ResourcerDoc/SelectResource.h"
#include "Tools/ScenePort/Port.h"
#include "DarkSpace/GameProfile.h"
//#include "DarkSpace/GameContext.h"

/////////////////////////////////////////////////////////////////////////////
// CPortView

IMPLEMENT_DYNCREATE(CPortView, CFormView)

CPortView::CPortView()
	: CFormView(CPortView::IDD)
{
	//{{AFX_DATA_INIT(CPortView)
	m_Ship = _T("");
	m_Badges = 0;
	m_Rank = -1;
	//}}AFX_DATA_INIT
}

CPortView::~CPortView()
{
}

void CPortView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPortView)
	DDX_Text(pDX, IDC_EDIT4, m_Ship);
	DDX_Text(pDX, IDC_EDIT3, m_Badges);
	DDX_CBIndex(pDX, IDC_COMBO1, m_Rank);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO1, m_cRank);
}


BEGIN_MESSAGE_MAP(CPortView, CFormView)
	//{{AFX_MSG_MAP(CPortView)
	ON_BN_CLICKED(IDC_BUTTON1, OnOpenShip)
	ON_BN_CLICKED(IDC_BUTTON3, OnBrowseShip)
	ON_BN_CLICKED(IDC_BUTTON2, OnBadges)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnUpdateShip)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortView diagnostics

#ifdef _DEBUG
void CPortView::AssertValid() const
{
	CFormView::AssertValid();
}

void CPortView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPortView message handlers

void CPortView::OnInitialUpdate() 
{
	ShipContextPort * pDoc = dynamic_cast< ShipContextPort * >( GetDocument() );
	ASSERT( pDoc );

	CFormView::OnInitialUpdate();
	
	// modify the style of our frame to prevent user from resizing
	GetParentFrame()->ModifyStyle(WS_THICKFRAME | WS_MAXIMIZEBOX,0);
	// resize the parent frame to the size of the dialog box
    ResizeParentToFit(FALSE);

	// initialize the rank control...
	m_cRank.ResetContent();
	for(int i=FIRST_RANK;i<=LAST_RANK;++i)
	{
		int nItem = m_cRank.AddString( GameProfile::rankText( i ) );
		if ( i == pDoc->m_Rank )
			m_cRank.SetCurSel( nItem );
	}
}

void CPortView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	ShipContextPort * pDoc = dynamic_cast< ShipContextPort * >( GetDocument() );
	ASSERT( pDoc );

	if ( lHint )
	{
		UpdateData( true );
		pDoc->m_Rank = m_Rank;
	}
	else
	{
		m_Badges = pDoc->m_Badges;
		m_Rank = pDoc->m_Rank;
		m_Ship = pDoc->m_Ship;

		UpdateData( false );
	}
}

void CPortView::OnUpdateShip() 
{
	OnUpdate(NULL,true,NULL);
}

void CPortView::OnOpenShip() 
{
	AfxGetApp()->OpenDocumentFile( m_Ship );
}

void CPortView::OnBrowseShip() 
{
	ShipContextPort * pDoc = dynamic_cast< ShipContextPort * >( GetDocument() );
	ASSERT( pDoc );

	CSelectResource dialog( CLASS_KEY( ScenePort) );
	if ( dialog.DoModal() == IDOK)
	{
		pDoc->m_Ship = dialog.m_Selected;
		pDoc->UpdateAllViews(NULL);
		pDoc->SetModifiedFlag();
	}
}

void CPortView::OnBadges() 
{
	ShipContextPort * pDoc = dynamic_cast< ShipContextPort * >( GetDocument() );
	ASSERT( pDoc );

	CMask dialog;
	dialog.m_Mask = m_Badges;

	for(int i=FIRST_BADGE;i<=LAST_BADGE;i++)
		dialog.m_Items.push( GameProfile::badgeText( i ) );

	if ( dialog.DoModal() == IDOK )
	{
		pDoc->m_Badges = dialog.m_Mask;
		pDoc->UpdateAllViews(NULL);
		pDoc->SetModifiedFlag();
	}
}

