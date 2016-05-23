/*
	NounScenery.cpp
	(c)1999 Palestar, Richard Lyle
*/


#include "Constants.h"
#include "NounScenery.h"
#include "GameContext.h"

//-------------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounScenery, NounGame );
REGISTER_FACTORY_KEY( NounScenery, 4033932272827480182LL );

BEGIN_PROPERTY_LIST( NounScenery, NounGame )
	ADD_PROPERTY( m_Time );
END_PROPERTY_LIST();

NounScenery::NounScenery() : m_Time( 0.0f )
{}

//-------------------------------------------------------------------------------

void NounScenery::render( RenderContext &context, 
				const Matrix33 & frame, 
				const Vector3 & position )
{
	// update the current time
	m_Time += context.elapsed();

	// set the current time
	float previousTime = context.time();
	context.setTime( m_Time );

	Noun::render( context, frame, position );

	// restore the time
	context.setTime( previousTime );
}

//----------------------------------------------------------------------------

NounType NounScenery::nounType() const
{
	return TYPE_EFFECT;
}

//-------------------------------------------------------------------------------
// EOF
