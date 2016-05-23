/*
	VerbOrderShip.h
	(c)1999 Palestar, Richard Lyle
*/

#ifndef VERB_ORDER_SHIP_H
#define VERB_ORDER_SHIP_H

#include "NounShip.h"
#include "World/Verb.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbOrderShip : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef NounShip::Order	Order;

	// Construction
	VerbOrderShip();
	VerbOrderShip( NounShip * pShip, Order order, Noun * pTarget, NounShip * pFrom );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;		// is this a client verb, meaning okay to receive from a client

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Data
	Order					m_nOrder;
	Noun::tRef				m_pTarget;
	NounShip::wRef			m_pFrom;

	// Static
	static bool				canOrder( Noun * pWho, NounShip * pFrom );
	static bool				canOrderShip( NounShip * pShip, Order order, Noun * pTarget, NounShip * pFrom );
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF

