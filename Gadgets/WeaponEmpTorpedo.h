/*	
	WeaponProtonTorpedo.h
	(c)2009 Palestar, Richard Lyle
	Created by Robert Kelford, June 2009
*/

#ifndef WEAPON_EMP_TORPEDO_H
#define WEAPON_EMP_TORPEDO_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponEmpTorpedo : public GadgetWeapon
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
		return (4125 + (level() * 1532));
	}
	int addValue() const
	{
		return 2200;
	}
	int buildTechnology() const
	{
		return 40;
	}
	int	buildCost() const
	{
		return 100;
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
		return (360 + (level() * 90));
	}
	int energyCharge() const
	{
		return 7;
	}
	int ammoCost() const
	{
		return 1;
	}
	int ammoMax() const
	{
		return 50 - (level() * 2);
	}
	int ammoReload() const
	{
		return 1;
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
		return 55.0f - (level() * 1.5f);
	}
	float projectileLife() const
	{
		return (6.0f + level());
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
		return PI / 64.0f;
	}
	int tracerRate() const
	{
		return 25;
	}
	int tracerLife() const
	{
		return TICKS_PER_SECOND * 4;
	}
	dword damageType() const
	{
		return DAMAGE_EMP;
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
		return 1;
	}
	int areaDamage() const
	{
		return (1400 + (level() * 210));
	}
	int areaDamageRandom() const
	{
		return (445 + (level() * 89));
	}
	float areaDamageRadius() const
	{
		return 12.5f + (level() * 1.25f);
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
