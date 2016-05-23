/**
	@file VerbGadgetOff.h
	@brief This verb turns a gadget off...
	
	(c)2006 Palestar Inc
	@author Richard Lyle @date 5/26/2008 12:36:06 AM
*/

#ifndef VERBGADGETOFF_H
#define VERBGADGETOFF_H

#include "NounGadget.h"
#include "World/Verb.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL VerbGadgetOff : public Verb
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	VerbGadgetOff();
	VerbGadgetOff( NounGadget * pGadget );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;		// is this a client verb, meaning okay to receive from a client
	bool					queue() const;

	bool					canAttach( Noun * pNoun );
	void					onExecute();
};

#endif

//---------------------------------------------------------------------------------------------------
//EOF
