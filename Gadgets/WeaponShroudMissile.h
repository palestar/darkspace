/*	
	WeaponShroudMissile.h
	(c)2011 Palestar, Richard Lyle
	Shroud Missile
	Created by Robert Kelford, February 28, 2011
*/

#ifndef WEAPON_SHROUD_MISSILE_H
#define WEAPON_SHROUD_MISSILE_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponShroudMissile : public GadgetWeapon
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
		return 500 + ( level() * 1650 );
	}
	int addValue() const
	{
		return 500 + ( level() * 1650 );
	}
	int buildTechnology() const
	{
		return level() * 10;
	}
	int	buildCost() const
	{
		return 10 + ( level() * 40 );
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 420;
	}
	// GadgetWeapon interface
	int energyCost() const	
	{
		return ( 2400 + (level() * 480 ));
	}
	int energyCharge() const
	{
		return 24;
	}
	int ammoCost() const
	{
		return 1;
	}
	int ammoMax() const
	{
		return ( 900 ) / ( ( energyCost() / energyCharge() ) / 20 );
	}
	int ammoReload() const
	{
		return ammoMax() / 10;
	}
	int ammoResources() const
	{
		return 3;
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
		return 5;
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
		return (level() * 25.0f);
	}
	float projectileLife() const
	{
		return 2250.0f / projectileVelocity();
	}
	float projectileSig() const
	{
		return 2.0f;
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
		return PI / ((1.75 * level()) - 4.25f);
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
		return ( 1800 + ( level() * 360 ));
	}
	int damageRandom() const
	{
		return ( 10800 + ( level() * 2160 ));
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
		return ( 1800 + ( level() * 360 ));
	}
	int areaDamageRandom() const
	{
		return ( 10800 + ( level() * 2160 ));
	}
	float areaDamageRadius() const
	{
		return (level() * 12.0f);
	}
	float armTime() const
	{
		return ((100 * level()) / projectileVelocity());
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
