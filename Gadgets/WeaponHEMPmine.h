/*	
	WeaponHEMPmine.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef WEAPON_HEMP_MINE_H
#define WEAPON_HEMP_MINE_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponHEMPmine : public GadgetWeapon
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
	int maxDamage() const
	{
		return (3000 + (level() * 75));
	}
	int addValue() const
	{
		return 5000;
	}
	int buildTechnology() const
	{
		return 15;
	}
	int	buildCost() const
	{
		return 80;
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
		return (320 + (level() * 32));
	}
	int energyCharge() const	
	{
		return 4;
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
		return 12;
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
		return DAMAGE_ENERGY | DAMAGE_KINETIC | DAMAGE_EMP;
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
		return (1962 + (level() * 32));
	}
	int areaDamageRandom() const
	{
		return (654 + (level() * 18));
	}
	float areaDamageRadius() const
	{
		return 30.0f;
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
