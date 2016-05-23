/*
	VerbTradeRoute.h

	This verb adds or removes a trade route to a planet
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VERBTRADEROUTE_H
#define VERBTRADEROUTE_H

#include "NounShip.h"
#include "NounPlanet.h"
#include "World/Verb.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbTradeRoute : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbTradeRoute();
	VerbTradeRoute( NounPlanet * pOrigin, NounPlanet * pDestination, NounShip * pWho );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;			// is this a client verb, meaning okay to receive from a client

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Helpers
	static bool				isTradeRoute( NounPlanet * pOrigin, NounPlanet * pDestination );
	static int				getTradeRoute( NounPlanet * pOrigin, NounPlanet * pDestination);
	static bool				canTradeRoute( NounPlanet * pOrigin, NounPlanet * pDestination, NounShip * pWho );

	// Data
	NounPlanet::wRef		m_pDestination;
	NounShip::wRef			m_pWho;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF

