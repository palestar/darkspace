/*	
	WeaponPSGmissile.h
	(c)2008 Palestar, Richard Lyle
	dual Mount PSI Missile
*/

#ifndef WEAPON_PSG_MISSILE2_H
#define WEAPON_PSG_MISSILE2_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"

//----------------------------------------------------------------------------

class WeaponPSGMissile2 : public GadgetWeapon
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	Type type() const
	{
		return WEAPON_RANGED;
	}
	int maxDamage() const
	{
		return (5200 + (level() * 200));
	}
	int addValue() const
	{
		return 12000;
	}
	int buildTechnology() const
	{
		return 50;
	}
	int	buildCost() const
	{
		return 180;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 840;
	}
	// GadgetWeapon interface
	int energyCost() const	
	{
		return 1200;
	}
	int energyCharge() const
	{
		return 4;
	}
	int ammoCost() const
	{
		return 1;
	}
	int ammoMax() const
	{
		return 20;
	}
	int ammoReload() const
	{
		return 2;
	}
	int ammoResources() const
	{
		return 19;
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
		return 2;
	}
	int projectileDelay() const
	{
		return 5;
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
		return 80.0f;
	}
	float projectileLife() const
	{
		return 25.0f;
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
		return PI / 2.5f + (level() * 0.25);
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
		return (2135 + (level()*16));
	}
	int damageRandom() const
	{
		return (580 + (level() * 4));
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
		return 2550;
	}
	int areaDamageRandom() const
	{
		return 974;
	}
	float areaDamageRadius() const
	{
		return 20.0f;
	}
	float armTime() const
	{
		return 4.0f;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
