// ClientUpdate.cpp : implementation file
//

#include "stdafx.h"
#include "ClientUpdate.h"

#include "System/LocalizedString.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientUpdate dialog


CClientUpdate::CClientUpdate(const wchar * mirrorDirectory, 
							 const wchar * mirrorServer, int port, 
							 CWnd* pParent /*=NULL*/)
							: CDialog(CClientUpdate::IDD, pParent), m_Exit( false ), m_pUpdateThread( NULL )
{
	m_MirrorDirectory = mirrorDirectory;
	m_MirrorServer = mirrorServer;
	m_MirrorPort = port;

	// start the update thread
	m_pUpdateThread = new UpdateThread( this );
	m_pUpdateThread->resume();

	//{{AFX_DATA_INIT(CClientUpdate)
	m_FileStatus = _T("");
	m_Status = _T("");
	//}}AFX_DATA_INIT
}

CClientUpdate::~CClientUpdate()
{
	// set the exit flag to true
	m_Exit = true;
	// close the connection
	close();

	if ( m_pUpdateThread != NULL )
	{
		delete m_pUpdateThread;
		m_pUpdateThread = NULL;
	}
}


void CClientUpdate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientUpdate)
	DDX_Control(pDX, IDC_PROGRESS2, m_FileProgress);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Text(pDX, IDC_FILESTATUS, m_FileStatus);
	DDX_Text(pDX, IDC_STATUS, m_Status);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CClientUpdate, CDialog)
	//{{AFX_MSG_MAP(CClientUpdate)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientUpdate message handlers

BOOL CClientUpdate::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_Status.Format(_T("Connecting to '%s : %d'"), m_MirrorServer, m_MirrorPort );
	UpdateData( false );

	// start update event
	SetTimer( 0x2, 100, NULL );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CClientUpdate::OnTimer(UINT nIDEvent) 
{
	CDialog::OnTimer(nIDEvent);

	switch( nIDEvent )
	{
	case 0x2:	// update event
		{
			lock();

			if ( m_Bytes > 0 )
			{
				CString title;
				title.Format( _T("%d%% Complete..."), (m_BytesComplete * 100) / m_Bytes );

				SetWindowText( title );

				m_Progress.SetRange32( 0, m_Bytes / 1000 );
				m_Progress.SetPos( m_BytesComplete / 1000 );
			}

			if ( m_FileSize > 0 )
			{
				m_FileStatus.Format( _T("Downloading '%s', %d%% Complete"), 
					m_FileName, (m_FileReceived * 100) / m_FileSize );
				m_FileProgress.SetRange32( 0, m_FileSize / 1000 );
				m_FileProgress.SetPos( m_FileReceived / 1000 );
			}
			
			unlock();

			UpdateData( false );

			// check for exit
			if ( m_Exit )
			{
				// close the connection
				close();
				// end this dialog
				EndDialog( IDOK );
			}
		}
		break;
	}
}

void CClientUpdate::OnCancel() 
{
	// stop the update timer
	KillTimer( 0x2 );
	// set the exit flag to true, to avoid a message box when onError() is called
	m_Exit = true;
	// close the connection
	close();

	CDialog::OnCancel();
}

//----------------------------------------------------------------------------

void CClientUpdate::onStatus( const wchar * status )
{
	m_Status = status; 
}

void CClientUpdate::onError( const wchar * error )
{
	if (!m_Exit)
	{
		::MessageBox( m_hWnd, CString( error ), _T("Mirror Client"), MB_OK );
		m_Exit = true;
	}
}

void CClientUpdate::onDone()
{
	m_Exit = true;
}

//----------------------------------------------------------------------------

CString	CClientUpdate::formatBytes( dword bytes )
{
	CString string;

	if ( bytes > 1000000 )
		string.Format( _T("%.1f mb"), float( bytes ) / 1000000.0f );
	else
		string.Format( _T("%.1f k"), float(bytes) / 1000.0f );

	return string;
}

void CClientUpdate::doUpdate()
{
	if (! open( String(m_MirrorServer), m_MirrorPort, String(m_MirrorDirectory) ) )
	{
		// failed to open and the done flag not set means we didn't get the connection
		MessageBox( _T("Failed to connect to server; server may be down. Try again later!") );
		m_Exit = true;
		return;
	}
	if ( getCRC() == getLocalCRC() )
	{
		// files have not changed since last update
		m_Exit = true;
		return;
	}
	if (! getCatalog() )
	{ 
		MessageBox( _T("Failed to get catalog from server!"));
		m_Exit = true;
		return;
	}

	if (! getFiles() )
		onDone();
}

//----------------------------------------------------------------------------
