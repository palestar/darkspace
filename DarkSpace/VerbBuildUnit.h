/*
	VerbBuildUnit.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VERBBUILDUNIT_H
#define VERBBUILDUNIT_H

#include "NounShip.h"
#include "StructureBarracks.h"
#include "World/Verb.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbBuildUnit : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbBuildUnit();
	VerbBuildUnit( StructureBarracks * pBarracks, NounShip * pWho, NounUnit * pUnit );	

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;		// is this a client verb, meaning okay to receive from a client

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Helpers
	static bool				canBuild( StructureBarracks * pBarracks, NounShip * pWho, NounUnit * pUnit );
	static void				getUnits( StructureBarracks * pBarracks, NounShip * pWho,
								Array< NounUnit * > & units );

	// Data
	NounShip::wRef			m_Who;
	NounUnit::wRef			m_Unit;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF

