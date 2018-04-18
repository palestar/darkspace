#ifndef INCREASE_MASS_H 
#define INCREASE_MASS_H

#include "DarkSpace/GadgetModifierEW.h"

//----------------------------------------------------------------------------

class QuantumHiggsNet : public GadgetModifierEW 
{
public:
	DECLARE_WIDGET_CLASS();
	QuantumHiggsNet()
	{
		m_Modifiers.push(MT_MASS);
	}
	Type type() const
	{
		return SPECIAL_OFFENSIVE;
	}
	int	addValue() const
	{
		return 1200;
	}
	int buildTechnology() const
	{
		return 10;
	}
	int	buildCost() const
	{
		return 300;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 140;
	}
	int	energyCost() const
	{
		return 10;
	}
	float strength() const
	{
		return -100.0f;
	}
	float range() const
	{
		return 300.0f;
	}
	int cooldown() const
	{
		return TICKS_PER_SECOND * 30;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
