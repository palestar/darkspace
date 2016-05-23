/*	
	WeaponSABOT2.h
	(c)2008 Palestar, Richard Lyle
	Multi-Rack Sabot Rocket
*/

#ifndef WEAPON_SABOT2_H
#define WEAPON_SABOT2_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponSABOT2 : public GadgetWeapon
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	Type type() const
	{
		return WEAPON;
	}
	int	 maxDamage() const
	{
		return (4600 + (level() * 150));
	}
	int addValue() const
	{
		return 3400;
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
		return 620;
	}
	// GadgetWeapon interface
	int energyCost() const	
	{
		return 300;
	}
	int energyCharge() const
	{
		return 1;
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
		return false;
	}
	bool turret() const
	{
		return true;
	}
	int	maxProjectiles() const
	{
		return 12;
	}
	int projectileCount() const
	{
		return 6;
	}
	int projectileDelay() const
	{
		return 4;
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
		return 120.0f;
	}
	float projectileLife() const
	{
		return 5.0f;
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
		return 15;
	}
	int tracerLife() const
	{
		return TICKS_PER_SECOND * 3;
	}
	dword damageType() const
	{
		return DAMAGE_KINETIC;
	}
	int damage() const
	{
		return 0;
	}
	int damageRandom() const
	{
		return 0;
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
		return 1800;
	}
	int areaDamageRandom() const
	{
		return 1200;
	}
	float areaDamageRadius() const
	{
		return 4.0f;
	}
	float armTime() const
	{
		return 0.8f;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
