/*
	VerbApplyEnhancement.h

	This verb calls the use() mutator on an enhancement object
	(c)2008 Palestar, Jeff Everett
*/

#ifndef VERB_APPLY_ENHANCEMENT_H
#define VERB_APPLY_ENHANCEMENT_H

#include "NounGame.h"
#include "World/Verb.h"
#include "NounShip.h"
#include "CargoEnhancement.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------


class DLL VerbApplyEnhancement : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbApplyEnhancement();
	VerbApplyEnhancement( Noun * a_pUser, CargoEnhancement * a_pEnhancement);

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;		// is this a client verb, meaning okay to receive from a client
	bool					queue() const;

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Data
	CargoEnhancement::wRef	m_pCargo;

	// Static
	static bool				canApply( CargoEnhancement * a_pCargo, NounShip * a_pUser );

};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
