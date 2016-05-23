/*
	NounPod.cpp
	This noun is used to transport cargo from one noun to another

	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "NounPod.h"
#include "NounUnit.h"
#include "SceneryEffect.h"
#include "GameContext.h"
#include "NounDropTarget.h"

//----------------------------------------------------------------------------

const char *	POD_NC = "misc/cargo_pod/nc_pod.prt";

NounContext::Link NounPod::sm_pPodContext;

Constant POD_MAX_VELOCITY( "POD_MAX_VELOCITY", 10.0f );			// in gu/s
Constant POD_MAX_DAMAGE( "POD_MAX_DAMAGE", 500 );				// maxmimum amount of damage pod can take
Constant POD_LIFE( "POD_LIFE", TICKS_PER_SECOND * 120 );		// pod lifespan
Constant POD_TRAIL_LIFE( "POD_TRAIL_LIFE", TICKS_PER_SECOND * 2 );

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounPod, NounGame );
REGISTER_FACTORY_KEY( NounPod, 4529908377707539595LL );

BEGIN_PROPERTY_LIST( NounPod, NounGame )
	ADD_TRANSMIT_PROPERTY( m_nTeamId );
	ADD_TRANSMIT_PROPERTY( m_Life );
	ADD_TRANSMIT_PROPERTY( m_Damage );
	ADD_TRANSMIT_PROPERTY( m_Cargo );
	ADD_TRANSMIT_PROPERTY( m_Target );
END_PROPERTY_LIST();

NounPod::NounPod() : m_Life( POD_LIFE ), m_Damage( 0 )
{
	if ( sm_pPodContext.valid() || loadResource( POD_NC, sm_pPodContext ) )
		setNounContext( sm_pPodContext );
}

//----------------------------------------------------------------------------

void NounPod::render( RenderContext &context, const Matrix33 & frame, const Vector3 & position )
{
	// render the drone object
	Noun::render( context, frame, position );
	// render our trail
	m_Trail.renderProjectileTrail( context, frame, position, worldPosition() );
}

//----------------------------------------------------------------------------

NounType NounPod::nounType() const
{
	return TYPE_LOCAL_OBJECT;
}

bool NounPod::canCollide() const
{
	return true;
}

bool NounPod::collidable() const
{
	return true;
}

bool NounPod::canDamage( dword type ) const
{
	return (type & (DAMAGE_ENERGY|DAMAGE_KINETIC)) != 0;
}

bool NounPod::canBeDetected() const
{
	return true;
}

bool NounPod::enableECM() const
{
	return true;
}

bool NounPod::enablePD() const
{
	return true;
}

float NounPod::baseSignature() const
{
	return 1.0f;
}
	
//----------------------------------------------------------------------------

void NounPod::initialize()
{
	Noun::initialize();

	// initialize the trail
	Material * pTrail = WidgetCast<Material>( nounContext()->resource( "M_TRAIL" ) );
	if ( pTrail != NULL )
		m_Trail.setTrailMaterial( pTrail );
	m_Trail.setTrailLife( POD_TRAIL_LIFE );

	if ( WidgetCast<NounShip>( m_Target.pointer() ) )
		((NounShip *)m_Target.pointer())->onPodAttached( this );
}

void NounPod::release()
{
	Noun::release();

	if ( WidgetCast<NounShip>( m_Target.pointer() ) )
		((NounShip *)m_Target.pointer())->onPodDetached( this );

	m_Target = NULL;
	m_Cargo.release();
}

void NounPod::simulate( dword nTick )
{
	// validate the target
	if ( validateTarget( m_Target ) && m_Life > 0 )
	{
		m_Life--;

		Vector3 delta( m_Target->worldPosition() - worldPosition() );
		float distance = delta.magnitude() - m_Target->radius();
		delta.normalize();

		// move this pod towards the target
		setPosition( position() + ((delta * POD_MAX_VELOCITY) * TICK_DURATION_S) );
		// set the frame
		setFrame( Matrix33( delta ) );
		// update our trail
		m_Trail.updateTrail( worldPosition() );

		if ( distance < 1.0f )
		{
			if ( isServer() )
			{
				// unattach the pod from the ship before getting our cargo slot
				if ( WidgetCast<NounShip>( m_Target.pointer() ) )
					((NounShip *)m_Target.pointer())->onPodDetached( this );

				Widget::Ref pUncasted = m_Cargo.unwrap();
				if ( pUncasted.valid() )
				{
					Noun::Ref pCargo = WidgetCast<Noun>( pUncasted.pointer() );
					if ( pCargo.valid() )
					{
						// attach the cargo to the target
						Noun * pHold = NounGame::findCargoHold( m_Target, pCargo );
						if ( pHold != NULL )
						{
							Vector3 vLocalPosition( pHold->worldFrame() * (m_Target->worldPosition() - pHold->worldPosition()) );
							
							// invalidate the hex if NounUnit...
							if ( WidgetCast<NounUnit>( pCargo ) )
								((NounUnit *)pCargo.pointer())->setHex( 0xffff );
							// set the position of the cargo in object space
							pCargo->setPosition( vLocalPosition );
							pCargo->setTick( tick() );
							// attach the child to the parent
							pHold->attachNode( pCargo );
							m_Target->context()->user()->saveStorage(m_Target);

							if ( validateNoun( m_From ) )
							{
								m_From->privateChat( CharString().format( "<color;ffffff>Comms: %s successfully unloaded onto %s.", 
									pCargo->name(), m_Target->name() ) );
							}
						}
						else
						{
							// no space available on target now, jettison the cargo into space
							pCargo->setPosition( worldPosition() );
							pCargo->setTick( tick() );
							context()->attachNoun( pCargo );
						}
					}
				}

				// clear the target, so onDetachPod() doesn't get called twice!
				m_Target = NULL;
			}

			// detach from the universe now
			setDetach();
		}
	}
	else
	{
		if ( isServer() )
		{
			Widget::Ref pUncasted = m_Cargo.unwrap();
			if ( pUncasted.valid() )
			{
				Noun::Ref pCargo = WidgetCast<Noun>( pUncasted.pointer() );
				if ( pCargo.valid() )
				{
					// dump cargo into space, out of time or invalid target
					pCargo->setPosition( worldPosition() );
					pCargo->setTick( tick() );

					context()->attachNoun( pCargo );
				}
			}
		}

		// detach the pod from the universe
		setDetach();
	}

	Noun::simulate( nTick );
}

void NounPod::collision( Noun * pCollide, const Vector3 & intersect )
{
	Noun * pTarget = m_Target;
	if ( WidgetCast<NounDropTarget>( pTarget ) )
		pTarget = ((NounDropTarget *)pTarget)->planet();

	if ( WidgetCast<NounPlanet>( pCollide ) && pCollide != pTarget )
	{
		Vector3 vDelta = m_Position - pCollide->position();
		float fDistance = vDelta.magnitude();
		float R2 = radius() + pCollide->radius();

		if ( fDistance < R2 )
		{
			float fInside = R2 - fDistance;

			vDelta *= 1.0f / fDistance;		// normalize the vector
			m_Position += vDelta * fInside;	// push the object outside the collision..
		}
	}
}

void NounPod::onWorldTransform( const Vector3 & transform )
{
	Noun::onWorldTransform( transform );
	// update the trail
	m_Trail.offsetTrail( transform );
}

void NounPod::inflictDamage( dword nWhen, Noun * pFrom, int damage, dword type, const Vector3 & direction )
{
	m_Damage += damage;
	if ( m_Damage >= POD_MAX_DAMAGE )
		explode();
}

//----------------------------------------------------------------------------

int NounPod::life() const
{
	return m_Life;
}

int NounPod::damage() const
{
	return m_Damage;
}

Noun * NounPod::target() const
{
	return m_Target;
}

//----------------------------------------------------------------------------

void NounPod::setLife( int life )
{
	m_Life = life;
}

void NounPod::setDamage( int damage )
{
	m_Damage = damage;
}

void NounPod::setCargo( Noun * pCargo )
{
	m_Cargo = pCargo;
}

void NounPod::setTarget( Noun * pTarget )
{
	m_Target = pTarget;
}

void NounPod::setFrom( NounShip * pFrom )
{
	m_From = pFrom;
}

//----------------------------------------------------------------------------

void NounPod::explode()
{
	ASSERT( context() != NULL );

	// create explosion client side only
	if ( context()->isClient() )
	{
		Scene * pExplosion = WidgetCast<Scene>( nounContext()->resource( "EXPLOSION" ) );
		if ( pExplosion != NULL )
		{
			// create the hit effect
			SceneryEffect * pHit = new SceneryEffect;
			pHit->setNounContext( new NounContext( pExplosion ) );
			pHit->setPosition( worldPosition() );

			if (! pHit->setSegment( "Effect" ) )
				pHit->setLife( 5.0f );

			context()->attachNoun( pHit );
		}
	}

	setDetach();
}

//----------------------------------------------------------------------------
//EOF
