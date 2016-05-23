/*
	ArmorReflective.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef ARMORREFLECTIVE_H
#define ARMORREFLECTIVE_H

#include "DarkSpace/GadgetArmor.h"

//----------------------------------------------------------------------------

class ArmorReflective : public GadgetArmor
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	addValue() const
	{
		return 6000;
	}
	Type type() const
	{
		return ARMOR;
	}
	int buildTechnology() const
	{
		return 25;
	}
	int	buildCost() const
	{
		return 900;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 660;
	}
	// GadgetArmor interface
	float addMass() const
	{
		return armor() / 500000.0f;
	}
	float addSignature() const
	{
		return (level() * .15f) * armorPercent();
	}
	bool canDeflect( dword nType ) const
	{
		return (nType & (DAMAGE_KINETIC | DAMAGE_ENERGY )) != 0;
	}
	int	maxArmor() const
	{
		return (modifier() * (45000 + (15000 * level())));
	}
	float damageModifier( dword nType ) const
	{
		if ( ( nType & (DAMAGE_KINETIC) ) != 0 )
		{
			if      ( ( nType & (DAMAGE_ENERGY) ) != 0 )
				return 1.00f;	// 100%	 from Kinetic Energy
			else if ( ( nType & (DAMAGE_PSI) ) != 0 )
				return 1.75f;	// 175%  from Kinetic PSI
			else
				return 1.50f;	// 150%  from Kinetic
		}
		else if ( ( nType & (DAMAGE_ENERGY) ) != 0 )
		{
			if      ( (nType & (DAMAGE_PSI) ) != 0 )
				return 0.75f;	// 75%  from Energy PSI
			else if ( ( nType & (DAMAGE_EMP) ) != 0 )
				return 0.50f;	// 50%  from Energy EMP
			else
				return 0.50f;	// 50%  from Energy
		}
			
		return 1.0f;
	}
	int	repairRate() const
	{
		return (modifier() * ( 45 + ( 15 * level())));
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
