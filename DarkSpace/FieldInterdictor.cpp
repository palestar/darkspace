/*
	FieldInterdictor.cpp
	(c)2004 Palestar Inc, Richard Lyle
*/


#include "Debug/Profile.h"
#include "FieldInterdictor.h"
#include "GadgetJD.h"
#include "NounShip.h"
#include "VerbJumpAbort.h"
#include "resource.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( FieldInterdictor, NounField );
REGISTER_FACTORY_KEY( FieldInterdictor, 4635880574829179581LL );

BEGIN_PROPERTY_LIST( FieldInterdictor, NounField )
END_PROPERTY_LIST();

FieldInterdictor::FieldInterdictor()
{}

//----------------------------------------------------------------------------

void FieldInterdictor::simulate( dword nTick )
{
	NounField::simulate( nTick );
}

//----------------------------------------------------------------------------

void FieldInterdictor::onEnterField( Noun * pNoun )
{}

void FieldInterdictor::onInsideField( Noun * pNoun )
{
	if ( WidgetCast<NounShip>( pNoun ) )
	{
		NounShip * pShip = (NounShip *)pNoun;

		// disable any non-friendly ships inside the field...
		if ( ! isFriend( pShip ) )
		{
			if ( !pShip->testFlags( NounShip::FLAG_JUMP_DISABLED ) )	// no need to disable, if already disabled
			{
				pShip->addFlags( NounShip::FLAG_JUMP_DISABLED );

				if ( pShip->isLocal() )
				{
					pShip->message( "<color;ffffff>Helm: Jump drives disabled by interdictor.." );
					GadgetJumpDrive * pJD = pShip->jumpDrive();
					if ( pJD != NULL && (pJD->engaged() || pJD->jumping()) )
						Verb::Ref( new VerbJumpAbort( pShip ) );
				}
			}
		}
		// This causes the interdictor message spam when a friendly and enemy fields overlap each other... leave it off.
		//else if ( pShip->testFlags( NounShip::FLAG_JUMP_DISABLED ) )
		//{
		//	// ship is no longer an enemy ship, enable jump drives..
		//	pShip->clearFlags( NounShip::FLAG_JUMP_DISABLED );
		//}
	}
}

void FieldInterdictor::onLeaveField( Noun * pNoun )
{
	if ( WidgetCast<NounShip>( pNoun ) )
	{
		NounShip * pShip = (NounShip *)pNoun;
		// ship has left interdictor field, enable jump drive
		pShip->clearFlags( NounShip::FLAG_JUMP_DISABLED );
	}
}

//----------------------------------------------------------------------------
//EOF
