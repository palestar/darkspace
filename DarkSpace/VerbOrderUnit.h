/*
	VerbOrderUnit.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VERBORDERUNIT_H
#define VERBORDERUNIT_H

#include "NounUnit.h"
#include "NounShip.h"
#include "World/Verb.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbOrderUnit : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef NounUnit::Order	Order;

	// Construction
	VerbOrderUnit();
	VerbOrderUnit( NounUnit * pUnit, Order order, Noun * pTarget, NounShip * pWho );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;		// is this a client verb, meaning okay to receive from a client

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Data
	Order					m_Order;
	Noun::tRef				m_pTarget;
	NounShip::wRef			m_pWho;

	static bool				canOrder( Noun * pTarget, NounShip * pWho );
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF

