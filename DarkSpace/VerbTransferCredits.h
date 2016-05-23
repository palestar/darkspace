/*
	VerbTransferCredits.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VERBTRANSFERCREDITS_H
#define VERBTRANSFERCREDITS_H

#include "NounShip.h"
#include "World/Verb.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbTransferCredits : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbTransferCredits();
	VerbTransferCredits( NounShip * pSrc, Noun * pDst, int credits );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;			// is this a client verb, meaning okay to receive from a client

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Data
	NounShip::wRef			m_pOrigin;
	Noun::wRef				m_pDestination;
	int						m_Credits;

	static bool				canTransferCredits( NounShip * pSrc, Noun * pDst, int credits );
};

//----------------------------------------------------------------------------

#endif

//----------------------------------------------------------------------------
// EOF
