/*
	VerbDestroyStructure.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VERBDESTROYSTRUCTURE_H
#define VERBDESTROYSTRUCTURE_H

#include "NounStructure.h"
#include "World/Verb.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbDestroyStructure : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbDestroyStructure();
	VerbDestroyStructure( NounStructure * pStructure, Noun * pKiller );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;		// is this a client verb, meaning okay to receive from a client

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Data
	Noun::wRef				m_pKiller;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
