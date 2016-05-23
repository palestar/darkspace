/*
	GadgetBeacon.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef GADGETBEACON_H
#define GADGETBEACON_H

#include "GadgetWeapon.h"
#include "NounShip.h"
#include "GameDll.h"



//----------------------------------------------------------------------------

class DLL GadgetBeacon : public GadgetWeapon
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef WidgetReference< GadgetBeacon >	wRef;

	// Construction
	GadgetBeacon();

	// NounGadget interface
	Type					type() const;
	// GadgetWeapon interface
	bool					hit( Noun * pTarget, NounProjectile * pProj, const Vector3 & intersect );
	
	// Accessors
	virtual	int				beaconLife() const = 0;					// life span of the beacon in ticks
	virtual float			beaconSignature() const = 0;			// signature added to the beacon parent

	// Helpers
	static GadgetBeacon *	getGadgetBeacon( NounShip * pShip );
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
