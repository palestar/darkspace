/*
	MonsterGafin.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef MONSTERGAFIN_H
#define MONSTERGAFIN_H

#include "NounShip.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL MonsterGafin : public NounShip
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	MonsterGafin(); 

	// Noun interface
	void			simulate( dword nTick );
	void			inflictDamage( dword nWhen, Noun * pFrom, int damage, 
						dword type, const Vector3 & direction );	

	// NounShip interface
	Type type() const
	{
		return UNKNOWN;
	}
	int buildTechnology() const
	{
		return 1000;
	}
	int	buildCost() const
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
		return 175000;
	}
	float baseVelocity() const
	{
		return 50.0f;
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
	int	explodeDamage() const
	{
		return 45000;
	}
	float explodeArea() const
	{
		return 40.0f;
	}
	bool isMonster() const
	{
		return true;
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
		return 10;
	}
	float planetDefenseModifier() const
	{
		return 1.0f;
	}

private:
	// Data
	Noun::wRef		m_LastAttacker;
	int				m_DamageCaused;
	dword			m_nLogicTick;
};

//---------------------------------------------------------------------------------------------------

class DLL MonsterGafin2 : public MonsterGafin
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	MonsterGafin2(); 

	// NounShip interface
	int	baseEnergy() const
	{
		return 40000;
	}
	int	baseDamage() const
	{
		return 50000;
	}
	float baseVelocity() const
	{
		return 40.0f;
	}
	int	explodeDamage() const
	{
		return 15000;
	}
	float explodeArea() const
	{
		return 40.0f;
	}
	bool isMonster() const
	{
		return true;
	}
	float velocityModifier() const
	{
		return 4.0f;
	}
	int	gadgetLevel() const
	{
		return 3;
	}
	float planetDefenseModifier() const
	{
		return 1.0f;
	}
};

//---------------------------------------------------------------------------------------------------

class DLL MonsterGafin3 : public MonsterGafin
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	MonsterGafin3(); 

	// NounShip interface
	int	baseEnergy() const
	{
		return 75000;
	}
	int	baseDamage() const
	{
		return 150000;
	}
	float baseVelocity() const
	{
		return 50.0f;
	}
	int	explodeDamage() const
	{
		return 45000;
	}
	float explodeArea() const
	{
		return 40.0f;
	}
	bool isMonster() const
	{
		return true;
	}
	float velocityModifier() const
	{
		return 3.0f;
	}
	int	gadgetLevel() const
	{
		return 6;
	}
	float planetDefenseModifier() const
	{
		return 1.0f;
	}
};

//---------------------------------------------------------------------------------------------------

class DLL MonsterGafin4 : public MonsterGafin
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	MonsterGafin4(); 

	// NounShip interface
	int	baseEnergy() const
	{
		return 300000;
	}
	int	baseDamage() const
	{
		return 575000;
	}
	float baseVelocity() const
	{
		return 60.0f;
	}
	int	explodeDamage() const
	{
		return 175000;
	}
	float explodeArea() const
	{
		return 80.0f;
	}
	bool isMonster() const
	{
		return true;
	}
	float velocityModifier() const
	{
		return 2.0f;
	}
	float armorModifier() const
	{
		return 1.5f;
	}
	float shieldModifier() const
	{
		return 1.5f;
	}
	int	gadgetLevel() const
	{
		return 15;
	}
	float planetDefenseModifier() const
	{
		return 1.0f;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
