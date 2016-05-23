/*	
	WeaponTKBomb.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef WEAPON_TK_BOMB_H
#define WEAPON_TK_BOMB_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"

//----------------------------------------------------------------------------

class WeaponTKBomb : public GadgetWeapon
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
		return (1300 + (level() * 50));
	}
	int addValue() const
	{
		return 3000;
	}
	int buildTechnology() const
	{
		return 10;
	}
	int buildCost() const
	{
		return 220;
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
		return (100 + (level() * 20));
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
		return ( 90 ) / ( ( energyCost() / energyCharge() ) / 20 );
	}
	int ammoReload() const
	{
		return 1;
	}
	int ammoResources() const
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
		return 12;
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
		return 50.0f;
	}
	float projectileSig() const
	{
		return level();
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
		return PI / 24.0f;
	}
	int tracerRate() const
	{
		return 20;
	}
	int tracerLife() const
	{
		return TICKS_PER_SECOND * 6;
	}
	dword damageType() const
	{
		return DAMAGE_KINETIC | DAMAGE_BIO | DAMAGE_PLANET;
	}
	int damage() const
	{
		return 1470;
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
		return 5;
	}
	int areaDamage() const
	{
		return 420;
	}
	int areaDamageRandom() const
	{
		return 0;
	}
	float areaDamageRadius() const
	{
		return 7.5f;
	}
	float armTime() const
	{
		return 6.0f;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
