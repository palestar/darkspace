/*
	FighterBayInterceptor.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef FIGHTERBAY_INTERCEPTOR_H
#define FIGHTERBAY_INTERCEPTOR_H

#include "DarkSpace/GadgetFighterBay.h"



//----------------------------------------------------------------------------

class FighterBayInterceptor : public GadgetFighterBay
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int maxDamage() const
	{
		return 14500;
	}

	dword hotkey() const
	{
		return 'N';
	}

	NounGadget::Type type() const
	{
		return WEAPON_FIGHTER;
	}

	int durability() const
	{
		return 0;
	}

	int addValue() const
	{
		return 8000;
	}
	int buildTechnology() const
	{
		return 10;
	}
	int	buildCost() const
	{
		return 1000;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 150;
	}

	// GadgetWeapon interface
	int energyCost() const	
	{
		return 300 * 60;
	}

	int energyCharge() const	
	{
		return 15;
	}

	int ammoCost() const
	{
		return 3;
	}

	int ammoMax() const
	{
		return 24;
	}

	int ammoReload() const
	{
		return 1;
	}

	int ammoResources() const
	{
		return 25;
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
		return 1;
	}
	int projectileCount() const
	{
		return 3;
	}

	int	projectileDelay() const
	{
		return 20;
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
		return 45.0f;
	}

	float projectileLife() const
	{
		return 300.0f;
	}

	float projectileSig() const
	{
		return 2.5f;
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
		return PI / (3.5f + (level() * 0.1));
	}
	int tracerRate() const
	{
		return 10;
	}

	int tracerLife() const
	{
		return TICKS_PER_SECOND * 1;
	}

	dword damageType() const
	{
		return DAMAGE_KINETIC | DAMAGE_ENERGY;
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
		return 1;
	}

	int areaDamage() const
	{
		return 1100;
	}

	int areaDamageRandom() const
	{
		return 500;
	}

	float areaDamageRadius() const
	{
		return 20.0f;
	}
	float armTime() const
	{
		return 0.0f;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
