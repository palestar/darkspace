/*
	ShipStation.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef SHIP_STATION_H
#define SHIP_STATION_H

#include "DarkSpace/NounShip.h"



//----------------------------------------------------------------------------

class ShipStation : public NounShip
{
public:
	DECLARE_WIDGET_CLASS();

	// NounShip interface
	Type type() const
	{
		return STATION;
	}
	int buildTechnology() const
	{
		return 100;
	}
	int	buildCost() const
	{
		return 225000;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 6000;
	}
	int baseValue() const
	{
		return 15000000;
	}
	int baseEnergy() const
	{
		return 128000;
	}
	int baseDamage() const
	{
		return 2468100;
	}
	float baseVelocity() const
	{
		return 1.0f;
	}
	int baseCargo() const
	{
		return 12;
	}
	float baseSignature() const
	{
		return 20.0f;
	}
	float baseSensor() const
	{
		return 375.0f;
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
		return 7500;
	}
	int explodeDamage() const
	{
		return 500000;
	}
	float explodeArea() const
	{
		return 300.0f;
	}
	bool enableRollDynamics() const
	{
		return false;
	}
	float velocityModifier() const
	{
		return 0.5f;
	}
	float cloakEnergyModifier() const
	{
		return 1.0f;
	}
	float cloakTime() const
	{
		return 20.0f;
	}
	float uncloakTime() const
	{
		return 2.5f;
	}
	float maxCloakTime() const
	{
		return 150.0f;
	}
	float shieldEnergyModifier() const
	{
		return 1.7f;
	}
	float armorModifier() const
	{
		return 1.9f;
	}
	float shieldModifier() const
	{
		return 1.9f;
	}
	int	gadgetLevel() const
	{
		return 10;
	}
	float planetDefenseModifier() const
	{
		return 1.5f;
	}
	virtual dword despawnTime() const
	{
		return 20 * TICKS_PER_SECOND;
	}
	virtual float controlWeight() const
	{
		return 0.0f;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
