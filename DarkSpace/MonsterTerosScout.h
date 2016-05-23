/*
	MonsterTerosScout.h
	(c)2008 Palestar Inc, Jeff Everett
*/

#ifndef MONSTERTEROSSCOUT_H
#define MONSTERTEROSSCOUT_H

#include "MonsterTeros.h"
#include "GameDll.h"



//----------------------------------------------------------------------------
class DLL MonsterTerosScout : public MonsterTeros
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	MonsterTerosScout(); 

	// Noun interface
	void			simulate( dword nTick );

	// NounShip interface
	Type type() const
	{
		return SCOUT;
	}
	int buildTechnology() const
	{
		return 1000;
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
		return 50000;
	}
	int	baseDamage() const
	{
		return 50000;
	}
	float baseVelocity() const
	{
		return 35.0f;
	}
	int	baseCargo() const
	{
		return 1;
	}
	float baseSignature() const
	{
		return 1.0f;
	}
	float baseSensor() const
	{
		return 2000.0f;
	}
	float baseView() const
	{
		return 500.0f;
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
		return 1000;
	}
	int	explodeDamage() const
	{
		return 9000;
	}
	float explodeArea() const
	{
		return 140.0f;
	}

	bool isMonster() const
	{
		return true;
	}
	bool enableRollDynamics() const
	{
		return false;
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
		return 1.0f;
	}
	float armorModifier() const
	{
		return 0.5f;
	}
	float shieldModifier() const
	{
		return 0.5f;
	}
	int	gadgetLevel() const
	{
		return 4;
	}
	float planetDefenseModifier() const
	{
		return 0.25f;
	}

private:
	// Data
};

class DLL MonsterTerosScout2 : public MonsterTeros
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	MonsterTerosScout2(); 

	// Noun interface
	void			simulate( dword nTick );

	// NounShip interface
	Type type() const
	{
		return SCOUT;
	}
	int buildTechnology() const
	{
		return 1000;
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
		return 60000;
	}
	int	baseDamage() const
	{
		return 50000;
	}
	float baseVelocity() const
	{
		return 35.0f;
	}
	int	baseCargo() const
	{
		return 1;
	}
	float baseSignature() const
	{
		return 1.0f;
	}
	float baseSensor() const
	{
		return 2000.0f;
	}
	float baseView() const
	{
		return 500.0f;
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
		return 1000;
	}
	int	explodeDamage() const
	{
		return 9000;
	}
	float explodeArea() const
	{
		return 140.0f;
	}

	bool isMonster() const
	{
		return true;
	}
	bool enableRollDynamics() const
	{
		return false;
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
		return 1.0f;
	}
	float armorModifier() const
	{
		return 0.75f;
	}
	float shieldModifier() const
	{
		return 0.75f;
	}
	int	gadgetLevel() const
	{
		return 4;
	}
	float planetDefenseModifier() const
	{
		return 0.25f;
	}

private:
	// Data
};


class DLL MonsterTerosScout3 : public MonsterTeros
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	MonsterTerosScout3(); 

	// Noun interface
	void			simulate( dword nTick );

	// NounShip interface
	Type type() const
	{
		return SCOUT;
	}
	int buildTechnology() const
	{
		return 1000;
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
		return 80000;
	}
	int	baseDamage() const
	{
		return 50000;
	}
	float baseVelocity() const
	{
		return 35.0f;
	}
	int	baseCargo() const
	{
		return 1;
	}
	float baseSignature() const
	{
		return 1.0f;
	}
	float baseSensor() const
	{
		return 2000.0f;
	}
	float baseView() const
	{
		return 500.0f;
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
		return 1000;
	}
	int	explodeDamage() const
	{
		return 9000;
	}
	float explodeArea() const
	{
		return 140.0f;
	}

	bool isMonster() const
	{
		return true;
	}
	bool enableRollDynamics() const
	{
		return false;
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
		return 4;
	}
	float planetDefenseModifier() const
	{
		return 0.25f;
	}

private:
	// Data
};

class DLL MonsterTerosScout4 : public MonsterTeros
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	MonsterTerosScout4(); 

	// Noun interface
	void			simulate( dword nTick );

	// NounShip interface
	Type type() const
	{
		return SCOUT;
	}
	int buildTechnology() const
	{
		return 1000;
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
		return 100000;
	}
	int	baseDamage() const
	{
		return 50000;
	}
	float baseVelocity() const
	{
		return 35.0f;
	}
	int	baseCargo() const
	{
		return 1;
	}
	float baseSignature() const
	{
		return 1.0f;
	}
	float baseSensor() const
	{
		return 2000.0f;
	}
	float baseView() const
	{
		return 500.0f;
	}
	float baseThrust() const
	{
		return 4.0f;
	}
	float baseYaw() const
	{
		return 1.14f;
	}
	virtual int	maxRepair() const
	{
		return 1000;
	}
	int	explodeDamage() const
	{
		return 9000;
	}
	float explodeArea() const
	{
		return 140.0f;
	}

	bool isMonster() const
	{
		return true;
	}
	bool enableRollDynamics() const
	{
		return false;
	}

	float velocityModifier() const
	{
		return 1.0f;
	}
	float maxCloakTime() const
	{
		return 1.0f;
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
		return 4;
	}
	float planetDefenseModifier() const
	{
		return 0.25f;
	}

private:
	// Data
};


#endif

//----------------------------------------------------------------------------
//EOF
