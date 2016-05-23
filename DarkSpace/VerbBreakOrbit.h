/*
	VerbBreakOrbit.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef VERB_BREAK_ORBIT_H
#define VERB_BREAK_ORBIT_H

#include "World/Verb.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbBreakOrbit : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbBreakOrbit();
	VerbBreakOrbit( Noun * pTarget );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;		// is this a client verb, meaning okay to receive from a client
	bool					queue() const;

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Data
	Vector3					m_vPosition;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
