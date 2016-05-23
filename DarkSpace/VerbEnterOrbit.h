/*
	VerbEnterOrbit.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef VERB_ENTER_ORBIT_H
#define VERB_ENTER_ORBIT_H

#include "World/Verb.h"
#include "NounBody.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbEnterOrbit : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbEnterOrbit();
	VerbEnterOrbit( Noun * pTarget, Noun * pOrbit, float orbitVelocity );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;		// is this a client verb, meaning okay to receive from a client
	bool					queue() const;

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Data
	Noun::wRef				m_pOrbit;
	float					m_fPosition;
	float					m_fDistance;
	float					m_fVelocity;

	// static
	static bool				canEnterOrbit( Noun * pTarget, Noun * pOrbit );		// can pTarget enter orbit around pOrbit
	static bool				canOrbit( Noun * pTarget, Noun * pOrbit );			// can these 2 objects possibly orbit each other
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
