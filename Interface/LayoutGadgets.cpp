/*
	LayoutGadgets.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Interface/GameDocument.h"
#include "Interface/LayoutGadgets.h"
#include "Interface/ButtonGadget.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( LayoutGadgets, WindowLayout );
REGISTER_FACTORY_KEY( LayoutGadgets, 4239973191560147640 );

LayoutGadgets::LayoutGadgets()
{}

//----------------------------------------------------------------------------

void LayoutGadgets::onActivate()
{
	detachAllNodes();
}

void LayoutGadgets::onDeactivate()
{
	detachAllNodes();
}

void LayoutGadgets::onUpdate( float t )
{
	GameDocument * pDoc = WidgetCast<GameDocument>( document() );
	ASSERT( pDoc );

	Array< NounGadget * > gadgets;

	NounShip * pShip = pDoc->ship();
	if ( pShip != NULL )
	{
		// find and sort all the gadgets
		for(int i=0;i<pShip->childCount();i++)
		{
			NounGadget * pGadget = WidgetCast<NounGadget>( pShip->child(i) );
			if ( pGadget != NULL )
			{
				// insert gadget into gadget array
				int j;
				for(j=0;j<gadgets.size();j++)
				{
					if ( pGadget->type() == gadgets[j]->type() && pGadget->hotkey() < gadgets[j]->hotkey() )
						break;
					if ( pGadget->type() < gadgets[j]->type() )
						break;
				}

				gadgets.insert( j, pGadget );
			}
		}
	}

	// update the buttons, creating them if needed
	for(int i=0;i<gadgets.size();i++)
		GetButton<ButtonGadget>( this, i )->setGadget( gadgets[i] );
	// remove any extra buttons
	cullChildren( gadgets.size() );
}

//----------------------------------------------------------------------------
//EOF
