/*
	LayoutCargo.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Interface/GameDocument.h"
#include "Interface/LayoutCargo.h"
#include "Interface/ButtonCargo.h"
#include "DarkSpace/NounUnit.h"
#include "DarkSpace/CargoEnhancement.h"
#include "DarkSpace/NounCargo.h"
#include "Interface/TipHelp.h"
#include "Interface/ViewGame.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( LayoutCargo, WindowLayout );
REGISTER_FACTORY_KEY( LayoutCargo, 4241004244943204683 );

LayoutCargo::LayoutCargo()
{}

//----------------------------------------------------------------------------

void LayoutCargo::onActivate()
{
	detachAllNodes();
}

void LayoutCargo::onDeactivate()
{
	detachAllNodes();
}

void LayoutCargo::onUpdate( float t )
{
	WindowLayout::onUpdate( t );

	GameDocument * pDoc = WidgetCast<GameDocument>( document() );
	ASSERT( pDoc );

	NounShip * pShip = pDoc->ship();
	if ( pShip != NULL )
	{
		// make a list of all cargo items
		Array< Noun * > cargo;
		for(int i=0;i<pShip->childCount();i++)
		{
			BaseNode * pNode = pShip->child(i);
			if ( WidgetCast<NounUnit>( pNode ) )
				cargo.push( (Noun *)pNode );
			else if ( WidgetCast<NounCargo>( pNode ) )
			{
				if ( ((NounCargo *)pNode)->quantity() > 0 )
					cargo.push( (Noun *)pNode );
			}
		}

		// add/update buttons
		for(int i=0;i<cargo.size();i++)
			GetButton<ButtonCargo>( this, i )->setCargo( cargo[i] );
		
		// remove excess buttons
		cullChildren( cargo.size() );
	}
}

//----------------------------------------------------------------------------
//EOF
