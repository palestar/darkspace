/*	
	WeaponAMmine.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef WEAPON_AM_MINE_H
#define WEAPON_AM_MINE_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"



//----------------------------------------------------------------------------

class WeaponAMmine : public GadgetWeapon
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
		return 4200;
	}
	int maxDamage() const
	{
		return (3000 + (level() * 150));
	}
	int buildTechnology() const
	{
		return 25;
	}
	int	buildCost() const
	{
		return 65;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 320;
	}
	// GadgetWeapon interface
	int energyCost() const	
	{
		return ( 1080 + (level() * 216 ));
	}
	int energyCharge() const	
	{
		return 12;
	}
	int ammoCost() const
	{
		return 1;
	}
	int ammoMax() const
	{
		return ( 300 ) / ( ( energyCost() / energyCharge() ) / 20 );
	}
	int ammoReload() const
	{
		return Max( (ammoMax() / 10), 1);
	}
	int ammoResources() const
	{
		return 15;
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
		return true;
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
		return true;
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
		return 900.0f;
	}
	float projectileSig() const
	{
		return 0.25f;
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
		return PI / ((0.1f * level() * level()) - 0.4f);
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
		return (level() * 180 );
	}
	int damageRandom() const
	{
		return (level() * 1080 );
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
		return 900;
	}
	int areaDamageRandom() const
	{
		return 5400;
	}
	float areaDamageRadius() const
	{
		return (level() * 24.0f);
	}
	float armTime() const
	{
		return ( projectileVelocity() / areaDamageRadius() );
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
