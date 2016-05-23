/*
	VerbSelfDestruct.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef VERBSELFDESTRUCT_H
#define VERBSELFDESTRUCT_H

#include "NounShip.h"
#include "World/Verb.h"
#include "GameDll.h"



//-------------------------------------------------------------------------------

class DLL VerbSelfDestruct : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbSelfDestruct();
	VerbSelfDestruct( NounShip * pShip );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;		// is this a client verb, meaning okay to receive from a client

	bool					canAttach( Noun * pNoun );
	void					onExecute();
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
