/*
	MonsterTeros.h
	(c)2008 Palestar Inc, Jeff Everett

	Base teros class
*/

#ifndef MONSTERTEROS_H
#define MONSTERTEROS_H

#include "NounShip.h"
#include "GameDll.h"



//----------------------------------------------------------------------------
// make a base ai ship type, add in how smart, use that numebr to decide how often to update order
class DLL MonsterTeros : public NounShip
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	MonsterTeros(); 

	// Noun interface
	void			simulate( dword nTick );

	// NounShip interface
	virtual Type type() const
	{
		return UNKNOWN;
	}
	virtual int buildTechnology() const
	{
		return 1000;
	}
	virtual int buildCost() const
	{
		return 0;
	}
	virtual dword buildFlags() const
	{
		return 0;
	}
	virtual int	baseValue() const
	{
		return 50;
	}
	virtual int	baseEnergy() const
	{
		return 100000;
	}
	virtual int	baseDamage() const
	{
		return 50000;
	}
	virtual float baseVelocity() const
	{
		return 35.0f;
	}
	virtual int	baseCargo() const
	{
		return 1;
	}
	virtual float baseSignature() const
	{
		return 1.0f;
	}
	virtual float baseSensor() const
	{
		return 2000.0f;
	}
	virtual float baseView() const
	{
		return 500.0f;
	}
	virtual float baseThrust() const
	{
		return 4.0f;
	}
	virtual float baseYaw() const
	{
		return 1.14f;
	}
	virtual int	maxRepair() const
	{
		return 1000;
	}
	virtual int	explodeDamage() const
	{
		return 150000;
	}
	virtual float explodeArea() const
	{
		return 140.0f;
	}

	virtual bool isMonster() const
	{
		return true;
	}
	virtual bool enableRollDynamics() const
	{
		return false;
	}

	virtual float velocityModifier() const
	{
		return 1.0f;
	}
	virtual float cloakEnergyModifier() const
	{
		return 1.0f;
	}
	virtual float cloakTime() const
	{
		return 1.0f;
	}
	virtual float uncloakTime() const
	{
		return 1.0f;
	}
	virtual float maxCloakTime() const
	{
		return 1.0f;
	}
	virtual float shieldEnergyModifier() const
	{
		return 1.0f;
	}
	virtual float armorModifier() const
	{
		return 1.0f;
	}
	virtual float shieldModifier() const
	{
		return 1.0f;
	}
	virtual int	gadgetLevel() const
	{
		return 4;
	}
	float planetDefenseModifier() const
	{
		return 1.0f;
	}

protected:
	// Data
	dword			m_nLogicTick;
};



#endif

//----------------------------------------------------------------------------
//EOF
