/*	
	WeaponPsiwaveSeekerMissile.h
	(c)2004 Palestar, Richard Lyle
	PSI Missile
*/

#ifndef WEAPON_PSIWAVE_SEEKER_MISSILE_H
#define WEAPON_PSIWAVE_SEEKER_MISSILE_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponPsiwaveSeekerMissile : public GadgetWeapon
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
		return (1650 + (level() * 75));
	}
	int addValue() const
	{
		return 2000;
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
		return 140;
	}
	// GadgetWeapon interface
	int energyCost() const	
	{
		return ( 2160 + (level() * 432 ));
	}
	int energyCharge() const
	{
		return 24;
	}
	int ammoCost() const
	{
		return 1;
	}
	int ammoMax() const
	{
		return ( 600 ) / ( ( energyCost() / energyCharge() ) / 20 );
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
		return 8;
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
		return 150.0f;
	}
	float projectileLife() const
	{
		return 2000.0f / projectileVelocity();
	}
	float projectileSig() const
	{
		return 8.0f;
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
		return PI / 8;
	}
	int tracerRate() const
	{
		return 5;
	}
	int tracerLife() const
	{
		return TICKS_PER_SECOND * 3;
	}
	dword damageType() const
	{
		return DAMAGE_KINETIC | DAMAGE_PSI;
	}
	int damage() const
	{
		return ( 3038 + (level() * 608 ));
	}
	int damageRandom() const
	{
		return ( 18225 + (level() * 3645 ));
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
		return ( 1013 + (level() * 203 ));
	}
	int areaDamageRandom() const
	{
		return ( 6075 + (level() * 1215 ));
	}
	float areaDamageRadius() const
	{
		return 90.0f;
	}
	float armTime() const
	{
		return (1000 / projectileVelocity());
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
