/*
	NounProjectile.cpp

	This effect object travels straight along the specified velocity
	(c)1999 Palestar, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Display/PrimitiveLineList.h"
#include "Display/PrimitiveTriangleStrip.h"
#include "Display/PrimitiveSetTransform.h"
#include "Render3D/NodeLight.h"
#include "Constants.h"
#include "SceneryEffect.h"
#include "NounNebula.h"
#include "NounProjectile.h"
#include "NounShip.h"
#include "NounBeacon.h"
#include "NounFighter.h"
#include "NounJumpGate.h"
#include "NounTrail.h"
#include "World/NounTarget.h"
#include "NounField.h"
#include "GadgetWeapon.h"
#include "GameContext.h"
#include "NounUnit.h"
#include "NounDropTarget.h"

//----------------------------------------------------------------------------

float NounProjectile::sm_fTrailScalar = 1.0f;
bool NounProjectile::sm_bRenderProjectiles = true;

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounProjectile, NounGame );
REGISTER_FACTORY_KEY( NounProjectile, 4045458655991613118LL );

BEGIN_PROPERTY_LIST( NounProjectile, NounGame )
	CLEAR_PROPERTY_FLAGS( m_Traits, PF_TRANSMIT );
	CLEAR_PROPERTY_FLAGS( m_vOrientation, PF_TRANSMIT );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_DetectionFlags );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_nLife );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_vVelocity );
	ADD_TRANSMIT_PROPERTY( m_nTeamId );
	ADD_TRANSMIT_PROPERTY( m_pWeapon );
	ADD_TRANSMIT_PROPERTY( m_pTarget );
END_PROPERTY_LIST();

NounProjectile::NounProjectile() : m_nLife( 0 ), m_fBaseSignature( 0 ), 
	m_nTracerLife( 0 ), m_nTracerRate( 0 ), m_Time( 0 )
{
	// do NOT cast shadows for this type of Noun
	m_nNodeFlags |= NF_NOSHADOW;
}

//----------------------------------------------------------------------------

const int VERSION_20050807 = 20050807;

bool NounProjectile::read( const InStream & input )
{
	if (! Noun::read( input ) )
		return false;

	if ( *(name()) == 0 )
		setName( nounContext()->name() );
	setFrame( Matrix33( m_vVelocity ) );

	return true;
}

//----------------------------------------------------------------------------

void NounProjectile::onAttached()
{
	Noun::onAttached();

	if ( m_pWeapon.valid() )
		m_pWeapon->onProjectileActive( this );
}

void NounProjectile::onDetaching()
{
	Noun::onDetaching();

	if ( m_pWeapon.valid() )
	{
		m_pWeapon->onProjectileDead( this );
		m_pWeapon = NULL;
	}
}

void NounProjectile::render( RenderContext &context, 
						const Matrix33 & frame, 
						const Vector3 & position )
{
	// render our trail
	if ( flags() & FLAG_DO_TRAIL )
		m_Trail.renderProjectileTrail( context, frame, position, position );

	if (! m_pProjectile.valid() )
		m_pProjectile = WidgetCast<Scene>( nounContext()->resource( "PROJECTILE", false ) );

	// render the projectile object
	if ( sm_bRenderProjectiles && m_pProjectile.valid() )
	{
		// disallow lights in projectile scenes..
		//ASSERT( FindNodeClass<NodeLight>( m_pProjectile->node() ) == NULL );

		float previousTime = context.time();
		context.setTime( m_Time );

		m_pProjectile->render( context, frame, position );

		context.setTime( previousTime );
	}
}

//----------------------------------------------------------------------------

NounType NounProjectile::nounType() const
{
	return ((flags() & FLAG_COLLIDABLE) != 0) ? TYPE_LOCAL_OBJECT : TYPE_EFFECT;
}

bool NounProjectile::canCollide() const
{
	return true;
}

bool NounProjectile::collidable() const
{
	return (flags() & FLAG_COLLIDABLE) != 0;
}

bool NounProjectile::canBeDetected() const
{
	return (flags() & FLAG_COLLIDABLE) != 0;
}

bool NounProjectile::enableECM() const
{
	return (flags() & FLAG_COLLIDABLE) != 0;
}

bool NounProjectile::enablePD() const
{
	return (flags() & FLAG_COLLIDABLE) != 0;
}

float NounProjectile::baseSignature() const
{
	return m_fBaseSignature;
}

void NounProjectile::initialize()
{
	Noun::initialize();

	if ( m_pWeapon.valid() )
	{
		m_fBaseSignature = m_pWeapon->projectileSig();

		// initialize the trail
		if ( m_pWeapon->tracerLife() > 0 )
		{
			addFlags( FLAG_DO_TRAIL );

			m_nTracerLife = m_pWeapon->tracerLife();
			m_nTracerRate = m_pWeapon->tracerRate();
			m_Trail.setTrailMaterial( WidgetCast<Material>( nounContext()->resource( "TRACER" ) ) );
			m_Trail.setTrailLife( m_nTracerLife );
			m_Trail.setTrailRate( m_nTracerRate );
			m_Trail.updateTrail( worldPosition() );		// this must be called now, so that the trail appears at the weapon origin..
		}

		m_pOwner = m_pWeapon->parentBody();
	}
}

void NounProjectile::release()
{
	Noun::release();

	if ( m_pWeapon.valid() )
		m_pWeapon->onProjectileDead( this );
	if ( WidgetCast<NounShip>( m_pTarget ) )
		((NounShip *)m_pTarget.pointer())->removeIncoming( this );

	m_pWeapon = NULL;
	m_pOwner = NULL;
	m_pTarget = NULL;
	m_pProjectile.reset();
	m_pIntersect = NULL;
}

void NounProjectile::simulate( dword nTick )
{
	// explode the projectile if we run out of life or lose our weapon
	int nElapsed = nTick - m_Tick;
	if ( m_nLife > 0 )
	{
		if ( nElapsed > 0 )
		{
			if ( nElapsed > m_nLife )
				nElapsed = m_nLife;

			// update the projectile animation time
			m_Time += nElapsed * TICK_DURATION_S;
			// reduce the projectile life
			m_nLife -= nElapsed;

			// move the projectile
			m_Position += m_vVelocity * nElapsed * TICK_DURATION_S;
			// update the trail if we have one
			if ( isClient() && (flags() & FLAG_DO_TRAIL) != 0 )
				m_Trail.updateTrail( worldPosition() );
		}
	}
	else
		explode();

	Noun::simulate( nTick );
}

void NounProjectile::collision( Noun * pCollide, const Vector3 & intersect )
{
	ASSERT( pCollide );

	// don't collide with parents of our weapon
	Noun * pParent = owner();
	if ( pCollide == pParent || pParent == NULL )
		return;			// don't collide with our own parent (usually the ship)
	if ( WidgetCast<NounField>( pCollide ) )
		return;			// don't collide with field objects
	if ( WidgetCast<NounJumpGate>( pCollide ) )
		return;			// let the jumpgate handle projectiles
	if ( WidgetCast<NounProjectile>( pCollide ) )
		return;			// don't allow projectiles to collide with other projectiles!
	if ( WidgetCast<NounNebula>( pCollide ) )
		return;			// don't collide with nebulas
	if ( WidgetCast<NounBeacon>( pCollide ) )
		return;			// don't collide with beacons

	// move projectile to collision point so that the explosion will appear at the correct location..
	setWorldPosition( intersect );
	// call the hit code of the gadget
	if ( m_pWeapon.valid() )
	{
		// call the weapon hit() function, if it returns false we reject the collision as well...
		if (! m_pWeapon->hit( pCollide, this, intersect ) )
			return;		// weapon rejected the hit, ignore the collision
	}

	// explode this projectile if we are still attached to the universe
	explode();
}

void NounProjectile::onWorldTransform( const Vector3 & transform )
{
	Noun::onWorldTransform( transform );

	// update the tracer world positions
	if ( isClient() )
		m_Trail.offsetTrail( transform );
}

//----------------------------------------------------------------------------

bool NounProjectile::initializeProjectile( GadgetWeapon * pWeapon, Noun * pTarget, const Vector3 & velocity, dword tick )
{
	if ( pWeapon == NULL )
		return false;

	setTeamId( pWeapon->teamId() );
	setPosition( pWeapon->worldPosition() );
	setFrame( Matrix33( velocity ) );
	setTick( tick );
	setNounContext( pWeapon->nounContext() );
	setUserId( pWeapon->userId() );

	// if target is infantry, get the hex instead
	NounUnit * pUnit = WidgetCast<NounUnit>( pTarget );
	if ( pUnit != NULL )
	{
		NounPlanet * pPlanet = WidgetCast<NounPlanet>( pUnit->parent() );
		if ( pPlanet != NULL )
			pTarget = new NounDropTarget( pPlanet, pUnit->hex() );
	}
		

	if ( pWeapon->projectileCollidable() )
		addFlags( FLAG_COLLIDABLE );

	m_fBaseSignature = pWeapon->projectileSig();
	m_pWeapon = pWeapon;
	m_pTarget = pTarget;
	m_vVelocity = velocity;
	m_nLife = (pWeapon->projectileLife() * TICKS_PER_SECOND) 
		* pWeapon->calculateModifier( MT_WEAPON_RANGE );

	if ( WidgetCast<NounShip>( pTarget ) )
		((NounShip *)pTarget)->addIncoming( this );

	return true;
}

void NounProjectile::setLife( int a_nLife )
{
	m_nLife = a_nLife;
}

void NounProjectile::setTarget( Noun * pTarget )
{
	m_pTarget = pTarget;

	if ( WidgetCast<NounShip>( pTarget ) )
		((NounShip *)pTarget)->addIncoming( this );
}

void NounProjectile::setOwner( Noun * pOwner )
{
	m_pOwner = pOwner;
}

void NounProjectile::explode()
{
	if ( detach() || zone() == NULL )
		return;		// projectile already detached from the universe, just return then...

	// cause area damage from weapon if any
	if ( m_pWeapon.valid() )
		m_pWeapon->explode( this );

	// attach explosion effect if any to the universe
	if ( isClient() )
	{
		Scene * pHitScene = WidgetCast<Scene>( nounContext()->resource( "HIT", false ) );
		if ( pHitScene != NULL )
		{
			// create the hit effect
			SceneryEffect * pHit = new SceneryEffect;
			pHit->setNounContext( new NounContext( pHitScene ) );
			pHit->setContext( context() );
			pHit->setFrame( worldFrame() );
			pHit->setPosition( worldPosition() );

			if (! pHit->setSegment( "Effect" ) )
				pHit->setLife( 5.0f );

			context()->attachNoun( pHit );
		}

		if ( flags() & FLAG_DO_TRAIL )
		{
			NounTrail * pTrail = new NounTrail( m_Trail );
			pTrail->setPosition( worldPosition() );
			pTrail->setFrame( worldFrame() );

			context()->attachNoun( pTrail );
		}
	}

	// set the life to -1
	m_nLife = -1;
	// projectile is spent, detach from the universe now...
	setDetach();
}

//----------------------------------------------------------------------------

const int MAX_RECURSE_COUNT = 2;

void NounProjectile::intercept( const Vector3 & target,				// target world position
							const Vector3 & velocity,				// target velocity in world space
							Vector3 & i,
							float fOvershoot /*= 0.0f*/ ) const				// the distance	
{
	Vector3 myVelocity( m_vVelocity );
	float fVelocity = myVelocity.magnitude();
	if ( fVelocity <= 0.0 )
		fVelocity = 0.01f;

	// get the position of this gadget
	Vector3 myPosition( worldPosition() );
	// calculate the distance to the target
	float fDistance = (target - myPosition).magnitude();
	fDistance += fOvershoot;
	// calculate the projectile time to the target
	float fT = fDistance / fVelocity;	
	// calculate the beginning intercept point
	i = target + (velocity * fT);// - (myVelocity * fT);
}


//----------------------------------------------------------------------------
// EOF

