/*	
	WeaponGuardianMissile.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef WEAPON_GUARDIAN_MISSILE_H
#define WEAPON_GUARDIAN_MISSILE_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponGuardianMissile : public GadgetWeapon
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
		return (10500 + (level() * 325));
	}
	int addValue() const
	{
		return 19000;
	}
	int buildTechnology() const
	{
		return 20;
	}
	int	buildCost() const
	{
		return 350;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 480;
	}
	// GadgetWeapon interface
	int energyCost() const	
	{
		return 1400;
	}
	int energyCharge() const
	{
		return 5;
	}
	int ammoCost() const
	{
		return 1;
	}
	int ammoMax() const
	{
		return 10;
	}
	int ammoReload() const
	{
		return 1;
	}
	int ammoResources() const
	{
		return 15;
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
		return 20;
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
		return 65.0f + level();
	}
	float projectileLife() const
	{
		return (44.0f + (level() * 2));
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
		return PI / 16.0f;
	}
	int tracerRate() const
	{
		return 10;
	}
	int tracerLife() const
	{
		return TICKS_PER_SECOND * 6;
	}
	dword damageType() const
	{
		return DAMAGE_KINETIC | DAMAGE_PSI;
	}
	int damage() const
	{
		return (1875 + (level() * 10));
	}
	int damageRandom() const
	{
		return 625;
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
		return (6750 + (level() * 50));
	}
	int areaDamageRandom() const
	{
		return 2250;
	}
	float areaDamageRadius() const
	{
		return 50.0f + (level() * 3);
	}
	float armTime() const
	{
		return 0.5f;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
