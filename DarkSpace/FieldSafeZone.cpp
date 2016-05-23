/*
	FieldSafeZone.cpp
	(c)2004 Palestar Inc, Richard Lyle
*/


#include "FieldSafeZone.h"
#include "NounJumpGate.h"
#include "NounShip.h"
#include "NounProjectile.h"
#include "resource.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( FieldSafeZone, NounField );
REGISTER_FACTORY_KEY( FieldSafeZone, 4635884409631020732LL );

BEGIN_PROPERTY_LIST( FieldSafeZone, NounField )
END_PROPERTY_LIST();

FieldSafeZone::FieldSafeZone()
{}

//----------------------------------------------------------------------------

void FieldSafeZone::simulate( dword nTick )
{
	NounField::simulate( nTick );
}

//----------------------------------------------------------------------------

void FieldSafeZone::onEnterField( Noun * pNoun )
{
	if ( WidgetCast<NounShip>( pNoun ) )
	{
		NounShip * pShip = (NounShip *)pNoun;
		pShip->message( "<color;ffffff>Tactical: Entering Safe Zone." );
		pShip->addFlags( NounShip::FLAG_IN_SAFE_ZONE );
	}
	else if ( WidgetCast<NounProjectile>( pNoun ) )
	{
		// destroy any projectiles entering the safe zone (i.e. missiles)
		((NounProjectile *)pNoun)->explode();
	}
}

void FieldSafeZone::onInsideField( Noun * pNoun )
{}

void FieldSafeZone::onLeaveField( Noun * pNoun )
{
	if ( WidgetCast<NounShip>( pNoun ) )
	{
		NounShip * pShip = (NounShip *)pNoun;
		// ship has left safe zone
		pShip->message( "<color;ffffff>Tactical: Leaving Safe Zone." );
		pShip->clearFlags( NounShip::FLAG_IN_SAFE_ZONE );
	}
}

//----------------------------------------------------------------------------
//EOF
