/**
	@file ProxyShip.h
	@brief This ship type is created for proxy client connections... it is NEVER actually added to the world.

	(c)2009 Palestar Inc
	@author Richard Lyle @date 8/5/2010 9:48:57 PM
*/

#ifndef PROXYSHIP_H
#define PROXYSHIP_H

#include "NounShip.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL ProxyShip : public NounShip
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	ProxyShip()
	{}

	// Noun interface
	virtual NounType nounType() const
	{
		return TYPE_SERVER_OBJECT;
	}

	virtual void simulate( dword nTick )
	{}
	virtual bool canCollide() const
	{
		return false;
	}
	virtual bool collidable() const
	{
		return false;
	}

	// NounShip interface
	Type type() const
	{
		return UNKNOWN;
	}
	int buildTechnology() const
	{
		return 0;
	}
	int buildCost() const
	{
		return 0;
	}
	dword buildFlags() const
	{
		return 0;
	}
	int	baseValue() const
	{
		return 50;
	}
	int	baseEnergy() const
	{
		return 0;
	}
	int	baseDamage() const
	{
		return 0;
	}
	float baseVelocity() const
	{
		return 0.0f;
	}
	int	baseCargo() const
	{
		return 0;
	}
	float baseSignature() const
	{
		return 0.0f;
	}
	float baseSensor() const
	{
		return 0.0f;
	}
	float baseView() const
	{
		return 0.0f;
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
		return 0;
	}
	int	explodeDamage() const
	{
		return 0;
	}
	float explodeArea() const
	{
		return 0.0f;
	}

	bool isMonster() const
	{
		return false;
	}
	bool enableRollDynamics() const
	{
		return false;
	}

	float velocityModifier() const
	{
		return 0.0f;
	}
	float cloakEnergyModifier() const
	{
		return 0.0f;
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
		return 0.0f;
	}
	float armorModifier() const
	{
		return 0.0f;
	}
	float shieldModifier() const
	{
		return 0.0f;
	}
	int	gadgetLevel() const
	{
		return 1;
	}
	float planetDefenseModifier() const
	{
		return 1.0f;
	}
};


#endif

//---------------------------------------------------------------------------------------------------
//EOF
