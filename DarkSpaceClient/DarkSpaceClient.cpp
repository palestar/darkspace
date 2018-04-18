// Client.cpp : Defines the class behaviors for the application.
//

#define MEDUSA_TRACE_ON

#include "stdafx.h"
#include "DarkSpaceClient.h"
#include "ClientUpdate.h"
#include "SelectTutorial.h"
#include "ProgDlg.h"

#include "Debug/Trace.h"
#include "Debug/Assert.h"
#include "Debug/ExceptionHandler.h"
#include "Display/PrimitiveMaterial.h"
#include "Display/PrimitiveSurface.h"

#include "Standard/CommandLine.h" 
#include "Standard/Process.h"
#include "Standard/CharString.h"
#include "Standard/Types.h"
#include "Standard/Settings.h"
#include "Standard/Timer.h"
#include "Standard/Bits.h"
#include "File/FileDisk.h"
#include "Factory/BrokerFolder.h"
#include "System/Platform.h"
#include "Render3D/NodeParticleSystem.h"
#include "Render3D/NodeStarField.h"
#include "Render3D/NodeBoolean.h"
#include "Render3D/NodeComplexMesh2.h"
#include "DarkSpace/NounProjectile.h"
#include "DarkSpace/Trail.h"
#include "DarkSpace/VerbDistress.h"
#include "DarkSpace/NounPod.h"
#include "Interface/GameDocument.h"
#include "Interface/ViewGame.h"


//#include "SelfUpdate/ClientUpdate.h"

#pragma warning( disable: 4800 )

//----------------------------------------------------------------------------

const char * DEFAULT_INTERFACE = "Interface.prt";

#ifdef _DEBUG
const char * LOBBY_EXE = "GCQLD.exe";
const char * CLIENT_SETUP_EXE = "DarkSpaceSetupD.exe";
const char * BUG_REPORT_EXE = "BugReportD.exe DarkSpace";
const char * LOCAL_SERVER_EXE = "LocalServerD.exe";
#else
const char * LOBBY_EXE = "GCQL.exe";
const char * CLIENT_SETUP_EXE = "DarkSpaceSetup.exe";
const char * BUG_REPORT_EXE = "BugReport.exe DarkSpace";
const char * LOCAL_SERVER_EXE = "LocalServer.exe";
#endif

static CharString MakeAddress( const char * pAddress, int nPort )
{
	return CharString().format( "%s:%d", pAddress, nPort );
}

static void ParseAddress( CharString & sAddress, int & nPort )
{
	int nSeperator = sAddress.find( ':' );
	if ( nSeperator > 0 )
	{
		nPort = atoi( sAddress.cstr() + nSeperator + 1 );
		sAddress.left( nSeperator );
	}
}

//----------------------------------------------------------------------------

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientApp

BEGIN_MESSAGE_MAP(CClientApp, CWinApp)
	//{{AFX_MSG_MAP(CClientApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientApp construction

CClientApp::CClientApp()
{
	//m_RunLobbyOnExit = true;
	m_SessionId = 0;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CClientApp object

CClientApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CClientApp initialization

// Main game loop, and code to limit the FPS to reduce CPU usage
void CClientApp::RunGame( InterfaceContext * pInterface )
{
#ifndef _DEBUG
	__try {
#endif
		GameDocument * pDoc = WidgetCast<GameDocument>(pInterface->document());
		float fSVolume = 0.0f;
		int fMVolume = 0;
		HWND hWnd = NULL;
		// THE MAIN GAME LOOP 
		while (pInterface->render())
		{	
			// get our main window
			hWnd = FindWindowA("DarkSpace",NULL);

			// update our saved values if the volume is changed
			if (pInterface->audio()->volume() > 0)
				fSVolume = pInterface->audio()->volume();
			if (pDoc->jukeBox()->volume() > 0)
				fMVolume = pDoc->jukeBox()->volume();

			// mute our device if we lose focus
			if (hWnd != GetForegroundWindow())
			{
				pInterface->audio()->setVolume(0.0f);
				pDoc->jukeBox()->setVolume(0.0f);
			}
			else
			{
				pInterface->audio()->setVolume(fSVolume);
				pDoc->jukeBox()->setVolume(fMVolume);
			}
		};

#ifndef _DEBUG
	}
	__except( ProcessException( GetExceptionInformation() ) )
	{
		m_ErrorMessage = _T("An unknown error occured while running DarkSpace, please send in bug report!");
		m_Error = true;
	}
#endif
}

int CClientApp::StartGame()
{
#ifdef _DEBUG
	// this stops the massive memory dumps on program exit..
	_CrtSetReportMode(_CRT_WARN, 0);
#endif

	m_Error = false;
	SetProcessErrorMode(EM_DIALOG);

#ifdef _DEBUG
	Settings settings("ClientD");
#else
	Settings settings("Client");
#endif

#if !defined(_DEBUG)
	// BEFORE we initialize the loggin, check if we need to send a bug report before the client log gets cleared
	if (settings.get("exit", (dword)0) != 0)
	{
		// program did not exit properly, display dialog so the user can submit a bug report
		Process::wait(Process::start(BUG_REPORT_EXE));
		settings.put("exit", (dword)0);
	}
#endif

	// initialize logging first thing...
	std::string sLogFile(settings.get("logFile", "Client.log"));
	std::string sLogExclude(settings.get("logExclude", ""));
	unsigned int nLogLevel = settings.get("logLevel", LL_DEBUG_HIGH);

	FileReactor fileReactor(sLogFile, nLogLevel, sLogExclude,
		FileReactor::O_DEFAULTS | FileReactor::O_RESET);

#ifdef _DEBUG
	//DebugReactor debugReactor;
#endif

	// initialize the localized text system, this will force the locale.txt file
	// to be loaded from disk.
	LocalizedString::locale();


	int CPUInfo[4] = { -1 };
	unsigned   nExIds, i = 0;
	char CPUBrandString[0x40];
	// Get the information associated with each extended ID.
	__cpuid(CPUInfo, 0x80000000);
	nExIds = CPUInfo[0];
	for (i = 0x80000000; i <= nExIds; ++i)
	{
		__cpuid(CPUInfo, i);
		// Interpret CPU brand string
		if (i == 0x80000002)
			memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000003)
			memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000004)
			memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
	}
	//string includes manufacturer, model and clockspeed
	LOG_STATUS("SysInfo", CharString().format("CPU Type: %s", (CharString)CPUBrandString));

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	LOG_STATUS("SysInfo", CharString().format("Number of Cores: %d", sysInfo.dwNumberOfProcessors));

	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	LOG_STATUS("SysInfo", CharString().format("Total System Memory: %d MB", (statex.ullTotalPhys / 1024) / 1024));

	bool bCPUFail = true;

	if (strstr(CPUBrandString, "AMD Ryzen"))
		bCPUFail = false;
	if (strstr(CPUBrandString, "Intel") && strstr(CPUBrandString, "Core"))
		bCPUFail = false;

	if (!bCPUFail)
		LOG_STATUS("SysInfo", "Multicore supported...");
	else
		LOG_STATUS("SysInfo", "Multicore not supported on this platform...");

	// default to a single core unless someone forces multi-core support on..
	if ( settings.get( "ForceMultiCore", (dword)0) == 0 || bCPUFail )
	{
		// Force ALL threads to run on the first core found..
		DWORD nProcessAffMask = 0, nSystemAffMask = 0;
		if (! GetProcessAffinityMask( GetCurrentProcess(), &nProcessAffMask, &nSystemAffMask ) )
			THROW_EXCEPTION( "Failed to get process affinity!" );
		
		DWORD nNewProcessAffMask = 1 << GetFirstBit( nProcessAffMask );
		if (! SetProcessAffinityMask( GetCurrentProcess(), nNewProcessAffMask ) )
			THROW_EXCEPTION( "Failed to set process affinity to core 1" );
	}

	CharString sCommandLine = m_lpCmdLine;
	CommandLine commands( sCommandLine );

	// pre-update options
	int nOption = -1;
	if ( (nOption = commands.find( "-setup" )) >= 0 ||
		(nOption = commands.find( "setup" )) >= 0 )
	{
		//m_RunLobbyOnExit = false;
		Process::start( CLIENT_SETUP_EXE );
		return FALSE;
	}

	SetRegistryKey( _T("Palestar") );

	CProgressDlg progress;
	progress.Create();

	// LocalServer.exe will signal this event after it has started
	Event serverStarted( "serverStart" );

#ifdef _DEBUG
	progress.SetStatus( _T("Saving constants...") );

	// load the constants
	Settings constants( "Constants", ".\\Constants.ini" );
	for(int i=0;i<Constant::constantCount();i++)
	{
		Constant * pConstant = Constant::constant( i );
		ASSERT( pConstant );

		constants.put( pConstant->name(), 
			CharString().format("%f", pConstant->value()) );

		TRACE( CharString().format("%s = %f", pConstant->name(), pConstant->value()) );
	}
#else
	progress.SetStatus( _T("Loading constants...") );

	// load the constants
	Settings constants( "Constants", ".\\Constants.ini" );
	for(int i=0;i<Constant::constantCount();i++)
	{
		Constant * pConstant = Constant::constant( i );
		ASSERT( pConstant );

		CharString value( constants.get( pConstant->name(), "" ) );
		if ( value.length() > 0 )
			(*pConstant) = strtod( value, NULL );
	}
#endif

	// make sure the settings are valid, run ClientSetup.exe if necessary
	if ( settings.get( "setup", (dword)0 ) != 3 )
	{
		progress.SetStatus( _T("Running setup...") );
		void * setup = Process::start( CLIENT_SETUP_EXE );
		if ( setup == NULL )
		{
			MessageBox( NULL, _T("Failed to run 'DarkSpaceSetup.exe'"), _T("Failed"), MB_OK );
			return 0;
		}

		if ( Process::wait( setup ) < 0 )
			return 0;		// user canceled setup, exit

		settings.put( "setup", 3 );
	}

	// enable the object cache..
	Broker::sm_bEnableWidgetCache = true;

	progress.SetStatus( _T("Opening data folder...") );
	// open the data files
	BrokerFolder theData;
	if (! theData.open( settings.get( "data", ".\\data\\" ), false, false ) )
	{
		MessageBox( NULL, _T("Failed to open the data directory!"), _T("Failed"), MB_OK );
		return 0;
	}

	// parse the command line options..
	int nLastOption = 0;
	for(int nArg=0;nArg<commands.argumentCount();)
	{
		const char * pArg = commands.argument( nArg );
		if ( _stricmp( pArg, "-tutorial" ) == 0 || 
			_stricmp( pArg, "tutorial" ) == 0 )				// depreciated command
		{
			CSelectTutorial dialog;
			if ( dialog.DoModal() != IDOK )
				return 0;

			m_sAddress = "localhost";
			m_nPort = 9001;

			commands.append( CommandLine( CharString().format("-run %s", dialog.m_Selected.string()) ) );
			nArg += 1;
		}
		else if ( _stricmp( pArg, "-run" ) == 0 && commands.isValid( nArg + 1 ) )
		{
			m_sAddress = "localhost";
			m_nPort = 9001;

			progress.SetStatus( _T("Starting local server...") );

			void * hServer = Process::start( CharString().format("%s %s", LOCAL_SERVER_EXE, commands.argument(nArg + 1)) );
			// wait for server to startup
			while( serverStarted.wait( 1000 ) )
			{
				if ( !Process::active( hServer ) )
				{
					MessageBox( NULL, _T("Failed to start local server!"), _T("Failed"), MB_OK );
					return 0;
				}
			}
			nArg += 2;
		}
		else if ( _stricmp( pArg, "-connect" ) == 0 && commands.isValid( nArg + 2 ) )
		{
			m_sAddress = commands.argument( nArg + 1);
			m_nPort = commands.argumentInt( nArg + 2 );
			nArg += 3;
		}
		else if ( _stricmp( pArg, "-login" ) == 0 && commands.isValid( nArg + 2 ) )
		{
			m_SessionId = MetaClient::getSessionId( commands.argument( nArg + 1 ), 
				commands.argument( nArg + 2 ) ); 
			if ( m_SessionId == 0 )
			{
				MessageBox( NULL, _T("Failed to login to server!"), _T("Failed"), MB_OK );
				return 0;
			}
			nArg += 3;
		}

		if ( nLastOption == nArg )
			break;			// no option was found, stop parsing for options...
		nLastOption = nArg;
	}

	// nLastOption is not really the last option, but the first argument after the options..
	// last arguments may be <address> <port> <session>
	int nStart = nLastOption;
	if ( commands.isValid( nStart + 2 ) )
	{
		m_sAddress = commands.argument( nStart );
		m_nPort = commands.argumentInt( nStart + 1 );
		m_SessionId = commands.argumentDWORD( nStart + 2 );
	}

	// add search paths
	FileDisk::resetSearchPaths();
	FileDisk::addSearchPath(".\\");

	progress.SetStatus( _T("Loading interface...") );
	// load the interface
	InterfaceContext::Link pInterface;
	if (! loadResource( settings.get( "interface", DEFAULT_INTERFACE ) , pInterface ) )
	{
		MessageBox( NULL, _T("Failed to load the interface!"), _T("Failed"), MB_OK );
		return 0;
	}

	progress.SetStatus( _T("Connecting to server...") );

	// get the GameClient object
	GameDocument * pDoc = WidgetCast<GameDocument>( pInterface->document() );
	if ( pDoc == NULL )
	{
		MessageBox( NULL, _T("Invalid document object!"), _T("Error"), MB_OK );
		return 0;
	}

	CharString sFallbackAddress = m_sAddress;
	int	       nFallbackPort = m_nPort;

	CharString sLastAddress( settings.get( "lastAddress", "" ) );
	if ( sLastAddress == MakeAddress( m_sAddress, m_nPort ) )
	{
		CharString sTransferAddress = settings.get( "transferAddress", "" );
		if ( sTransferAddress.length() > 0 )
		{
			// connect to our last server we had been transfered too, this will avoid 
			// issues with players relogging and not being able to get back into the game
			// until their storage gets unlocked.
			m_sAddress = sTransferAddress;
			ParseAddress( m_sAddress, m_nPort );
		}
	}
	else
	{
		// update our last address..
		settings.put( "lastAddress", MakeAddress( m_sAddress, m_nPort ) );
		settings.put( "transferAddress", "" );		// clear the transfer address as well
	}

	GameClient * pClient = pDoc->client();
	ASSERT( pClient );

	pClient->setEnableFilter( settings.get( "enableWordFilter", 1 ) != 0 );
	pClient->setEnableUDP( settings.get("EnableUDP", 1 ) != 0 );
	if (! pDoc->connect( m_SessionId, m_sAddress, m_nPort ) )
	{
		// failed to connect, try fallback address if we have one..
		if ( sFallbackAddress.length() == 0 || !pDoc->connect( m_SessionId, sFallbackAddress, nFallbackPort ) )
		{
			MessageBox( NULL, _T("Failed to connect to server; the server may be down or check your internet connection!"), _T("Failed"), MB_OK );
			return 0;
		}
	}
	
#ifndef _DEBUG
	if ( pClient->serverVersion() != 0 && pClient->version() != pClient->serverVersion() )
	{
		MessageBox( NULL, _T("Invalid server version or server is full; please wait and try again later!"), _T("Failed"), MB_OK );
		return 0;
	}
#endif

	progress.SetStatus( _T("Initializing platform...") );
	// create the platform object
	Platform::Ref pPlatform = Platform::create();
	if (! pPlatform )
	{
		MessageBox( NULL, _T("Failed to initialize the platform!"), _T("Error"), MB_OK );
		return 0;
	}

	// initialize the platform object
	Platform::Config config;
	config.instance = AfxGetInstanceHandle();
	config.icon = MAKEINTRESOURCE( IDR_MAINFRAME );
	config.className = "DarkSpace";
	config.applicationName.format("DarkSpace v%u (Build %u)", GetProgramVersion(), GetProgramBuild() );
	config.displayMode = settings.get( "displayMode", "" );
	config.bWindowed = settings.get( "windowed", (dword)0) != 0;
	config.eFSAA = DisplayDevice::FSAA_NONE; //DisplayDevice::findFSAA( settings.get( "FSAA", "FSAA_NONE" ) );
	config.bSoftwareCursor = settings.get( "softwareCursor", (dword)0 ) != 0;
	config.bInvertButtons = settings.get( "invertButtons", (dword)0 ) != 0;
	config.audioKhz = settings.get( "audioKhz", 44100 );
	config.audioChan = settings.get( "audioChan", 2 ); 
	config.audioVoices = settings.get( "audioVoices", 32 );
	config.audioBits = settings.get( "audioBits", 16 );

	PrimitiveMaterial::sm_bEnableSpecular = settings.get( "enableSpecular", 1 );
	PrimitiveSurface::sm_bEnableLightMaps = settings.get( "enableLightmap", 1 );
	PrimitiveSurface::sm_nAnisotropy = settings.get( "AF", 1 );
	DisplayDevice::sm_bWaitVB = settings.get( "waitVB", 1 );
	Font::sm_bEnableAlpha = settings.get( "alphaFonts", 1 );
	ViewGame::sm_bDisableTacticalActions = settings.get( "disableRightActions", (dword)0 ) != 0;

	GameContext::sm_bEnableHDR = settings.get( "HDR", 1 ) != 0;

	Trail::sm_fTrailScalar = Clamp( float( settings.get( "shipTrails", 100 ) ) / 100.0f, 0.0f, 1.0f );
	NounProjectile::sm_fTrailScalar = Clamp( float( settings.get( "missileTrails", 100) ) / 100.0f, 0.0f, 1.0f );
	NodeParticleSystem::sm_fDetail = Clamp( float( settings.get( "particleEffects", 100 ) ) / 100.0f, 0.0f, 1.0f );

	// set the texture detail
	CharString textureDetail = settings.get( "textureDetail", "HIGH" );
	if ( textureDetail == "LOW" )
		Material::sm_MaxTextureSize = SizeInt( 1024, 1024 );
	else if ( textureDetail == "MEDIUM" )
		Material::sm_MaxTextureSize = SizeInt( 2048, 2048 );
	else
		Material::sm_MaxTextureSize = SizeInt( 4096, 4096 );
		
	// enable shadows
	bool bEnableShadows = settings.get( "enableShadows", 1 ) != 0;
	WorldContext::sm_bEnableShadows = bEnableShadows;		// for now, default to one light projecting shadows

	// set the shadow detail...
	CharString shadowDetail = settings.get( "shadowDetail", "HIGH" );
	
	if ( shadowDetail == "LOW" )
		RenderContext::sm_nShadowMapSize = 512;
	else if ( shadowDetail == "MEDIUM" )
		RenderContext::sm_nShadowMapSize = 1024;
	else if ( shadowDetail == "HIGH" )
		RenderContext::sm_nShadowMapSize = 2048;		
	else
		RenderContext::sm_nShadowMapSize = 4096;		

	// set the graphics detail...
	NodeBoolean::s_DetailLevel = (NodeBoolean::DetailLevel)settings.get( "graphicsDetail", 1 );		// default to medium

	if ( NodeBoolean::s_DetailLevel == NodeBoolean::LOW )
		RenderContext::sm_fDefaultDetail = 0.0f;
	else if ( NodeBoolean::s_DetailLevel == NodeBoolean::MEDIUM )
		RenderContext::sm_fDefaultDetail = 0.5f;
	else
		RenderContext::sm_fDefaultDetail = 1.0f;	// disabled all detail levels on models, show full details always

	// get the FPS limit
	int nLimitFPS = settings.get( "FPSLimit", (dword)0 );
	pInterface->setFPSLimit( nLimitFPS );

	TRACE( CharString().format("Material::sm_MaxTextureSize = %d x %d", 
		Material::sm_MaxTextureSize.width, Material::sm_MaxTextureSize.height ) );

	if ( config.displayMode.length() == 0 )
	{
		MessageBox( NULL, _T("Failed to load settings; please run 'DarkSpace Setup' before running the Client!"), _T("Invalid Settings"), MB_OK );
		return FALSE;
	}

	if (! pPlatform->initialize( config ) )
	{
		MessageBox( NULL, _T("Failed to initialize the platform; please run the 'DarkSpace Setup' to check your client settings."), _T("Error"), MB_OK );
		return FALSE;
	}

	// set the sound effects volume
	if ( pPlatform->audio() )
	{
		float fVolume = Clamp<float>( settings.get( "sfxVolume", 75 ), 0.0f, 100.0f);
		pPlatform->audio()->setVolume(  fVolume / 100.0f );
	}

	// check the pixel/vertex shader, disable by default if less than 3.0
	DisplayDevice::Ref pDevice = pPlatform->display();
	bool bUseFixedFunction = pDevice->pixelShaderVersion() < 3.0f || pDevice->vertexShaderVersion() < 3.0f;
	DisplayDevice::sm_bUseFixedFunction = settings.get( "useFixedFunction", bUseFixedFunction ) != 0;

	// set the exit value to non-zero, so if we crash then the bug report will run next time
	settings.put( "exit", 1 );
	// close the progress dialog
	progress.DestroyWindow();
	
	// set this to allow textures to background load..
	DisplayDevice::sm_pCacheDevice = pDevice;
	// run the game
	RunGame( pInterface );
	// lock our device until it's destroyed...
	pDevice->lock();

	// clean up..
	Broker::flushCache();
	Material::flushSurfaceCache();

	VerbDistress::sm_LastDistress = NULL;
	DisplayDevice::sm_pCacheDevice = NULL;
	NounPod::sm_pPodContext = NULL;

	pInterface = NULL;
	pPlatform = NULL;

	if ( m_Error )
	{
		MessageBox( NULL, m_ErrorMessage, _T("Caught Exception"), MB_OK );
		return FALSE;
	}

	// clean exit, set the exit value to zero
	settings.put( "exit", (dword)0 );

	return 0;
}

//----------------------------------------------------------------------------

BOOL CClientApp::InitInstance()
{
	//AfxEnableControlContainer();

	return StartGame();
}

int CClientApp::ExitInstance() 
{
	// terminate any running local server
	Event serverEnd( "serverEnd" );
	serverEnd.signal();
	
	return CWinApp::ExitInstance();
}

//----------------------------------------------------------------------------
// EOF

