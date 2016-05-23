/*	
	WeaponHarpexMissile.h
	(c)2010 Palestar, Richard Lyle
	Created by Robert Kelford, 19th February 2010
*/

#ifndef WEAPON_HARPEX_MISSILE_H
#define WEAPON_HARPEX_MISSILE_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponHarpexMissile : public GadgetWeapon
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
		return 18000;
	}
	int addValue() const
	{
		return 18000;
	}
	int buildTechnology() const
	{
		return 70;
	}
	int	buildCost() const
	{
		return 450;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 1;
	}

	// GadgetWeapon interface
	int energyCost() const	
	{
		return ( 1600 + (level() * 320 ));
	}
	int energyCharge() const
	{
		return 16;
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
		return 18;
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
		return 8;
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
		return 2750.0f / projectileVelocity();
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
		return 20;
	}
	int tracerLife() const
	{
		return TICKS_PER_SECOND * 8;
	}
	dword damageType() const
	{
		return DAMAGE_KINETIC;
	}
	int damage() const
	{
		return ( 3600 + (level() * 720 ));
	}
	int damageRandom() const
	{
		return ( 2400 + (level() * 480 ));
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
		return ( 3600 + (level() * 720 ));
	}
	int areaDamageRandom() const
	{
		return ( 2400 + (level() * 480 ));
	}
	float areaDamageRadius() const
	{
		return (level() * 8.0f);
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
