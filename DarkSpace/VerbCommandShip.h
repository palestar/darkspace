/*
	VerbCommandShip.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef VERBCOMMANDSHIP_H
#define VERBCOMMANDSHIP_H

#include "NounShip.h"
#include "World/Verb.h"
#include "GameDll.h"



//-------------------------------------------------------------------------------

class DLL VerbCommandShip : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef NounShip::Command Command;

	// Construction
	VerbCommandShip();
	VerbCommandShip( NounShip * pShip, Command nCommand, Noun * pTarget );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;		// is this a client verb, meaning okay to receive from a client

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Data
	Command					m_nCommand;
	Noun::tRef				m_pTarget;

	// Static
	static bool				canCommand( NounShip * pShip, Command nCommand, Noun * pTarget );
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF


