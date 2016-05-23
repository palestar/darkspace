// PortView.cpp : implementation file
//

#define NOUNBODYPORT_DLL
#include "stdafx.h"
#include "PortView.h"
#include "Port.h"

#include "Tools/ScenePort/SelectNode.h"

/////////////////////////////////////////////////////////////////////////////
// CPortView

IMPLEMENT_DYNCREATE(CPortView, CFormView)

CPortView::CPortView()
	: CFormView(CPortView::IDD)
{
	//{{AFX_DATA_INIT(CPortView)
	m_Owner = 0;
	m_Orbiting = _T("");
	m_OrbitVelocity = 0.0f;
	//}}AFX_DATA_INIT
}

CPortView::~CPortView()
{
}

void CPortView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPortView)
	DDX_Text(pDX, IDC_EDIT2, m_Owner);
	DDV_MinMaxInt(pDX, m_Owner, -1, 31);
	DDX_Text(pDX, IDC_EDIT3, m_Orbiting);
	DDX_Text(pDX, IDC_EDIT4, m_OrbitVelocity);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPortView, CFormView)
	//{{AFX_MSG_MAP(CPortView)
	ON_BN_CLICKED(IDC_BUTTON1, OnBrowseOrbit)
	ON_EN_KILLFOCUS(IDC_EDIT2, OnUpdateBody)
	ON_EN_KILLFOCUS(IDC_EDIT4, OnUpdateBody)
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
	NounBodyPort * pDoc = dynamic_cast<NounBodyPort *>( GetDocument() );
	if ( pDoc != NULL )
	{
		if ( lHint )
		{
			UpdateData( true );

			pDoc->m_Owner = m_Owner;
			pDoc->m_OrbitVelocity = m_OrbitVelocity;

			pDoc->UpdateAllViews(NULL);
			pDoc->setUpdate();
			pDoc->updateParent();
		}
		else
		{
			m_Owner = pDoc->m_Owner;
			m_Orbiting = pDoc->m_Orbiting;
			m_OrbitVelocity = pDoc->m_OrbitVelocity;

			UpdateData( false );
		}
	}
}

void CPortView::OnUpdateBody() 
{
	OnUpdate(NULL,true,NULL);
}

void CPortView::OnBrowseOrbit() 
{
	NounBodyPort * pDoc = dynamic_cast<NounBodyPort *>( GetDocument() );
	ASSERT( pDoc );

	CSelectNode dialog( (ScenePort *)pDoc->parent() );
	if ( dialog.DoModal() == IDOK )
	{
		pDoc->m_Orbiting = dialog.m_pSelected != NULL ? dialog.m_pSelected->name() : "";

		pDoc->UpdateAllViews( NULL );
		pDoc->setUpdate();
		pDoc->updateParent();
	}
}
