/*
	SceneryDebris.cpp
	(c)1999 Palestar, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Constants.h"
#include "SceneryDebris.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( SceneryDebris, SceneryEffect );
REGISTER_FACTORY_KEY( SceneryDebris, 4045106604108921626LL );

BEGIN_PROPERTY_LIST( SceneryDebris, SceneryEffect )
	ADD_PROPERTY( m_Velocity );
	ADD_PROPERTY( m_RotationVelocity );
	ADD_PROPERTY( m_EndEffect );
END_PROPERTY_LIST();

SceneryDebris::SceneryDebris() : m_Velocity( true ), m_RotationVelocity( true )
{}

//----------------------------------------------------------------------------

void SceneryDebris::simulate( dword nTick )
{
	float fElapsed = TICK_DURATION_S * (nTick - tick());
	if ( m_Delay > 0 )
	{
		m_Delay -= fElapsed;
		if ( m_Delay > 0 )
			return;

		m_Life += m_Delay;	// reduce life by the amount delay in the negative
	}

	// spin the debris, don't ortho-normalize
	m_Frame.rotate( m_RotationVelocity.x * fElapsed,
		m_RotationVelocity.y * fElapsed,
		m_RotationVelocity.z * fElapsed, false );

	// move the debris
	m_Position += m_Velocity * fElapsed;

	m_Life -= fElapsed;
	if ( m_Life < 0 )
	{
		NodeZone * pZone = zone();
		ASSERT( pZone );

		// create the end effect
		if ( m_EndEffect.valid() )
		{
			SceneryEffect * pEnd = new SceneryEffect;
			pEnd->setNounContext( new NounContext( m_EndEffect ) );
			pEnd->setPosition( position() );

			if (! pEnd->setSegment( "Effect" ) )
				pEnd->setLife( 5.0f );

			// attach the end effect
			pZone->attachNoun( pEnd );
		}
		// detach ourselves
		setDetach();
	}

	// NOT calling SceneryEffect::simulate() on purpose..
	NounScenery::simulate( nTick );
}

//----------------------------------------------------------------------------

void SceneryDebris::setVelocity( const Vector3 & velocity )
{
	m_Velocity = velocity;
}

void SceneryDebris::setRotationVelocity( const Vector3 & velocity )
{
	m_RotationVelocity = velocity;
}

void SceneryDebris::setEndEffect( Scene * pScene )
{
	m_EndEffect = pScene;
}

//----------------------------------------------------------------------------
// EOF

