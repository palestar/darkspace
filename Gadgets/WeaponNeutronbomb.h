/*	
	WeaponNeutronbomb.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef WEAPON_NEUTRON_BOMB_H
#define WEAPON_NEUTRON_BOMB_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"

//----------------------------------------------------------------------------

class WeaponNeutronBomb : public GadgetWeapon
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
	int	maxDamage() const
	{
		return (4200 + (level() * 75));
	}
	int	addValue() const
	{
		return 5000;
	}
	int buildTechnology() const
	{
		return 20;
	}
	int	buildCost() const
	{
		return 135;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 360;
	}
	// GadgetWeapon interface
	int	energyCost() const	
	{
		return (150 + (level() * 30));
	}
	int	energyCharge() const	
	{
		return 6;
	}
	int	ammoCost() const
	{
		return 1;
	}
	int	ammoMax() const
	{
		return ( 60 ) / ( ( energyCost() / energyCharge() ) / 20 );
	}
	int	ammoReload() const
	{
		return 3;
	}
	int	ammoResources() const
	{
		return 10;
	}
	bool needsTarget() const
	{
		return true;
	}
	bool needsLock() const
	{
		return false;
	}
	int	lockTime() const
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
		return 12;
	}
	int	projectileCount() const
	{
		return 1;
	}
	int	projectileDelay() const
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
				return 10.0f;
			default:
				return (45.0f - (level() * 5.0f));
		}
	}
	float projectileLife() const
	{
		return 750.0f / projectileVelocity();
	}
	float projectileSig() const
	{
		return (level() * 1.5f);
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
		return PI / 20.0f;
	}
	int	tracerRate() const
	{
		return 20;
	}
	int	tracerLife() const
	{
		return TICKS_PER_SECOND * 6;
	}
	dword damageType() const
	{
		return DAMAGE_BIO | DAMAGE_PLANET;
	}
	int	damage() const
	{
		return 0;
	}
	int	damageRandom() const
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
	int	repairRate() const
	{
		return 5;
	}
	int	areaDamage() const
	{
		return 900;
	}
	int	areaDamageRandom() const
	{
		return 400;
	}
	float areaDamageRadius() const
	{
		return 7.5f;
	}
	float armTime() const
	{
		return 100.0f / projectileVelocity();
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
