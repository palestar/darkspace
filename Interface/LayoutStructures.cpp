/*
	LayoutStructures.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Interface/GameDocument.h"
#include "Interface/WindowSeperator.h"
#include "Interface/ButtonStructure.h"
#include "Interface/LayoutStructures.h"

//----------------------------------------------------------------------------

Constant UPDATE_STRUCTURE_LAYOUT( "UPDATE_STRUCTURE_LAYOUT", 5.0f );

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( LayoutStructures, WindowLayout );
REGISTER_FACTORY_KEY( LayoutStructures, 4543029000871151783 );

LayoutStructures::LayoutStructures()
{}

//----------------------------------------------------------------------------

void LayoutStructures::onActivate()
{
	WindowLayout::onActivate();
	detachAllNodes();
}

void LayoutStructures::onDeactivate()
{
	WindowLayout::onDeactivate();
	m_Planet = NULL;
	detachAllNodes();
}

void LayoutStructures::onUpdate( float t )
{
	WindowLayout::onUpdate( t );

	if ( visible() )
	{
		GameDocument * pDoc = WidgetCast<GameDocument>( document() );
		ASSERT( pDoc );
		NounShip * pShip = pDoc->ship();
		if (! pShip )
			return;
		int myFactionID = pShip->factionId();

		NounPlanet * pPlanet = pDoc->planetTarget();
		if ( m_Planet != pPlanet || activeTime() > UPDATE_STRUCTURE_LAYOUT )
		{
			int buttonCount = 0;

			setActiveTime( 0 );

			m_Planet = pPlanet;
			if ( pPlanet != NULL )
			{
				// get all structures, sort them by their class key
				for(int i=0;i<m_Planet->childCount();i++)
				{
					NounStructure * pStructure = WidgetCast<NounStructure>( m_Planet->child(i) );
					if ( pStructure != NULL && pStructure->isDetected( myFactionID ) )
						GetButton<ButtonStructure>( this, buttonCount++ )->setButton( pStructure );
				}
			}

			// remove any extra buttons
			cullChildren( buttonCount );
		}
	}

}

//----------------------------------------------------------------------------
//EOF
