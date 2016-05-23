/*
	StructureFactory.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "StructureFactory.h"
#include "StructurePort.h"
#include "CargoGadget.h"
#include "resource.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( StructureFactory, NounStructure );
REGISTER_FACTORY_KEY( StructureFactory, 4231013339585452323LL );

StructureFactory::StructureFactory()
{}

//----------------------------------------------------------------------------

int StructureFactory::sortId() const
{
	return 4;
}

int StructureFactory::groupId() const
{
	return 0;
}

int StructureFactory::maxDamage() const
{
	return 22680;
}

int StructureFactory::buildTechnology() const
{
	return 15;
}

int StructureFactory::buildTime() const
{
	return 50;
}

int StructureFactory::buildCost() const
{
	return 500;
}

int StructureFactory::repairRate() const
{
	return 300;
}

int StructureFactory::workers() const
{
	return 3;
}

int StructureFactory::power() const
{
	return -2;
}

int StructureFactory::production() const
{
	return 1.0f;
}

int StructureFactory::technology() const
{
	return 15;
}

//----------------------------------------------------------------------------
//EOF
