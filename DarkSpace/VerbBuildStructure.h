/*
	VerbBuildStructure.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VERBBUILDSTRUCTURE_H
#define VERBBUILDSTRUCTURE_H

#include "NounDropTarget.h"
#include "NounStructure.h"
#include "GadgetBuild.h"
#include "World/Verb.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbBuildStructure : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbBuildStructure();
	VerbBuildStructure( NounShip * pBuilder, Noun * pTarget, NounStructure * pBuild );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;		// is this a client verb, meaning okay to receive from a client

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Helpers
	static bool				canBuild( NounShip * pBuilder, Noun * pTarget, NounStructure * pBuild );
	static GadgetBuild *	getGadgetBuild( NounShip * pShip );
	static bool				getStructures( NounShip * pShip, Noun * pTarget, 
								Array< NounStructure * > & structures );
	static void				sortStructures( Array< NounStructure * > & structures );		// sort structures by groupId then sortId

	// Data
	Noun::tRef				m_pTarget;
	NounStructure::wRef		m_pBuild;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF


