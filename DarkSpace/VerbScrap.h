/*
	VerbScrap.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VERBSCRAP_H
#define VERBSCRAP_H

#include "World/Verb.h"
#include "NounPlanet.h"
#include "NounShip.h"
#include "NounStructure.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbScrap : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbScrap();
	VerbScrap( NounStructure * pScrap, NounShip * pWho );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;	

	bool					canAttach( Noun * pNoun );
	void					onExecute();
	
	// Data
	NounShip::wRef			m_pWho;

	static bool				canScrap( NounStructure * pScrap, NounShip * pWho );
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
