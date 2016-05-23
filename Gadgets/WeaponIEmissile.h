/*	
	WeaponIEmissile.h
	(c)2004 Palestar, Richard Lyle
	Ion Tracker Missile
*/

#ifndef WEAPON_IE_MISSILE_H
#define WEAPON_IE_MISSILE_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponIEmissile : public GadgetWeapon
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
		return (4500 + (level() * 200));
	}
	int addValue() const
	{
		return 5250;
	}
	int buildTechnology() const
	{
		return 45;
	}
	int	buildCost() const
	{
		return 120;
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
		return 800;
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
		return 15 + level();
	}
	int ammoReload() const
	{
		return 1;
	}
	int ammoResources() const
	{
		return 4;
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
		return 70.0f;
	}
	float projectileLife() const
	{
		return (35.0f + level());
	}
	float projectileSig() const
	{
		return 0.75f;
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
		/* Old value
		return PI / 5.8f;
		*/
		return PI / 2.9f + (level() * 0.29);
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
		/* old
		return 1440;
		*/
		return 2520;
	}
	int damageRandom() const
	{
		return 480;
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
		return (2700 + ceil (level() * 37.5));
	}
	int areaDamageRandom() const
	{
		return 900;
	}
	float areaDamageRadius() const
	{
		return 30.0f + (level() *2);
	}
	float armTime() const
	{
		return 3.0f;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
