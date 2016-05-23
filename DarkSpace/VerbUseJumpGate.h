/*
	VerbUseJumpGate.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VERBUSEJUMPGATE_H
#define VERBUSEJUMPGATE_H

#include "World/Verb.h"
#include "NounJumpGate.h"
#include "GameDll.h"



//-------------------------------------------------------------------------------

class DLL VerbUseJumpGate : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbUseJumpGate();
	VerbUseJumpGate( Noun * pUser, NounJumpGate * pGate );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;		// is this a client verb, meaning okay to receive from a client
	bool					queue() const;

	bool					canAttach( Noun * pNoun );
	void					onExecute();
	
	// Data
	NounJumpGate::wRef		m_pGate;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
