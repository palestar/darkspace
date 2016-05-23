/*
	MonsterEnergy.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef MONSTERENERGY_H
#define MONSTERENERGY_H

#include "NounShip.h"
#include "GameDll.h"



//----------------------------------------------------------------------------

class DLL MonsterEnergy : public NounShip
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	MonsterEnergy(); 

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
		return 50000;
	}
	float baseVelocity() const
	{
		return 45.0f;
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
		return 10.0f;
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
		return 10;
	}
	float planetDefenseModifier() const
	{
		return 1.0f;
	}

private:
	// Data
	dword		m_nLogicTick;
};



#endif

//----------------------------------------------------------------------------
//EOF
