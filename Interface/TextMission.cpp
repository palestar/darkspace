/*
	TextMission.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Interface/GameDocument.h"
#include "Interface/TextMission.h"

//----------------------------------------------------------------------------

Constant UPDATE_MISSION_TEXT( "UPDATE_MISSION_TEXT", 5.0f );

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( TextMission, WindowText );
REGISTER_FACTORY_KEY( TextMission, 4552817675418601865 );

TextMission::TextMission()
{}

//----------------------------------------------------------------------------

void TextMission::onActivate()
{
	WindowText::onActivate();
}

void TextMission::onUpdate( float t )
{
	WindowText::onUpdate( t );

	if ( visible() )
	{
		if ( activeTime() > UPDATE_MISSION_TEXT )
		{
			GameDocument * pDoc = WidgetCast<GameDocument>( document() );
			ASSERT( pDoc );
			NounShip * pShip = pDoc->ship();
			if (! pShip )
				return;

			static char * ORDER_DESC[] =
			{
				"NO ORDER %s",								// NOORDER
				"Engage %s and destroy.",					// ATTACK
				"Defend %s from attack by enemy ships.",	// DEFEND
				"Capture %s.",								// CAPTURE
				"Proceed to %s.",							// MOVE
				"Reload and repair %s.",					// RELOAD
				"Attach beacon to %s.",						// BEACON
				"Hold current position %s.",				// HOLD
				"Trade %s",									// TRADE
				"Recon enemy positions at %s.",				// RECON
				"Build structures on %s.",					// BUILD
				"Fallback and repair at %s."				// FALLBACK
			};

			CharString sText;
			sText.format( ORDER_DESC[ pShip->order() ], pShip->orderTarget() ? pShip->orderTarget()->name() : "" );

			setText( sText );
			setActiveTime( 0 );
		}
	}
}

//----------------------------------------------------------------------------
//EOF
