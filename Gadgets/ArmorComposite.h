/*	
	ArmorComposite.h
	(c)2011 Palestar, Jim Starluck
*/

#ifndef ARMOR_COMPOSITE_H
#define ARMOR_COMPOSITE_H

#include "DarkSpace/GadgetArmor.h"



//----------------------------------------------------------------------------

class ArmorComposite : public GadgetArmor
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	addValue() const
	{
		return 4000;
	}
	Type type() const
	{
		return ARMOR;
	}
	int buildTechnology() const
	{
		return 5;
	}
	int	buildCost() const
	{
		return 450;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 420;
	}
	// GadgetArmor interface
	float addMass() const
	{
		return  armor() / 250000.0f;	
	}
	bool 	canDeflect( dword nType ) const
	{
		return (nType & (DAMAGE_KINETIC | DAMAGE_ENERGY )) != 0;
	}
	int	maxArmor() const
	{
		return (modifier() * ( 36000 + ( 12000 * level())));
	}
	float 	damageModifier( dword nType ) const
	{
		if ( ( nType & (DAMAGE_KINETIC) ) != 0 )
		{
			if      ( ( nType & (DAMAGE_ENERGY) ) != 0 )
				return 1.00f;	// 100%  from Kinetic Energy
			else if ( ( nType & (DAMAGE_PSI) ) != 0 )
				return 1.25f;	// 125%  from Kinetic PSI
			else
				return 1.00f;	// 100%  from Kinetic
		}
		else if ( ( nType & (DAMAGE_ENERGY) ) != 0 )
		{
			if      ( (nType & (DAMAGE_PSI) ) != 0 )
				return 1.25f;	// 125%  from Energy PSI
			else if ( ( nType & (DAMAGE_EMP) ) != 0 )
				return 0.75f;	// 75%   from Energy EMP
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
