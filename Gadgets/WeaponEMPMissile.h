/*	
	WeaponEMPMissile.h
	(c)2013 Palestar, Richard Lyle
*/

#ifndef WEAPON_EMP_MISSILE_H
#define WEAPON_EMP_MISSILE_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponEMPMissile : public GadgetWeapon
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
		return 6000;
	}
	int addValue() const
	{
		return 6000;
	}
	int buildTechnology() const
	{
		return 20;
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
		return 1;
	}

	// GadgetWeapon interface
	int energyCost() const	
	{
		return (3000 + (level() * 600));
	}
	int energyCharge() const	
	{
		return 10;
	}
	int ammoCost() const
	{
		return 3;
	}
	int ammoMax() const
	{
		return ( 300 ) / ( ( energyCost() / energyCharge() ) / 20 ) * 3;
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
		return false;
	}
	bool turret() const
	{
		return false;
	}
	int	maxProjectiles() const
	{
		return 10;
	}
	int projectileCount() const
	{
		return 3;
	}
	int projectileDelay() const
	{
		return 15;
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
		return (level() * 25.0f);
	}
	float projectileLife() const
	{
		return 1250.0f / projectileVelocity();
	}
	float projectileSig() const
	{
		return 5.0f;
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
		return PI / ((0.875 * level()) - 2.125f);
	}
	int tracerRate() const
	{
		return 10;
	}
	int tracerLife() const
	{
		return TICKS_PER_SECOND * 4;
	}
	dword damageType() const
	{
		return DAMAGE_KINETIC | DAMAGE_EMP;
	}
	int damage() const
	{
		return ( 1500 + (level() * 300 ));
	}
	int damageRandom() const
	{
		return ( 3000 + (level() * 600 ));
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
		return ( 1500 + (level() * 300 ));
	}
	int areaDamageRandom() const
	{
		return ( 3000 + (level() * 600 ));
	}
	float areaDamageRadius() const
	{
		return (level() * 5.0f);
	}
	float armTime() const
	{
		return ((75 * level()) / projectileVelocity());
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
