/*	
	ArmorChitin.h
	(c)2007 Palestar, Robert Kelford
*/

#ifndef ARMOR_CHITIN_H
#define ARMOR_CHITIN_H

#include "DarkSpace/GadgetArmor.h"



//----------------------------------------------------------------------------

class ArmorChitin : public GadgetArmor
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	addValue() const
	{
		return 10000;
	}
	Type type() const
	{
		return ARMOR;
	}
	int buildTechnology() const
	{
		return 50;
	}
	int	buildCost() const
	{
		return 700;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 700;
	}
	// GadgetArmor interface
	float addMass() const
	{
		return  armor() / 180000.0f;
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
		return (modifier() * (36000 + (12000 * level())));
	}
	float damageModifier( dword nType ) const
	{
		if ( ( nType & (DAMAGE_KINETIC) ) != 0 )
		{
			if      ( ( nType & (DAMAGE_ENERGY) ) != 0 )
				return 1.00f;	// 100% from Kinetic Energy
			else if ( ( nType & (DAMAGE_PSI) ) != 0 )
				return 0.75f;	// 75%  from Kinetic PSI
			else
				return 1.00f;	// 100% from Kinetic
		}
		else if ( ( nType & (DAMAGE_ENERGY) ) != 0 )
		{
			if      ( (nType & (DAMAGE_PSI) ) != 0 )
				return 0.75f;	// 75%   from Energy PSI
			else if ( ( nType & (DAMAGE_EMP) ) != 0 )
				return 1.00f;	// 100%  from Energy EMP
			else
				return 1.00f;	// 100%  from Energy
		}
			
		return 1.0f;
	}
	int	repairRate() const
	{
		return (modifier() * ( 30 + ( 10 * level())));
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
