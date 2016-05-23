/*	
	WeaponARmissile.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef WEAPON_AR_MISSILE_H
#define WEAPON_AR_MISSILE_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponARmissile : public GadgetWeapon
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
		return (2250 + (level() * 325));
	}
	int addValue() const
	{
		return 1750;
	}
	int buildTechnology() const
	{
		return 25;
	}
	int	buildCost() const
	{
		return 140;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 300;
	}

	// GadgetWeapon interface
	int energyCost() const	
	{
		return (300 + (level() * 15));
	}
	int energyCharge() const	
	{
		return 3;
	}
	int ammoCost() const
	{
		return 1;
	}
	int ammoMax() const
	{
		return 30 + level();
	}
	int ammoReload() const
	{
		return 1;
	}
	int ammoResources() const
	{
		return 2;
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
		return 85.0f;
	}
	float projectileLife() const
	{
		return (12.0f + (level() * 0.75f));
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
		return PI / 1.3f + (level() * 0.13);
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
		return DAMAGE_KINETIC;
	}
	int damage() const
	{
		return 1120;
	}
	int damageRandom() const
	{
		return (288 + ( level() * 16 ));
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
		return (1000 + ( level() * 10 ));
	}
	int areaDamageRandom() const
	{
		return (300 + ( level() * 30 ));
	}
	float areaDamageRadius() const
	{
		return 15.0f + ( level() * 3 );
	}
	float armTime() const
	{
		return 1.5f;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
