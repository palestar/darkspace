/*	
	WeaponPCmissile.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef WEAPON_PC_MISSILE_H
#define WEAPON_PC_MISSILE_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponPCmissile : public GadgetWeapon
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
		return (9000 + (level() * 300));
	}
	int addValue() const
	{
		return 15750;
	}
	int buildTechnology() const
	{
		return 35;
	}
	int	buildCost() const
	{
		return 400;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 675;
	}

	// GadgetWeapon interface
	int energyCost() const	
	{
		return 1500;
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
		return 12;
	}
	int ammoReload() const
	{
		return 1;
	}
	int ammoResources() const
	{
		return 8;
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
		return 12;
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
		return 50.0f + level();
	}
	float projectileLife() const
	{
		return (120.0f + (level() * 2.0f));
	}
	float projectileSig() const
	{
		return 1.0f + (level() * 0.25f);
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
		return 20;
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
		return (4360 + (level() * 8));
	}
	int damageRandom() const
	{
		return 1220;
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
		return (6500 + (level() * 19));
	}
	int areaDamageRandom() const
	{
		return (2200 + (level() * 25));
	}
	float areaDamageRadius() const
	{
		return 60.0f + level();
	}
	float armTime() const
	{
		return 5.0f;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
