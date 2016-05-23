/*
	ShieldAuxillary.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef SHIELD_AUXILLARY_H
#define SHIELD_AUXILLARY_H

#include "DarkSpace/GadgetShield.h"



//----------------------------------------------------------------------------

class ShieldAuxillary : public GadgetShield
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	Type type() const
	{
		return SHIELD;
	}
	int	addValue() const
	{
		return 4000;
	}
	float addSignature() const
	{
		return active() ? (level() * .1f) * shieldPercent() : 0.0f;
	}
	int	maxDamage() const
	{
		return 3000 + (450 * level());
	}
	int buildTechnology() const
	{
		return 50;
	}
	int	buildCost() const
	{
		return 300;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 400;
	}
	// GadgetShield interface
	bool canDeflect( dword nType ) const
	{
		return (nType & (DAMAGE_KINETIC | DAMAGE_ENERGY | DAMAGE_ELF )) != 0;
	}
	int	energyCost() const
	{
		return 0;
	}
	int 	chargeEnergyCost() const
	{
		return 0;
	}
	int 	maxCharge() const
	{
		return (modifier() * ( 42000 + ( 14000 * level())));
	}
	float 	damageModifier( dword nType ) const
	{
		if ( ( nType & (DAMAGE_KINETIC) ) != 0 )
		{
			if      ( ( nType & (DAMAGE_ENERGY) ) != 0 )
			{
				if 	( ( nType & (DAMAGE_EMP) ) != 0 )
					return 1.50f;	// 100%  from Kinetic Energy EMP
				else
					return 1.00f;	// 100%  from Kinetic Energy
			}
			else if ( ( nType & (DAMAGE_PSI) ) != 0 )
				return 1.00f;	// 100%  from Kinetic PSI
			else if ( ( nType & (DAMAGE_EMP) ) != 0 )
				return 1.25f;	// 125%   from Kinetic EMP
			else
				return 1.00f;	// 100%  from Kinetic
		}
		else if ( ( nType & (DAMAGE_ENERGY) ) != 0 )
		{
			if      ( (nType & (DAMAGE_PSI) ) != 0 )
				return 1.00f;	// 100%  from Energy PSI
			else if ( ( nType & (DAMAGE_EMP) ) != 0 )
				return 1.25f;	// 125%   from Energy EMP
			else
				return 1.00f;	// 100%  from Energy
		}
		else if ( ( nType & (DAMAGE_ELF) ) != 0 )
		{
				return 1.50f;	// 150%  from ELF
		}	
		return 1.0f;
	}
	int 	chargeRate() const
	{
		return (modifier() * ( 15 + ( 5 * level())));
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
