// PageAudio.cpp : implementation file
//

#include "stdafx.h"
#include "DarkSpaceSetup.h"
#include "PageAudio.h"
#include "DirDialog.h"

#include "Audio/AudioDevice.h"
#include "Standard/Settings.h"
#include "System/LocalizedString.h"



#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageAudio property page

IMPLEMENT_DYNCREATE(CPageAudio, CPropertyPage)

CPageAudio::CPageAudio() : CPropertyPage(CPageAudio::IDD)
{
	//{{AFX_DATA_INIT(CPageAudio)
	m_SoundVolume = 0;
	m_MusicVolume = 0;
	m_SoundVolumePercent = _T("");
	m_MusicVolumePercent = _T("");
	m_MusicDirectory = _T("");
	//}}AFX_DATA_INIT

	m_psp.dwFlags &= ~PSP_HASHELP;
}

CPageAudio::~CPageAudio()
{
}

void CPageAudio::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageAudio)
	DDX_Control(pDX, IDC_COMBO7, m_AudioVoices);
	DDX_Control(pDX, IDC_COMBO5, m_AudioChan);
	DDX_Control(pDX, IDC_COMBO4, m_AudioKhz);
	DDX_Slider(pDX, IDC_SLIDER2, m_SoundVolume);
	DDX_Slider(pDX, IDC_SLIDER3, m_MusicVolume);
	DDX_Text(pDX, IDC_SFXVOLUME, m_SoundVolumePercent);
	DDX_Text(pDX, IDC_MUSICVOLUME, m_MusicVolumePercent);
	DDX_Text(pDX, IDC_EDIT1, m_MusicDirectory);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageAudio, CPropertyPage)
	//{{AFX_MSG_MAP(CPageAudio)
	ON_BN_CLICKED(IDC_BUTTON1, OnBrowseMusic)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageAudio message handlers

BOOL CPageAudio::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
#ifdef _DEBUG
	Settings settings( "ClientD" );
#else
	Settings settings( "Client" );
#endif

	// read settings from the registry
	m_MusicDirectory = settings.get( "music", ".\\Music\\" );
	m_MusicVolume = settings.get( "musicVolume", 100 );
	m_SoundVolume = settings.get( "sfxVolume", 100 );

	m_MusicVolumePercent.Format( _T("%d%%"), m_MusicVolume );
	m_SoundVolumePercent.Format( _T("%d%%"), m_SoundVolume );

	dword audioKhz = settings.get( "audioKhz", 44100 );
	dword audioChan = settings.get( "audioChan", 2 );
	dword audioVoices = settings.get( "audioVoices", 32 );

	m_AudioKhz.SelectString(-1, (CString)CharString().format("%u", audioKhz ) );
	m_AudioChan.SelectString(-1, (CString)CharString().format("%u", audioChan ) );
	m_AudioVoices.SelectString( -1, (CString)CharString().format("%u", audioVoices ) );

	UpdateData( false );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageAudio::OnOK() 
{
	if ( UpdateData( true ) )
	{
#ifdef _DEBUG
		Settings settings( "ClientD" );
#else
		Settings settings( "Client" );
#endif

		CString audioKhz;
		m_AudioKhz.GetLBText( m_AudioKhz.GetCurSel(), audioKhz );
		CString audioChan;
		m_AudioChan.GetLBText( m_AudioChan.GetCurSel(), audioChan );
		CString audioVoices;
		m_AudioVoices.GetLBText( m_AudioVoices.GetCurSel(), audioVoices );

		settings.put( "musicVolume", m_MusicVolume );
		settings.put( "sfxVolume", m_SoundVolume );
		settings.put( "audioKhz", _ttoi( audioKhz ) );
		settings.put( "audioChan", _ttoi( audioChan ) );
		settings.put( "audioVoices", _ttoi( audioVoices ) );
		settings.put( "music", (CharString)m_MusicDirectory );

		CPropertyPage::OnOK();
	}
}

void CPageAudio::OnBrowseMusic() 
{
	if ( UpdateData( true ) )
	{
		CDirDialog dialog;
		dialog.m_strWindowTitle = _T("Music Directory");
		dialog.m_strTitle = _T("Select the music directory?");
		//dialog.m_strInitDir = m_MusicDirectory;
		
		if ( dialog.DoBrowse() )
		{
			m_MusicDirectory = dialog.m_strPath + _T("\\");
			UpdateData( false );
		}
	}
}

void CPageAudio::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);

	if( UpdateData( true ) )
	{
		m_MusicVolumePercent.Format( _T("%d%%"), m_MusicVolume );
		m_SoundVolumePercent.Format( _T("%d%%"), m_SoundVolume );

		UpdateData( false );
	}
}
