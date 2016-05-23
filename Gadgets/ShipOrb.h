/*
	ShipOrb.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef SHIPORB_H
#define SHIPORB_H

#include "DarkSpace/NounShip.h"



//----------------------------------------------------------------------------

class ShipOrb : public NounShip
{
public:
	DECLARE_WIDGET_CLASS();

	// Noun interface
	bool canDamage( dword type ) const
	{
		return false;
	}

	// NounShip interface
	Type type() const
	{
		return STATION;
	}
	int buildTechnology() const
	{
		return 0;
	}
	int	buildCost() const
	{
		return 0;
	}
	dword buildFlags() const
	{
		return 0;
	}
	int baseValue() const
	{
		return 0;
	}
	int baseEnergy() const
	{
		return 50000;
	}
	int baseDamage() const
	{
		return 50000000;
	}
	float baseVelocity() const
	{
		return 10.0f;
	}
	int baseCargo() const
	{
		return 30;
	}
	float baseSignature() const
	{
		return 0.0f;
	}
	float baseSensor() const
	{
		return 50000.0f;
	}
	float baseView() const
	{
		return 50.0f;
	}
	float baseThrust() const
	{
		return 10.0f;
	}
	float baseYaw() const
	{
		return 1.5f;
	}
	virtual int	maxRepair() const
	{
		return 1000;
	}
	int explodeDamage() const
	{
		return 2500000;
	}
	float explodeArea() const
	{
		return 250.0f;
	}
	float velocityModifier() const
	{
		return 1.0f;
	}
	float cloakEnergyModifier() const
	{
		return 1.0f;
	}
	float cloakTime() const
	{
		return 4.0f;
	}
	float uncloakTime() const
	{
		return 1.0f;
	}
	float maxCloakTime() const
	{
		return 1.0f;
	}
	float shieldEnergyModifier() const
	{
		return 1.0f;
	}
	float armorModifier() const
	{
		return 10.0f;
	}
	float shieldModifier() const
	{
		return 10.0f;
	}
	int	gadgetLevel() const
	{
		return 15;
	}
	float planetDefenseModifier() const
	{
		return 0.0f;
	}
	virtual float controlWeight() const
	{
		return 1.0f;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
