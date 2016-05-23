/*
	ShipDestroyer.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef SHIP_DESTROYER_H
#define SHIP_DESTROYER_H

#include "DarkSpace/NounShip.h"



//----------------------------------------------------------------------------

class ShipDestroyer : public NounShip
{
public:
	DECLARE_WIDGET_CLASS();

	// NounShip interface
	Type type() const
	{
		return DESTROYER;
	}
	int buildTechnology() const
	{
		return 45;
	}
	int	buildCost() const
	{
		return 25000;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 2600;
	}
	int baseValue() const
	{
		return 175000;
	}
	int baseEnergy() const
	{
		return 24000;
	}
	int baseDamage() const
	{
		return 160000;
	}
	float baseVelocity() const
	{
		return 7.0f;
	}
	int baseCargo() const
	{
		return 3;
	}
	float baseSignature() const
	{
		return 3.0f;
	}
	float baseSensor() const
	{
		return 250.0f;
	}
	float baseView() const
	{
		return 4.0f;
	}
	float baseThrust() const
	{
		return 4.0f;
	}
	float baseYaw() const
	{
		return 3.14f;
	}
	virtual int	maxRepair() const
	{
		return 2000;
	}
	int explodeDamage() const
	{
		return 40000;
	}
	float explodeArea() const
	{
		return 80.0f;
	}
	float velocityModifier() const
	{
		return 9.0f;
	}
	float cloakEnergyModifier() const
	{
		return 1.0f;
	}
	float cloakTime() const
	{
		return 8.0f;
	}
	float uncloakTime() const
	{
		return 1.0f;
	}
	float maxCloakTime() const
	{
		return 60.0f;
	}
	float shieldEnergyModifier() const
	{
		return 1.00f;
	}
	float armorModifier() const
	{
		return 1.00f;
	}
	float shieldModifier() const
	{
		return 1.00f;
	}
	int	gadgetLevel() const
	{
		return 4;
	}
	float planetDefenseModifier() const
	{
		return 0.45f;
	}
	virtual dword despawnTime() const
	{
		return 5 * TICKS_PER_SECOND;
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
