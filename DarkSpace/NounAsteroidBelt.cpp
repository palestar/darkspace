/*
	NounAsteroidBelt.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Math/Helpers.h"
#include "NounAsteroid.h"
#include "NounAsteroidBelt.h"
#include "GameContext.h"
#include "TraitOrbit.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounAsteroidBelt, NounBody );
REGISTER_FACTORY_KEY( NounAsteroidBelt, 4153201853889885108LL );

BEGIN_PROPERTY_LIST( NounAsteroidBelt, NounBody )
	ADD_PROPERTY( m_Asteroids );
END_PROPERTY_LIST();

NounAsteroidBelt::NounAsteroidBelt() : m_nSpawnTick( 0 )
{}

//----------------------------------------------------------------------------

NounType NounAsteroidBelt::nounType() const
{
	return TYPE_SERVER_OBJECT;
}

bool NounAsteroidBelt::collidable() const
{
	return false;
}

bool NounAsteroidBelt::canBeDetected() const
{
	return false;
}

void NounAsteroidBelt::release()
{
	NounBody::release();

	// DO NOT release the asteroids, otherwise they will accumulate in the MV when the universe gets reloaded...
	//m_Asteroids.release();
}

const int		ASTEROID_COUNT = 25;
const int		SPAWN_ASTERIOID_RATE = 4;			// how many to spawn in one update..
const float		ASTEROID_ORBIT_VARIATION = 30.0f;
const float		ASTEROID_FIELD_SIZE = 400.0f;

void NounAsteroidBelt::simulate( dword nTick )
{
	if ( isServer() && nTick >= m_nSpawnTick )
	{
		m_nSpawnTick = nTick + (TICKS_PER_SECOND * 30);

		// remove destroyed asteroids
		for(int i=m_Asteroids.size()-1;i>=0;i--)
		{
			NounAsteroid * pAsteroid = m_Asteroids[i];
			if ( pAsteroid == NULL || pAsteroid->parent() == NULL )
				m_Asteroids.remove( i );
		}

		TraitOrbit * pMyOrbit = FindTrait<TraitOrbit>( this );

		// generate new asteroids
		int nSpawned = 0;
		while( m_Asteroids.size() < ASTEROID_COUNT && nSpawned < SPAWN_ASTERIOID_RATE )
		{
			NounAsteroid::Ref pAsteroid = new NounAsteroid;
			pAsteroid->setName( name() );
			pAsteroid->setNounContext( nounContext() );
			pAsteroid->setFrame( Matrix33( RandomVector( -1, 1 ).normalize() ) );

			// attach, then set position because zone is needed to set the orbit..
			context()->attachNoun( pAsteroid );

			bool bOrbitSet = false;
			if ( pMyOrbit != NULL && pMyOrbit->orbiting() )
			{
				TraitOrbit * pOrbit = FindTrait<TraitOrbit>( pAsteroid );
				if ( pOrbit != NULL )
				{
					pOrbit->setOrbit( pMyOrbit->orbiting(), 
						pMyOrbit->orbitDistance() + RandomFloat( -ASTEROID_ORBIT_VARIATION, ASTEROID_ORBIT_VARIATION), 
						RandomFloat( -PI, PI ), 
						pMyOrbit->orbitVelocity(),
						nTick );
					bOrbitSet = true;
				}
			}

			if (! bOrbitSet )
			{
				pAsteroid->setPosition( worldPosition() + 
					Vector3( RandomFloat( 0, ASTEROID_FIELD_SIZE ), 0, 0 ).rotateXZ( RandomFloat( -PI, PI ) ) );
			}
			// prevent a collision with star at 0,0,0
			pAsteroid->setLastPosition( pAsteroid->position() );
			// save asteroid
			m_Asteroids.push( pAsteroid.pointer() );
			++nSpawned;
		}
	}

	NounBody::simulate( nTick );
}

//----------------------------------------------------------------------------
//EOF
