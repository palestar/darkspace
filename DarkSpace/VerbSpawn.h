/*
	VerbSpawn.h

	This verb spawns a ship near the spawn point... the ship should already have it's userId(), teamId(), rank() and name() set by the caller. 
	This verb will set the spawn position of the ship in a random position around the spawn point and will set the tick of the ship and it's gadgets

	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VERBSPAWN_H
#define VERBSPAWN_H

#include "Factory/WidgetWrap.h"
#include "World/Verb.h"
#include "NounShip.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbSpawn : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbSpawn();
	VerbSpawn( Noun * pSpawn, NounShip * pShip, int nResourceCost );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;						// is this a client verb, meaning okay to receive from a client

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Data
	NounShip::wRef			m_pSelf;
	int						m_nResourceCost;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
