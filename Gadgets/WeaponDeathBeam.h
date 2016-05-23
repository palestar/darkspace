/*
	WeaponDeathBeam.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef WEAPONDEATHBEAM_H
#define WEAPONDEATHBEAM_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetBeamWeapon.h"



//----------------------------------------------------------------------------

class WeaponDeathBeam : public GadgetBeamWeapon
{
public:
	DECLARE_WIDGET_CLASS();

	WeaponDeathBeam()
	{
		// don't detach the hit effect on completion.. it has more animations to play
		m_bDetachHitEffect = false;
	}

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
		return 5000;
	}
	int durability() const
	{
		return 0;
	}
	int addValue() const
	{
		return 50000;
	}
	int buildTechnology() const
	{
		return 75;
	}
	int	buildCost() const
	{
		return 0;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 120;
	}
	// GadgetWeapon interface
	int energyCost() const
	{
		return 6000;
	}
	int energyCharge() const
	{
		return 1;
	}
	int damage() const
	{
		switch (level()) 
		{
			case 1:
				return 5000;
			case 4:
				return 6000;
			case 6:
				return 8000;
			case 10:
				return 50000;
			default:
				return 5000;
		}
		return 5000;

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
	dword damageType() const
	{
		return DAMAGE_BIO | DAMAGE_PLANET | DAMAGE_KINETIC | DAMAGE_ENERGY;
	}
	int duration() const
	{
		return 200;
	}
	float length() const
	{
		return 400.0f+(level()*50);
	}
	bool allowPointDefense() const
	{
		return false;
	}

	bool updateLogic()
	{
		return GadgetBeamWeapon::updateLogic();
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
