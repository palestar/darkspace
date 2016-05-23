/*
	ShipFrigate.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef SHIP_FRIGATE_H
#define SHIP_FRIGATE_H

#include "DarkSpace/NounShip.h"



//----------------------------------------------------------------------------

class ShipFrigate : public NounShip
{
public:
	DECLARE_WIDGET_CLASS();

	// NounShip interface
	Type type() const
	{
		return FRIGATE;
	}
	int buildTechnology() const
	{
		return 10;
	}
	int	buildCost() const
	{
		return 7250;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 1500;
	}
	int baseValue() const
	{
		return 50000;
	}
	int baseEnergy() const
	{
		return 16000;
	}
	int baseDamage() const
	{
		return 53500;
	}
	float baseVelocity() const
	{
		return 10.0f;
	}
	int baseCargo() const
	{
		return 2;
	}
	float baseSignature() const
	{
		return 2.0f;
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
		return 0.65f;
	}
	float baseYaw() const
	{
		return 3.14f;
	}
	virtual int	maxRepair() const
	{
		return 1500;
	}
	int explodeDamage() const
	{
		return 20000;
	}
	float explodeArea() const
	{
		return 60.0f;
	}
	bool enableGateSpawn() const
	{
		return true;
	}
	float velocityModifier() const
	{
		return 10.0f;
	}
	float cloakEnergyModifier() const
	{
		return 1.0f;
	}
	float cloakTime() const
	{
		return 6.0f;
	}
	float uncloakTime() const
	{
		return 0.75f;
	}
	float maxCloakTime() const
	{
		return 45.0f;
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
		return 3;
	}
	float planetDefenseModifier() const
	{
		return 0.35f;
	}
	virtual dword despawnTime() const
	{
		return 2 * TICKS_PER_SECOND;
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
