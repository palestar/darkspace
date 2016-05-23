/*
	VerbCaptureShip.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VERBCAPTURESHIP_H
#define VERBCAPTURESHIP_H

#include "World/Verb.h"
#include "NounShip.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbCaptureShip : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbCaptureShip();
	VerbCaptureShip( NounShip * pShip, int teamId, Noun * pWho );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Data
	int						m_nFleetId;
	Noun::wRef				m_pWho;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
