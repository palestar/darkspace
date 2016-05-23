/*	
	WeaponHAMmine.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef WEAPON_HAM_MINE_H
#define WEAPON_HAM_MINE_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponHAMmine : public GadgetWeapon
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	Type type() const
	{
		return WEAPON_MINE;
	}
	dword hotkey() const
	{
		return 'B';
	}
	int addValue() const
	{
		return 5500;
	}
	int maxDamage() const
	{
		return (2700 + (level() * 75));
	}
	int buildTechnology() const
	{
		return 25;
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
		return 360;
	}
	// GadgetWeapon interface
	int energyCost() const	
	{
		return (270 + (level() * 27));
	}
	int energyCharge() const	
	{
		return 3;
	}
	int ammoCost() const
	{
		return 1;
	}
	int ammoMax() const
	{
		return 10;
	}
	int ammoReload() const
	{
		return 2;
	}
	int ammoResources() const
	{
		return 14;
	}
	bool needsTarget() const
	{
		return false;
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
		return false;
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
		return true;
	}
	float projectileVelocity() const
	{
		return 6.0f;
	}
	float projectileLife() const
	{
		return (600.0f + (level() * 50.0f));
	}
	float projectileSig() const
	{
		return 0.1f + (level() * 0.05f);
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
		return 0.0f;
	}
	int tracerRate() const
	{
		return 0;
	}
	int tracerLife() const
	{
		return TICKS_PER_SECOND * 0;
	}
	dword damageType() const
	{
		return DAMAGE_ENERGY | DAMAGE_KINETIC;
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
		return 3;
	}
	int areaDamage() const
	{
		return (2079 + (level() * 32));
	}
	int areaDamageRandom() const
	{
		return (693 + (level() * 15));
	}
	float areaDamageRadius() const
	{
		return 35.0f + level();
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
