/*
	ShipScout.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef SHIP_SCOUT_H
#define SHIP_SCOUT_H

#include "DarkSpace/NounShip.h"

//----------------------------------------------------------------------------

class ShipScout : public NounShip
{
public:
	DECLARE_WIDGET_CLASS();

	// NounShip interface
	Type type() const
	{
		return SCOUT;
	}
	int buildTechnology() const
	{
		return 0;
	}
	int	buildCost() const
	{
		return 4200;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 900;
	}
	int baseValue() const
	{
		return 10000;
	}
	int baseEnergy() const
	{
		return 8000;
	}
	int baseDamage() const
	{
		return 35200;
	}
	float baseVelocity() const
	{
		return 14.0f;
	}
	int baseCargo() const
	{
		return 1;
	}
	float baseSignature() const
	{
		return 1.0f;
	}
	float baseSensor() const
	{
		return 500.0f;
	}
	float baseView() const
	{
		return 4.0f;
	}
	float baseThrust() const
	{
		return 0.5f;
	}
	float baseYaw() const
	{
		return 3.14f;
	}
	virtual int	maxRepair() const
	{
		return 1000;
	}
	int explodeDamage() const
	{
		return 10000;
	}
	float explodeArea() const
	{
		return 40.0f;
	}
	bool enableGateSpawn() const
	{
		return true;
	}
	float velocityModifier() const
	{
		return 21.0f;
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
		return 0.5f;
	}
	float maxCloakTime() const
	{
		return 30.0f;
	}
	float shieldEnergyModifier() const
	{
		return 1.0f;
	}
	float armorModifier() const
	{
		return 1.0f;
	}
	float shieldModifier() const
	{
		return 1.0f;
	}
	int	gadgetLevel() const
	{
		return 2;
	}
	float planetDefenseModifier() const
	{
		return 0.25f;
	}
	virtual dword despawnTime() const
	{
		return 1 * TICKS_PER_SECOND;
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
