/*
	VerbSetStructureActive.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VERBSETSTRUCTUREACTIVE_H
#define VERBSETSTRUCTUREACTIVE_H

#include "World/Verb.h"
#include "NounShip.h"
#include "NounStructure.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbSetStructureActive : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbSetStructureActive();
	VerbSetStructureActive( NounStructure * pStructure, NounShip * pWho, bool active);

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;	

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Data
	NounShip::wRef			m_pWho;
	bool					m_bActive;

	static bool				canControl( NounStructure * pStructure, NounShip * pWho );
	static bool				canPauseStructure( NounStructure * pStructure, NounShip * pWho );
	static bool				canContinueStructure( NounStructure * pStructure, NounShip * pWho );
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
