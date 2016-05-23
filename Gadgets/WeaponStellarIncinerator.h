/*	
	WeaponStellarIncinerator.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef WEAPON_STELLAR_INCINERATOR_H
#define WEAPON_STELLAR_INCINERATOR_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponStellarIncinerator : public GadgetWeapon
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
		return 30000 ;
	}
	int addValue() const
	{
		return 20000;
	}
	int buildTechnology() const
	{
		return 45;
	}
	int buildCost() const
	{
		return 450;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 940;
	}
	// GadgetWeapon interface
	int energyCost() const	
	{
		return (2400 + (level() * 480));
	}
	int energyCharge() const
	{
		return 30;
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
		return 4;
	}
	int projectileCount() const
	{
		return 2;
	}
	int projectileDelay() const
	{
		return 9;
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
		return 960.0f / projectileVelocity();
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
		return 50;
	}
	int tracerLife() const
	{
		return TICKS_PER_SECOND * 12;
	}
	dword damageType() const
	{
		return DAMAGE_KINETIC;
	}
	int damage() const
	{
		return (1800 + (level() * 360));
	}
	int damageRandom() const
	{
		return (10800 + (level() * 2160));
	}
	bool reverseFalloff() const
	{
		return false;
	}
	float damageFalloff() const
	{
		return 0.667f;
	}
	int repairRate() const
	{
		return 4;
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
