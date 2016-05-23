// PortView.cpp : implementation file
//

#define NOUNSPAWNPORT_DLL
#include "stdafx.h"
#include "PortView.h"
#include "Port.h"

#include "Tools/ShipContextPort/Port.h"
#include "Tools/ScenePort/SelectNode.h"
#include "Tools/ResourcerDoc/SelectResource.h"

#include "Game/Constants.h"

/////////////////////////////////////////////////////////////////////////////
// CPortView

IMPLEMENT_DYNCREATE(CPortView, CFormView)

CPortView::CPortView()
	: CFormView(CPortView::IDD)
{
	//{{AFX_DATA_INIT(CPortView)
	m_SpawnDelay = 0.0f;
	m_ShipContext = _T("");
	m_Order = -1;
	m_OrderTarget = _T("");
	m_Recur = FALSE;
	m_Random = FALSE;
	m_MaxSpawn = 0;
	m_Chance = 0;
	m_SpawnArea = 0.0f;
	//}}AFX_DATA_INIT
}

CPortView::~CPortView()
{
}

void CPortView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPortView)
	DDX_Text(pDX, IDC_EDIT1, m_SpawnDelay);
	DDX_Text(pDX, IDC_EDIT2, m_ShipContext);
	DDX_CBIndex(pDX, IDC_COMBO1, m_Order);
	DDX_Text(pDX, IDC_EDIT3, m_OrderTarget);
	DDX_Check(pDX, IDC_CHECK1, m_Recur);
	DDX_Check(pDX, IDC_CHECK2, m_Random);
	DDX_Text(pDX, IDC_EDIT4, m_MaxSpawn);
	DDX_Text(pDX, IDC_EDIT5, m_Chance);
	DDV_MinMaxInt(pDX, m_Chance, 0, 100);
	DDX_Text(pDX, IDC_EDIT6, m_SpawnArea);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPortView, CFormView)
	//{{AFX_MSG_MAP(CPortView)
	ON_EN_KILLFOCUS(IDC_EDIT1, OnUpdateNode)
	ON_BN_CLICKED(IDC_BUTTON1, OnOpenContext)
	ON_BN_CLICKED(IDC_BUTTON2, OnBrowseContext)
	ON_BN_CLICKED(IDC_BUTTON3, OnOrderTarget)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnUpdateNode)
	ON_EN_KILLFOCUS(IDC_EDIT4, OnUpdateNode)
	ON_EN_KILLFOCUS(IDC_EDIT5, OnUpdateNode)
	ON_BN_CLICKED(IDC_CHECK1, OnUpdateNode)
	ON_BN_CLICKED(IDC_CHECK2, OnUpdateNode)
	ON_EN_KILLFOCUS(IDC_EDIT6, OnUpdateNode)
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
	CFormView::OnInitialUpdate();
	
	// modify the style of our frame to prevent user from resizing
	GetParentFrame()->ModifyStyle(WS_THICKFRAME | WS_MAXIMIZEBOX,0);
	// resize the parent frame to the size of the dialog box
    ResizeParentToFit(FALSE);
}

void CPortView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	NounSpawnPort * pDoc = dynamic_cast<NounSpawnPort *>( GetDocument() );
	ASSERT( pDoc );

	if ( lHint )
	{
		UpdateData( true );

		pDoc->m_SpawnDelay = m_SpawnDelay * TICKS_PER_SECOND;
		pDoc->m_SpawnArea = m_SpawnArea;
		pDoc->m_Order = m_Order;
		pDoc->m_Recur = m_Recur ? true : false;
		pDoc->m_MaxSpawn = m_MaxSpawn;
		pDoc->m_Random = m_Random ? true : false;
		pDoc->m_Chance = m_Chance;
	}
	else
	{
		m_SpawnDelay = float( pDoc->m_SpawnDelay ) / TICKS_PER_SECOND;
		m_SpawnArea = pDoc->m_SpawnArea;
		m_ShipContext = pDoc->m_ShipContext;
		m_Order = pDoc->m_Order;
		m_OrderTarget = pDoc->m_OrderTarget;
		m_Recur = pDoc->m_Recur;
		m_MaxSpawn = pDoc->m_MaxSpawn;
		m_Random = pDoc->m_Random;
		m_Chance = pDoc->m_Chance;
		UpdateData( false );
	}
}

void CPortView::OnUpdateNode() 
{
	OnUpdate( NULL, true, NULL );
}

void CPortView::OnOpenContext() 
{
	AfxGetApp()->OpenDocumentFile( m_ShipContext );
}

void CPortView::OnBrowseContext() 
{
	NounSpawnPort * pDoc = dynamic_cast<NounSpawnPort *>( GetDocument() );
	ASSERT( pDoc );

	CSelectResource dialog( CLASS_KEY( ShipContextPort ) );
	if ( dialog.DoModal() == IDOK)
	{
		pDoc->m_ShipContext = (const char *)dialog.m_Selected;

		pDoc->UpdateAllViews(NULL);
		pDoc->setUpdate();
		pDoc->updateParent();
	}
}

void CPortView::OnOrderTarget() 
{
	NounSpawnPort * pDoc = dynamic_cast<NounSpawnPort *>( GetDocument() );
	ASSERT( pDoc );

	CSelectNode dialog( (ScenePort *)pDoc->parent() );
	if ( dialog.DoModal() == IDOK )
	{
		pDoc->m_OrderTarget = dialog.m_pSelected != NULL ? dialog.m_pSelected->name() : "";

		pDoc->UpdateAllViews( NULL );
		pDoc->setUpdate();
		pDoc->updateParent();
	}
}
