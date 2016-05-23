/*
	VerbTransferResource.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "VerbTransferResource.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbTransferResource, Verb );

BEGIN_PROPERTY_LIST( VerbTransferResource, Verb )
	ADD_TRANSMIT_PROPERTY( m_pOrigin );
	ADD_TRANSMIT_PROPERTY( m_pDestination );
	ADD_TRANSMIT_PROPERTY( m_Quantity );
END_PROPERTY_LIST();

VerbTransferResource::VerbTransferResource() : m_Quantity( 0 )
{}

VerbTransferResource::VerbTransferResource( NounPlanet * pFrom, NounPlanet * pDestination, int quantity ) 
	: m_pOrigin( pFrom ), m_pDestination( pDestination ), m_Quantity( quantity )
{
	attachVerb( pFrom );
}

//----------------------------------------------------------------------------

Verb::Priority VerbTransferResource::priority() const
{
	return MEDIUM;
}

Verb::Scope VerbTransferResource::scope() const
{
	return LOCAL;
}

bool VerbTransferResource::client() const
{
	return false;
}

bool VerbTransferResource::canAttach( Noun * pNoun )
{
	if ( pNoun->isServer() )
	{
		if ( pNoun == m_pOrigin )
			return canTransferResource( WidgetCast<NounPlanet>( pNoun ), m_pDestination, m_Quantity );
		else if ( pNoun == m_pDestination )
			return WidgetCast<NounPlanet>( pNoun ) != NULL;

		return false;
	}

	return true;
}

void VerbTransferResource::onExecute()
{
	NounPlanet * pTarget = WidgetCast<NounPlanet>( target() );
	if ( validate( pTarget ) && validate( m_pOrigin ) && validate( m_pDestination ) )
	{
		if ( pTarget == m_pOrigin )
		{
			// take the resources from the planet
			pTarget->useResources( m_Quantity );
			
			// now attach this verb to the destination, this will resend this verb to all local clients in the destination area
			if ( context()->isServer() )
				attachVerb( m_pDestination );
		}
		else if ( pTarget == m_pDestination )
		{
			// add the resources to the planet
			pTarget->addResources( m_Quantity );
		}
	}
}

//----------------------------------------------------------------------------

bool VerbTransferResource::canTransferResource( NounPlanet * pOrigin, NounPlanet * pDestination, int quantity )
{
	if ( quantity <= 0 )
		return false;		// quanity must be positive!
	if ( pOrigin == NULL || pDestination == NULL )
		return false;
	if ( pOrigin == pDestination )
		return false;		// cannot transfer to self
	if ( pOrigin->factionId() != pDestination->factionId() )
		return false;		// planets not in the same faction
	if ( (pOrigin->flags() & NounPlanet::FLAG_BLOCKADE) != 0 )
		return false;		// origin planet is under blockade
	if ( (pDestination->flags() & NounPlanet::FLAG_BLOCKADE) != 0 )
		return false;		// destination planet is under blockade
	if ( pOrigin->resources() < quantity )
		return false;		// not enough resources on the origin planet

	return true;
}

//----------------------------------------------------------------------------
//EOF
