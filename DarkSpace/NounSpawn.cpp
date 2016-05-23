/*
	NounSpawn.cpp
	(c)2005 Palestar Inc, Richard Lyle
*/


#include "NounSpawn.h"
#include "GameContext.h"
#include "Math/Helpers.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounSpawn, NounGame );
REGISTER_FACTORY_KEY( NounSpawn, 4791986199793190157LL );

BEGIN_PROPERTY_LIST( NounSpawn, NounGame )
	ADD_PROPERTY( m_bActive );
	ADD_PROPERTY( m_fSpawnDelay );
	ADD_PROPERTY( m_fSpawnArea );
	ADD_PROPERTY( m_pScene );
	ADD_PROPERTY( m_bRecur );
	ADD_PROPERTY( m_bRandom );
	ADD_PROPERTY( m_nMaxSpawn );
	ADD_PROPERTY( m_nChance );
	ADD_PROPERTY( m_Spawn );
END_PROPERTY_LIST();

NounSpawn::NounSpawn() : m_bActive( false ), m_fSpawnDelay( 0.0f ), m_fSpawnArea( 0.0f ), m_bRecur( false ), m_bRandom( false ), 
	m_nMaxSpawn( 0 ), m_nChance( 0 ), m_fSpawnTimer( 0.0f )
{}

//----------------------------------------------------------------------------

const int VERSION_20050510 = 20050510;

bool NounSpawn::read( const InStream & input )
{
	if (! Noun::read( input ) )
		return false;
	m_pScene.load();
	return true;
}

//----------------------------------------------------------------------------

NounType NounSpawn::nounType() const
{
	return TYPE_EFFECT;
}

bool NounSpawn::canBeDetected() const
{
	return false;		// object cannot be detected
}

void NounSpawn::simulate( dword nTick )
{
	m_fSpawnTimer += TICK_DURATION_S * (nTick - tick());
	if ( m_fSpawnTimer >= m_fSpawnDelay )
	{
		// reset the count
		m_fSpawnTimer -= m_fSpawnDelay;

		// remove invalid nouns from our spawn array
		for(int i=m_Spawn.size()-1;i>=0;i--)
			if ( m_Spawn[i] == NULL || m_Spawn[i]->zone() == NULL )
				m_Spawn.remove( i );

		// check the type of noun we would be spawning, this determines if we can spawn or not... if not we do nothing
		if ( m_pScene.valid() && m_Spawn.size() < m_nMaxSpawn 
			&& (!m_bRandom  || ((rand() % 100) < m_nChance)) )
		{
			// get the root node, choose a random child node from the root..
			BaseNode * pRoot = m_pScene->node();
			if ( pRoot->childCount() > 0 )
			{
				int nRandomChild = rand() % pRoot->childCount();
				BaseNode * pTemplate = pRoot->child( nRandomChild );
				Widget::Ref pUncasted = pTemplate->copy();

				// spawn the noun
				Noun * pSpawn = WidgetCast<Noun>( pUncasted );
				if ( pSpawn != NULL && canSpawnNounType( pSpawn ) )
				{
					Vector3 offset( RandomFloat(-m_fSpawnArea,m_fSpawnArea), 0.0f, RandomFloat(-m_fSpawnArea,m_fSpawnArea) );

					pSpawn->setName( name() );
					pSpawn->setPosition( worldPosition() + offset );
					pSpawn->setFrame( worldFrame() );
					pSpawn->setup();

					context()->attachNoun( pSpawn );
					m_Spawn.push( pSpawn );
				}
			}
		}

		// detach this node from the world if this will never recur again
		if (! m_bRecur )
			setDetach();
	}

	Noun::simulate( nTick );
}

//----------------------------------------------------------------------------

bool NounSpawn::canSpawnNounType( Noun * pType ) const
{
	switch( pType->nounType() )
	{
	case TYPE_EFFECT:
		return isClient();
	case TYPE_SERVER_OBJECT:
	case TYPE_OBJECT:
	case TYPE_LANDMARK:
		return isServer();
	}

	TRACE( "NounSpawn::canSpawnNounType() - Unknown noun type!" );
	return false;
}

//----------------------------------------------------------------------------
//EOF
