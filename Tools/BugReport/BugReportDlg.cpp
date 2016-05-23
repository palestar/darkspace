// BugReportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "afxinet.h"

#include "BugReport.h"
#include "BugReportDlg.h"
#include "ProgDlg.h"

#include "Standard/UniqueNumber.h"
#include "System/LocalizedString.h"
#include "File/Path.h"


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
// CBugReportDlg dialog

CBugReportDlg::CBugReportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBugReportDlg::IDD, pParent)
	, m_sEmail(_T(""))
{
	//{{AFX_DATA_INIT(CBugReportDlg)
	m_Description = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_sEmail = AfxGetApp()->GetProfileString( "CBugReportDlg", "m_sEmail", "" );
}

void CBugReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBugReportDlg)
	DDX_Control(pDX, IDC_LIST2, m_Attachments);
	DDX_Text(pDX, IDC_EDIT1, m_Description);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT2, m_sEmail);
	DDV_MaxChars(pDX, m_sEmail, 255);
}

BEGIN_MESSAGE_MAP(CBugReportDlg, CDialog)
	//{{AFX_MSG_MAP(CBugReportDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnAddAttachment)
	ON_BN_CLICKED(IDC_BUTTON2, OnRemoveAttachment)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBugReportDlg message handlers

BOOL CBugReportDlg::OnInitDialog()
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CWinApp * pApp = AfxGetApp();
	ASSERT( pApp );

	// the product name on the command line, let's us know where in our registry to find information on how to send this bug
	CString productName( pApp->m_lpCmdLine );
	if ( productName.IsEmpty() )
		productName = "Default";
	
	// automatically attach .log files
	CFileFind ff;

	BOOL found = ff.FindFile( _T("*.log") );
	while( found )
	{
		found = ff.FindNextFile();
		m_Attachments.InsertItem( 0, ff.GetFilePath() );
	}

	// attach any .dmp files...
	found = ff.FindFile( _T("*.dmp") );
	while( found )
	{
		found = ff.FindNextFile();
		m_Attachments.InsertItem( 0, ff.GetFilePath() );
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBugReportDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBugReportDlg::OnPaint() 
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
HCURSOR CBugReportDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CBugReportDlg::OnOK() 
{
	CWinApp * pApp = AfxGetApp();
	ASSERT( pApp );

	// get the data from the dialog
	UpdateData( true );

	// check the description length
	if ( m_Description.GetLength() < 1 )
	{
		MessageBox( _T("Please provide a description of the bug or problem!") );
		return;
	}

	// save email entered for next time.. 
	AfxGetApp()->WriteProfileString( "CBugReportDlg", "m_sEmail", m_sEmail );

	CProgressDlg progress;
	progress.Create( this );

	// the product name on the command line, let's us know where in our registry to find information on how to send this bug
	CString productName( pApp->m_lpCmdLine );
	if ( productName.IsEmpty() )
		productName = "DarkSpace";

	CString ftpAddress( "ftp.palestar.com" );
	CString ftpUser(  "bugs" );
	CString ftpPassword( "splat3483" );
	
	CString status;
	status.Format( _T("Connecting to %s ..."), ftpAddress );
	progress.SetStatus( status );

	// create a internet session object
	CInternetSession session;
	// get a connection to the ftp server
	try {
		//CFtpConnection * pFtp = session.GetFtpConnection( ftpAddress, ftpUser, ftpPassword, INTERNET_INVALID_PORT_NUMBER, true );
		CFtpConnection * pFtp = session.GetFtpConnection( ftpAddress, ftpUser, ftpPassword );
		
		status.Format( _T("Connected to %s ... Sending Bug Report.."), ftpAddress );
		progress.SetStatus( status );

		// change to the product directory
		if (! pFtp->SetCurrentDirectory( productName ) )
		{
			// create the product directory 
			if (! pFtp->CreateDirectory( productName ) ||
				! pFtp->SetCurrentDirectory( productName ) )
				AfxThrowInternetException( 0 );
		}

		CString bugDirectory( UniqueNumber().string() );
		if ( pFtp->CreateDirectory( bugDirectory ) &&
			pFtp->SetCurrentDirectory( bugDirectory ) )
		{
			CString sReport = _T("Email: ") + m_sEmail + _T("\r\n-------------------------------\r\n") + m_Description;

			CInternetFile * pDesc = pFtp->OpenFile( _T("Bug.txt"), GENERIC_WRITE );
			pDesc->Write( sReport, sReport.GetLength() );
			pDesc->Close();

			// send the attachments
			for(int i=0;i<m_Attachments.GetItemCount();i++)
			{
				Path attachment( (CharString)m_Attachments.GetItemText( i, 0 ) );
				
				status.Format( _T("Sending attachment '%s'..."), attachment.file() );
				progress.SetStatus( status );

				pFtp->PutFile( (CString)attachment.path(), (CString)attachment.file() );
			}

			pFtp->Close();

			// Remove all .dmp files after sending the report..
			CFileFind ff;
			BOOL bFound = ff.FindFile( _T("*.dmp") );
			while( bFound )
			{
				bFound = ff.FindNextFile();
				DeleteFile( ff.GetFilePath() );
			}
		}
		else
			AfxThrowInternetException( 0 );
		
		CDialog::OnOK();
	}
	catch( ... )
	{
		MessageBox( _T("An unknown error has occured while trying to send bug report; please try again later!") );
	}
}

void CBugReportDlg::OnAddAttachment() 
{
	CFileDialog dialog( true );
	if ( dialog.DoModal() == IDOK )
		m_Attachments.InsertItem( 0, dialog.GetPathName() );
}

void CBugReportDlg::OnRemoveAttachment() 
{
	for(int i=0;i<m_Attachments.GetItemCount();)
		if ( m_Attachments.GetItemState( i, LVIS_SELECTED ) )
			m_Attachments.DeleteItem( i );
		else
			i++;
}
