/*	
	WeaponMassPulseCannon.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef WEAPON_MASS_PULSE_CANNON_H
#define WEAPON_MASS_PULSE_CANNON_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponMassPulseCannon : public GadgetWeapon
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	Type type() const
	{
		return WEAPON;
	}
	int	maxDamage() const
	{
		return (700 + (level() * 50));
	}
	int addValue() const
	{
		return 850;
	}
	int buildTechnology() const
	{
		return 5;
	}
	int	buildCost() const
	{
		return 20;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 30;
	}
	// GadgetWeapon interface
	int energyCost() const	
	{
		return 10;
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
		return 20 + ceil (level() * 0.5);
	}
	int ammoReload() const
	{
		return 1;
	}
	int ammoResources() const
	{
		return 1;
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
		return 24;
	}
	int projectileCount() const
	{
		return 6;
	}
	int projectileDelay() const
	{
		return 2;
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
		return 200.0f;
	}
	float projectileLife() const
	{
		return 1.0f;
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
		return 80;
	}
	int tracerLife() const
	{
		return TICKS_PER_SECOND * 1;
	}
	dword damageType() const
	{
		return DAMAGE_KINETIC;
	}
	int damage() const
	{
		return 84;
	}
	int damageRandom() const
	{
		return (28 + level());
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
		return 0;
	}
	int areaDamageRandom() const
	{
		return 0;
	}
	float areaDamageRadius() const
	{
		return 0.0f;
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
