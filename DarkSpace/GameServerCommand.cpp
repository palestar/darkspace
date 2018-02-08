/**
	@file GameServerCommand.cpp

	(c)2006 Palestar Inc
	@author Richard Lyle @date 2/23/2007 11:05:52 PM
*/

#include "GameServer.h"
#include "Standard/CommandLine.h"
#include "Standard/StringHelpers.h"
#include "VerbCaptureShip.h"
#include "VerbCapturePlanet.h"
#include "GameProfile.h"
#include "DarkSpace/VerbLockPlanet.h"
#include "DarkSpace/TraitDisplayName.h"

//---------------------------------------------------------------------------------------------------

enum Command
{
	CMD_INVALID,	// invalid command
	CMD_CLEAN,		// wipe the worldContext clean
	CMD_LOOT,		// spawn a random loot at my position
	CMD_CAP,		// TESTING: Capture own ship with enemy troops
	CMD_WINCOND,	// TESTING: Force winning condition
	CMD_LOCK,		// Lock a planet
	CMD_NAMESHIP,	// Set the name of a ship
};

struct CommandLookup
{
	const char *	pPattern;
	Command			nCommand;
	dword			flags;
	const char *	pHelp;
	dword			color;
};

static CommandLookup COMMAND_LOOKUP[] = 
{
	{ "/clean",		CMD_CLEAN,			MetaClient::SERVER,			"Clean all planets of structures/units.",	GREY.RGB() },
	{ "/loot",		CMD_LOOT,			MetaClient::ADMINISTRATOR,	"Spawn random loot on my position."		,	GREY.RGB() },
	{ "/cap",		CMD_CAP,			MetaClient::DEVELOPER,		"Capture ship.",							GREY.RGB() },
	{ "/wincond",	CMD_WINCOND,		MetaClient::DEVELOPER,		"Force win condition.",						GREY.RGB() },
	{ "/lock",		CMD_LOCK,			0,							"Lock a friendly planet.",					GREY.RGB() },
	{ "/nameship",	CMD_NAMESHIP,		MetaClient::SUBSCRIBED,		"Set the name of your ship.",				GREY.RGB() },
};
static int COMMAND_LOOKUP_COUNT = sizeof(COMMAND_LOOKUP)/sizeof(COMMAND_LOOKUP[0]);

//---------------------------------------------------------------------------------------------------

bool GameServer::processCommand( ClientContext & context, const char * pChat )
{
	dword nClientId = context.nClientId;
	dword nUserId = context.nUserId;

	// copy into a buffer so we can tokenize it
	CommandLine arguments( pChat, true );
	if ( arguments.argumentCount() < 1 )
		return false;
	CharString pattern( arguments.argument( 0 ) );

	Command nCommand = CMD_INVALID;
	Color color( WHITE );

	// search the commands
	int n = pattern.length();			
	for(int c=0;c<COMMAND_LOOKUP_COUNT;++c)
	{
		// allow /aw to match against /away command by using strnicmp
		if ( strnicmp<char>( pattern, COMMAND_LOOKUP[c].pPattern, n ) == 0 )
		{
			// found a matching string, check the access rights
			if ( COMMAND_LOOKUP[c].flags != 0 && (context.MetaProfile.flags & COMMAND_LOOKUP[c].flags) == 0 )
			{
				sendChat( nClientId, "<color;808080>You do not have the rights to use this command!" );
				return false;
			}

			nCommand = COMMAND_LOOKUP[c].nCommand;
			color = COMMAND_LOOKUP[c].color;
			break;
		}
	}

	NounShip *pShip =NULL;
	switch( nCommand )
	{
	case CMD_LOOT:
		pShip = WidgetCast<NounShip>(context.pFocus);
		if(pShip)
		{
			if ( arguments.argumentCount() > 1 )
			{
				CharString sArg = arguments.restring( 1 );
				pShip->addLoot(sArg);
			}
			return true;
		}
		break;
	case CMD_CAP:
		{
			// get our teamId (use first argument if given)
			int nTeam = 0;
			if (arguments.argumentCount() > 1)
				nTeam = arguments.argumentInt(1);
			else
				nTeam = context.nTeamId;

			// capture ship if ship is target
			NounShip * pShip = WidgetCast<NounShip>( context.pTarget );
			if (pShip != NULL)
			{
				Verb::Ref(new VerbCaptureShip(pShip, nTeam, context.pSelf));
				return true;
			}
			
			// capture planet if planet is target
			NounPlanet * pPlanet = WidgetCast<NounPlanet>(context.pTarget);
			if (pPlanet != NULL)
			{
				Verb::Ref(new VerbCapturePlanet( pPlanet, nTeam ));
				return true;
			}

			// else capture ourselves
			Verb::Ref(new VerbCaptureShip(pShip, nTeam, context.pSelf));
				
			return true;
		}
		break;
	case CMD_WINCOND:
		{
			if ( arguments.argumentCount() > 1 )
				gameContext()->executeCondition( atoi( arguments.argument( 1 ) ) );
			else
				sendUserChat( nUserId, "<color;808080>Usage: /wincond [CONDITION INDEX]" );
		}
		break;
	case CMD_CLEAN:
		{
			if ( arguments.argumentCount() > 1 )
			{
				CharString sConfirm = arguments.argument( 1 );
				if ( sConfirm.compareNoCase( "planets" ) == 0 )
				{
					sendUserChat( nUserId, "Cleaning planets..." );

					// reset the entire map, remove all structures from all planets, set nTeamId to 0, reset all planet resources and population
					for(int i=0;i<worldContext()->zoneCount();i++)
					{
						NodeZone * pZone = worldContext()->zone( i );
						for(int j=0;j<pZone->childCount();j++)
						{
							if ( WidgetCast<NounPlanet>( pZone->child(j) ) )
							{
								NounPlanet * pPlanet = (NounPlanet *)pZone->child(j);
								
								pPlanet->setTeamId( 0 );
								pPlanet->setPopulation( 0 );
								pPlanet->setResources( 1000 );
								pPlanet->setup();
								pPlanet->detachAllNodes();

								pPlanet->resync();
							}
						}
					}
				}
				else if ( sConfirm.compareNoCase( "ships" ) == 0 )
				{
					Queue< NounShip * > remove;

					// reset the entire map, remove all structures from all planets, set nTeamId to 0, reset all planet resources and population
					for(int i=0;i<worldContext()->zoneCount();i++)
					{
						NodeZone * pZone = worldContext()->zone( i );
						for(int j=0;j<pZone->childCount();j++)
						{
							NounShip * pSelf = WidgetCast<NounShip>( pZone->child(j) );
							if ( pSelf != NULL )
								remove.push( pSelf );
						}
					}

					sendUserChat( nUserId, CharString().format("Removing %d ships from the worldContext...", remove.size()) );
					while( remove.valid() )
					{
						(*remove)->detachSelf();
						remove.next();
					}

				}
			}
			else
				sendUserChat( nUserId, "<color;808080>Usage: /clean [planets|ships]" );
		}
		return true;
	case CMD_LOCK:
		{
			if(arguments.argumentCount() > 1)
			{
				//get players rank
				GameProfile * pProfile = WidgetCast<GameProfile>(context.pProfile);
				int nRank = pProfile->calculateRank();
				
				CharString sTargetPlanet = arguments.argument( 1 );
				sTargetPlanet.replace("\"", "");
				if ( arguments.argumentCount() > 2 )
				{
					int nLockRank = -1;

					//checkfirst for lettered code
					CharString sRank = arguments.argument(2);
					for(int k=0;k<LAST_RANK;++k)
					{
						if ( stricmp<char>( sRank, GameProfile::rankShortText( k ) ) == 0 )
						{
							nLockRank = k;
							break;
						}
					}

					if ( nLockRank < 0 )
						nLockRank = atoi( sRank );
					if ( nLockRank < 0 )
						nLockRank = 0;
					if ( nLockRank > nRank && (context.MetaProfile.flags & MetaClient::ADMINISTRATOR) == 0)
						nLockRank = nRank;

					nRank = nLockRank;
				}
				
				// Find planet
				bool bPlanetFound = false;
				for(int i=0;i<worldContext()->zoneCount();i++)
				{
					NodeZone * pZone = worldContext()->zone( i );
					for(int j=0;j<pZone->childCount();j++)
					{
						if ( WidgetCast<NounPlanet>( pZone->child(j) ) )
						{
							NounPlanet * pPlanet = (NounPlanet *)pZone->child(j);
							if(sTargetPlanet.compareNoCase(pPlanet->name()) == 0)
							{ 	
								bPlanetFound = true;
								if(pPlanet->canGovern(context.pSelf))
								{
									pPlanet->setLockRank( nRank );
									sendUserChat( nUserId, "<color;ffffff>Comms: Planet locked under your orders." );
								}
								else
								{
                                    sendUserChat( nUserId, "<color;ffffff>Comms: I'm sorry, you do not have sufficient authority to order that." );
								}
								break;
							}
						}
					}
				}

				if (! bPlanetFound )
					sendUserChat(nUserId, CharString().format( "<color;ffffff>Comms: Couldn't find a planet with the name '%s'.", sTargetPlanet.cstr() ) );
			}
			else
				sendUserChat( nUserId, "<color;808080>Usage: /lock planet [rank#]" );
		}
		return true;
	case CMD_NAMESHIP:
		{
			NounShip * pShip = WidgetCast<NounShip>( context.pSelf );
			if ( pShip != NULL )
			{
				if ( !pShip->testFlags( NounShip::FLAG_JUMPING ) )
				{
					if ( arguments.argumentCount() > 1 )
					{
						CharString sName = arguments.argument( 1 );
						sName.replace("\"", "");
						sName.replace("*", "");
						sName.replace("_", " ");
						sName.replace("/", "");
						sName.replace("[", "");
						sName.replace("]", "");		
						sName.trim();		// remove outer white spaces
						sName.left( 32 );	// allow up to 32 characters

						if ( sName.length() > 0 )
						{
							TraitDisplayName * pDisplayName = pShip->attachTrait<TraitDisplayName>();
							pDisplayName->setDisplayName( sName );

							// force ship to be resent to all clients
							pShip->resync();		

							sendUserChat( nUserId, CharString().format( "<color;808080>Ship name changed to %s.", pShip->displayName( false ).cstr() ) );
						}
						else
							sendUserChat( nUserId, "<color;808080>Ship name is empty." );
					}
					else
					{
						if ( pShip->detachTrait<TraitDisplayName>() )
						{
							sendUserChat( nUserId, "<color;808080>Ship name removed." );
							pShip->resync();
						}
						else
							sendUserChat( nUserId, "<color;808080>Usage: /nameship \"U.W.S Executioner\" (use quotation marks or underscores for names with spaces)\nIllegal characters: \\, *, _, /, [, ]." );					
					}
				}
				else
					sendUserChat( nUserId, "<color;808080>You cannot name your ship whilst in jump." );
			}
			else
				sendUserChat( nUserId, "<color;808080>Not a valid ship." );
		}
		return true;
	}

	return WorldServer::processCommand( context, pChat );
}

//---------------------------------------------------------------------------------------------------
//EOF
