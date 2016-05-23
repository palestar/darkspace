/*	
	WeaponSABOT.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef WEAPON_SABOT_H
#define WEAPON_SABOT_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponSABOT : public GadgetWeapon
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	Type type() const
	{
		return WEAPON_MEDIUM;
	}
	int	 maxDamage() const
	{
		return (3394 + (level() * 1217));
	}
	int addValue() const
	{
		return 1700;
	}
	int buildTechnology() const
	{
		return 10;
	}
	int buildCost() const
	{
		return 60;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 310;
	}
	// GadgetWeapon interface
	int energyCost() const	
	{
		return ( 840 + (level() * 168 ));
	}
	int energyCharge() const
	{
		return 5;
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
		return 2;
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
		switch (level()) 
		{
			case 10:
				return 84.0f;
			default:
				return 280.0f - (level() * 28.0f);
		}
	}
	float projectileLife() const
	{
		return 720.0f / projectileVelocity();
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
		return ( 525 + (level() * 105 ));
	}
	int damageRandom() const
	{
		return ( 350 + (level() * 70 ));
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
		return 0;
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
