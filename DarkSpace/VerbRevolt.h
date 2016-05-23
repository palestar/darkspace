/*
	VerbRevolt.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VERBREVOLT_H
#define VERBREVOLT_H

#include "World/Verb.h"
#include "NounPlanet.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbRevolt : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbRevolt();
	VerbRevolt( NounPlanet * pPlanet, int nFleetId );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Data
	int						m_nFleetId;
};

//----------------------------------------------------------------------------


#endif

//----------------------------------------------------------------------------
//EOF
