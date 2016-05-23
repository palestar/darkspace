/**
	@file VerbBuildPlatform.h
	@brief TODO

	(c)2006 Palestar Inc
	@author Richard Lyle @date 5/27/2008 11:40:50 PM
*/

#ifndef VERBBUILDPLATFORM_H
#define VERBBUILDPLATFORM_H

#include "ShipPlatform.h"
#include "GadgetBuild.h"
#include "World/NounZoneTarget.h"
#include "World/Verb.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbBuildPlatform : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbBuildPlatform();
	VerbBuildPlatform( NounShip * pBuilder, NounZoneTarget * pTarget, ShipPlatform * pBuild );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;		// is this a client verb, meaning okay to receive from a client

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Helpers
	static bool				canBuild( NounShip * pBuilder, NounZoneTarget * pTarget, ShipPlatform * pBuild );
	static bool				getPlatforms( NounShip * pBuilder, NounZoneTarget * pTarget, 
								Array< ShipPlatform * > & platforms );

	// Data
	NounZoneTarget::tRef	m_pTarget;
	ShipPlatform::wRef		m_pBuild;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF

