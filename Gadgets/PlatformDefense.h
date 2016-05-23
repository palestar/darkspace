/**
	@file PlatformDefense.h
	@brief TODO

	(c)2006 Palestar Inc
	@author Richard Lyle @date 5/26/2008 11:21:19 PM
*/

#ifndef PLATFORMDEFENSE_H
#define PLATFORMDEFENSE_H

#include "DarkSpace/ShipPlatform.h"

//----------------------------------------------------------------------------

class PlatformDefense : public ShipPlatform
{
public:
	DECLARE_WIDGET_CLASS();

	// NounShip interface
	Type type() const
	{
		return PLATFORM;
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
		return 4000;
	}
	int baseValue() const
	{
		return 20000;
	}
	int baseEnergy() const
	{
		return 55000;
	}
	int baseDamage() const
	{
		return 102000;
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
		return 95000;
	}
	float explodeArea() const
	{
		return 95.0f;
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
		return 2.5f;
	}
	float armorModifier() const
	{
		return 5.1f;
	}
	float shieldModifier() const
	{
		return 5.1f;
	}
	int	gadgetLevel() const
	{
		return 6;
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
		return 120;
	}
};

#endif

//---------------------------------------------------------------------------------------------------
//EOF
