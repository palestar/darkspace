/**
	@file GameZone.cpp

	(c)2006 Palestar Inc
	@author Richard Lyle @date 10/18/2006 8:30:41 PM
*/

#include "GameZone.h"
#include "GameContext.h"
#include "NounPlanet.h"
#include "NounJumpGate.h"

//---------------------------------------------------------------------------------------------------

IMPLEMENT_FACTORY( GameZone, NodeZone );

GameZone::GameZone() : m_nPlanetCount( 0 ), m_nShipCount( 0 )
{}

//---------------------------------------------------------------------------------------------------

int GameZone::factionPlanets( int factionId ) const
{
	int planets = 0;
	for(int i=0;i<childCount();i++)
	{
		NounPlanet * pPlanet = WidgetCast<NounPlanet>( child(i) );
		if ( pPlanet != NULL && pPlanet->maxUnits() > 0 && pPlanet->factionId() == factionId )
			planets++;
	}

	return planets;
}

int GameZone::factionHome() const
{
	int factionId = 0;
	for(int i=0;i<childCount();i++)
		if ( WidgetCast<NounJumpGate>( child(i) ) )
		{
			NounJumpGate * pGate = (NounJumpGate *)child(i);

			// don't consider neutral jumpgates at all
			if ( pGate->factionId() > 0 )
			{
				if ( factionId > 0 && factionId != pGate->factionId() )
					return 0;		// multiple factions have jumpgates
				
				factionId = pGate->factionId();
			}
		}

	return factionId;
}

//---------------------------------------------------------------------------------------------------

bool GameZone::hookNouns( BaseNode::Ref pNode )
{
	if (! NodeZone::hookNouns( pNode ) )
		return false;

	if ( WidgetCast<NounPlanet>( pNode ) )
		++m_nPlanetCount;
	else if ( WidgetCast<NounShip>( pNode ) )
		++m_nShipCount;

	return true;
}

void GameZone::unhookNouns( BaseNode::Ref pNode )
{
	NodeZone::unhookNouns( pNode );

	if ( WidgetCast<NounPlanet>( pNode ) )
		--m_nPlanetCount;
	else if ( WidgetCast<NounShip>( pNode ) )
		--m_nShipCount;
}

//---------------------------------------------------------------------------------------------------
//EOF
