/*
	VerbUnload.h

	This verb is used to unload cargo from an ship onto a planet or another ship
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VERBUNLOAD_H
#define VERBUNLOAD_H

#include "World/Verb.h"
#include "NounDropTarget.h"
#include "NounShip.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbUnload : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbUnload();
	VerbUnload( NounShip * pShip, Noun * pTarget, Noun * pUnload );

	// Verb Interface
	bool					queue() const;
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;		// is this a client verb, meaning okay to receive from a client

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Data
	Noun::tRef				m_pTarget;
	Noun::wRef				m_pUnload;

	// Static
	static bool				mayUnload( Noun * pTarget, Noun * pUnload, NounShip * pShip );
	static bool				canUnload( Noun * pTarget, Noun * pUnload, NounShip * pShip );
};

//----------------------------------------------------------------------------


#endif

//----------------------------------------------------------------------------
//EOF
