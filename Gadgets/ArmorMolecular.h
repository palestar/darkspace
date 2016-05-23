/*	
	ArmorMolecular.h
	(c)2011 Palestar, Eledore Massis
*/

#ifndef ARMORMOLECULAR_H
#define ARMORMOLECULAR_H

#include "DarkSpace/GadgetArmor.h"



//----------------------------------------------------------------------------

class ArmorMolecular : public GadgetArmor
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
		return 15;
	}
	int	buildCost() const
	{
		return 1200;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 810;
	}
	// GadgetArmor interface
	float addMass() const
	{
		return  armor() / 500000.0f;	
	}
	bool canDeflect( dword nType ) const
	{
		return (nType & (DAMAGE_KINETIC | DAMAGE_ENERGY )) != 0;
	}
	int	maxArmor() const
	{
		return (modifier() * ( 60000 + ( 20000 * level())));
	}
	float damageModifier( dword nType ) const
	{
		if ( ( nType & (DAMAGE_KINETIC) ) != 0 )
		{
			if      ( ( nType & (DAMAGE_ENERGY) ) != 0 )
				return 1.00f;	// 100%  from Kinetic Energy
			else if ( ( nType & (DAMAGE_PSI) ) != 0 )
				return 0.50f;	// 50%   from Kinetic PSI
			else
				return 1.00f;	// 100%  from Kinetic
		}
		else if ( ( nType & (DAMAGE_ENERGY) ) != 0 )
		{
			if      ( (nType & (DAMAGE_PSI) ) != 0 )
				return 0.50f;	// 50%   from Energy PSI
			else if ( ( nType & (DAMAGE_EMP) ) != 0 )
				return 1.50f;	// 150%  from Energy EMP
			else
				return 1.00f;	// 100%  from Energy
		}
			
		return 1.0f;
	}
	int	repairRate() const
	{
		return (modifier() * ( 120 + ( 40 * level())));
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
