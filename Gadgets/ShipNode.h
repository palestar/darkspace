/*
	ShipNode.h
	(c)2008 Palestar Inc, Richard Lyle
*/

#ifndef SHIP_NODE_H
#define SHIP_NODE_H

#include "DarkSpace/NounShip.h"



//----------------------------------------------------------------------------

class ShipNode : public NounShip
{
public:
	DECLARE_WIDGET_CLASS();

	// NounShip interface
	Type type() const
	{
		return UNKNOWN;
	}
	int buildTechnology() const
	{
		return 150;
	}
	int	buildCost() const
	{
		return 125000;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 12000;
	}
	int baseValue() const
	{
		return 10000000;
	}
	int baseEnergy() const
	{
		return 75000;
	}
	int baseDamage() const
	{
		return 1950000;
	}
	float baseVelocity() const
	{
		return 5.0f;
	}
	int baseCargo() const
	{
		return 20;
	}
	float baseSignature() const
	{
		return 50.0f;
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
		return 750000;
	}
	float explodeArea() const
	{
		return 450.0f;
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
		return 2.5f;
	}
	float shieldModifier() const
	{
		return 2.5f;
	}
	int	gadgetLevel() const
	{
		return 10;
	}
	float planetDefenseModifier() const
	{
		return 1.5f;
	}
	virtual float controlWeight() const
	{
		return 1.0f;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
