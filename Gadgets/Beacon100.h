/*
	Beacon100.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef BEACON100_H
#define BEACON100_H

#include "DarkSpace/GadgetBeacon.h"



//----------------------------------------------------------------------------

class Beacon100 : public GadgetBeacon
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	Type type() const
	{
		return BEACON;
	}
	int maxDamage() const
	{
		return 2700 + (level() * 150);
	}
	dword hotkey() const
	{
		return 'T';
	}
	int addValue() const
	{
		return 1000;
	}
	int buildTechnology() const
	{
		return 5;
	}
	int	buildCost() const
	{
		return 70;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 220;
	}

	// GadgetWeapon interface
	int energyCost() const	
	{
		return 80;
	}

	int energyCharge() const	
	{
		return 2;
	}

	int ammoCost() const
	{
		return 1;
	}

	int ammoMax() const
	{
		return 30;
	}

	int ammoReload() const
	{
		return 5;
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
		return true;
	}
	int	maxProjectiles() const
	{
		return 30;
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
		return false;
	}

	bool isMine() const
	{
		return false;
	}

	float projectileVelocity() const
	{
		return 55.0f;
	}

	float projectileLife() const
	{
		return 10.0f;			// fighter has an range of 10,000 gu one way
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
		return false;
	}
	float turnRate() const
	{
		return PI / 64.0f;
	}

	int tracerRate() const
	{
		return 10;
	}

	int tracerLife() const
	{
		return TICKS_PER_SECOND * 7;
	}

	dword damageType() const
	{
		return 0;
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
		return 3;
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
		return 0.0f;
	}

	// GadgetBeacon interface
	int beaconLife() const
	{
		return TICKS_PER_SECOND * 120;		// 2 minutes
	}
	float beaconSignature() const
	{
		return 10.0f;						
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
