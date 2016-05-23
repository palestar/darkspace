// PortView.cpp : implementation file
//

#define NOUNJUMPGATEPORT_DLL
#include "stdafx.h"
#include "PortView.h"
#include "Port.h"

#include "Game/UniverseContext.h"

#include "Tools/ScenePort/Port.h"
#include "Tools/ScenePort/SelectNode.h"
#include "Tools/UniverseContextPort/Port.h"
#include "Tools/ResourcerDoc/SelectResource.h"

/////////////////////////////////////////////////////////////////////////////
// CPortView

IMPLEMENT_DYNCREATE(CPortView, CFormView)

CPortView::CPortView()
	: CFormView(CPortView::IDD)
{
	//{{AFX_DATA_INIT(CPortView)
	m_Destination = _T("");
	m_Context = _T("");
	//}}AFX_DATA_INIT
}

CPortView::~CPortView()
{
}

void CPortView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPortView)
	DDX_Text(pDX, IDC_EDIT1, m_Destination);
	DDX_Text(pDX, IDC_EDIT2, m_Context);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPortView, CFormView)
	//{{AFX_MSG_MAP(CPortView)
	ON_BN_CLICKED(IDC_BUTTON1, OnDestination)
	ON_BN_CLICKED(IDC_BUTTON2, OnOpenContext)
	ON_BN_CLICKED(IDC_BUTTON3, OnBrowseContext)
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
	NounJumpGatePort * pDoc = dynamic_cast<NounJumpGatePort *>( GetDocument() );
	ASSERT( pDoc );

	if ( lHint )
	{
		UpdateData( true );
	}
	else
	{
		m_Destination = pDoc->m_Destination;
		m_Context = pDoc->m_Context;
		UpdateData( false );
	}
}


void CPortView::OnDestination() 
{
	NounJumpGatePort * pDoc = dynamic_cast<NounJumpGatePort *>( GetDocument() );
	ASSERT( pDoc );

	ScenePort *    pScenePort = NULL;

	ScenePort::Ref rScenePort;
	if ( m_Context.GetLength() > 0 )
	{
		// This jumpgate is linked to a new system, thus a different server!
		UniverseContextPort::Ref rContext = dynamic_cast<UniverseContextPort *>( Port::portLoad( m_Context ) );
		if (! rContext.valid() )
		{
			MessageBox( "Failed to load destination context!" );
			return;
		}

		rScenePort = dynamic_cast<ScenePort *>( Port::portLoad( rContext->m_UniversePort ) );
		if (! rScenePort.valid() )
		{
			MessageBox( "Failed to load scene port for destination context!" );
			return;
		}

		pScenePort = rScenePort;
	}
	else
		pScenePort = (ScenePort *)pDoc->parent();

	CSelectNode dialog( pScenePort );
	if ( dialog.DoModal() == IDOK )
	{
		pDoc->m_Destination = dialog.m_pSelected != NULL ? dialog.m_pSelected->name() : "";

		pDoc->UpdateAllViews( NULL );
		pDoc->setUpdate();
		pDoc->updateParent();
	}
}


void CPortView::OnOpenContext() 
{
	AfxGetApp()->OpenDocumentFile( m_Context );
}

void CPortView::OnBrowseContext() 
{
	NounJumpGatePort * pDoc = dynamic_cast<NounJumpGatePort *>( GetDocument() );
	ASSERT( pDoc );

	CSelectResource dialog( ClassKey( 4037388579725782390 ) );
	if ( dialog.DoModal() == IDOK)
	{
		pDoc->m_Context = (const char *)dialog.m_Selected;

		pDoc->UpdateAllViews(NULL);
		pDoc->setUpdate();
		pDoc->updateParent();
	}
}
