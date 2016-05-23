/*
	VerbLockPlanet.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VERBLOCKPLANET_H
#define VERBLOCKPLANET_H

#include "NounPlanet.h"
#include "NounShip.h"
#include "World/Verb.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbLockPlanet : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();


	// Construction
	VerbLockPlanet();
	VerbLockPlanet( NounPlanet * pPlanet, NounShip * pWho, bool bLock);

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;			// is this a client verb, meaning okay to receive from a client

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Data
	NounShip::wRef			m_pWho;
	bool					m_bLock;

	static bool				canLock( NounPlanet * pTarget, NounShip * pShip );
	static bool				canUnlock( NounPlanet * pTarget, NounShip * pShip );
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
