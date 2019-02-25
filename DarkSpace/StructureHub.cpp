/*
	StructureHub.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "World/VerbChat.h"
#include "resource.h"
#include "StructureHub.h"
#include "StructureMIHub.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( StructureHub, NounStructure );
REGISTER_FACTORY_KEY( StructureHub, 4358191680267792349LL );

BEGIN_PROPERTY_LIST( StructureHub, NounStructure )
END_PROPERTY_LIST();

StructureHub::StructureHub()
{}

//----------------------------------------------------------------------------

int StructureHub::sortId() const
{
	return 0;
}

int StructureHub::groupId() const
{
	return 0;
}

int StructureHub::maxDamage() const
{
	return 45360;
}

int StructureHub::buildTechnology() const
{
	return 0;
}

int	StructureHub::buildTime() const
{
	return 60;
}

int StructureHub::buildCost() const
{
	return 0;
}

dword StructureHub::buildFlags() const
{
	return 0;
}

int StructureHub::repairRate() const
{
	return 300;
}

Color StructureHub::color() const
{
	return AQUAMARINE;
}

bool StructureHub::canBuild( NounPlanet * pPlanet, bool bUpgrade ) const
{
	if (! validateNoun( pPlanet ) )
		return false;
	for(int i=0;i<pPlanet->childCount();i++)
		if ( WidgetCast<StructureHub>( pPlanet->child(i) ) != NULL || WidgetCast<StructureMIHub>(pPlanet->child(i)) != NULL )
			return false;	// planet already has a colony hub or an MI hub

	return NounStructure::canBuild( pPlanet, bUpgrade );
}

dword StructureHub::planetFlags() const
{
	return NounPlanet::FLAG_HAS_HUB;
}

int StructureHub::workers() const
{
	return 0;		// no workers required, otherwise barren planets can never gain a population
}

int StructureHub::power() const
{
	return 10;
}

int StructureHub::food() const
{
	return 10;
}

int StructureHub::habitat() const
{
	return 10;
}

int StructureHub::mining() const
{
	return 5;
}

int StructureHub::research() const
{
	return 10;
}

int StructureHub::production() const
{
	return 1;
}

int StructureHub::technology() const
{
	return 0;
}

//----------------------------------------------------------------------------
//EOF
