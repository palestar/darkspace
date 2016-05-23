/*	
	WeaponHeavyMassDriver.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef WEAPON_HEAVY_MASS_DRIVER_H
#define WEAPON_HEAVY_MASS_DRIVER_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponHeavyMassDriver : public GadgetWeapon
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	Type type() const
	{
		return WEAPON;
	}
	int maxDamage() const
	{
		return 24000;
	}
	int addValue() const
	{
		return 16000;
	}
	int buildTechnology() const
	{
		return 55;
	}
	int	buildCost() const
	{
		return 250;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 970;
	}
	// GadgetWeapon interface
	int energyCost() const	
	{
		return (1800 + (level() * 360));
	}
	int energyCharge() const
	{
		return 25;
	}
	int ammoCost() const
	{
		return 1;
	}
	int ammoMax() const
	{
		return ( 600 ) / ( ( energyCost() / energyCharge() ) / 20 );
	}
	int ammoReload() const
	{
		return 2;
	}
	int ammoResources() const
	{
		return 9;
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
		return 8;
	}
	int projectileCount() const
	{
		return 2;
	}
	int projectileDelay() const
	{
		return 12;
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
		return 500.0f - (level() * 25.0f);
	}
	float projectileLife() const
	{
		return 1200.0f / projectileVelocity();
	}
	float projectileSig() const
	{
		return 4.0f;
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
		return 60;
	}
	int tracerLife() const
	{
		return TICKS_PER_SECOND * 12;
	}
	dword damageType() const
	{
		return DAMAGE_KINETIC;
	}
	int damage() const
	{
		return (2700 + (level() * 540));
	}
	int damageRandom() const
	{
		return (5400 + (level() * 1080));
	}
	bool reverseFalloff() const
	{
		return false;
	}
	float damageFalloff() const
	{
		return 0.833f;
	}
	int repairRate() const
	{
		return 6;
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
		return 0.0f;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
