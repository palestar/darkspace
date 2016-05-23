/*
	VerbDisband.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VERBDISBAND_H
#define VERBDISBAND_H

#include "World/Verb.h"
#include "NounUnit.h"
#include "NounShip.h"
#include "GameDll.h"
#include "GameDll.h"



//-------------------------------------------------------------------------------

class DLL VerbDisband : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbDisband();
	VerbDisband( NounUnit * pDisband, Noun * pWho );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;	

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Data
	Noun::wRef				m_pWho;

	static bool				canDisband( NounUnit * pDisband, Noun * pWho );
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
