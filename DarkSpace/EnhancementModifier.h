/**
	@file EnhancementModifier.h
	@brief This the base class for all modifiers that are attached onto a NounEnhancement object.

	(c)2009 Palestar Inc
	@author Richard Lyle @date 5/5/2010 9:29:32 PM
*/

#ifndef ENHANCEMENTMODIFIER_H
#define ENHANCEMENTMODIFIER_H

#include "Factory/FactoryTypes.h"
#include "GameDll.h"

//---------------------------------------------------------------------------------------------------

class NounEnhancement;		// forward declare
class NounShip;

// Abstract class for a modifier object that is attached onto the NounEhancment object.
class DLL EnhancementModifier : public Widget
{
public:
	DECLARE_WIDGET_CLASS();

	//! Types
	typedef Reference< EnhancementModifier >		Ref;

	EnhancementModifier()
	{}

	//! This is invoked to get a description of this modifier.
	virtual CharString	getDescription(NounShip * a_pShip) = 0;
	//! Checks if this modifier can be attached to the given ship
	virtual bool		canAttach( NounShip * a_pShip ) = 0;
	//! This is invoked to apply this modiier to the ship.
	virtual bool		onApply( NounShip * a_pShip ) = 0;
	//! This is invoked to remove this modifier from the ship.
	virtual void		onRemove( NounShip * a_pShip ) = 0;
};

#endif

//---------------------------------------------------------------------------------------------------
//EOF
