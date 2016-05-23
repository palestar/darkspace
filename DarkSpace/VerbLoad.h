/*
	VerbLoad.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VERBLOAD_H
#define VERBLOAD_H

#include "World/Verb.h"
#include "NounShip.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbLoad : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbLoad();
	VerbLoad( NounShip * pShip, Noun * pLoad );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;		// is this a client verb, meaning okay to receive from a client

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Data
	Noun::wRef				m_pLoad;

	// Static
	static bool				mayLoad( Noun * pLoad, NounShip * pShip );
	static bool				canLoad( Noun * pLoad, NounShip * pShip );
};

//----------------------------------------------------------------------------




#endif

//----------------------------------------------------------------------------
//EOF
