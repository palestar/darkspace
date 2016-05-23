/*
	NounJumpGate.cpp
	(c)1999 PaleStar, Richard Lyle
*/


#include "Debug/Assert.h"
#include "NounShip.h"
#include "NounJumpGate.h"
#include "NounProjectile.h"
#include "NounStar.h"
#include "SceneryEffect.h"
#include "VerbUseJumpGate.h"
#include "GameContext.h"
#include "resource.h"

//----------------------------------------------------------------------------

Constant SAFE_ZONE_SIZE( "SAFE_ZONE_SIZE", 1000.0f );

const dword CHECK_SAFE_ZONE = TICKS_PER_SECOND * 1;

//! true when building assets, prevents this jumpgate from clearing it's destionation when it can't find 
//! the target object..
bool NounJumpGate::sm_bBuildMode = false;

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounJumpGate, NounBody );
REGISTER_FACTORY_KEY( NounJumpGate, 4151364117427545990LL );

BEGIN_PROPERTY_LIST( NounJumpGate, NounBody )
	ADD_TRANSMIT_PROPERTY( m_Name );
	ADD_TRANSMIT_PROPERTY( m_nTeamId );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_bActive );
	ADD_TRANSMIT_PROPERTY( m_bAutoActivate );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_bRemoteServer );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_sAddress );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_nPort );
	ADD_TRANSMIT_PROPERTY( m_pDestination );
	ADD_TRANSMIT_PROPERTY( m_sDestination );
	ADD_TRANSMIT_PROPERTY( m_bSafeZone );
	ADD_TRANSMIT_PROPERTY( m_fSafeZoneDistance );
	ADD_TRANSMIT_PROPERTY( m_bDuration );
	ADD_TRANSMIT_PROPERTY( m_nDuration );
	ADD_TRANSMIT_PROPERTY( m_bInterval );
	ADD_TRANSMIT_PROPERTY( m_nInterval );
	ADD_TRANSMIT_PROPERTY( m_nGateTick );
END_PROPERTY_LIST();

NounJumpGate::NounJumpGate() : 
	m_bActive( true ), 
	m_bAutoActivate( false ),
	m_bSafeZone( false ), 
	m_fSafeZoneDistance( 0.0f ), 
	m_bDuration( false ), 
	m_nDuration( 0 ), 
	m_bInterval( false ), 
	m_nInterval( 0 ), 
	m_nGateTick( 0 ), 
	m_nLastUse( 0 ),
	m_bRemoteServer( false ), 
	m_nPort( 0 ),
	m_bDestinationInvalid( false )
{}

//---------------------------------------------------------------------------------------------------

bool NounJumpGate::read( const InStream & input )
{
	if (! NounBody::read( input ) )
		return false;

	return true;
}

//----------------------------------------------------------------------------

void NounJumpGate::render( RenderContext &context, const Matrix33 & frame, const Vector3 & position )
{
	if ( active() )
		NounBody::render( context, frame, position );
}

//----------------------------------------------------------------------------

NounType NounJumpGate::nounType() const
{
	return TYPE_LANDMARK;
}

bool NounJumpGate::collidable() const
{
	return true;
}

void NounJumpGate::initialize()
{
	NounBody::initialize();

	m_pSafeZone = NULL;
	NounField::removeAllFields( this );

	updateDestination();
}

void NounJumpGate::release()
{
	NounBody::release();
}

void NounJumpGate::simulate( dword nTick )
{
	updateDestination();

	// gates can have a safe zone, which is an area around the gate when all ships have their weapons deactivated.
	if ( active() && m_bSafeZone )
	{
		if ( !m_pSafeZone.valid() )
		{
			m_pSafeZone = new FieldSafeZone;
			m_pSafeZone->setFieldRadius( m_fSafeZoneDistance );

			attachNode( m_pSafeZone );
		}
	}
	else if ( m_pSafeZone.valid() )
	{
		detachNode( m_pSafeZone );
		m_pSafeZone = NULL;
	}

	if ( m_bDuration || m_bInterval )
	{
		// increment the gate tick
		m_nGateTick += nTick - tick();

		// loop the gate tick if we have an interval time set
		if ( m_bInterval && m_nGateTick > m_nInterval )
			m_nGateTick -= m_nInterval;

		// check the duration
		if ( m_nGateTick <= m_nDuration )
		{
			// gate is active
			if (! active() )
			{
				// create a use effect as the gate becomes active
				createUseEffect();
				// set the gate active
				setActive( true );
			}
		}
		else
		{
			// gate is inactive
			if ( active() )
			{
				// create a use effect as the gate goes inactive
				createUseEffect();
				// set the gate as inactive
				setActive( false );
				// if the gate has no interval, then detach the gate from the world
				if (! m_bInterval && isServer() )
					setDetach();
			}
		}
	}

	if ( m_bAutoActivate )
	{
		if ( m_bActive && !m_bRemoteServer && !m_pDestination.valid() )
			setActive( false );
		else if ( !m_bActive && (m_bRemoteServer || m_pDestination.valid() ) )
			setActive( true );
	}


	NounBody::simulate( nTick );
}

void NounJumpGate::collision( Noun * pCollide,  const Vector3 & intersect )
{
	if ( active() )
	{
		if ( pCollide->isLocal() )
		{
			bool bCanUse = false;
			if ( WidgetCast<NounShip>( pCollide ) )
				bCanUse = true;	

			if ( bCanUse && !pCollide->detach() )
				Verb::Ref( new VerbUseJumpGate( pCollide, this ) );
		}
	}
}

//---------------------------------------------------------------------------------------------------

CharString NounJumpGate::status() const
{
	CharString sStatus;

	NounGame * pDestination = WidgetCast<NounGame>( m_pDestination );
	if ( pDestination != NULL )
	{
		CharString sStarSystem( pDestination->starSystem() );
		if ( sStarSystem.length() > 0 )
			sStatus += CharString().format("\nSystem: %s", sStarSystem.cstr() );
		sStatus += CharString().format("\nDestination: %s", pDestination->name());
	}
	else
	{
		sStatus += "\nDestination: Home System";
	}

	return sStatus;
}

//----------------------------------------------------------------------------

void NounJumpGate::setActive( bool bActive )
{
	m_bActive = bActive;
	resync();
}

void NounJumpGate::setAutoActivate( bool a_bAutoActivate )
{
	m_bAutoActivate = a_bAutoActivate;
}

void NounJumpGate::setRemoteServer( const char * a_pAddress, int a_nPort )
{
	m_bRemoteServer = true;
	m_sAddress = a_pAddress;
	m_nPort = a_nPort;
	m_pDestination = NULL;

	resync();
}

void NounJumpGate::clearRemoteServer()
{
	m_bRemoteServer = false;
	m_sAddress = "";
	m_nPort = 0;

	resync();
}

void NounJumpGate::setDestination( const char * pName )
{
	m_sDestination = pName;
	m_pDestination = NULL;
	updateDestination( true );
}
 
void NounJumpGate::setDestination( Noun * pNoun )
{
	m_pDestination = pNoun;
	m_sDestination = pNoun != NULL ? pNoun->name() : "";
}

void NounJumpGate::setSafeZone( bool bSafeZone, float fDistance /*= 1000.0f*/ )
{
	m_bSafeZone = bSafeZone;
	m_fSafeZoneDistance = fDistance;
}

void NounJumpGate::setDuration( bool bDuration, dword nDuration )
{
	m_bDuration = bDuration;
	m_nDuration = nDuration;
}

void NounJumpGate::setInterval( bool bInterval, dword nInterval )
{
	m_bInterval = bInterval;
	m_nInterval = nInterval;
}

bool NounJumpGate::updateDestination( bool bForce /*= false*/ )
{
	WorldContext * pContext = context();
	if (! pContext )
		return false;						// not linked yet
	if ( m_bDestinationInvalid && !bForce )
		return false;						// destination is invalid..
	if ( sm_bBuildMode )
		return false;						// don't try to link up in build mode..

	// clear the invalid flag
	m_bDestinationInvalid = false;

	// we check if m_pDestination.key() is NULL_WIDGET because when a WH device is used, it sends down the 
	// two wormholes to the client, the client may receive and initialize one WH before it gets the other, which can
	// cause m_pDestination to be invalid until the other noun is received and added to the world.
	if ( !m_pDestination.valid() && m_pDestination.key() == NULL_WIDGET && m_sDestination.length() > 0  )
	{
		// resolve to the root context, since this may be a gate in a proxy world we want to find our destination
		// from the root world..
		while( pContext->parent() != NULL )
			pContext = pContext->parent();

		// try to find the destination in this context by name then..
		m_pDestination = pContext->findNoun( m_sDestination );

		// if we failed to find our desination, then set the invalid flag so we don't keep searching..
		if (! m_pDestination.valid() )
			m_bDestinationInvalid = true;
	}

	return m_pDestination.valid();
}

//----------------------------------------------------------------------------

const dword USE_DELAY = TICKS_PER_SECOND * 5;		// how often can we create a use effect, this 
													// keeps too many use effects from being created

Scene * NounJumpGate::useEffect() const
{
	return WidgetCast<Scene>( nounContext()->resource( "USE", false ) );
}

void NounJumpGate::createUseEffect()
{
	if ( isClient() )
	{
		if ( context()->tick() > m_nLastUse )
		{
			m_nLastUse = context()->tick() + USE_DELAY;

			Scene * pUseEffect = useEffect();
			if ( pUseEffect != NULL )
			{
				// create the scenery effect object
				SceneryEffect * pEffect = new SceneryEffect;
				pEffect->setNounContext( new NounContext(useEffect()) );
				pEffect->setPosition( position() );
				pEffect->setSegment( "Effect" );

				ASSERT( zone() != NULL );
				zone()->attachNoun( pEffect );
			}
		}
	}
}

//----------------------------------------------------------------------------
// EOF
