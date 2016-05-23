// PortView.cpp : implementation file
//

#define NOUNPORT_DLL
#include "stdafx.h"
#include "PortView.h"
#include "Port.h"

#include "Tools/ResourcerDoc/SelectClass.h"
#include "Tools/ResourcerDoc/SelectResource.h"
#include "Tools/ScenePort/Port.h"

/////////////////////////////////////////////////////////////////////////////
// CPortView

IMPLEMENT_DYNCREATE(CPortView, CFormView)

CPortView::CPortView()
	: CFormView(CPortView::IDD)
{
	//{{AFX_DATA_INIT(CPortView)
	m_NounContext = _T("");
	m_Class = _T("");
	//}}AFX_DATA_INIT
}

CPortView::~CPortView()
{
}

void CPortView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPortView)
	DDX_Text(pDX, IDC_EDIT2, m_NounContext);
	DDX_Text(pDX, IDC_EDIT1, m_Class);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPortView, CFormView)
	//{{AFX_MSG_MAP(CPortView)
	ON_BN_CLICKED(IDC_BUTTON2, OnOpenNounContext)
	ON_BN_CLICKED(IDC_BUTTON3, OnBrowseNounContext)
	ON_BN_CLICKED(IDC_BUTTON1, OnBrowseClass)
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
	NounPort * pDoc = dynamic_cast<NounPort *>( GetDocument() );
	ASSERT( pDoc );

	if ( lHint )
	{
		UpdateData( true );
	}
	else
	{
		m_Class = (const char *)Factory::className( pDoc->m_Class );
		m_NounContext = pDoc->m_NounContext;
		UpdateData( false );
	}
}


void CPortView::OnOpenNounContext() 
{
	AfxGetApp()->OpenDocumentFile( m_NounContext );
}

void CPortView::OnBrowseNounContext() 
{
	NounPort * pDoc = dynamic_cast<NounPort *>( GetDocument() );
	ASSERT( pDoc );

	CSelectResource dialog( CLASS_KEY(NounContextPort) );
	if ( dialog.DoModal() == IDOK)
	{
		pDoc->m_NounContext = (const char *)dialog.m_Selected;

		pDoc->UpdateAllViews(NULL);
		pDoc->setUpdate();
		pDoc->updateParent();
	}
}

void CPortView::OnBrowseClass() 
{
	NounPort * pDoc = dynamic_cast<NounPort *>( GetDocument() );
	ASSERT( pDoc );

	CSelectClass dialog( pDoc->m_Type );
	if ( dialog.DoModal() == IDOK )
	{
		pDoc->m_Class = dialog.m_Selected;

		pDoc->UpdateAllViews( NULL );
		pDoc->setUpdate();
		pDoc->updateParent();
	}
}
