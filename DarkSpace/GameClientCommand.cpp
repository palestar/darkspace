/**
	@file GameClientCommand.cpp

	(c)2013 Palestar Inc
	@author Richard Lyle @date 6/21/2013 7:31:30 PM
*/

#include "GameCLient.h"
#include "Standard/CommandLine.h"

//---------------------------------------------------------------------------------------------------

enum Command
{
	CMD_INVALID,

	CMD_TARGET,		// send message to target
	CMD_TEAM,		// send message to team
	CMD_FACTION,	// send message to enemy
	CMD_YELL,		// send message to area
};

struct CommandLookup
{
	const char *	pPattern;
	Command			command;
	dword			flags;
	const char *	pHelp;
	dword			color;
};

static CommandLookup COMMAND_LOOKUP[] = 
{
	{ "/target",	CMD_TARGET,			0,						"Send message to current target.",	SKYBLUE.RGB() },
	{ "/team",		CMD_TEAM,			0,						"Send message to team.",			TEAL.RGB() },
	{ "/fleet",		CMD_TEAM,			0,						"Send message to fleet.",			TEAL.RGB() },
	{ "/faction",	CMD_FACTION,		0,						"Send message to faction.",			GREEN.RGB() },
	{ "/yell",		CMD_YELL,			0,						"Send message to all players.",		RED.RGB() },
};

static int COMMAND_LOOKUP_COUNT = sizeof(COMMAND_LOOKUP)/sizeof(COMMAND_LOOKUP[0]);

//---------------------------------------------------------------------------------------------------

bool GameClient::processCommand( const char * pChat, dword nFromId /*= 0*/ )
{
	if ( pChat == NULL || pChat[ 0 ] == 0 )
		return false;

	// copy into a buffer so we can tokenize it
	CommandLine arguments( pChat, true );
	if ( arguments.argumentCount() < 1 )
		return false;

	// get the command
	CharString pattern( arguments.argument( 0 ) );

	Command command = CMD_INVALID;
	Color color( GREY );

	// search the commands
	for(int i=0;i<COMMAND_LOOKUP_COUNT;++i)
	{
		// allow /aw to match against /away command by using strnicmp
		if ( pattern.compareCommand( COMMAND_LOOKUP[i].pPattern ) == 0 )
		{
			// found a matching string, check the access rights
			if ( COMMAND_LOOKUP[i].flags != 0 && (profile()->flags() & COMMAND_LOOKUP[i].flags) == 0 )
			{
				pushChat( "<color;808080>Error, command is restricted!</color>" );
				return false;
			}

			command = COMMAND_LOOKUP[i].command;
			color = COMMAND_LOOKUP[i].color;
			break;
		}
	}

	switch( command )
	{
	case CMD_INVALID:
		return WorldClient::processCommand( pChat, nFromId );
	case CMD_TARGET:
		{
			if ( arguments.argumentCount() > 1 )
			{
				Noun::Ref pTarget = m_pTarget.pointer();
				if ( pTarget.valid() && pTarget->userId() != 0 )
					m_pMetaClient->sendPrivateChat( pTarget->userId(), arguments.restring( 1 ) );
				else
					pushChat( "<color;808080>No valid target to send message." );
			}
			else
				pushChat( "<color;808080>Usage: /target [message]" );
		}
		return true;
	case CMD_TEAM:
		{
			m_nDefaultRoomId = m_nFleetRoomId;
			if ( arguments.argumentCount() > 1 )
				m_pMetaClient->sendChat( m_nFleetRoomId, 0, arguments.restring(1) );
			else
				pushChat( "<color;808080>Usage: /team [message]" );
		}
		return true;
	case CMD_FACTION:
		{
			m_nDefaultRoomId = m_nFactionRoomId;
			if ( arguments.argumentCount() > 1 )
				m_pMetaClient->sendChat( m_nFactionRoomId, 0, arguments.restring(1) );
			else
				pushChat( "<color;808080>Usage: /faction [message]" );
		}
		return true;
	case CMD_YELL:
		{
			if ( m_nGlobalRoomId != 0 )
			{
				m_nDefaultRoomId = m_nGlobalRoomId;
				if ( arguments.argumentCount() > 1 )
					m_pMetaClient->sendChat( m_nGlobalRoomId, 0, arguments.restring(1) );
				else
					pushChat( "<color;808080>Usage: /yell [message]" );
			}
			else
				pushChat( "<color;808080>Yell is currently disabled." );
		}
		return true;
	}

	return false;
}

bool GameClient::pushMetaChat( const MetaClient::Chat & a_Chat )
{
	Color c = WHITE;
	if ( a_Chat.roomId == m_nGlobalRoomId )
		c = RED;			// global chat
	else if ( a_Chat.roomId == m_nFactionRoomId )
		c = GREEN;			// faction chat
	else if ( a_Chat.roomId == m_nFleetRoomId )
		c = TEAL;			// fleet chat
	else if ( a_Chat.recpId == m_pProfile->userId() )
		c = SKYBLUE;		// target chat

	if ( a_Chat.text[0] != '/' )
	{
		CharString message;
		message.format( "<color;%6.6x>%s %s: %s</color>", c.RGB(), 
			Time::format( a_Chat.time, "%H:%M:%S").cstr(), a_Chat.author.cstr(), 
			filterMetaChat( a_Chat.text ).cstr() );
		pushChat( message );
	}
	else
	{
		CharString message;
		message.format( "<color;%6.6x>* %s", c.RGB(),
			filterMetaChat( a_Chat.text.cstr() + 1 ).cstr() );
		pushChat( message );
	}

	return true;
}

//---------------------------------------------------------------------------------------------------
//EOF
