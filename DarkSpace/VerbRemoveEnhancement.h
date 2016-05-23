/*
	VerbRemoveEnhancement.h

	This verb calls the use() mutator on an enhancement object
	(c)2008 Palestar, Jeff Everett
*/

#ifndef VERB_REMOVE_ENHANCEMENT_H
#define VERB_REMOVE_ENHANCEMENT_H

#include "NounGame.h"
#include "World/Verb.h"
#include "DarkSpace/NounShip.h"
#include "DarkSpace/NounEnhancement.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------


class DLL VerbRemoveEnhancement : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbRemoveEnhancement();
	VerbRemoveEnhancement( Noun * a_pUser, NounEnhancement *a_pEnhancement, bool a_bScrap );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;		// is this a client verb, meaning okay to receive from a client
	bool					queue() const;

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Data
	NounEnhancement::wRef	m_pEnhancement;
	bool					m_bScrap;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
