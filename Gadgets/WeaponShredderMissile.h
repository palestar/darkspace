/*	
	WeaponShredderMissile.h
	(c)2004 Palestar, Richard Lyle
	PSI Missile
*/

#ifndef WEAPON_SHREDDER_MISSILE_H
#define WEAPON_SHREDDER_MISSILE_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponShredderMissile : public GadgetWeapon
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
		return (1800 + (level() * 75));
	}
	int addValue() const
	{
		return 2000;
	}
	int buildTechnology() const
	{
		return 20;
	}
	int buildCost() const
	{
		return 200;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 280;
	}
	// GadgetWeapon interface
	int energyCost() const	
	{
		return ( 1200 + (level() * 240 ));
	}
	int energyCharge() const
	{
		return 12;
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
		return TICKS_PER_SECOND * 4;
	}
	dword damageType() const
	{
		return DAMAGE_KINETIC;
	}
	int damage() const
	{
		return ( 900 + (level() * 180 ));
	}
	int damageRandom() const
	{
		return ( 5400 + (level() * 1080 ));
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
		return ( 900 + (level() * 180 ));
	}
	int areaDamageRandom() const
	{
		return ( 5400 + (level() * 1080 ));
	}
	float areaDamageRadius() const
	{
		return (level() * 6.0f);
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
