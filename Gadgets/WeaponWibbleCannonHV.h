/*	
	WeaponWibbleCannonHV.h
	Civilian gun repurposed for Pirate usage
*/

#ifndef WEAPON_WIBBLE_CANNONHV_H
#define WEAPON_WIBBLE_CANNONHV_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponWibbleCannonHV : public GadgetWeapon
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
		return (3300 + (level() * 375));
	}
	int addValue() const
	{
		return 2448;
	}
	int buildTechnology() const
	{
		return 15;
	}
	int	buildCost() const
	{
		return 60;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 340;
	}
	// GadgetWeapon interface
	int energyCost() const	
	{
		return (600 + (level() * 120));
	}
	int energyCharge() const
	{
		return 6;
	}
	int ammoCost() const
	{
		return 0;
	}
	int ammoMax() const
	{
		return 0;
	}
	int ammoReload() const
	{
		return 0;
	}
	int ammoResources() const
	{
		return 0;
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
		return 18;
	}
	int projectileCount() const
	{
		return 6;
	}
	int projectileDelay() const
	{
		return 2 + ceil(level() * 0.6f);
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
		switch (level()) 
		{
			case 10:
				return 180.0f;
			default:
				return 400.0f - (level() * 20.0f);
		}
	}
	float projectileLife() const
	{
		return 800.0f / projectileVelocity();
	}
	float projectileSig() const
	{
		return 0.0f;
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
		return 40;
	}
	int tracerLife() const
	{
		return TICKS_PER_SECOND;
	}
	dword damageType() const
	{
		return DAMAGE_KINETIC;
	}
	int damage() const
	{
		return (150 + (level() * 30));
	}
	int damageRandom() const
	{
		return (900 + (level() * 180));
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
		return 7;
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
