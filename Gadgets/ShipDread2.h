/*
	ShipDread2.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef SHIP_DREAD2_H
#define SHIP_DREAD2_H

#include "DarkSpace/NounShip.h"



//----------------------------------------------------------------------------

class ShipDread2 : public NounShip
{
public:
	DECLARE_WIDGET_CLASS();

	// NounShip interface
	Type type() const
	{
		return DREAD;
	}
	int buildTechnology() const
	{
		return 90;
	}
	int	buildCost() const
	{
		return 114000;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 4400;
	}
	int baseValue() const
	{
		return 6000000;
	}
	int baseEnergy() const
	{
		return 64000;
	}
	int baseDamage() const
	{
		return 1040000;
	}
	float baseVelocity() const
	{
		return 3.0f;
	}
	int baseCargo() const
	{
		return 6;
	}
	float baseSignature() const
	{
		return 10.0f;
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
		return 4500;
	}
	int explodeDamage() const
	{
		return 208000;
	}
	float explodeArea() const
	{
		return 150.0f;
	}
	float velocityModifier() const
	{
		return 3.0f;
	}
	float cloakEnergyModifier() const
	{
		return 1.0f;
	}
	float cloakTime() const
	{
		return 12.0f;
	}
	float uncloakTime() const
	{
		return 1.5f;
	}
	float maxCloakTime() const
	{
		return 90.0f;
	}
	float shieldEnergyModifier() const
	{
		return 1.2f;
	}
	float armorModifier() const
	{
		return 1.3f;
	}
	float shieldModifier() const
	{
		return 1.3f;
	}
	int	gadgetLevel() const
	{
		return 6;
	}
	float planetDefenseModifier() const
	{
		return 1.0f;
	}
	virtual dword despawnTime() const
	{
		return 15 * TICKS_PER_SECOND;
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
