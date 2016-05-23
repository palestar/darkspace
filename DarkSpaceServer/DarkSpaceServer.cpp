/*
	GameServer.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include <stdio.h>
#include <time.h>

#include "Debug/Assert.h"
#include "Debug/ExceptionHandler.h"
#include "Factory/BrokerFolder.h"
#include "Standard/CommandLine.h"
#include "Standard/Time.h"
#include "Standard/Settings.h"
#include "Standard/Process.h"
#include "Standard/StringHash.h"
#include "DarkSpace/GameServer.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

const char * DEFAULT_DATA_DIR = ".\\data\\";
#else

const char * DEFAULT_DATA_DIR = "./Data/";
#endif

//----------------------------------------------------------------------------

int RunServer( void * pINI )
{
	Event serverStop( CharString().format("StopProcess%u", Process::getCurrentProcessId()) );
	Settings settings( "GameServer", (const char *)pINI );

	// initialize the logging first thing before we do anything else..
	std::string logFile( settings.get( "logFile", "Server.log" ) );
	std::string logExclude( settings.get( "logExclude", "" ) );
	unsigned int nMinLogLevel = settings.get( "logLevel", LL_STATUS );
	FileReactor fileReactor( logFile, nMinLogLevel, logExclude );

	// initialize our unique number system now..
	dword nServerID = settings.get( "serverId", (dword)0 );
	if ( nServerID == 0 )
	{
		CharString sServerHash( settings.get( "address", "" ) + ":" + settings.get( "port", "" ) );
		nServerID = StringHash( sServerHash ) & ((1 << PERSISTID_BITS) - 1);

		LOG_STATUS( "DarkSpaceServer", "No serverID found in config, generating a new ID of %u.", nServerID );
		// generate a unique server ID using our address and port..
		settings.put( "serverID", nServerID );
	}

	UniqueNumber::setProvider( new PersistedUniqueNumberProvider( UNT_SERVER, nServerID, (const char *)pINI ) );

#if defined(_DEBUG)
	//DebugReactor debugReactor;
#endif

#if defined(_WIN32)
	StandardOutReactor stdoutReactor;
#endif

#if defined(_WIN32)
	// set processor affinity..
	dword nProcessor = settings.get( "processor", (dword)0 );
	if ( nProcessor != 0 )
	{
		// Force server to use specific CPU. On AMD multi-core systems, the profiling code
		// seems to get erroneous values unless we lock the process to a single processor.
		if (! SetProcessAffinityMask( GetCurrentProcess(), nProcessor ) )
			TRACE( "SetProcessAffinityMask failed...!" );
	}
#endif

	// open the data
	BrokerFolder theData;

	CharString dataFolder = settings.get( "data", DEFAULT_DATA_DIR );
	if (! theData.open( dataFolder , false, false ) )
	{
		LOG_ERROR( "DarkSpaceServer", CharString().format("Failed to open data directory '%s'", dataFolder.cstr()) );
		return 1;	// failed to open the data directory
	}

	// load the GameContext
	GameContext::Link universeContext;
	if (! loadResource( settings.get( "context", "Start.prt" ), universeContext ) )
	{
		LOG_ERROR( "DarkSpaceServer", "Failed to load GameContext!" );
		return 1;	// failed to load GameContext
	}

	GameServer::Context serverContext;
	serverContext.sConfigFile = settings.file();
	serverContext.nServerFlags = settings.get( "serverFlags", 2 );
	serverContext.nClientFlags = settings.get( "clientFlags", (dword)0 );
	serverContext.nModFlags = settings.get( "modFlags", MetaClient::MODERATOR|MetaClient::ADMINISTRATOR|MetaClient::DEVELOPER );
	serverContext.bPublish = settings.get( "publish", 1 ) != 0;
	serverContext.sMetaAddress = settings.get( "metaAddress", "meta-server.darkspace.net" );
	//serverContext.sMetaAddress = settings.get( "metaAddress", "meta-server.beta.darkspace.net" );
	serverContext.nMetaPort = settings.get( "metaPort", 9000 );
	serverContext.nUniverseContext = universeContext.key();
	serverContext.maxPlayers = settings.get( "maxPlayers", 32 );
	serverContext.maxRank = settings.get( "maxRank", CHIEF_MARSHAL );
	serverContext.bBalanced = settings.get( "balanced", 1 ) != 0;
	serverContext.bClan = settings.get( "clan", 1 ) != 0;
	serverContext.bTutorial = settings.get( "tutorial", (dword)0 ) != 0;
	serverContext.user = settings.get( "user", "DSS" );
	serverContext.password = settings.get( "password", "darkspace" );
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
	serverContext.bEditor = settings.get("editor", (dword)0 ) != 0;
	serverContext.bEnableUDP = settings.get("enableUDP", (dword)1 ) != 0;
	
	serverContext.nServerRole = settings.get( "serverRole", 0xffffffff );
	serverContext.sMasterAddress = settings.get( "masterAddress", "" );
	serverContext.nMasterPort = settings.get( "masterPort", (dword)0 );
	
	serverContext.bUseShipFlags = settings.get( "useShipFlags", (dword)0 ) != 0;
	serverContext.sShipFlags = settings.get( "shipFlags", "" );

	GameServer::sm_nAlwaysSpawnFlags = settings.get( "alwaysSpawnFlags", MetaClient::ADMINISTRATOR|MetaClient::DEVELOPER );
	GameServer::sm_bEnableTeamSpawners = settings.get("enableTeamSpawners", 1 ) != 0;
	GameServer::sm_bHiddenServer = settings.get( "hiddenServer", (dword)0 ) != 0;

	// how often to automatically stop the server
	CharString sRestartTimeMask = settings.get( "RestartTimeMask", "" );

	// create the server object
	GameServer theServer;
	if (! theServer.start( serverContext ) )
	{
		LOG_ERROR( "DarkSpaceServer", "Failed to start the server." );
		return 1;	// failed to start the server
	}

	// release our reference to our map, so it can be re-loaded...
	universeContext.unload();

	dword nSeconds = 0;
	Timer secondTimer( 1000 );

	while( theServer.update() )
	{
		if ( nSeconds == 0 && (Time::isTime( Time::seconds(), sRestartTimeMask ) || serverStop.signaled()) )
			nSeconds = settings.get( "ShutdownTime", 60 );

		if ( nSeconds != 0 && secondTimer.signaled() )
		{
			nSeconds--;
			if ( nSeconds == 0 )
				break;			// time to shutdown, exit the while loop...

			if ( (nSeconds > 10 && (nSeconds % 10) == 0) || nSeconds <= 10 )
				theServer.sendChat( CharString().format("SERVER: shutting down in %d seconds...", nSeconds) );
		}
	}

	theServer.stop();

	return 0;
}

//----------------------------------------------------------------------------

int main( int argc, char ** argv )
{
	if ( argc < 2 )
	{
		printf( "Usage: DarkSpaceServer <iniFile>\n" );
		return 1;
	}

	return HandleException( RunServer, argv[1] );
}

//----------------------------------------------------------------------------
//EOF
