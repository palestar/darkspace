/*
	WeaponQSbomb.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef WEAPONQSBOMB_H
#define WEAPONQSBOMB_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponQSbomb : public GadgetWeapon
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	Type type() const
	{
		return WEAPON_BOMB;
	}
	dword hotkey() const
	{
		return 'B';
	}
	int maxDamage() const
	{
		return 9000;
	}
	int addValue() const
	{
		return 20000;
	}
	int buildTechnology() const
	{
		return 100;
	}
	int buildCost() const
	{
		return 3000;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 450;
	}
	// GadgetWeapon interface
	int energyCost() const	
	{
		return 200000;
	}
	int energyCharge() const
	{
		return 50;
	}
	int ammoCost() const
	{
		return 100;
	}
	int ammoMax() const
	{
		return 100;
	}
	int ammoReload() const
	{
		return 1;
	}
	int ammoResources() const
	{
		return 10000;
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
		return 1;
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
		return false;
	}
	bool isMine() const
	{
		return false;
	}
	float projectileVelocity() const
	{
		return 10.0f;
	}
	float projectileLife() const
	{
		return 50.0f;
	}
	float projectileSig() const
	{
		return 10.0f;
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
		return DAMAGE_KINETIC | DAMAGE_ENERGY;
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
		return 5;
	}
	int areaDamage() const
	{
		return 800000;
	}
	int areaDamageRandom() const
	{
		return 400000;
	}
	float areaDamageRadius() const
	{
		return 250.0f;
	}
	float armTime() const
	{
		return 10.0f;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
