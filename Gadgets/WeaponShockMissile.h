/*	
	WeaponShockMissile.h
	(c)2004 Palestar, Richard Lyle
	PSI Missile
*/

#ifndef WEAPON_SHOCK_MISSILE_H
#define WEAPON_SHOCK_MISSILE_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponShockMissile : public GadgetWeapon
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
		return (7800 + (level() * 325));
	}
	int addValue() const
	{
		return 18000;
	}
	int buildTechnology() const
	{
		return 20;
	}
	int buildCost() const
	{
		return 150;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 630;
	}
	// GadgetWeapon interface
	int energyCost() const	
	{
		return ( 7200 + (level() * 1440 ));
	}
	int energyCharge() const
	{
		return 24;
	}
	int ammoCost() const
	{
		return 3;
	}
	int ammoMax() const
	{
		return ( 300 ) / ( ( energyCost() / energyCharge() ) / 20 ) * 3;
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
		return 3;
	}
	int projectileCount() const
	{
		return 3;
	}
	int projectileDelay() const
	{
		return 15;
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
		return 1000.0f / projectileVelocity();
	}
	float projectileSig() const
	{
		return 5.0f;
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
		return PI / ((0.875 * level()) - 2.125f);
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
		return ( 2400 + (level() * 480 ));
	}
	int damageRandom() const
	{
		return ( 14400 + (level() * 2880 ));
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
		return ((50 * level()) / projectileVelocity());
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
