/*	
	WeaponIonTorpedo.h
	(c)2009 Palestar, Richard Lyle
*/

#ifndef WEAPON_ION_TORPEDO_H
#define WEAPON_ION_TORPEDO_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponIonTorpedo : public GadgetWeapon
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
		return (3000 + (level() * 1250));
	}
	int addValue() const
	{
		return 3000;
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
		return (540 + (108 * level()));
	}
	int energyCharge() const	
	{
		return 6;
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
		return 1;
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
				return 66.0f;
			default:
				return 220.0f - (level() * 22.0f);
		}
	}
	float projectileLife() const
	{
		return 500.0f / projectileVelocity();
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
		return PI / 15.0f;
	}
	int tracerRate() const
	{
		return 20;
	}
	int tracerLife() const
	{
		return TICKS_PER_SECOND * 3;
	}
	dword damageType() const
	{
		return DAMAGE_ENERGY | DAMAGE_EMP;
	}
	int damage() const
	{
		return (level() * 216);
	}
	int damageRandom() const
	{
		return (level() * 432);
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
		return 1;
	}
	int areaDamage() const
	{
		return 1080;
	}
	int areaDamageRandom() const
	{
		return 2160;
	}
	float areaDamageRadius() const
	{
		return (20.0f * (1 + (level() / 5)));
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

