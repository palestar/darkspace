/*
	VerbCapturePlanet.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VERBCAPTUREPLANET_H
#define VERBCAPTUREPLANET_H

#include "World/Verb.h"
#include "NounPlanet.h"
#include "GameDll.h"



//-------------------------------------------------------------------------------

class DLL VerbCapturePlanet : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbCapturePlanet();
	VerbCapturePlanet( NounPlanet * pPlanet, int nFleetId );

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
