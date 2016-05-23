/*
	NounAsteroid.h
	(c)1999 PaleStar, Richard Lyle
*/


#include "Debug/Trace.h"
#include "Math/Helpers.h"
#include "NounAsteroid.h"
#include "NounStar.h"
#include "NounPlanet.h"
#include "NounProjectile.h"
#include "SceneryDebris.h"
#include "VerbBreakOrbit.h"
#include "TraitOrbit.h"
#include "GameContext.h"
#include "World/TraitMovement.h"

//----------------------------------------------------------------------------

static Constant	ASTEROID_FORCE_DAMAGE( "ASTEROID_FORCE_DAMAGE", 1000.0f );
static Constant ASTEROID_MAX_DAMAGE( "ASTEROID_MAX_DAMAGE", 150000.0f );
static Constant	ASTEROID_DAMAGE_PER_MASS( "ASTEROID_DAMAGE_PER_MASS", 7500.0f );
static Constant	ASTEROID_DAMAGE_FORCE( "ASTEROID_DAMAGE_FORCE", 0.0025f );
static Constant	ASTEROID_BOUNCE( "ASTEROID_BOUNCE", 0.05f );		
static Constant ASTEROID_ZERO_VELOCITY( "ASTEROID_ZERO_VELOCITY", 0.05f );
static Constant ASTEROID_MAX_VELOCITY( "ASTEROID_MAX_VELOCITY", 25.0f );

const int		DESTROY_FRAGMENTS = 5;
const float		FRAGMENT_SCALE = 0.25f; 
const float		DAMAGE_VELOCITY = 0.05f;

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounAsteroid, NounBody );
REGISTER_FACTORY_KEY( NounAsteroid, 4145275974904902467LL );

BEGIN_PROPERTY_LIST( NounAsteroid, NounBody )
	ADD_TRANSMIT_UPDATE_PROPERTY( m_DetectionFlags );
	ADD_REPLICATED_PROPERTY( m_Damage, TICKS_PER_SECOND * 5 );
END_PROPERTY_LIST();

NounAsteroid::NounAsteroid() : m_Velocity( 0,0,0 ), m_Damage( 0 ), m_bZeroVelocity( true )
{
	attachTrait( new TraitMovement() );
}

//----------------------------------------------------------------------------

NounType NounAsteroid::nounType() const
{
	return TYPE_LOCAL_OBJECT;
}

void NounAsteroid::simulate( dword nTick )
{
	// move the asteroid
	if ( isLocal() && !m_bZeroVelocity )
	{
		// if this object is in orbit, break the orbit
		TraitOrbit * pOrbit = FindTrait<TraitOrbit>( this );
		if ( pOrbit != NULL && pOrbit->orbiting() != NULL )
			Verb::Ref( new VerbBreakOrbit( this ) );

		// update our position
		float t = TICK_DURATION_S * (nTick - tick());
		setPosition( position() + (m_Velocity * t ) );
		// set the base velocity
		setVelocity( m_Velocity );
	}
	NounBody::simulate( nTick );
}

bool NounAsteroid::canCollide() const
{
	return true;
}

bool NounAsteroid::collidable() const
{
	return true;
}

void NounAsteroid::collision( Noun * pCollide, const Vector3 & intersect )
{
	// certain objects always destroy this asteroid, i.e. planets and stars
	bool bDestroy = WidgetCast<NounPlanet>( pCollide ) || WidgetCast<NounStar>( pCollide );
	// exclude all objects execept the ones we know will collide with this object
	bool bCollision = bDestroy || WidgetCast<NounShip>( pCollide ) || WidgetCast<NounAsteroid>( pCollide );

	if ( bCollision )
	{
		// calculate the collision position in object space of this asteroid
		Vector3 vDirection( worldPosition() - pCollide->worldPosition() );

		float fDistance = vDirection.magnitude();
		float fInner = radius() + pCollide->radius();

		if ( fDistance < fInner )
		{
			vDirection.normalize();

			// move asteroid outside the combined radius of both objects
			float fInside = fInner - fDistance;
			m_Position += vDirection * fInside;

			// apply velocity in direction of impact with some bounce
			float fVelocity = pCollide->worldVelocity().magnitude();
			fVelocity += ASTEROID_BOUNCE * fVelocity;
			
			if ( fVelocity > ASTEROID_MAX_VELOCITY )
				fVelocity = ASTEROID_MAX_VELOCITY;

			m_Velocity = vDirection * fVelocity;
			m_bZeroVelocity = m_Velocity.magnitude() < ASTEROID_ZERO_VELOCITY;
		}

		// get the direction of the collision
		Vector3 direction( pCollide->worldPosition() - intersect );
		// force is caluclated from velocity and the mass of this asteroid
		float fForce = direction.magnitude() * mass();
		// calulate the damage from the total force of the collision
		int damage = Min<int>( fForce * ASTEROID_FORCE_DAMAGE, ASTEROID_MAX_DAMAGE );
		// inflict damage on this asteroid
		if ( bDestroy || damage > 0 )
			inflictDamage( tick(), pCollide, bDestroy ? maxDamage() : damage, DAMAGE_KINETIC, worldFrame() * -direction );

		// inflict damage on the object we hit
		if ( damage > 0 && ((NounGame *)pCollide)->canDamage( DAMAGE_KINETIC|DAMAGE_BIO|DAMAGE_PLANET ) )
			((NounGame *)pCollide)->inflictDamage( tick(), this, damage, DAMAGE_KINETIC|DAMAGE_BIO|DAMAGE_PLANET, pCollide->worldFrame() * direction );
	}
}
 
bool NounAsteroid::canDamage( dword type ) const
{
	return (type & (DAMAGE_KINETIC|DAMAGE_ENERGY)) != 0;
}

void NounAsteroid::inflictDamage( dword nWhen, Noun * pFrom, int damage, dword type, const Vector3 & direction )
{
	if ( type & (DAMAGE_KINETIC|DAMAGE_ENERGY) )
	{
		// transform direction to world space
		Vector3 vForceDirection( worldFrame() % direction ); 
		// translate the amount of damage into a force and divide by the mass, more massive asteroids don't move quite as quickly
		float fForce = (ASTEROID_DAMAGE_FORCE * damage) / mass();
		// affect the velocity of this asteroid
		m_Velocity -= vForceDirection * fForce; 
		m_bZeroVelocity = m_Velocity.magnitude() < ASTEROID_ZERO_VELOCITY;

		// apply the damage to the asteroid
		m_Damage += damage;	

		//TRACE( String().format("NounAsteroid::inflictDamage, damage = %d, velocity = %.1f", damage, velocity ) );

		if ( m_Damage > maxDamage() )
		{
			TRACE("NounAsteroid::inflictDamage, destroyed!" );

			if ( context()->isClient() )
			{
				// create asteroid fragments
				for(int i=0;i<DESTROY_FRAGMENTS;i++)
				{
					Vector3 direction( RandomVector( -1, 1 ) );
					direction.normalize();

					SceneryDebris * pFragment = new SceneryDebris;
					pFragment->setNounContext( nounContext() );
					pFragment->setPosition( worldPosition() );
					pFragment->setFrame( Matrix33( direction ) * FRAGMENT_SCALE );
					pFragment->setVelocity( direction * RandomFloat( -3.0f, 3.0f ) );
					pFragment->setLife( RandomFloat( 30.0f, 60.0f) );

					context()->attachNoun( pFragment );
				}
			}

			// remove this asteroid from the world
			setDetach();
		}
	}
}

bool NounAsteroid::canShipCollide() const
{
	return true;
}

//----------------------------------------------------------------------------

int NounAsteroid::maxDamage() const
{
	return mass() * ASTEROID_DAMAGE_PER_MASS;
}

//----------------------------------------------------------------------------

void NounAsteroid::setVelocity( const Vector3 & velocity )
{
	m_Velocity = velocity;
	m_bZeroVelocity = m_Velocity.magnitude() < ASTEROID_ZERO_VELOCITY;
}

//----------------------------------------------------------------------------
// EOF
