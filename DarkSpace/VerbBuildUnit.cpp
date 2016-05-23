/*
	VerbBuildUnit.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

//----------------------------------------------------------------------------


#include "Debug/Assert.h"
#include "VerbBuildUnit.h"
#include "GameContext.h"

//-------------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbBuildUnit, Verb );

BEGIN_PROPERTY_LIST( VerbBuildUnit, Verb )
	ADD_TRANSMIT_PROPERTY( m_Who );
	ADD_TRANSMIT_PROPERTY( m_Unit );
END_PROPERTY_LIST();

VerbBuildUnit::VerbBuildUnit()
{}

VerbBuildUnit::VerbBuildUnit( StructureBarracks * pBarracks, NounShip * pWho, NounUnit * pUnit ) 
	: m_Who( pWho ), m_Unit( pUnit )
{
	attachVerb( pBarracks );
}

//----------------------------------------------------------------------------

Verb::Priority VerbBuildUnit::priority() const
{
	return MEDIUM;
}

Verb::Scope VerbBuildUnit::scope() const
{
	return LOCAL;
}

bool VerbBuildUnit::client() const
{
	return true;
}

bool VerbBuildUnit::canAttach( Noun * pNoun )
{
	if ( pNoun->isServer() )
		return canBuild( WidgetCast<StructureBarracks>( pNoun ), m_Who, m_Unit );
	return true;
}

void VerbBuildUnit::onExecute()
{
	StructureBarracks * pBarracks = WidgetCast<StructureBarracks>( target() );
	if ( validate( pBarracks ) && m_Unit.valid() )
		pBarracks->setBuild( m_Unit );
}

//----------------------------------------------------------------------------

bool VerbBuildUnit::canBuild( StructureBarracks * pBarracks, NounShip * pWho, NounUnit * pUnit )
{
	if ( pBarracks == NULL || pWho == NULL || pUnit == NULL )
		return false;		// invalid pointers

	NounPlanet * pPlanet = pBarracks->planet();
	if ( pPlanet == NULL )
		return false;		// structure has been scrapped
	if (! pPlanet->canGovern( pWho ) )
		return false;		// player doesn't have rights to build on this planet
	if (! pBarracks->canSetBuild( pUnit ) )
		return false;		// barracks cannot build this unit
	if ( pPlanet->technology() < pUnit->buildTechnology() )
		return false;		// technology level is not high enough

	return true;
}

void VerbBuildUnit::getUnits( StructureBarracks * pBarracks, NounShip * pShip, Array< NounUnit * > & units )
{
	if ( pBarracks != NULL )
		pBarracks->getUnits( units );
}

//----------------------------------------------------------------------------
//EOF
