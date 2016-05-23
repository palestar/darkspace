/*
	SceneryEffect.cpp

	This scenery noun is removed from the area after its life reaches zero. This object
	isn't removed from the area with a verb because it's a scenery object and has no bearing
	on the gameplay.

	(c)1999 PaleStar, Richard Lyle
*/


#include "Constants.h"
#include "SceneryEffect.h"
#include "GameContext.h"
#include "Debug/Assert.h"
#include "Render3D/NodeLight.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( SceneryEffect, NounScenery );
REGISTER_FACTORY_KEY( SceneryEffect, 4036997766720649122LL );

BEGIN_PROPERTY_LIST( SceneryEffect, NounScenery )
	ADD_PROPERTY( m_Delay );
	ADD_PROPERTY( m_Life );
END_PROPERTY_LIST();

SceneryEffect::SceneryEffect()
{
	m_Delay = 0.0f;
	m_Life = 5.0f;
}

//----------------------------------------------------------------------------

void SceneryEffect::preRender( RenderContext & context, 
					const Matrix33 &frame,
					const Vector3 &position )
{
	// until delay expires do not render
	if ( m_Delay > 0 )
		return;

	NounScenery::preRender( context, frame, position );
}

//----------------------------------------------------------------------------

void SceneryEffect::simulate( dword nTick )
{
	float fElapsed = TICK_DURATION_S * (nTick - tick());

	if ( m_Delay > 0 )
	{
		m_Delay -= fElapsed;
		if ( m_Delay > 0 )
			return;

		m_Life += m_Delay;	// reduce life by the amount delay in the negative
	}

	m_Life -= fElapsed;
	if ( m_Life < 0 )
		setDetach();

	NounScenery::simulate( nTick );
}

//----------------------------------------------------------------------------

float SceneryEffect::delay() const
{
	return( m_Delay );
}

float SceneryEffect::life() const
{
	return( m_Life );
}

void SceneryEffect::setDelay( float delay )
{
	m_Delay = delay;
}

void SceneryEffect::setLife( float life )
{
	m_Life = life;
}

bool SceneryEffect::setSegment( const char * segmentName )
{
	Scene * pScene = nounContext()->scene();
	if ( pScene == NULL )
		return false;
	// Verify nobody has put a light source into a effect, if this assert hits look up the callstack to find the weapon
	// that is making this effect through the projectile object.
	ASSERT( FindNodeClass<NodeLight>( pScene->node() ) == NULL );

	int segmentIndex = pScene->findSegment( segmentName );
	if ( segmentIndex < 0 )
		return false;

	const Scene::Segment & segment = pScene->segment( segmentIndex );
	m_Time = segment.beginTime;
	m_Life = segment.endTime - segment.beginTime;

	return true;
}

//----------------------------------------------------------------------------
// EOF

