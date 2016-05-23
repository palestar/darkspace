/*
	VerbJumpBegin.h
	(c)2005 Palestar Inc, Richard Lyle
*/

#ifndef VERBJUMPBEGIN_H
#define VERBJUMPBEGIN_H

#include "NounShip.h"
#include "World/Verb.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbJumpBegin : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbJumpBegin();
	VerbJumpBegin( NounShip * pShip, Noun * pTarget );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;		// is this a client verb, meaning okay to receive from a client
	bool					queue() const;

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	Noun::tRef				m_pTarget;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
