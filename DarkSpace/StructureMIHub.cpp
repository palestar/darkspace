/*
	StructureMIHub.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "World/VerbChat.h"
#include "resource.h"
#include "StructureMIHub.h"
#include "StructureHub.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY(StructureMIHub, NounStructure);

BEGIN_PROPERTY_LIST(StructureMIHub, NounStructure)
END_PROPERTY_LIST();

StructureMIHub::StructureMIHub()
{}

//----------------------------------------------------------------------------

int StructureMIHub::sortId() const
{
	return 0;
}

int StructureMIHub::groupId() const
{
	return 0;
}

int StructureMIHub::maxDamage() const
{
	return 45360;
}

int StructureMIHub::buildTechnology() const
{
	return 0;
}

int	StructureMIHub::buildTime() const
{
	return 60;
}

int StructureMIHub::buildCost() const
{
	return 0;
}

dword StructureMIHub::buildFlags() const
{
	return 0;
}

int StructureMIHub::repairRate() const
{
	return 300;
}

Color StructureMIHub::color() const
{
	return AQUAMARINE;
}

bool StructureMIHub::canBuild(NounPlanet * pPlanet, bool bUpgrade) const
{
	if (!validateNoun(pPlanet))
		return false;
	for (int i = 0; i < pPlanet->childCount(); i++)
		if ( WidgetCast<StructureHub>(pPlanet->child(i)) != NULL || WidgetCast<StructureMIHub>(pPlanet->child(i)) != NULL )
			return false;	// planet already has a colony hub or an MI hub

	return NounStructure::canBuild(pPlanet, bUpgrade);
}

dword StructureMIHub::planetFlags() const
{
	return NounPlanet::FLAG_HAS_HUB;
}

int StructureMIHub::workers() const
{
	return 0;		// no workers required, otherwise barren planets can never gain a population
}

int StructureMIHub::power() const
{
	return 30;
}

int StructureMIHub::food() const
{
	return 30;
}

int StructureMIHub::habitat() const
{
	return 30;
}

int StructureMIHub::mining() const
{
	return 30;
}

int StructureMIHub::research() const
{
	return 30;
}

int StructureMIHub::production() const
{
	return 5;
}

int StructureMIHub::technology() const
{
	return 0;
}

//----------------------------------------------------------------------------
//EOF
