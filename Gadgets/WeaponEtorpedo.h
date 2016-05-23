/*	
	WeaponEtorpedo.h
	(c)2009 Palestar, Richard Lyle
*/

#ifndef WEAPON_E_TORPEDO_H
#define WEAPON_E_TORPEDO_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponEtorpedo : public GadgetWeapon
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
		return (4500 + (level() * 1200));
	}
	int addValue() const
	{
		return 2800;
	}
	int buildTechnology() const
	{
		return 50;
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
		return 600;
	}

	// GadgetWeapon interface
	int energyCost() const	
	{
		return (700 + (140 * level()));
	}
	int energyCharge() const	
	{
		return 9;
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
		return 10;
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
		switch (level()) 
		{
			case 10:
				return 195.0f;
			default:
				return 650.0f - (level() * 65.0f);
		}
	}
	float projectileLife() const
	{
		return 2000.0f / projectileVelocity();
	}
	float projectileSig() const
	{
		return 5.0f;
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
		return PI / 6.0f;
	}
	int tracerRate() const
	{
		return 0;
	}
	int tracerLife() const
	{
		return TICKS_PER_SECOND * 1;
	}
	dword damageType() const
	{
		return DAMAGE_ENERGY;
	}
	int damage() const
	{
		return (level() * 350);
	}
	int damageRandom() const
	{
		return (level() * 700);
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
		return 3;
	}
	int areaDamage() const
	{
		return 1750;
	}
	int areaDamageRandom() const
	{
		return 3500;
	}
	float areaDamageRadius() const
	{
		return (35.0f * (1 + (level() / 5)));
	}
	float armTime() const
	{
		return 0;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
