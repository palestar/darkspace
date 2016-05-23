/*
	NounJumpTarget.cpp
	(c)1999 PaleStar Development, Richard Lyle
*/


#include "Math/Helpers.h"
#include "Constants.h"
#include "NounJumpTarget.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounJumpTarget, NounTarget );
REGISTER_FACTORY_KEY( NounJumpTarget, 4249912333968476259LL );

BEGIN_PROPERTY_LIST( NounJumpTarget, NounTarget )
	ADD_TRANSMIT_PROPERTY( m_fRadius );
END_PROPERTY_LIST();

NounJumpTarget::NounJumpTarget()
{}

NounJumpTarget::NounJumpTarget( const Vector3 & vPosition, float fRadius, WorldContext * pContext  ) 
	: NounZoneTarget( vPosition, pContext )
{
	m_fRadius = fRadius;
}

//----------------------------------------------------------------------------
// EOF

