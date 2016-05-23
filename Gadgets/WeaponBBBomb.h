/*	
	WeaponBBBomb.h
	(c)2004 Palestar, Richard Lyle
	Building-Buster Bomb
*/

#ifndef WEAPON_BB_BOMB_H
#define WEAPON_BB_BOMB_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponBBBomb : public GadgetWeapon
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	Type type() const
	{
		return WEAPON_BOMB;
	}
	dword hotkey() const
	{
		return 'B';
	}
	int maxDamage() const
	{
		return (1500 + (level() * 50));
	}
	int addValue() const
	{
		return 4000;
	}
	int buildTechnology() const
	{
		return 10;
	}
	int	buildCost() const
	{
		return 32;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 40;
	}
	// GadgetWeapon interface
	int energyCost() const	
	{
		return 600;
	}
	int energyCharge() const
	{
		return 2;
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
		return 1;
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
		return true;
	}
	bool isMine() const
	{
		return false;
	}
	float projectileVelocity() const
	{
		return 35.0f;
	}
	float projectileLife() const
	{
		return 17.0f;
	}
	float projectileSig() const
	{
		return 0.5f;
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
		return PI / 6.0f;
	}
	int tracerRate() const
	{
		return 10;
	}
	int tracerLife() const
	{
		return TICKS_PER_SECOND * 3;
	}
	dword damageType() const
	{
		return DAMAGE_KINETIC | DAMAGE_PLANET;
	}
	int damage() const
	{
		return 625;
	}
	int damageRandom() const
	{
		return 169;
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
		return 387;
	}
	int areaDamageRandom() const
	{
		return 169;
	}
	float areaDamageRadius() const
	{
		return 15.0f;
	}
	float armTime() const
	{
		return 10.0f;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
