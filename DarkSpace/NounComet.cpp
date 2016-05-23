/*
	NounComet.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "NounComet.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounComet, NounAsteroid );
REGISTER_FACTORY_KEY( NounComet, 4377215358202223059LL );

BEGIN_PROPERTY_LIST( NounComet, NounAsteroid )
END_PROPERTY_LIST();

NounComet::NounComet()
{}

//----------------------------------------------------------------------------

void NounComet::simulate( dword nTick )
{
	NounAsteroid::simulate( nTick );
	// rotate this object to face the center of the star system (i.e. the star)
	setFrame( Matrix33( -worldPosition() ) );
}

//----------------------------------------------------------------------------
//EOF
