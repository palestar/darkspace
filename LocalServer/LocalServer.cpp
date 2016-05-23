// LocalServer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#include "Debug/Trace.h"
#include "Standard/Library.h"
#include "Standard/Settings.h"
#include "Standard/Event.h"
#include "File/FileDisk.h"
#include "Factory/BrokerFolder.h"
#include "DarkSpace/GameContext.h"
#include "DarkSpace/GameServer.h"

const char * DEFAULT_CONTEXT = "Tutorial.prt";

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
 #ifdef _DEBUG
	Settings settings( "LocalServerD" );
#else
	Settings settings( "LocalServer" );
#endif

	// initialize the logging first thing before we do anything else..
	std::string logFile( settings.get( "logFile", "LocalServer.log" ) );
	std::string logExclude( settings.get( "logExclude", "" ) );
	int nMinLogLevel = settings.get( "logLevel", LL_STATUS );
	FileReactor logReactor( logFile, (LogLevels)nMinLogLevel, logExclude );

	// open the data files
	BrokerFolder theData;
	if ( !theData.open( settings.get( "data", ".\\data\\" ), false, true ) )
	{
		MessageBox( NULL, TEXT("Failed to open data source!"), TEXT("Failed"), MB_OK );
		return 0;
	}

	// add search paths
	FileDisk::resetSearchPaths();
	FileDisk::addSearchPath(".\\");

	// load the universe context
	GameContext::Link universeContext;
	if (! loadResource( WidgetKey( CharString( lpCmdLine ) ), universeContext ) &&
		!loadResource( lpCmdLine, universeContext ) )
	{
		if ( !loadResource( DEFAULT_CONTEXT, universeContext ) )
		{
			MessageBox( NULL, TEXT("Failed to load the universe; data files may be corrupt, missing, or misplaced!"), TEXT("Failed"), MB_OK );
			return 0;
		}
	}

	// start a local server
	GameServer::Context serverContext;
	serverContext.nServerFlags = 0;
	serverContext.nClientFlags = 0;
	serverContext.nModFlags = 0;
	//serverContext.bMaster = true;
	//serverContext.sMasterAddress = "";
	//serverContext.nMasterPort = 9000;
	serverContext.bPublish = false;
	serverContext.sMetaAddress = settings.get( "metaAddress", "meta-server.darkspace.net" );
	serverContext.nMetaPort = settings.get( "metaPort", 9000 );
	serverContext.nUniverseContext = universeContext.key();
	serverContext.maxRank = CHIEF_MARSHAL;
	serverContext.bBalanced = false;
	serverContext.bClan = false;
	serverContext.bTutorial = universeContext->type() == GameContext::TUTORIAL;
	serverContext.user = "DSS";
	serverContext.password = "darkspace";
	serverContext.nSessionId = 0;
	serverContext.gameId = settings.get( "gameId", 1 );
	
	serverContext.sLogAddress = "";
	serverContext.nLogPort = 0;

	serverContext.name = "LocalServer";
	serverContext.description = "";
	serverContext.address = "localhost";
	serverContext.port = 9001;
	serverContext.maxClients = 4;
	serverContext.nCompression = -1;
	serverContext.nDisconnectTime = 0;
	serverContext.runScript = "";
	serverContext.bPersistant = false;
	serverContext.sSaveFile = "";

	// disable automatic spawners since this is used for tutorials typically..
	GameServer::sm_bEnableTeamSpawners = false;

	// create the server object
	GameServer theServer;
	if ( ! theServer.start( serverContext ) )
	{
		MessageBox( NULL, TEXT("Failed to start local server!"), TEXT("Failed"), MB_OK );
		return 0;
	}
	universeContext.unload();

	bool bClientConnected = false;

	Event serverStart( "serverStart" );
	Event serverEnd( "serverEnd" );
	serverStart.signal();

	// wait until client disconnects, then terminate this server
	while( theServer.update() )
	{
		//Thread::sleep( 1000 );
		if ( serverEnd.signaled() )
			break;

		if ( theServer.clientCount() > 0 )
			bClientConnected = true;
		else if ( bClientConnected )
			break;		// client disconnectd!
	}

	theServer.stop();
	return 0;
}



