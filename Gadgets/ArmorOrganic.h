/*	
	ArmorOrganic.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef ARMOR_ORGANIC_H
#define ARMOR_ORGANIC_H

#include "DarkSpace/GadgetArmor.h"



//----------------------------------------------------------------------------

class ArmorOrganic : public GadgetArmor
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	addValue() const
	{
		return 5000;
	}
	Type type() const
	{
		return ARMOR;
	}
	int buildTechnology() const
	{
		return 15;
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
		return 630;
	}
	// GadgetArmor interface
	float addMass() const
	{
		return  armor() / 300000.0f;
	}
	bool canDeflect( dword nType ) const
	{
		return (nType & (DAMAGE_KINETIC | DAMAGE_ENERGY )) != 0;
	}
	int	maxArmor() const
	{
		return (modifier() * (30000 + (10000 * level())));
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
		return (modifier() * ( 60 + ( 20 * level())));
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
