/*
	VerbTransferResource.h

	Transfer a resources from one planet to another
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VERBTRANSFERRESOURCE_H
#define VERBTRANSFERRESOURCE_H

#include "StructurePort.h"
#include "NounPlanet.h"
#include "World/Verb.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbTransferResource : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbTransferResource();
	VerbTransferResource( NounPlanet * pFrom, NounPlanet * pDestination, int quantity );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;			// is this a client verb, meaning okay to receive from a client

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Data
	NounPlanet::wRef		m_pOrigin;
	NounPlanet::wRef		m_pDestination;
	int						m_Quantity;

	// Static
	static bool				canTransferResource( NounPlanet * pOrigin, NounPlanet * pDestination, int quantity );
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
