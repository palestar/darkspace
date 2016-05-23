/*
	VerbMoveUnit.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VERBMOVEUNIT_H
#define VERBMOVEUNIT_H

#include "World/Verb.h"
#include "NounUnit.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbMoveUnit : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbMoveUnit();
	VerbMoveUnit( NounUnit * pUnit, u16 hex );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;		// is this a client verb, meaning okay to receive from a client

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Data
	u16						m_nHex;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
