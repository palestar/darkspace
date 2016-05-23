// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DarkSpaceServerMFC.h"
#include "ServerDlg.h"
#include "Settings.h"


#include "Debug/Trace.h"
#include "Standard/MD5.h"
#include "Standard/Settings.h"
#include "Network/MirrorClient.h"



#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//----------------------------------------------------------------------------

const dword DIALOG_UPDATE_TIME = 5 * 1000;			// how often to update the dialog
const dword MIRROR_CHECK_TIME = (60 * 1000) * 30;	// how often to check for file udpates
const int MAX_CHAT_LINES = 512;

//----------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog

CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerDlg)
	m_Clients = _T("");
	m_MaxClients = _T("");
	m_Received = _T("");
	m_Sent = _T("");
	m_Status = _T("");
	m_Time = _T("");
	m_Message = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pLastChatMessage = 0;
	m_nRestartTime = 0;
	m_bEnableRestart = false;
	m_nShutdownTime = 60 * 5;
	m_pFileReactor = NULL;
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerDlg)
	DDX_Control(pDX, IDC_LIST2, m_Players);
	DDX_Control(pDX, IDC_LIST1, m_Chat);
	DDX_Text(pDX, IDC_CLIENTS, m_Clients);
	DDX_Text(pDX, IDC_MAXCLIENTS, m_MaxClients);
	DDX_Text(pDX, IDC_RECEIVED, m_Received);
	DDX_Text(pDX, IDC_SENT, m_Sent);
	DDX_Text(pDX, IDC_STATUS, m_Status);
	DDX_Text(pDX, IDC_TIME, m_Time);
	DDX_Text(pDX, IDC_EDIT1, m_Message);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	//{{AFX_MSG_MAP(CServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, OnStart)
	ON_BN_CLICKED(IDC_BUTTON2, OnStop)
	ON_BN_CLICKED(IDC_BUTTON3, OnSettings)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON4, OnSend)
	ON_BN_CLICKED(IDC_BUTTON5, OnDisconnect)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	CServerApp * pApp = (CServerApp *)AfxGetApp();
	ASSERT( pApp );

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// start the server
	OnStart();
	// start the timer
	SetTimer( 0x1, DIALOG_UPDATE_TIME, NULL );

	// Start the update timer, check every 30 minutes for new code updates
	if (! pApp->m_IsSlave )
		SetTimer( 0x2, MIRROR_CHECK_TIME, NULL );

	CRect rect;
	m_Chat.GetClientRect( &rect );
	m_Chat.InsertColumn( 0, _T("Messages"), LVCFMT_CENTER, rect.Width() );
	m_Chat.SetExtendedStyle( LVS_EX_FULLROWSELECT );
	
	m_Players.GetClientRect( &rect );
	m_Players.InsertColumn( 0, _T("Name"), LVCFMT_CENTER, rect.Width() / 2 );
	m_Players.InsertColumn( 1, _T("IP"), LVCFMT_CENTER, rect.Width() / 2 );
	m_Players.SetExtendedStyle( LVS_EX_FULLROWSELECT );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

static CharString RemoveTags( const char * pString )
{
	CharString clean;

	int messageLength = strlen( pString );
	for(int i=0;i<messageLength;i++)
	{
		char character = pString[ i ];
		if ( character == '<'  )
		{
			if ( (i + 1) < messageLength && pString[ i + 1 ] == '<' )
			{
				clean += "<<";
				i++;
				
				continue;
			}
			
			for(int j=i;j<messageLength;j++)
				if ( pString[ j ] == '>' )
				{
					i = j;
					break;
				}
		}
		else
			clean += character;
	}

	return clean;
}

void CServerDlg::OnTimer(UINT nIDEvent) 
{
	CServerApp * pApp = (CServerApp *)AfxGetApp();
	ASSERT( pApp );

	Event serverUpdate( "ServerUpdate" );
	if ( nIDEvent == 0x1 || nIDEvent == 0x0 )
	{
		if ( m_Server.active() )
		{
			UpdateData( true );

			m_Server.lock();
			// update the dialog
			dword runningTime = m_Server.serverTime();
			if ( runningTime < 1 )
				runningTime = 1;

			dword nReceived = m_Server.bytesReceived() + m_Server.bytesReceivedUDP();
			dword nSent = m_Server.bytesSent() + m_Server.bytesSentUDP();

			m_Clients.Format( _T("%d"), m_Server.clientCount() );
			m_MaxClients.Format( _T("%d"), m_Server.maxClients() );
			m_Received.Format(_T("%.1f k : %.1f k/s"), 
				float( nReceived ) / 1024.0f,
				float( nReceived ) / runningTime / 1024.0f );
			m_Sent.Format(_T("%.1f k : %.1f k/s"), 
				float( nSent ) / 1024.0f,
				float( nSent ) / runningTime / 1024.0f );

			if ( m_Server.worldContext() != NULL )
				m_Status.Format( _T("Running...%s"), CString(m_Server.worldContext()->name()) );

			m_Time.Format(_T("%d:%2.2d:%2.2d"), runningTime / 3600, (runningTime / 60) % 60, runningTime % 60);
			UpdateData( false );

			while( m_Server.m_Log.valid() )
			{
				CString message( RemoveTags( *m_Server.m_Log ) );
				m_Server.m_Log.pop();

				int item = m_Chat.InsertItem( m_Chat.GetItemCount(), message );
				m_Chat.EnsureVisible( item, false );

				// keep list size under a limit
				while( m_Chat.GetItemCount() > MAX_CHAT_LINES )
					m_Chat.DeleteItem( 0 );
			}

			// add new players
			for(int i=0;i<m_Server.clientCount();i++)
			{
				dword nClientId = m_Server.client( i );
				if ( nClientId == 0 )
					continue;

				bool bFound = false;
				for(int k=0;!bFound && k<m_Players.GetItemCount();k++) 
					if ( m_Players.GetItemData( k ) == nClientId )
					{
						bFound = true;

						m_Players.SetItemText( k, 0, CString( m_Server.clientName( nClientId )) );
						m_Players.SetItemText( k, 1, CString(m_Server.clientAddress( nClientId )) );
					}

				if (! bFound  )
				{
					int item = m_Players.InsertItem( 0, CString(m_Server.clientName( nClientId )) );
					m_Players.SetItemText( item, 1, CString(m_Server.clientAddress( nClientId )) );
					m_Players.SetItemData( item, nClientId );
				}
			}
		
			// remove disconnected clients
			for(int i=m_Players.GetItemCount()-1;i>=0;i--)
			{
				if ( m_Server.clientSocket( m_Players.GetItemData( i ) ) == NULL )
					m_Players.DeleteItem( i );
			}

			m_Server.unlock();
		}
		else
		{
			m_Players.DeleteAllItems();

			m_Status = "Inactive";
			UpdateData( false );

			if ( m_bEnableRestart )
				RestartServer();
		}

		// check the signal to see if we have been asked to shutdown
		if ( serverUpdate.signaled() && m_nRestartTime == 0 )
			m_nRestartTime = m_nShutdownTime;	

		if ( m_nRestartTime > 0 )
		{
			m_nRestartTime -= (DIALOG_UPDATE_TIME / 1000);
			if ( m_nRestartTime <= 0 || m_Server.clientCount() == 0 )
			{
				// restart the server now
				RestartServer();
				// clear the restart time, otherwise it will start shutting down again once DoModal() is called on this dialog again
				m_nRestartTime = 0;
			}
			else
				m_Server.sendChat( CharString().format("SERVER: shutting down in %d seconds...", m_nRestartTime) );
		}

		if ( nIDEvent != 0 )
			CDialog::OnTimer(nIDEvent);
	}
	else if ( nIDEvent == 0x2 )
	{
#ifndef _DEBUG
		if ( m_nRestartTime == 0 )
		{
			ASSERT(! pApp->m_IsSlave );

			Settings settings( CharString( pApp->m_ServerKey ) );

			MirrorClient mirror;
			if ( mirror.open( settings.get( "MirrorAddress", "mirror-server.darkspace.net"), 
				settings.get( "MirrorPort", 9101), ".\\", false ) ) 
			{
				dword crc = mirror.getCRC();
				if ( crc != 0 && crc != mirror.getLocalCRC() )
				{
					// set the signal so all running server processes will begin their update
					serverUpdate.signal();
				}
				mirror.close();
			}
		}
#endif
		CDialog::OnTimer(nIDEvent);
	}
	else if ( nIDEvent == 0x3 )
	{
		m_Server.update();
		CDialog::OnTimer(nIDEvent);
	}

}

void CServerDlg::OnStart() 
{
	OnStop();

	CServerApp * pApp = (CServerApp *)AfxGetApp();
	ASSERT( pApp );

	Settings settings( CharString(pApp->m_ServerKey) );

	// initialize the logging first thing before we do anything else..
	if ( settings.get( "enableLog", 1 ) != 0 )
	{
		std::string logFile( settings.get( "logFile", "Server.log" ) );
		std::string logExclude( settings.get( "logExclude", "" ) );
		unsigned int nMinLogLevel = settings.get( "logLevel", LL_STATUS );
		m_pFileReactor = new FileReactor( logFile, nMinLogLevel, logExclude );
	}

	m_nShutdownTime = settings.get( "shutdownTime", 60 * 5 );

	GameServer::Context serverContext;
	serverContext.nServerFlags = settings.get( "serverFlags", (dword)0 );
	serverContext.nClientFlags = settings.get( "clientFlags", (dword)0 );
	serverContext.nModFlags = settings.get( "modFlags", MetaClient::MODERATOR|MetaClient::ADMINISTRATOR|MetaClient::DEVELOPER );
	//serverContext.bMaster = settings.get( "master", 1 ) != 0;
	//serverContext.sMasterAddress = settings.get( "masterAddress", "" );
	//serverContext.nMasterPort = settings.get( "masterPort", 9000 );
	serverContext.bPublish = settings.get( "publish", 1 ) != 0;
	serverContext.sMetaAddress = settings.get( "metaAddress", "meta-server.darkspace.net" );
	serverContext.nMetaPort = settings.get( "metaPort", 9000 );
	serverContext.nUniverseContext = WidgetKey( settings.get( "context", "15530459713961322492" ) );
	serverContext.maxPlayers = settings.get( "maxPlayers", 32 );
	serverContext.maxRank = settings.get( "maxRank", CHIEF_MARSHAL );
	serverContext.bBalanced = settings.get( "balanced", 1 ) != 0;
	serverContext.bClan = settings.get( "clan", 1 ) != 0;
	serverContext.bTutorial = settings.get( "tutorial", (dword)0 ) != 0;
	serverContext.nSessionId = settings.get( "sessionId", (dword)0 );
	serverContext.gameId = settings.get( "gameId", 1 );
	
	serverContext.sLogAddress = settings.get( "logAddress", "" );
	serverContext.nLogPort = settings.get( "logPort", 10000 );

	serverContext.name = settings.get( "name", "GameServer" );
	serverContext.description = settings.get( "description", "" );
	serverContext.address = settings.get( "address", "" );
	serverContext.port = settings.get( "port", 9001 );
	serverContext.maxClients = settings.get( "maxClients", 32 );
	serverContext.nCompression = settings.get( "compression", 6 );
	serverContext.nDisconnectTime = settings.get( "disconnectTime", 120 );
	serverContext.runScript = settings.get( "runScript", "" );
	serverContext.bPersistant = settings.get( "persistant", (dword)0 ) != 0;
	serverContext.sSaveFile = settings.get( "saveFile", ".\\GameServer.wob" );
	serverContext.sStorage = settings.get( "storage", ".\\storage\\" );
	serverContext.bEditor = false;
	
	serverContext.bUseShipFlags = settings.get( "useShipFlags", (dword)0 ) != 0;
	serverContext.sShipFlags = settings.get( "shipFlags", "" );

	if ( settings.get( "trusted", (dword)0 ) )
	{
		serverContext.user = settings.get( "user", "" );
		serverContext.password = settings.get( "password", "" );
	}
	else
	{
		serverContext.user = "DSS";
		serverContext.password = "darkspace";
	}

	// this is DarkSpace specific, so it's not passed in using the Context object.
	GameServer::sm_nAlwaysSpawnFlags = settings.get( "alwaysSpawnFlags", MetaClient::ADMINISTRATOR|MetaClient::DEVELOPER );
	GameServer::sm_bEnableTeamSpawners = settings.get("enableTeamSpawners", 1 ) != 0;

	// start the server
	if ( m_Server.start( serverContext ) )
	{
		// update the dialog
		OnTimer( 0x0 );
		// enable restarting, if the server stops, will we restart it automatically
		m_bEnableRestart = true;

		SetTimer( 0x3, 50, NULL );
	}
	else
	{
		MessageBox( _T("Failed to start server, please check the settings!") );
		//OnSettings();
	}
}

void CServerDlg::OnStop() 
{
	if ( m_Server.active() )
	{
		if ( MessageBox( _T("Are you sure you want to stop the server?"), _T("Stop Server"), MB_YESNO) == IDYES )
		{
			m_Status = _T("Stopping...");
			UpdateData( false );

			KillTimer( 0x3 );

			m_Server.stop();
			// turn off auto-restart
			m_bEnableRestart = false;
			// stop logging..
			if ( m_pFileReactor != NULL )
			{
				delete m_pFileReactor;
				m_pFileReactor = NULL;
			}
		}
	}
	OnTimer( 0x0 );
}

void CServerDlg::OnSettings() 
{
	//OnStop();

	//// bring up the settings dialog only if the server is not active ... if we enum the GameContext objects, we
	//// will interfer with the GameContext object being used by the server if it is still running
	//if ( ! m_Server.active() )
	CSettings().DoModal();
}

void CServerDlg::OnDestroy() 
{
	CDialog::OnDestroy();
}

void CServerDlg::OnSend() 
{
	if ( UpdateData( true ) )
	{
		if ( m_Message.GetLength() > 0 )
		{
			if ( m_Message[0] == '/' )
			{
				// create a dummy client context with all flags..
				GameServer::ClientContext context;
				context.nUserId = 0;
				context.nClientId = 0;
				context.MetaProfile.flags = MetaClient::DEVELOPER|MetaClient::ADMINISTRATOR|MetaClient::MODERATOR;

				m_Server.processCommand( context, m_Message );
			}
			else
			{
				// send message to all clients prefixed with "SERVER"
				m_Server.sendChat( CharString().format("SERVER: %s", m_Message) );
			}

			m_Message = "";
			UpdateData( false );
		}
	}
}

//----------------------------------------------------------------------------

void CServerDlg::RestartServer()
{
	// stop this server firstly
	m_Server.stop();
	// send this dialog with the IDOK, this will cause the server to restart
	EndDialog( IDOK );
}


//----------------------------------------------------------------------------

void CServerDlg::OnDisconnect() 
{
	int selected = m_Players.GetNextItem( -1, LVNI_SELECTED );
	if ( selected < 0 )
		return;

	dword clientId = m_Players.GetItemData( selected );
	
	m_Server.disconnect( clientId );
	m_Players.DeleteItem( selected );
}

void CServerDlg::OnClose() 
{
	// stop the server
	OnStop();

	if ( !m_Server.active() )
		CDialog::OnClose();
}
