// Zone.cpp : implementation file
//

#include "stdafx.h"
#include "Zone.h"

#include "Tools/ScenePort/Port.h"
#include "Tools/ResourcerDoc/SelectResource.h"

// CZone dialog

IMPLEMENT_DYNAMIC(CZone, CDialog)
CZone::CZone(CWnd* pParent /*=NULL*/)
	: CDialog(CZone::IDD, pParent)
	, m_fX(0)
	, m_fY(0)
	, m_fZ(0)
	, m_sScenePort(_T(""))
{}

CZone::~CZone()
{}

void CZone::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_fX);
	DDX_Text(pDX, IDC_EDIT2, m_fY);
	DDX_Text(pDX, IDC_EDIT3, m_fZ);
	DDX_Text(pDX, IDC_EDIT11, m_sScenePort);
}


BEGIN_MESSAGE_MAP(CZone, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, OnOpenScene)
	ON_BN_CLICKED(IDC_BUTTON3, OnBrowseScene)
END_MESSAGE_MAP()


// CZone message handlers

void CZone::OnOpenScene()
{
	if ( m_sScenePort.GetLength() > 0 )
		AfxGetApp()->OpenDocumentFile( m_sScenePort );
	OnOK();
}

void CZone::OnBrowseScene()
{
	UpdateData( true );

	CSelectResource dialog( CLASS_KEY( ScenePort) );
	if ( dialog.DoModal() == IDOK)
	{
		m_sScenePort = dialog.m_Selected;
		UpdateData( false );
	}
}
