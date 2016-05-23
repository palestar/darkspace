/*	
	WeaponShredderMissile.h
	(c)2008 Palestar, Richard Lyle
	Quad MountShredder Launcher
*/

#ifndef WEAPON_SHREDDER_MISSILE2_H
#define WEAPON_SHREDDER_MISSILE2_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponShredderMissile2 : public GadgetWeapon
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
		return (3600 + (level() * 150));
	}
	int addValue() const
	{
		return 4000;
	}
	int buildTechnology() const
	{
		return 60;
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
		return 560;
	}
	// GadgetWeapon interface
	int energyCost() const	
	{
		return 300;
	}
	int energyCharge() const
	{
		return 2;
	}
	int ammoCost() const
	{
		return 1;
	}
	int ammoMax() const
	{
		return 25;
	}
	int ammoReload() const
	{
		return 1;
	}
	int ammoResources() const
	{
		return 7;
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
		return 30;
	}
	int projectileCount() const
	{
		return 4;
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
		return 95.0f;
	}
	float projectileLife() const
	{
		return (9.0f + (level() * 0.5f));
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
		/* Old value
		return PI / 3.4f;
		*/
		return PI / 1.7f + (level() * 0.17);
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
		return 688;
	}
	int damageRandom() const
	{
		return 135;
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
		return (504 + (level() * 7));
	}
	int areaDamageRandom() const
	{
		return (168 + level());
	}
	float areaDamageRadius() const
	{
		return 10.0f;
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
