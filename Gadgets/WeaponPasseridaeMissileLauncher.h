/*	
	WeaponPasseridaeMissileLancher.h
	A weapon that fires Multiple Sparrow class missiles
*/

#ifndef WEAPON_PASSERIDAE_MISSILE_LAUNCHER_H
#define WEAPON_PASSERIDAE_MISSILE_LAUNCHER_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetSpreadWeapon.h"



//----------------------------------------------------------------------------

class WeaponPasseridaeMissileLauncher : public GadgetSpreadWeapon
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
		return 4500;
	}
	int addValue() const
	{
		return 4500;
	}
	int buildTechnology() const
	{
		return 10;
	}
	int	buildCost() const
	{
		return 110;
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
		return ( 1000 + (level() * 200 )) * (numSpread() * 2) ;
	}
	int energyCharge() const	
	{
		return 10 * (numSpread() * 2);
	}
	int ammoCost() const
	{
		return 1;
	}
	int ammoMax() const
	{
		return ( level() * 120 ) / ( ( energyCost() / energyCharge() ) / 20 );
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
		return false;
	}
	float projectileVelocity() const
	{
		return (level() * 25.0f);
	}
	float projectileLife() const
	{
		return 2500.0f / projectileVelocity();
	}
	float projectileSig() const
	{
		return 2.0f;
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
		return PI / ((1.75 * level()) - 4.25f);
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
		return DAMAGE_KINETIC;
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
		return ((100 * level()) / projectileVelocity());
	}

	//GadgetSpreadWeapon interface
	float spreadDegree() const
	{
		return 15.0f;
	}
	int	numSpread() const
	{
		return 2;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
