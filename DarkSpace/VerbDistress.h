/*
	VerbDistress.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VERBDISTRESS_H
#define VERBDISTRESS_H

#include "World/Verb.h"
#include "NounGame.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbDistress : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	enum Type {
		INVASION,
		ATTACKED,
		SUPPLY,
		ENEMY,			// enemy sighted
		BLOCKADE,		// planet blockaded
	};

	// Construction
	VerbDistress();
	VerbDistress( Noun * pTarget, Type type );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;			// is this a client verb, meaning okay to receive from a client

	void					onExecute();

	// Data
	Type					m_Type;

	// last distress verb executed
	static Noun::wRef		sm_LastDistress;
};

IMPLEMENT_RAW_STREAMING( VerbDistress::Type );

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
