/**
	@file PlatformSupply.h
	@brief TODO

	(c)2006 Palestar Inc
	@author Richard Lyle @date 5/26/2008 11:19:05 PM
*/

#ifndef PLATFORMSUPPLY_H
#define PLATFORMSUPPLY_H

#include "DarkSpace/ShipPlatform.h"

//----------------------------------------------------------------------------

class PlatformSupply : public ShipPlatform
{
public:
	DECLARE_WIDGET_CLASS();

	// NounShip interface
	Type type() const
	{
		return PLATFORM_SUPPLY;
	}
	virtual int buildTechnology() const
	{
		return 0;
	}
	virtual dword buildFlags() const
	{
		return 0;
	}
	virtual int	buildCost() const
	{
		return 3000;
	}
	int baseValue() const
	{
		return 20000;
	}
	int baseEnergy() const
	{
		return 50000;
	}
	int baseDamage() const
	{
		return 75200;
	}
	float baseVelocity() const
	{
		return 0.0f;
	}
	int baseCargo() const
	{
		return 10;
	}
	float baseSignature() const
	{
		return 2.0f;
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
		return 0.0f;
	}
	float baseYaw() const
	{
		return 0.0f;
	}
	virtual int	maxRepair() const
	{
		return 1000;
	}
	int explodeDamage() const
	{
		return 125000;
	}
	float explodeArea() const
	{
		return 110.0f;
	}
	float velocityModifier() const
	{
		return 0.0f;
	}
	float cloakEnergyModifier() const
	{
		return 1.0f;
	}
	float cloakTime() const
	{
		return 1.0f;
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
		return 2.0f;
	}
	float armorModifier() const
	{
		return 4.5f;
	}
	float shieldModifier() const
	{
		return 4.5f;
	}
	int	gadgetLevel() const
	{
		return 4;
	}
	float planetDefenseModifier() const
	{
		return 1.0f;
	}

	// ShipPlatform interface
	virtual int	ticksPerUpdate() const
	{
		return TICKS_PER_SECOND * 60;
	}
	virtual int	resourceCost() const
	{
		return 2;
	}
	virtual int	maxResources() const
	{
		return 6000;
	}
	virtual int	buildTime() const
	{
		return 100;
	}
};

#endif

//---------------------------------------------------------------------------------------------------
//EOF
