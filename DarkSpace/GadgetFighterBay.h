/*
	GadgetFighterBay.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef GADGETFIGHTERBAY_H
#define GADGETFIGHTERBAY_H

#include "GadgetWeapon.h"
#include "NounFighter.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL GadgetFighterBay : public GadgetWeapon
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	//! Noun interface
	virtual bool		postInitialize();
	// NounGadget interface
	bool				usable( Noun * pTarget, 
							bool shift ) const;								// can gadget be used on target
	void				use( dword when, Noun * pTarget, bool shift);		// use gadget

	// GadgetWeapon interface
	NounProjectile *	projectileFactory();
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
