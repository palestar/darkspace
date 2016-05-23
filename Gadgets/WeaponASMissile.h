/*	
	WeaponASMissile.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef WEAPON_AS_MISSILE_H
#define WEAPON_AS_MISSILE_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponASMissile : public GadgetWeapon
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
		return (2700 + (level() * 75));
	}
	int addValue() const
	{
		return 4000;
	}
	int buildTechnology() const
	{
		return 25;
	}
	int	buildCost() const
	{
		return 35;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 40;
	}

	// GadgetWeapon interface
	int energyCost() const	
	{
		return 20;
	}
	int energyCharge() const	
	{
		return 1;
	}
	int ammoCost() const
	{
		return 1;
	}
	int ammoMax() const
	{
		return 6;
	}
	int ammoReload() const
	{
		return 3;
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
		return false;
	}
	int	maxProjectiles() const
	{
		return 6;
	}
	int projectileCount() const
	{
		return 1;
	}
	int projectileDelay() const
	{
		return 0;
	}
	bool projectileSmart() const
	{
		return true;
	}
	bool isMine() const
	{
		return false;
	}
	float projectileVelocity() const
	{
		return 90.0f;
	}
	float projectileLife() const
	{
		return 4.0f;
	}
	float projectileSig() const
	{
		return 1.2f;
	}
	bool projectileCollidable() const
	{
		return true;
	}
	bool projectileAutoTarget() const
	{
		return true;
	}
	float turnRate() const
	{
		return PI / 4.0f + (level() * 0.5f);
	}
	int tracerRate() const
	{
		return 5;
	}
	int tracerLife() const
	{
		return TICKS_PER_SECOND * 6;
	}
	dword damageType() const
	{
		return DAMAGE_KINETIC;
	}
	int damage() const
	{
		return 960;
	}
	int damageRandom() const
	{
		return (320 + (level() * 16));
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
		return 1;
	}
	int areaDamage() const
	{
		return 480;
	}
	int areaDamageRandom() const
	{
		return (160 + (level() * 16));
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
