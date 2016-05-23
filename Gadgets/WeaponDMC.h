/*	
	WeaponDMC.h
	Dark Matter Cannon, Pirate T3 Weapon
*/

#ifndef WEAPON_DMC_H
#define WEAPON_DMC_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponDMC : public GadgetWeapon
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	Type type() const
	{
		return WEAPON_MEDIUM;
	}
	int maxDamage() const
	{
		return 27000;
	}
	int addValue() const
	{
		return 18000;
	}
	int buildTechnology() const
	{
		return 35;
	}
	int	buildCost() const
	{
		return 500;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 980;
	}

	// GadgetWeapon interface
	int energyCost() const	
	{
		return (5250 + (1050 * level()));
	}
	int energyCharge() const
	{
		return 45;
	}
	int ammoCost() const
	{
		return 1;
	}
	int ammoMax() const
	{
		return ( 300 ) / ( ( energyCost() / energyCharge() ) / 20 );
	}
	int ammoReload() const
	{
		return ammoMax() / 10;
	}
	int ammoResources() const
	{
		return 5;
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
		return true;
	}
	int	maxProjectiles() const
	{
		return 4;
	}
	int projectileCount() const
	{
		return 1;
	}
	int projectileDelay() const
	{
		return 20;
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
				return 54.0f;
			default:
				return 180.0f - (level() * 18.0f);
		}
	}
	float projectileLife() const
	{
		return 600.0f / projectileVelocity();
	}
	float projectileSig() const
	{
		return 10.0f;
	}
	bool projectileCollidable() const
	{
		return true;
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
		return TICKS_PER_SECOND * 2;
	}
	dword damageType() const
	{
		return DAMAGE_ENERGY;
	}
	int damage() const
	{
		return (level() * 2625);
	}
	int damageRandom() const
	{
		return (level() * 5250);
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
		return 13125;
	}
	int areaDamageRandom() const
	{
		return 26250;
	}
	float areaDamageRadius() const
	{
		return (70.0f * (1 + (level() / 5)));
	}
	float armTime() const
	{
		return 2.0f;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
