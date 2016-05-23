/*
	GadgetSpreadWeapon.h

	(c)2013 Palestar
*/

#ifndef GADGET_SPREAD_WEAPON_H
#define GADGET_SPREAD_WEAPON_H

#include "NounGadget.h"
#include "NounPlanet.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class NounProjectile;		// forward declare

//----------------------------------------------------------------------------

class DLL GadgetSpreadWeapon : public GadgetWeapon
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	GadgetSpreadWeapon();

	CharString useTip( Noun * pTarget, bool shift ) const;

	NounProjectile *	createProjectile( dword when, Noun * pTarget );

	//Accessors
	virtual float		spreadDegree() const = 0;		//how much each interation will spread in degrees off the base centershot
	virtual int			numSpread() const = 0;			//how many iterations of spread
};

#endif