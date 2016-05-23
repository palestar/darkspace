// PageVideo.cpp : implementation file
//

#include "stdafx.h"
#include "DarkSpaceSetup.h"
#include "PageVideo.h"

#include "Display/DisplayDevice.h"
#include "Standard/Settings.h"



#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageVideo property page

IMPLEMENT_DYNCREATE(CPageVideo, CPropertyPage)

CPageVideo::CPageVideo() : CPropertyPage(CPageVideo::IDD)
, m_bWindowed(FALSE)
, m_bHDR(FALSE)
, m_nBloomScale(0)
, m_sBloomScale(_T(""))
, m_bSoftwareCursor(FALSE)
, m_bEnableShadows(TRUE)
, m_bUseFixedFunction(FALSE)
{
	//{{AFX_DATA_INIT(CPageVideo)
	m_AutoConfigure = -1;
	m_MissilePercent = _T("");
	m_ParticlePercent = _T("");
	m_ShipPercent = _T("");
	m_ShipTrailLength = 0;
	m_MissileTrailLength = 0;
	m_ParticleDetail = 0;
	m_EnableSpecular = FALSE;
	m_EnableLightmaps = FALSE;
	m_WaitVB = FALSE;
	m_AlphaFonts = FALSE;
	//}}AFX_DATA_INIT

	m_psp.dwFlags &= ~PSP_HASHELP;
}

CPageVideo::~CPageVideo()
{
}

void CPageVideo::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageVideo)
	DDX_Control(pDX, IDC_COMBO6, m_TextureDetail);
	DDX_Control(pDX, IDC_COMBO2, m_DisplayModes);
	DDX_CBIndex(pDX, IDC_COMBO3, m_AutoConfigure);
	DDX_Text(pDX, IDC_MISSILE_PERCENT, m_MissilePercent);
	DDX_Text(pDX, IDC_PARTICLE_PERCENT, m_ParticlePercent);
	DDX_Text(pDX, IDC_SHIPTRAIL_PERCENT, m_ShipPercent);
	DDX_Slider(pDX, IDC_SLIDER1, m_ShipTrailLength);
	DDX_Slider(pDX, IDC_SLIDER4, m_MissileTrailLength);
	DDX_Slider(pDX, IDC_SLIDER5, m_ParticleDetail);
	DDX_Check(pDX, IDC_CHECK2, m_EnableSpecular);
	DDX_Check(pDX, IDC_CHECK3, m_EnableLightmaps);
	DDX_Check(pDX, IDC_CHECK4, m_WaitVB);
	DDX_Check(pDX, IDC_CHECK5, m_AlphaFonts);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO7, m_GraphicsDetail);
	DDX_Control(pDX, IDC_COMBO9, m_ShadowDetail);
	DDX_Check(pDX, IDC_CHECK6, m_bWindowed);
	DDX_Check(pDX, IDC_CHECK1, m_bHDR);
	DDX_Slider(pDX, IDC_SLIDER6, m_nBloomScale);
	DDX_Text(pDX, IDC_MISSILE_PERCENT2, m_sBloomScale);
	DDX_Control(pDX, IDC_COMBO8, m_cFSAA);
	DDX_Control(pDX, IDC_COMBO10, m_cAF);
	DDX_Check(pDX, IDC_CHECK8, m_bSoftwareCursor);
	DDX_Check(pDX, IDC_CHECK9, m_bEnableShadows);
	DDX_Check(pDX, IDC_CHECK10, m_bUseFixedFunction);
}


BEGIN_MESSAGE_MAP(CPageVideo, CPropertyPage)
	//{{AFX_MSG_MAP(CPageVideo)
	ON_CBN_SELCHANGE(IDC_COMBO3, OnAutoConfigure)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageVideo message handlers

BOOL CPageVideo::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
#ifdef _DEBUG
	Settings settings( "ClientD" );
#else
	Settings settings( "Client" );
#endif

	// read settings from the registry
	CString displayMode = settings.get( "displayMode", "" );
	CString fsaa = settings.get( "FSAA", "FSAA_NONE" );
	int af = settings.get( "AF", 1 );

	m_bWindowed = settings.get( "windowed", (dword)0 );
	m_bHDR = settings.get( "HDR", 1 );
	m_EnableSpecular = settings.get( "enableSpecular", 1 );
	m_EnableLightmaps = settings.get( "enableLightmap", 1 );
	m_WaitVB = settings.get( "waitVB", (dword)0 );
	m_bSoftwareCursor = settings.get( "softwareCursor", (dword)0 );
	m_AlphaFonts = settings.get( "alphaFonts", 1 );
	m_ShipTrailLength = settings.get( "shipTrails", 100 );
	m_MissileTrailLength = settings.get( "missileTrails", 100 );
	m_ParticleDetail = settings.get( "particleEffects", 100 );
	m_nBloomScale = settings.get( "bloomScale", 100 );

	m_MissilePercent.Format( _T("%d%%"), m_MissileTrailLength );
	m_ShipPercent.Format( _T("%d%%"), m_ShipTrailLength );
	m_ParticlePercent.Format( _T("%d%%"), m_ParticleDetail );
	m_sBloomScale.Format( _T("%d%%"), m_nBloomScale );

	CString textureDetail = settings.get( "textureDetail", "High" );
	m_TextureDetail.SelectString( -1, textureDetail );

	m_GraphicsDetail.SetCurSel( settings.get( "graphicsDetail", 1 ) );

	m_bEnableShadows = settings.get( "enableShadows", 1 );
	CString shadowDetail = settings.get( "shadowDetail", "High" );
	m_ShadowDetail.SelectString( -1, shadowDetail );

	// update the display modes
	DisplayDevice::Ref device = DisplayDevice::create( "DisplayDeviceD3D" );
	if (! device )
		return FALSE;

	m_bUseFixedFunction = settings.get( "useFixedFunction", 
		device->pixelShaderVersion() < 3.0f || device->vertexShaderVersion() < 3.0f );

	m_DisplayModes.ResetContent();
	for(int i=0;i<device->modeCount();i++)
	{
		const DisplayDevice::Mode * pMode = device->mode(i);

		// exclude certain graphics modes
		if ( pMode->colorFormat == ColorFormat::INVALID )
			continue;
		if ( pMode->screenSize.width < 800 )
			continue;
		if ( pMode->screenSize.height < 600 )
			continue;

		int comboIndex = m_DisplayModes.AddString( CString( pMode->modeDescription ) );
		if ( strstr( pMode->modeDescription, "1024x768x16" ) || 
			strstr( pMode->modeDescription, "1024x768x32") )
		{
			m_DisplayModes.SetCurSel( comboIndex );
		}
	}

	// select the current display mode
	if ( displayMode.GetLength() > 0 )
		m_DisplayModes.SelectString(-1, displayMode );

	// setup the FSAA combo
	m_cFSAA.ResetContent();
	for(int i=0;i<DisplayDevice::FSAA_COUNT;++i)
		m_cFSAA.AddString( DisplayDevice::describeFSAA( (DisplayDevice::FSAA)i ) );
	m_cFSAA.SelectString( -1, fsaa );

	// setup AF
	m_cAF.ResetContent();
	for ( int i=1; i<17; i*=2 )
		m_cAF.AddString( CharString().format( "%u", i ) );
	m_cAF.SelectString( -1, CharString().format( "%u", af ) );

	UpdateData( false );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageVideo::OnOK() 
{
	if ( UpdateData( true ) )
	{
#ifdef _DEBUG
		Settings settings( "ClientD" );
#else
		Settings settings( "Client" );
#endif

		CString displayMode;
		m_DisplayModes.GetLBText( m_DisplayModes.GetCurSel(), displayMode );
		if ( displayMode.IsEmpty() )
		{
			MessageBox( "Please select a valid Display Mode", "Invalid Display Mode!" );
			return;
		}
		CString textureDetail;
		m_TextureDetail.GetLBText( m_TextureDetail.GetCurSel(), textureDetail );
		
		CString fsaa;
		m_cFSAA.GetLBText( m_cFSAA.GetCurSel(), fsaa );

		CString af;
		m_cAF.GetLBText( m_cAF.GetCurSel(), af );

		int nGraphicsDetail = m_GraphicsDetail.GetCurSel();
		if ( nGraphicsDetail >= 0 && nGraphicsDetail <= 2 )
			settings.put( "graphicsDetail", nGraphicsDetail );

		CString shadowDetail;
		m_ShadowDetail.GetLBText( m_ShadowDetail.GetCurSel(), shadowDetail );

		settings.put( "windowed", m_bWindowed );
		settings.put( "HDR", m_bHDR );
		settings.put( "displayMode", (CharString)displayMode );
		settings.put( "textureDetail", (CharString)textureDetail );
		settings.put( "useFixedFunction", (dword)m_bUseFixedFunction );
		settings.put( "enableShadows", (dword)m_bEnableShadows );
		settings.put( "shadowDetail", (CharString)shadowDetail );
		settings.put( "FSAA", (CharString)fsaa );
		int nAF = atoi(af); // convert to int before we save out
		settings.put( "AF", (dword)nAF );
		settings.put( "enableSpecular", dword( m_EnableSpecular ));
		settings.put( "enableLightmap", dword( m_EnableLightmaps ));
		settings.put( "waitVB", dword(m_WaitVB ));
		settings.put( "softwareCursor", (dword)m_bSoftwareCursor );
		settings.put( "alphaFonts", dword(m_AlphaFonts));
		settings.put( "shipTrails", dword(m_ShipTrailLength));
		settings.put( "missileTrails", dword(m_MissileTrailLength));
		settings.put( "particleEffects", dword(m_ParticleDetail));
		settings.put( "bloomScale", (dword)m_nBloomScale );

		CPropertyPage::OnOK();
	}
}

void CPageVideo::OnAutoConfigure() 
{
	if ( UpdateData( true ) )
	{
		switch( m_AutoConfigure )
		{
		case 0:		// Highest
			m_bHDR = true;
			m_EnableSpecular = true;
			m_EnableLightmaps = true;
			m_WaitVB = false;
			m_AlphaFonts = true;
			m_ShipTrailLength = 100;
			m_MissileTrailLength = 100;
			m_ParticleDetail = 100;
			m_nBloomScale = 100;
			m_TextureDetail.SelectString( -1, _T("HIGH") );
			m_GraphicsDetail.SelectString( -1, _T("HIGH") );
			m_ShadowDetail.SelectString( -1, _T("EXTREME") );
			break;
		case 1:		// Fast
			m_bHDR = true;
			m_EnableSpecular = true;
			m_EnableLightmaps = true;
			m_WaitVB = false;
			m_AlphaFonts = true;
			m_ShipTrailLength = 100;
			m_MissileTrailLength = 75;
			m_ParticleDetail = 95;
			m_nBloomScale = 100;
			m_TextureDetail.SelectString( -1, _T("HIGH") );
			m_GraphicsDetail.SelectString( -1, _T("MEDIUM") );
			m_ShadowDetail.SelectString( -1, _T("HIGH") );
			break;
		case 2:		// Medium
			m_bHDR = false;
			m_EnableSpecular = false;
			m_EnableLightmaps = true;
			m_WaitVB = false;
			m_AlphaFonts = true;
			m_ShipTrailLength = 75;
			m_MissileTrailLength = 50;
			m_ParticleDetail = 50;
			m_nBloomScale = 100;
			m_TextureDetail.SelectString( -1, _T("MEDIUM") );
			m_GraphicsDetail.SelectString( -1, _T("MEDIUM") );
			m_ShadowDetail.SelectString( -1, _T("MEDIUM") );
			break;
		case 3:		// Slow
			m_bHDR = false;
			m_EnableSpecular = false;
			m_EnableLightmaps = false;
			m_WaitVB = false;
			m_AlphaFonts = false;
			m_ShipTrailLength = 25;
			m_MissileTrailLength = 25;
			m_ParticleDetail = 25;
			m_nBloomScale = 100;
			m_TextureDetail.SelectString( -1, _T("LOW") );
			m_GraphicsDetail.SelectString( -1, _T("LOW") );
			m_ShadowDetail.SelectString( -1, _T("LOW") );
			break;
		}

		m_MissilePercent.Format( _T("%d%%"), m_MissileTrailLength );
		m_ShipPercent.Format( _T("%d%%"), m_ShipTrailLength );
		m_ParticlePercent.Format( _T("%d%%"), m_ParticleDetail );

		UpdateData( false );
	}
}

void CPageVideo::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);

	if( UpdateData( true ) )
	{
		m_MissilePercent.Format( _T("%d%%"), m_MissileTrailLength );
		m_ShipPercent.Format( _T("%d%%"), m_ShipTrailLength );
		m_ParticlePercent.Format( _T("%d%%"), m_ParticleDetail );
		m_sBloomScale.Format( _T("%d%%"), m_nBloomScale );

		UpdateData( false );
	}
}
