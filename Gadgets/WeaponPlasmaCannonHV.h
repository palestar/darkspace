/*	
	WeaponPlasmaCannonHV.h
	(c)2008 Palestar, Richard Lyle
	Created by Robert Kelford, October 2008
*/

#ifndef WEAPON_PLASMA_CANNON_HV_H
#define WEAPON_PLASMA_CANNON_HV_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponPlasmaCannonHV : public GadgetWeapon
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	Type type() const
	{
		return WEAPON;
	}
	int maxDamage() const
	{
		return (7900 + (level() * 500));
	}
	int addValue() const
	{
		return 4500;
	}
	int buildTechnology() const
	{
		return 70;
	}
	int	buildCost() const
	{
		return 160;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 520;
	}
	// GadgetWeapon interface
	int energyCost() const	
	{
		return (1440 + (level() * 288));
	}
	int energyCharge() const
	{
		return 12;
	}
	int ammoCost() const
	{
		return 0;
	}
	int ammoMax() const
	{
		return 0;
	}
	int ammoReload() const
	{
		return 0;
	}
	int ammoResources() const
	{
		return 0;
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
		return true;
	}
	bool turret() const
	{
		return true;
	}
	int	maxProjectiles() const
	{
		return 9;
	}
	int projectileCount() const
	{
		return 4;
	}
	int projectileDelay() const
	{
		return 6;
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
		return 450.0f - (level() * 22.5f);
	}
	float projectileLife() const
	{
		return 640.0f / projectileVelocity();
	}
	float projectileSig() const
	{
		return 0.0f;
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
		return 20;
	}
	int tracerLife() const
	{
		return TICKS_PER_SECOND * 3;
	}
	dword damageType() const
	{
		return DAMAGE_ENERGY;
	}
	int damage() const
	{
		return 0;
	}
	int damageRandom() const
	{
		return 0;
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
		return 2;
	}
	int areaDamage() const
	{
		return (450 + (level() * 90));
	}
	int areaDamageRandom() const
	{
		return (2700 + (level() * 540));
	}
	float areaDamageRadius() const
	{
		return 5.0f;
	}
	float armTime() const
	{
		return 0.0f;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
