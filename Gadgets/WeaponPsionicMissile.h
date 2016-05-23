/*	
	WeaponPsionicMissile.h
	(c)20011 Palestar, Richard Lyle
	Psionic Missile
	Created by Robert Kelford, February 28, 2011
*/

#ifndef WEAPON_PSIONIC_MISSILE_H
#define WEAPON_PSIONIC_MISSILE_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponPsionicMissile : public GadgetWeapon
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	Type type() const
	{
		return WEAPON_RANGED;
	}
	int maxDamage() const
	{
		return 500 + ( level() * 1500 );
	}
	int addValue() const
	{
		return 500 + ( level() * 1500 );
	}
	int buildTechnology() const
	{
		return 1 + ( level() * 10 );
	}
	int	buildCost() const
	{
		return 10 + ( level() * 40 );
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 420;
	}
	// GadgetWeapon interface
	int energyCost() const	
	{
		return ( 3600 + ( level() * 720 ));
	}
	int energyCharge() const
	{
		return 12;
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
		return 3;
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
		return 1000.0f / projectileVelocity();
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
		return TICKS_PER_SECOND * 6;
	}
	dword damageType() const
	{
		return DAMAGE_KINETIC | DAMAGE_PSI;
	}
	int damage() const
	{
		return ( 1200 + (level() * 240 ));
	}
	int damageRandom() const
	{
		return ( 7200 + (level() * 1440 ));
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
		return 0;
	}
	int areaDamageRandom() const
	{
		return 0;
	}
	float areaDamageRadius() const
	{
		return 0.0f;
	}
	float armTime() const
	{
		return ((50 * level()) / projectileVelocity());
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
