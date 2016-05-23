/*
	VerbPlanetEvent.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VERBPLANETEVENT_H
#define VERBPLANETEVENT_H

#include "World/Verb.h"
#include "NounPlanet.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbPlanetEvent : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	enum Event
	{
		PLAGUE,
		REVOLUTION,
		RESESSION,
		STRIKE,
		BOOM,
		SMUGGLERS,
		MINING_FIND,

		FOOD_SHORT,
		POWER_SHORT,
		WORKER_SHORT,
		TECH_SHORT,
		ENEMY,
		BLOCKADE,
		NOT_CAPTURABLE,
	};

	// Construction
	VerbPlanetEvent();
	VerbPlanetEvent( NounPlanet * pPlanet, Event event, bool active );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Data
	Event					m_Event;
	bool					m_bActive;
};

IMPLEMENT_RAW_STREAMING( VerbPlanetEvent::Event );

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
