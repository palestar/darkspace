/*
	NounBody.h - This class has been depreciated, use the enableOrbit() and canFleetOwn() virtuals of Noun to provide the same functionality
	(c)1999 Palestar Development, Richard Lyle
*/

//#define PROFILE_OFF

#include "Debug/Assert.h"
#include "World/TraitMovement.h"
#include "Constants.h"
#include "NounProjectile.h"
#include "NounShip.h" 
#include "NounBody.h"
#include "NounPlanet.h"
#include "NounStar.h"
#include "NounBeacon.h"
#include "NounStructure.h"
#include "TraitOrbit.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounBody, NounGame );
REGISTER_FACTORY_KEY( NounBody, 4037043109001541538LL );

BEGIN_PROPERTY_LIST( NounBody, NounGame )
	ADD_TRANSMIT_UPDATE_PROPERTY( m_nTeamId );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_DetectionFlags );
	ADD_TRANSMIT_PROPERTY( m_Traits );
END_PROPERTY_LIST();

NounBody::NounBody()
{
	attachTrait( new TraitOrbit() );
	//attachTrait( new TraitMovement() );
}
 
//----------------------------------------------------------------------------

NounType NounBody::nounType() const
{
	return TYPE_LOCAL_OBJECT;
}

bool NounBody::enableOrbit() const
{
	return true;
}

bool NounBody::canBeDetected() const
{
	return true;
}

//----------------------------------------------------------------------------
// EOF
