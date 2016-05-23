/*
	VerbUseStructure.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VERBUSESTRUCTURE_H
#define VERBUSESTRUCTURE_H

#include "NounStructure.h"
#include "NounShip.h"
#include "World/Verb.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL VerbUseStructure : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbUseStructure();
	VerbUseStructure( NounStructure * pStructure, NounShip * pUser );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;		// is this a client verb, meaning okay to receive from a client

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Data
	NounShip::wRef			m_pUser;

	// Static
	static bool				canUse( NounStructure * pStructure, NounShip * pUser );
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
