/*
	VerbSetFlags.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef VERBSETFLAGS_H
#define VERBSETFLAGS_H

#include "World/Verb.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbSetFlags : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbSetFlags();
	VerbSetFlags( Noun * pNoun, dword nFlags );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;

	bool					canAttach( Noun * pNoun );
	void					onExecute();
	// Data
	dword					m_nFlags;
};

//----------------------------------------------------------------------------

#endif

//----------------------------------------------------------------------------
// EOF
