/*	
	WeaponValenceAutocannon.h
	(c)2013 Palestar, Richard Lyle
*/

#ifndef WEAPON_VALENCE_AUTOCANNON_H
#define WEAPON_VALENCE_AUTOCANNON_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponValenceAutocannon : public GadgetWeapon
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
		return (2400 + (level() * 2650));
	}
	int addValue() const
	{
		return 1750;
	}
	int buildTechnology() const
	{
		return 10;
	}
	int	buildCost() const
	{
		return 50;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 260;
	}
	// GadgetWeapon interface
	int energyCost() const	
	{
		return (160 + (level() * 32));
	}
	int energyCharge() const
	{
		return 6;
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
		return 24;
	}
	int projectileCount() const
	{
		return 12;
	}
	int projectileDelay() const
	{
		return 3;
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
		return 500.0f - (level() * 25.0f);
	}
	float projectileLife() const
	{
		return 600.0f / projectileVelocity();
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
		return 10;
	}
	int tracerLife() const
	{
		return TICKS_PER_SECOND * 1;
	}
	dword damageType() const
	{
		return DAMAGE_ENERGY;
	}
	int damage() const
	{
		return (60 + (level() * 12));
	}
	int damageRandom() const
	{
		return (40 + (level() * 8));
	}
	bool reverseFalloff() const
	{
		return false;
	}
	float damageFalloff() const
	{
		return 0.5f;
	}
	int repairRate() const
	{
		return 3;
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
