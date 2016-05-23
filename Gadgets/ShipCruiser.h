/*
	ShipCruiser.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef SHIP_CRUISER_H
#define SHIP_CRUISER_H

#include "DarkSpace/NounShip.h"



//----------------------------------------------------------------------------

class ShipCruiser : public NounShip
{
public:
	DECLARE_WIDGET_CLASS();

	// NounShip interface
	Type type() const
	{
		return CRUISER;
	}
	int buildTechnology() const
	{
		return 75;
	}
	int	buildCost() const
	{
		return 48600;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 3200;
	}
	int baseValue() const
	{
		return 1000000;
	}
	int baseEnergy() const
	{
		return 32000;
	}
	int baseDamage() const
	{
		return 406400;
	}
	float baseVelocity() const
	{
		return 5.0f;
	}
	int baseCargo() const
	{
		return 4;
	}
	float baseSignature() const
	{
		return 5.0f;
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
		return 0.5f;
	}
	float baseYaw() const
	{
		return 3.14f;
	}
	virtual int	maxRepair() const
	{
		return 3000;
	}
	int explodeDamage() const
	{
		return 95000;
	}
	float explodeArea() const
	{
		return 100.0f;
	}
	float velocityModifier() const
	{
		return 5.0f;
	}
	float cloakEnergyModifier() const
	{
		return 1.0f;
	}
	float cloakTime() const
	{
		return 10.0f;
	}
	float uncloakTime() const
	{
		return 1.25f;
	}
	float maxCloakTime() const
	{
		return 75.0f;
	}
	float shieldEnergyModifier() const
	{
		return 1.1f;
	}
	float armorModifier() const
	{
		return 1.1f;
	}
	float shieldModifier() const
	{
		return 1.1f;
	}
	int	gadgetLevel() const
	{
		return 5;
	}
	float planetDefenseModifier() const
	{
		return 0.75f;
	}
	virtual dword despawnTime() const
	{
		return 10 * TICKS_PER_SECOND;
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
