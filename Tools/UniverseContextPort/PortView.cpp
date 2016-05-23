// PortView.cpp : implementation file
//

#include "stdafx.h"
#include "PortView.h"
#include "Port.h"
#include "ChildFrame.h"

#include "Tools/ResourcerDoc/SelectResource.h"
#include "Tools/ScenePort/Port.h"
#include "Tools/TextPort/Port.h"
#include "Standard/Process.h"

#include "TeamDialog.h"
#include "Condition.h"
#include "Zone.h"

/////////////////////////////////////////////////////////////////////////////
// CPortView

IMPLEMENT_DYNCREATE(CPortView, CFormView)

CPortView::CPortView()
	: CFormView(CPortView::IDD)
	, m_sServerStatus(_T(""))
	, m_pServerProcess( NULL )
	, m_nServerStartTime( 0 )
	, m_fCenterX(0)
	, m_fCenterY(0)
	, m_fCenterZ(0)
{
	//{{AFX_DATA_INIT(CPortView)
	m_Name = _T("");
	m_Description = _T("");
	m_Scenery = _T("");
	m_Script = _T("");
	m_Type = -1;
	m_TimeLimit = 0.0f;
	//}}AFX_DATA_INIT
}

CPortView::~CPortView()
{
}

void CPortView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPortView)
	DDX_Control(pDX, IDC_LIST3, m_ConditionList);
	DDX_Control(pDX, IDC_LIST1, m_TeamList);
	DDX_Text(pDX, IDC_EDIT1, m_Name);
	DDX_Text(pDX, IDC_EDIT2, m_Description);
	DDX_Text(pDX, IDC_EDIT7, m_Scenery);
	DDX_Text(pDX, IDC_EDIT8, m_Script);
	DDX_CBIndex(pDX, IDC_COMBO1, m_Type);
	DDX_Text(pDX, IDC_EDIT10, m_TimeLimit);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_SERVER_STATUS, m_sServerStatus);
	DDX_Control(pDX, IDC_BUTTON19, m_cStopButton);
	DDX_Control(pDX, IDC_BUTTON13, m_cStartButton);
	DDX_Control(pDX, IDC_LIST2, m_ZoneList);
	DDX_Text(pDX, IDC_EDIT3, m_fCenterX);
	DDX_Text(pDX, IDC_EDIT11, m_fCenterY);
	DDX_Text(pDX, IDC_EDIT12, m_fCenterZ);
}


BEGIN_MESSAGE_MAP(CPortView, CFormView)
	//{{AFX_MSG_MAP(CPortView)
	ON_EN_KILLFOCUS(IDC_EDIT1, OnUpdateContext)
	ON_BN_CLICKED(IDC_BUTTON5, OnAddTeam)
	ON_BN_CLICKED(IDC_BUTTON6, OnEditTeam)
	ON_BN_CLICKED(IDC_BUTTON7, OnRemoveTeam)
	ON_BN_CLICKED(IDC_BUTTON11, OnOpenScenery)
	ON_BN_CLICKED(IDC_BUTTON12, OnBrowseScenery)
	ON_BN_CLICKED(IDC_BUTTON13, OnRun)
	ON_BN_CLICKED(IDC_BUTTON14, OnAddCondition)
	ON_BN_CLICKED(IDC_BUTTON15, OnEditCondition)
	ON_BN_CLICKED(IDC_BUTTON16, OnRemoveCondition)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnEditTeamDC)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST3, OnEditConditionDC)
	ON_BN_CLICKED(IDC_BUTTON17, OnOpenScript)
	ON_BN_CLICKED(IDC_BUTTON18, OnBrowseScript)
	ON_EN_KILLFOCUS(IDC_EDIT2, OnUpdateContext)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnUpdateContext)
	ON_EN_KILLFOCUS(IDC_EDIT5, OnUpdateContext)
	ON_EN_KILLFOCUS(IDC_EDIT6, OnUpdateContext)
	ON_EN_KILLFOCUS(IDC_EDIT9, OnUpdateContext)
	ON_EN_KILLFOCUS(IDC_EDIT10, OnUpdateContext)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON19, OnStopSever )
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON9, OnEditZone)
	ON_BN_CLICKED(IDC_BUTTON8, OnAddZone)
	ON_BN_CLICKED(IDC_BUTTON10, OnRemoveZone)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, OnEditZoneDC)
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
	RECT rect;

	CFormView::OnInitialUpdate();
	
	// modify the style of our frame to prevent user from resizing
	GetParentFrame()->ModifyStyle(WS_THICKFRAME | WS_MAXIMIZEBOX,0);
	// resize the parent frame to the size of the dialog box
    ResizeParentToFit(FALSE);

	m_ZoneList.GetWindowRect( &rect );
	int columnWidth = (rect.right - rect.left) / 2;

	m_ZoneList.InsertColumn( 0, _T("Scene"), LVCFMT_LEFT, columnWidth );
	m_ZoneList.InsertColumn( 1, _T("Position"), LVCFMT_LEFT, columnWidth );

	// setup the conditions list control
	m_ConditionList.GetWindowRect(&rect);
	columnWidth = (rect.right - rect.left) / 3;

	m_ConditionList.InsertColumn(0,_T("Message"), LVCFMT_LEFT, columnWidth );
	m_ConditionList.InsertColumn(1,_T("Next"), LVCFMT_LEFT, columnWidth );
	m_ConditionList.InsertColumn(2,_T("Objectives"), LVCFMT_LEFT, columnWidth );

	// setup the team list control
	m_TeamList.GetWindowRect(&rect);
	columnWidth = (rect.right - rect.left) / 3;

	m_TeamList.InsertColumn(0,_T("Team"), LVCFMT_LEFT, columnWidth * 2 );
	m_TeamList.InsertColumn(1,_T("Index"), LVCFMT_LEFT, columnWidth );
	
	m_cStopButton.EnableWindow( false );
	m_sServerStatus = _T("Server not running...");

	OnUpdate(NULL,false,NULL);
}

void CPortView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	UniverseContextPort * pDoc = dynamic_cast< UniverseContextPort * >( GetDocument() );
	ASSERT( pDoc );

	if ( lHint )
	{
		UpdateData( true );

		pDoc->m_Position.x = m_fCenterX;
		pDoc->m_Position.y = m_fCenterY;
		pDoc->m_Position.z = m_fCenterZ;
		pDoc->m_Name = m_Name;
		pDoc->m_Description = m_Description;
		pDoc->m_Type = m_Type;
		pDoc->m_TimeLimit = m_TimeLimit * TICKS_PER_HOUR;

		pDoc->UpdateAllViews( NULL );
	}
	else
	{
		m_fCenterX = pDoc->m_Position.x;
		m_fCenterY = pDoc->m_Position.y;
		m_fCenterZ = pDoc->m_Position.z;
		m_Name = pDoc->m_Name;
		m_Description = pDoc->m_Description;
		m_Type = pDoc->m_Type;
		m_Script = pDoc->m_Script;
		m_Scenery = pDoc->m_Scenery;
		m_TimeLimit = float( pDoc->m_TimeLimit ) / TICKS_PER_HOUR;

		m_ZoneList.DeleteAllItems();
		for(int i=0;i<pDoc->m_Zones.size();++i)
		{
			int nItem = m_ZoneList.InsertItem( 0, (CString)pDoc->m_Zones[i].sScene );
			m_ZoneList.SetItemText( nItem, 1, ConvertType<CharString>( pDoc->m_Zones[i].vPosition ) );
			m_ZoneList.SetItemData( nItem, i );
		}

		m_TeamList.DeleteAllItems();
		for(int i=0;i<pDoc->m_Teams.size();i++)
		{
			int listItem = m_TeamList.InsertItem( 0, (CString)pDoc->m_Teams[i].name );
			m_TeamList.SetItemText( listItem, 1, (CString)CharString().format("%d", i) );
			m_TeamList.SetItemData( listItem, i );
		}

		m_ConditionList.DeleteAllItems();
		for(int i=0;i<pDoc->m_Conditions.size();i++)
		{
			int listItem = m_ConditionList.InsertItem( 0, (CString)pDoc->m_Conditions[i].message );
			m_ConditionList.SetItemText( listItem, 1, (CString)pDoc->m_Conditions[i].next );
			m_ConditionList.SetItemText( listItem, 2, (CString)CharString().format("%d", pDoc->m_Conditions[i].objectives.size()) );
			m_ConditionList.SetItemData( listItem, i );
		}

		UpdateData( false );
	}
}


void CPortView::OnUpdateContext() 
{
	OnUpdate(NULL,true,NULL);
}

void CPortView::OnAddZone()
{
	UniverseContextPort * pDoc = dynamic_cast< UniverseContextPort * >( GetDocument() );
	ASSERT( pDoc );

	CZone dialog;
	if ( dialog.DoModal() == IDOK )
	{
		UniverseContextPort::Zone zone;
		zone.sScene = dialog.m_sScenePort;
		zone.vPosition = Vector3( dialog.m_fX, dialog.m_fY, dialog.m_fZ );

		pDoc->m_Zones.push( zone );
		pDoc->UpdateAllViews( NULL );
	}
}

void CPortView::OnEditZone()
{
	UniverseContextPort * pDoc = dynamic_cast< UniverseContextPort * >( GetDocument() );
	ASSERT( pDoc );

	int nSelected = m_ZoneList.GetNextItem( -1, LVNI_SELECTED );
	if ( nSelected < 0 )
		return;
	int nZone = m_ZoneList.GetItemData( nSelected );
	if ( nZone < 0 || nZone >= pDoc->m_Zones.size() )
		return;
	UniverseContextPort::Zone & zone = pDoc->m_Zones[ nZone ];
	
	CZone dialog;
	dialog.m_sScenePort = zone.sScene;
	dialog.m_fX = zone.vPosition.x;
	dialog.m_fY = zone.vPosition.y;
	dialog.m_fZ = zone.vPosition.z;
	if ( dialog.DoModal() == IDOK )
	{
		zone.sScene = dialog.m_sScenePort;
		zone.vPosition.x = dialog.m_fX;
		zone.vPosition.y = dialog.m_fY;
		zone.vPosition.z = dialog.m_fZ;

		pDoc->UpdateAllViews( NULL );
	}
}

void CPortView::OnRemoveZone()
{
	UniverseContextPort * pDoc = dynamic_cast< UniverseContextPort * >( GetDocument() );
	ASSERT( pDoc );

	int nSelected = m_ZoneList.GetNextItem( -1, LVNI_SELECTED );
	if ( nSelected < 0 )
		return;
	int nZone = m_ZoneList.GetItemData( nSelected );
	if ( nZone < 0 || nZone >= pDoc->m_Zones.size() )
		return;

	pDoc->m_Zones.remove( nZone );
	pDoc->UpdateAllViews( NULL );
}

void CPortView::OnEditZoneDC(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnEditZone();
	*pResult = 0;
}

void CPortView::OnAddTeam() 
{
	UniverseContextPort * pDoc = dynamic_cast< UniverseContextPort * >( GetDocument() );
	ASSERT( pDoc );

	if ( pDoc->m_Teams.size() > 31 )
	{
		MessageBox(_T("Maximum number of teams is 32!"));
		return;
	}

	CTeamDialog dialog( pDoc );
	if ( dialog.DoModal() == IDOK )
	{
		pDoc->m_Teams.push( dialog.m_Team );
		pDoc->UpdateAllViews( NULL );
	}
}

void CPortView::OnEditTeam() 
{
	int selectedItem = m_TeamList.GetNextItem( -1, LVNI_SELECTED );
	if ( selectedItem < 0 )
		return;

	UniverseContextPort * pDoc = dynamic_cast< UniverseContextPort * >( GetDocument() );
	ASSERT( pDoc );

	int index = m_TeamList.GetItemData( selectedItem );

	CTeamDialog dialog( pDoc );
	dialog.m_Team = pDoc->m_Teams[ index ];
	if ( dialog.DoModal() == IDOK )
	{
		pDoc->m_Teams[ index ] = dialog.m_Team;
		pDoc->UpdateAllViews( NULL );
	}
	
}

void CPortView::OnEditTeamDC(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnEditTeam();
	*pResult = 0;
}


void CPortView::OnRemoveTeam() 
{
	int selectedItem = m_TeamList.GetNextItem( -1, LVNI_SELECTED );
	if ( selectedItem < 0 )
		return;

	UniverseContextPort * pDoc = dynamic_cast< UniverseContextPort * >( GetDocument() );
	ASSERT( pDoc );

	pDoc->m_Teams.remove( m_TeamList.GetItemData( selectedItem ) );
	pDoc->UpdateAllViews( NULL );
}

void CPortView::OnOpenScenery() 
{
	AfxGetApp()->OpenDocumentFile( m_Scenery );
}

void CPortView::OnBrowseScenery() 
{
	UniverseContextPort * pDoc = dynamic_cast< UniverseContextPort * >( GetDocument() );
	ASSERT( pDoc );

	CSelectResource dialog( CLASS_KEY( ScenePort) );
	if ( dialog.DoModal() == IDOK)
	{
		pDoc->m_Scenery = dialog.m_Selected;
		pDoc->UpdateAllViews(NULL);
	}
}

void CPortView::OnOpenScript() 
{
	if ( m_Script.GetLength() > 0 )
		AfxGetApp()->OpenDocumentFile( m_Script );
}

void CPortView::OnBrowseScript() 
{
	UniverseContextPort * pDoc = dynamic_cast< UniverseContextPort * >( GetDocument() );
	ASSERT( pDoc );

	CSelectResource dialog( CLASS_KEY( TextPort ) );
	if ( dialog.DoModal() == IDOK)
	{
		pDoc->m_Script = dialog.m_Selected;
		pDoc->UpdateAllViews(NULL);
	}
}

void CPortView::OnAddCondition() 
{
	UniverseContextPort * pDoc = dynamic_cast< UniverseContextPort * >( GetDocument() );
	ASSERT( pDoc );

	CCondition dialog( pDoc );
	if ( dialog.DoModal() == IDOK )
	{
		pDoc->m_Conditions.push( dialog.m_Condition );
		pDoc->UpdateAllViews( NULL );
	}
}

void CPortView::OnEditCondition() 
{
	int selectedItem = m_ConditionList.GetNextItem( -1, LVNI_SELECTED );
	if ( selectedItem < 0 )
		return;

	int index = m_ConditionList.GetItemData( selectedItem );

	UniverseContextPort * pDoc = dynamic_cast< UniverseContextPort * >( GetDocument() );
	ASSERT( pDoc );

	CCondition dialog( pDoc );
	dialog.m_Condition = pDoc->m_Conditions[ index ];
	if ( dialog.DoModal() == IDOK )
	{
		pDoc->m_Conditions[ index ] = dialog.m_Condition;
		pDoc->UpdateAllViews( NULL );
	}
	
}

void CPortView::OnEditConditionDC(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnEditCondition();
	*pResult = 0;
}

void CPortView::OnRemoveCondition() 
{
	int selectedItem = m_ConditionList.GetNextItem( -1, LVNI_SELECTED );
	if ( selectedItem < 0 )
		return;

	int index = m_ConditionList.GetItemData( selectedItem );

	UniverseContextPort * pDoc = dynamic_cast< UniverseContextPort * >( GetDocument() );
	ASSERT( pDoc );

	pDoc->m_Conditions.remove( index );
	pDoc->UpdateAllViews( NULL );
}

void CPortView::OnRun() 
{
	if (! m_pServerProcess )
	{
		UniverseContextPort * pDoc = dynamic_cast< UniverseContextPort * >( GetDocument() );
		ASSERT( pDoc );

		// build the GameContext, then release it .. 
		GameContext::Ref pContext = WidgetCast<GameContext>( pDoc->createResource() );
		if (! pContext )
			return;
		pContext = NULL;

		CharString context( pDoc->GetPathName() );
		CharString root( CResourcerDoc::document()->rootDirectory() );
		context.right( context.length() - root.length() );
		
		TCHAR tempDir[ MAX_PATH ];
		GetTempPath( MAX_PATH, tempDir );

		// write the ini file for the server
		CharString ini;
		ini += "[GameServer]\r\n\r\n";
		
		ini += "logFile=";
		ini += CharString(tempDir);
		ini += "DSServer.log\r\n";

		ini += "data=";	
		ini += CharString(CResourcerDoc::document()->brokerFolder()); 
		ini += "\r\n";

		ini += "context=";
		ini += CharString(context);
		ini += "\r\n";

		ini += "address=localhost\r\n";
		ini += "port=9010\r\n";
		ini += "maxClients=1\r\n";
		ini += "publish=0\r\n";
		ini += "metaAddress=vm4.palestar.com\r\n";		// connect to the beta meta server..

		CharString iniFile;
		iniFile.format( "%sDSServer.ini", tempDir );
		FileDisk::saveTextFile( iniFile, ini );

		// start the server
	#ifdef _DEBUG
		m_pServerProcess = Process::start( CharString().format("DarkSpaceServerD.exe %s",iniFile) );
	#else
		m_pServerProcess = Process::start( CharString().format("DarkSpaceServer.exe %s",iniFile) );
	#endif
		if ( m_pServerProcess != NULL )
		{
			m_sServerStatus = CharString().format("Running '%s'", pDoc->m_Name );
			UpdateData( false );

			m_nServerStartTime = time( NULL );
			m_cStartButton.EnableWindow( false );
			m_cStopButton.EnableWindow( true );

			SetTimer( 0x1, 1000, NULL );
		}
		else
		{
			m_sServerStatus = _T("Failed to start server, GameSever.exe not found!");
			UpdateData( false );
		}
	}
}

void CPortView::OnStopSever()
{
	if ( m_pServerProcess )
	{
		KillTimer( 0x1 );

		Process::stop( m_pServerProcess );

		m_pServerProcess = NULL;
		m_cStartButton.EnableWindow( true );
		m_cStopButton.EnableWindow( false );
		m_nServerStartTime = 0;

		m_sServerStatus = "Server stopped...";
		UpdateData( false );
	}
}

void CPortView::OnTimer( UINT nIDEvent )
{
	if ( nIDEvent == 0x1 && m_pServerProcess )
	{
		UniverseContextPort * pDoc = dynamic_cast< UniverseContextPort * >( GetDocument() );
		ASSERT( pDoc );

		time_t nUpTime = time( NULL ) - m_nServerStartTime;
		m_sServerStatus.Format( "Running '%s'... %2.2d:%2.2d", pDoc->m_Name, nUpTime / 60, nUpTime % 60 );
		UpdateData( false );

		if (! Process::active( m_pServerProcess ) )
			OnStopSever();
	}
	else
		CFormView::OnTimer( nIDEvent );
}

void CPortView::OnDestroy() 
{
	// Stop the server if this window is closed..
	OnStopSever();
	CFormView::OnDestroy();
}


