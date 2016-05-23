/*
	VerbLockPlanet.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "VerbLockPlanet.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbLockPlanet, Verb );

BEGIN_PROPERTY_LIST( VerbLockPlanet, Verb )
	ADD_TRANSMIT_PROPERTY( m_pWho );
	ADD_TRANSMIT_PROPERTY( m_bLock );
END_PROPERTY_LIST();

VerbLockPlanet::VerbLockPlanet()
{}

VerbLockPlanet::VerbLockPlanet( NounPlanet * pPlanet, NounShip * pWho, bool bLock) 
	: m_pWho( pWho ), m_bLock( bLock )
{
	attachVerb( pPlanet );
}

//----------------------------------------------------------------------------

Verb::Priority VerbLockPlanet::priority() const
{
	return MEDIUM;
}

Verb::Scope VerbLockPlanet::scope() const
{
	return LOCAL;
}

bool VerbLockPlanet::client() const
{
	return true;
}

bool VerbLockPlanet::canAttach( Noun * pNoun )
{
	if ( pNoun->isServer() )
	{
		return m_bLock ? canLock( WidgetCast<NounPlanet>( pNoun ), m_pWho ) :
			canUnlock( WidgetCast<NounPlanet>( pNoun ), m_pWho );
	}

	return true;
}

void VerbLockPlanet::onExecute()
{
	NounPlanet * pPlanet = WidgetCast<NounPlanet>( target() );
	if ( validate( pPlanet ) && validate( m_pWho ) )
	{
		if ( m_bLock )
		{
			pPlanet->setLockRank( m_pWho->rank() );
		}
		else
			pPlanet->setLockRank( 0 );
	}
}

//----------------------------------------------------------------------------

bool VerbLockPlanet::canLock( NounPlanet * pPlanet, NounShip * pWho )
{
	if ( pPlanet == NULL || pWho == NULL )
		return false;		// invalid pointers
	if ( pPlanet->lockRank() >= pWho->rank() )
		return false;		// planet is already locked at or above our rank
	return pPlanet->factionId() == pWho->factionId();
}

bool VerbLockPlanet::canUnlock( NounPlanet * pPlanet, NounShip * pWho )
{
	if ( pPlanet == NULL || pWho == NULL )
		return false;
	if ( pPlanet->lockRank() == 0 )
		return false;		// planet is already unlocked
	if ( pWho->rank() < pPlanet->lockRank() )
		return false;		// not enough rank to unlock the planet
	return pPlanet->factionId() == pWho->factionId();
}

//----------------------------------------------------------------------------
//EOF
