/*	
	WeaponVarianceSiegeTorpedo.h
	(c)2014 Palestar, Richard Lyle
	Quantum Singularity Torpedo
*/

#ifndef WEAPON_VARIANCE_SIEGE_TORPEDO_H
#define WEAPON_VARIANCE_SIEGE_TORPEDO_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"

//----------------------------------------------------------------------------

class WeaponVarianceSiegeTorpedo : public GadgetWeapon
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	Type type() const
	{
		return WEAPON_BOMB;
	}
	int maxDamage() const
	{
		return 27000;
	}
	int addValue() const
	{
		return 18000;
	}
	int buildTechnology() const
	{
		return 35;
	}
	int	buildCost() const
	{
		return 500;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 980;
	}

	// GadgetWeapon interface
	int energyCost() const	
	{
		return (5600 + (1120 * level()));
	}
	int energyCharge() const
	{
		return 12;
	}
	int ammoCost() const
	{
		return 1;
	}
	int ammoMax() const
	{
		return ( 900 ) / ( ( energyCost() / energyCharge() ) / 20 );
	}
	int ammoReload() const
	{
		return ammoMax() / 10;
	}
	int ammoResources() const
	{
		return 5;
	}
	bool needsTarget() const
	{
		return true;
	}
	bool needsLock() const
	{
		return false;
	}
	int lockTime() const
	{
		return 0;
	}
	dword lockType() const
	{
		return 0;
	}
	bool checkRange() const
	{
		return true;
	}
	bool inheritVelocity() const
	{
		return false;
	}
	bool turret() const
	{
		return true;
	}
	int	maxProjectiles() const
	{
		return 12;
	}
	int projectileCount() const
	{
		return 4;
	}
	int projectileDelay() const
	{
		return (10 * (1 + (level() / 5)));
	}
	bool projectileSmart() const
	{
		return false;
	}
	bool isMine() const
	{
		return false;
	}
	float projectileVelocity() const
	{
		switch (level()) 
		{
			case 10:
				return 10.0f;
			default:
				return 45.0f - (level() * 5.0f);
		}
	}
	float projectileLife() const
	{
		return 1200.0f / projectileVelocity();
	}
	float projectileSig() const
	{
		return 4.0f;
	}
	bool projectileCollidable() const
	{
		return false;
	}
	bool projectileAutoTarget() const
	{
		return false;
	}
	float turnRate() const
	{
		return 0.0f;
	}
	int tracerRate() const
	{
		return 60;
	}
	int tracerLife() const
	{
		return TICKS_PER_SECOND * 8;
	}
	dword damageType() const
	{
		return DAMAGE_ENERGY | DAMAGE_KINETIC;
	}
	int damage() const
	{
		return (0);
	}
	int damageRandom() const
	{
		return (0);
	}
	bool reverseFalloff() const
	{
		return false;
	}
	float damageFalloff() const
	{
		return 1;
	}
	int repairRate() const
	{
		return 5;
	}
	int areaDamage() const
	{
		return (7875 * (1 + (level() / 5)));
	}
	int areaDamageRandom() const
	{
		return (5250 * (1 + (level() / 5)));
	}
	float areaDamageRadius() const
	{
		return (60 * (1 + (level() / 5)));
	}
	float armTime() const
	{
		return ( projectileLife() * 0.5f );
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
