// PortView.cpp : implementation file
//

#define NOUNPLANETPORT_DLL
#include "stdafx.h"
#include "PortView.h"
#include "Port.h"

/////////////////////////////////////////////////////////////////////////////
// CPortView

IMPLEMENT_DYNCREATE(CPortView, CFormView)

CPortView::CPortView()
	: CFormView(CPortView::IDD)
{
	//{{AFX_DATA_INIT(CPortView)
	m_Population = 0;
	m_Hydrogen = FALSE;
	m_Oxygen = FALSE;
	m_Metals = FALSE;
	m_HeavyMetals = FALSE;
	m_CyroMetals = FALSE;
	m_HyperMatter = FALSE;
	m_DarkMatter = FALSE;
	m_Urdaniam = FALSE;
	m_Resources = 0;
	//}}AFX_DATA_INIT
}

CPortView::~CPortView()
{
}

void CPortView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPortView)
	DDX_Text(pDX, IDC_EDIT1, m_Population);
	DDX_Check(pDX, IDC_CHECK1, m_Hydrogen);
	DDX_Check(pDX, IDC_CHECK2, m_Oxygen);
	DDX_Check(pDX, IDC_CHECK3, m_Metals);
	DDX_Check(pDX, IDC_CHECK4, m_HeavyMetals);
	DDX_Check(pDX, IDC_CHECK7, m_CyroMetals);
	DDX_Check(pDX, IDC_CHECK8, m_HyperMatter);
	DDX_Check(pDX, IDC_CHECK9, m_DarkMatter);
	DDX_Check(pDX, IDC_CHECK10, m_Urdaniam);
	DDX_Text(pDX, IDC_EDIT2, m_Resources);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPortView, CFormView)
	//{{AFX_MSG_MAP(CPortView)
	ON_EN_KILLFOCUS(IDC_EDIT1, OnUpdateNode)
	ON_BN_CLICKED(IDC_CHECK1, OnUpdateNode)
	ON_BN_CLICKED(IDC_CHECK2, OnUpdateNode)
	ON_BN_CLICKED(IDC_CHECK3, OnUpdateNode)
	ON_BN_CLICKED(IDC_CHECK4, OnUpdateNode)
	ON_BN_CLICKED(IDC_CHECK7, OnUpdateNode)
	ON_BN_CLICKED(IDC_CHECK8, OnUpdateNode)
	ON_BN_CLICKED(IDC_CHECK9, OnUpdateNode)
	ON_BN_CLICKED(IDC_CHECK10, OnUpdateNode)
	ON_EN_KILLFOCUS(IDC_EDIT2, OnUpdateNode)
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
	NounPlanetPort * pDoc = dynamic_cast<NounPlanetPort *>( GetDocument() );
	ASSERT( pDoc );

	if ( lHint )
	{
		UpdateData( true );

		pDoc->m_Population = m_Population;
		pDoc->m_Resources = m_Resources;
		pDoc->m_Flags = 0;
		pDoc->m_Flags |= m_Hydrogen ? NounPlanet::FLAG_HYDROGEN : 0;
		pDoc->m_Flags |= m_Oxygen ? NounPlanet::FLAG_OXYGEN : 0;
		pDoc->m_Flags |= m_Metals ? NounPlanet::FLAG_METALS : 0;
		pDoc->m_Flags |= m_HeavyMetals ? NounPlanet::FLAG_HEAVY_METALS : 0;
		pDoc->m_Flags |= m_CyroMetals ? NounPlanet::FLAG_CYRO_METALS : 0;
		pDoc->m_Flags |= m_HyperMatter ? NounPlanet::FLAG_HYPER_MATTER : 0;
		pDoc->m_Flags |= m_DarkMatter ? NounPlanet::FLAG_DARK_MATTER : 0;
		pDoc->m_Flags |= m_Urdaniam ? NounPlanet::FLAG_URDANIAM : 0;

		pDoc->UpdateAllViews(NULL);
		pDoc->setUpdate();
		pDoc->updateParent();
	}
	else
	{
		m_Population = pDoc->m_Population;
		m_Resources = pDoc->m_Resources;
		m_Hydrogen = (pDoc->m_Flags & NounPlanet::FLAG_HYDROGEN) ? true : false;
		m_Oxygen = (pDoc->m_Flags & NounPlanet::FLAG_OXYGEN) ? true : false;
		m_Metals = (pDoc->m_Flags & NounPlanet::FLAG_METALS) ? true : false;
		m_HeavyMetals = (pDoc->m_Flags & NounPlanet::FLAG_HEAVY_METALS) ? true : false;
		m_CyroMetals = (pDoc->m_Flags & NounPlanet::FLAG_CYRO_METALS) ? true : false;
		m_HyperMatter = (pDoc->m_Flags & NounPlanet::FLAG_HYPER_MATTER) ? true : false;
		m_DarkMatter = (pDoc->m_Flags & NounPlanet::FLAG_DARK_MATTER) ? true : false;
		m_Urdaniam = (pDoc->m_Flags & NounPlanet::FLAG_URDANIAM) ? true : false;

		UpdateData( false );
	}
}

void CPortView::OnUpdateNode() 
{
	// update the node
	OnUpdate( NULL, true, NULL );
}
