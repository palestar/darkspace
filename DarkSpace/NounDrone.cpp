/*
	NounDrone.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Standard/Queue.h"
#include "NounShip.h"
#include "NounDrone.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounDrone, NounGame );
REGISTER_FACTORY_KEY( NounDrone, 4252534406730040832LL );

BEGIN_PROPERTY_LIST( NounDrone, NounGame )
	ADD_TRANSMIT_PROPERTY( m_State );
	ADD_TRANSMIT_PROPERTY( m_Speed );
	ADD_TRANSMIT_PROPERTY( m_Target );
	ADD_TRANSMIT_PROPERTY( m_TargetOffset );
	ADD_TRANSMIT_PROPERTY( m_Duration );
	ADD_TRANSMIT_PROPERTY( m_Home );
	ADD_TRANSMIT_PROPERTY( m_HomeOffset );
	ADD_TRANSMIT_PROPERTY( m_Velocity );
END_PROPERTY_LIST();

NounDrone::NounDrone()
{
	m_State = GO_TARGET;
	m_Speed = 1.0f;
	m_TargetOffset = Vector3( 0.0f );
	m_Duration = TICKS_PER_SECOND * 2;
	m_HomeOffset = Vector3( 0.0f );
	m_Velocity = 0.0f;
}

//----------------------------------------------------------------------------

void NounDrone::render( RenderContext &context, const Matrix33 & frame, const Vector3 & position )
{
	// render the drone object
	if ( m_Drone.valid() )
		m_Drone->render( context, frame, position );
	else
		loadDrone();

	// render our trail
	m_Trail.renderProjectileTrail( context, frame, position, worldPosition() );
}

//----------------------------------------------------------------------------

NounType NounDrone::nounType() const
{
	return TYPE_EFFECT;
}

void NounDrone::initialize()
{
	Noun::initialize();
	loadDrone();
}

void NounDrone::release()
{
	Noun::release();
	m_Target = NULL;
	m_Home = NULL;
	m_Drone.reset();
}

const float MAX_RANGE = 1000.0f;			// maximum range
const int	DRONE_UPDATE_RATE = TICKS_PER_SECOND;

void NounDrone::simulate( dword nTick )
{
	// update our trail
	m_Trail.updateTrail( worldPosition() );

	Action * pAction = dynamic_cast<Action *>( home() );
	if ( pAction != NULL )
		pAction->droneUpdate( this );

	switch( m_State )
	{
	case GO_HOME:
		if ( home() != NULL )
		{
			Vector3 destination( home()->worldPosition() + m_HomeOffset );
			float distance = move( destination );
			if ( distance < 1.0f || distance > MAX_RANGE)
				m_State = AT_HOME;
		}
		else
			m_State = AT_HOME;
		break;
	case GO_TARGET:
		if ( target() != NULL )
		{
			Vector3 destination( target()->worldPosition() + m_TargetOffset );
		
			float distance = move( destination );
			if ( distance < 1.0f )
				m_State = AT_TARGET;
			else if ( distance > MAX_RANGE )
				m_State = GO_HOME;
		}
		else
			m_State = GO_HOME;
		break;
	case AT_TARGET:
		{
			m_Duration--;
			if ( m_Duration <= 0 )
			{
				m_State = GO_HOME;

				//TRACE( "NounDrone::AT_TARGET" );
				Action * pAction = dynamic_cast<Action *>( home() );
				if ( pAction != NULL )
					pAction->droneAction( this );
			}
		}
		break;
	case AT_HOME:
		{
			//TRACE( "NounDrone::AT_HOME" );

			Action * pAction = dynamic_cast<Action *>( home() );
			if ( pAction != NULL )
				pAction->droneHome( this );

			// home, detach from the world our job is done
			setDetach();
		}
		break;
	}

	Noun::simulate( nTick );
}

void NounDrone::onWorldTransform( const Vector3 & transform )
{
	Noun::onWorldTransform( transform );
	// update the trail
	m_Trail.offsetTrail( transform );
}


//----------------------------------------------------------------------------

float NounDrone::move( const Vector3 & destination )
{
	Vector3 delta( destination - worldPosition() );
	float distance = delta.magnitude();

	// keep delta under our maximum speed
	if ( distance > m_Speed )
		delta *= m_Speed / distance;
	// move drone
	setPosition( position() + (delta * TICK_DURATION_S) );
	// set the frame
	setFrame( Matrix33( delta ) );

	return distance;
}

//----------------------------------------------------------------------------

void NounDrone::loadDrone()
{
	NounGadget * pHome = WidgetCast<NounGadget>( m_Home );
	if ( pHome != NULL )
	{
		m_Trail.setTrailMaterial( pHome->tracerMaterial() );
		m_Drone = pHome->projEffect();
	}
}

//----------------------------------------------------------------------------
//EOF

