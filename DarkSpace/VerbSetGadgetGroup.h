/*
	VerbSetGadgetGroup.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VERBSETGADGETGROUP_H
#define VERBSETGADGETGROUP_H

#include "NounGadget.h"
#include "World/Verb.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbSetGadgetGroup : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbSetGadgetGroup();
	VerbSetGadgetGroup( NounGadget * pGadget, dword group );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;		// is this a client verb, meaning okay to receive from a client

	bool					canAttach( Noun * pNoun );
	void					onExecute();

protected:
	// Data
	dword					m_Group;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF

