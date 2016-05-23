/*
	VerbDestroyShip.h
	(c)1999 Palestar, Richard Lyle
*/

#ifndef VERB_DESTROY_SHIP_H
#define VERB_DESTROY_SHIP_H

#include "NounShip.h"
#include "World/Verb.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbDestroyShip : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbDestroyShip();
	VerbDestroyShip( NounShip * pShip, Noun * pKiller, bool bZeroVelocity );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;		// is this a client verb, meaning okay to receive from a client

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Data
	Noun::wRef				m_pKiller;
	bool					m_bZeroVelocity;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
