/*
	VerbPlanetEvent.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "VerbPlanetEvent.h"
#include "resource.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

static const dword EVENT_PLANET_FLAGS[] =
{
	NounPlanet::FLAG_PLAGUE,		// PLAGUE
	NounPlanet::FLAG_REVOLUTION,	// REVOLUTION
	NounPlanet::FLAG_RESESSION,		// RESESSION,
	NounPlanet::FLAG_STRIKE,		// STRIKE,
	NounPlanet::FLAG_BOOM,			// BOOM,
	NounPlanet::FLAG_SMUGGLERS,		// SMUGGLERS,
	NounPlanet::FLAG_MINING_FIND,	// MINING_FIND,
	NounPlanet::FLAG_FOOD_SHORT,	// FOOD_SHORT,
	NounPlanet::FLAG_POWER_SHORT,	// POWER_SHORT,
	NounPlanet::FLAG_WORKER_SHORT,	// WORKER_SHORT,
	NounPlanet::FLAG_TECH_SHORT,	// TECH_SHORT
	NounPlanet::FLAG_ENEMY,			// ENEMY
	NounPlanet::FLAG_BLOCKADE,		// BLOCKADE
	NounPlanet::FLAG_NOT_CAPTURABLE,// NOT_CAPTURABLE
};

static const char * EVENT_TEXT[] = 
{
	"<color;ffffffff>NEWS: Plague has erupted on %s...</color>",		// PLAGUE
	"<color;ffffffff>NEWS: Political unrest on %s...</color>",	// REVOLUTION
	"<color;ffffffff>NEWS: Global resession on %s, production reduced...</color>",		// RESESSION,
	"<color;ffffffff>NEWS: Worker strike on %s, production greatly reduced...</color>",		// STRIKE,
	"<color;ffffffff>NEWS: Economic boom on %s, production increased...</color>",			// BOOM,
	"<color;ffffffff>NEWS: Smuggler activity on %s...</color>",		// SMUGGLERS,
	"<color;ffffffff>NEWS: Rich mineral deposits found on %s, mining output increased...</color>",	// MINING_FIND,
	"<color;ffffffff>NEWS: Food shortages are causing starvation on %s...</color>",	// FOOD_SHORT,
	"<color;ffffffff>NEWS: Power shortage on %s...</color>",	// POWER_SHORT,
	"<color;ffffffff>NEWS: Worker shortage on %s...</color>",	// WORKER_SHORT,
	"<color;ffffffff>NEWS: Technology shortage on %s...</color>",	// TECH_SHORT,
	0,	//"<color;ffffff>NEWS: Enemy detected near %s...",			// ENEMY
	0,	//"<color;ffffff>NEWS: %s is under enemy blockade...",		// BLOCKADE
	0,																// NOT_CAPTURABLE
};

static const char * EVENT_TEXT_END[] = 
{
	"<color;ffffffff>NEWS: Plague has ended on %s...</color>",					// PLAGUE
	"<color;ffffffff>NEWS: Political stablity on %s...</color>",				// REVOLUTION
	"<color;ffffffff>NEWS: Global resession has ended on %s...</color>",		// RESESSION,
	"<color;ffffffff>NEWS: Worker strike has ended on %s...</color>",			// STRIKE,
	"<color;ffffffff>NEWS: Economy on %s has stabilized...</color>",			// BOOM,
	"<color;ffffffff>NEWS: Smuggler activitiy has stopped on %s...</color>",	// SMUGGLERS,
	"<color;ffffffff>NEWS: Rich mineral deposits exhausted on %s...</color>",	// MINING_FIND,
	"<color;ffffffff>NEWS: Starvation has ended on %s...</color>",				// FOOD_SHORT,
	"<color;ffffffff>NEWS: Power shortage resolved on %s...</color>",			// POWER_SHORT,
	"<color;ffffffff>NEWS: Worker shortage resolved on %s...</color>",			// WORKER_SHORT,
	"<color;ffffffff>NEWS: Technology shortage resolved on %s...</color>",		// TECH_SHORT,
	0,																			// ENEMY
	0,																			// BLOCKADE
	0,																			// NOT_CAPTURABLE
};

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbPlanetEvent, Verb );

BEGIN_PROPERTY_LIST( VerbPlanetEvent, Verb )
	ADD_TRANSMIT_PROPERTY( m_Event );
	ADD_TRANSMIT_PROPERTY( m_bActive );
END_PROPERTY_LIST();

VerbPlanetEvent::VerbPlanetEvent()
{}

VerbPlanetEvent::VerbPlanetEvent( NounPlanet * pPlanet, Event event, bool active ) 
	: m_Event( event ), m_bActive( active )
{
	attachVerb( pPlanet );
}

//----------------------------------------------------------------------------

Verb::Priority VerbPlanetEvent::priority() const
{
	return MEDIUM;
}

Verb::Scope VerbPlanetEvent::scope() const
{
	return GLOBAL;
}

bool VerbPlanetEvent::client() const
{
	return false;
}

bool VerbPlanetEvent::canAttach( Noun * pNoun )
{
	return WidgetCast<NounPlanet>( pNoun ) != NULL;
}

void VerbPlanetEvent::onExecute()
{
	NounPlanet * pPlanet = WidgetCast<NounPlanet>( target() );
	if ( validate( pPlanet ) )
	{
		// turn flags on or off
		if ( m_bActive )
			pPlanet->addFlags( EVENT_PLANET_FLAGS[ m_Event ] );
		else
			pPlanet->clearFlags( EVENT_PLANET_FLAGS[ m_Event ] );

		// send chat message to all players in the same faction as the planet
		if ( pPlanet->inLockedZone() )
		{
			if ( m_bActive && EVENT_TEXT[ m_Event ] != 0 )
				pPlanet->clientChat( CharString().format( EVENT_TEXT[ m_Event ], pPlanet->name() ) );
			else if (! m_bActive && EVENT_TEXT_END[ m_Event ] != 0 )
				pPlanet->clientChat( CharString().format( EVENT_TEXT_END[ m_Event ], pPlanet->name() ) );
		}
	}
}

//----------------------------------------------------------------------------
//EOF
