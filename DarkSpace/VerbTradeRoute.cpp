/*
	VerbTradeRoute.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "VerbTradeRoute.h"
#include "StructurePort.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbTradeRoute, Verb );

BEGIN_PROPERTY_LIST( VerbTradeRoute, Verb )
	ADD_TRANSMIT_PROPERTY( m_pDestination );
	ADD_TRANSMIT_PROPERTY( m_pWho );
END_PROPERTY_LIST();

VerbTradeRoute::VerbTradeRoute()
{}

VerbTradeRoute::VerbTradeRoute( NounPlanet * pOrigin, NounPlanet * pDestination, NounShip * pWho ) 
	: m_pDestination( pDestination ), m_pWho( pWho )
{
	attachVerb( pOrigin );
}

//----------------------------------------------------------------------------

Verb::Priority VerbTradeRoute::priority() const
{
	return MEDIUM;
}

Verb::Scope VerbTradeRoute::scope() const
{
	return LOCAL;
}

bool VerbTradeRoute::client() const
{
	return true;
}

bool VerbTradeRoute::canAttach( Noun * pNoun )
{
	if ( pNoun->isServer() )
		return canTradeRoute( WidgetCast<NounPlanet>( pNoun ), m_pDestination, m_pWho );
	return true;
}

void VerbTradeRoute::onExecute()
{
	NounPlanet * pTarget = WidgetCast<NounPlanet>( target() );
	if ( validate( pTarget ) && validate( m_pDestination ) )
	{
		int route = getTradeRoute( pTarget, m_pDestination );
		if ( route < 0 )
		{
			// add trade route to the destination
			pTarget->addTradeRoute( m_pDestination );	
		}
		else
		{
			// trade route already exists, remove the existing route
			pTarget->removeTradeRoute( route );	
		}
	}
}

//----------------------------------------------------------------------------

bool VerbTradeRoute::isTradeRoute( NounPlanet * pOrigin, NounPlanet * pDestination )
{
	return getTradeRoute( pOrigin, pDestination ) >= 0;
}

int VerbTradeRoute::getTradeRoute( NounPlanet * pOrigin, NounPlanet * pDestination )
{
	for(int i=0;i<pOrigin->tradeRoutes();i++)
	{
		if ( pDestination == pOrigin->tradeRoute( i ) )
			return i;
	}

	// no trade route found
	return -1;
}

bool VerbTradeRoute::canTradeRoute( NounPlanet * pOrigin, NounPlanet * pDestination, NounShip * pWho )
{
	if ( pOrigin == NULL || pDestination == NULL )
		return false;	// invalid pointers
	if ( pOrigin == pDestination )
		return false;	// cannot trade with self
	if ( (pOrigin->flags() & NounPlanet::FLAG_HAS_PORT) == 0 )
		return false;	// no port on the origin planet
	if ( (pDestination->flags() & NounPlanet::FLAG_HAS_PORT) == 0 )
		return false;	// no port on the destination planet
	if ( pOrigin->factionId() != pDestination->factionId() )
		return false;	// cannot trade with planets of a different faction
	// check players rights if this is being done by a ship
	if ( pWho != NULL && !pOrigin->canGovern( pWho ) )
		return false;	// player has no rights to govern this planet

	return true;
}

//----------------------------------------------------------------------------
//EOF

