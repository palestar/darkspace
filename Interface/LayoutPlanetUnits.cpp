/*
	LayoutPlanetUnits.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "DarkSpace/CargoGadget.h"
#include "Interface/GameDocument.h"
#include "Interface/ButtonUnit.h"
#include "Interface/LayoutPlanetUnits.h"

//----------------------------------------------------------------------------

Constant UPDATE_PLANET_UNITS_LAYOUT( "UPDATE_PLANET_UNITS_LAYOUT", 5.0f );

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( LayoutPlanetUnits, WindowLayout );
REGISTER_FACTORY_KEY( LayoutPlanetUnits, 4542829362394508586 );

LayoutPlanetUnits::LayoutPlanetUnits()
{}

//----------------------------------------------------------------------------

void LayoutPlanetUnits::onActivate()
{
	WindowLayout::onActivate();

	detachAllNodes();
}

void LayoutPlanetUnits::onDeactivate()
{
	WindowLayout::onDeactivate();

	m_Planet = NULL;
	detachAllNodes();
}

void LayoutPlanetUnits::onUpdate( float t )
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
		if ( m_Planet != pPlanet || activeTime() > UPDATE_PLANET_UNITS_LAYOUT )
		{
			int buttonCount = 0;

			setActiveTime( 0 );

			m_Planet = pPlanet;
			if ( pPlanet != NULL )
			{
				for(int i=0;i<pPlanet->childCount();i++)
				{
					NounUnit * pUnit = WidgetCast<NounUnit>( pPlanet->child(i) );
					if ( pUnit != NULL && pUnit->isDetected( myFactionID ) )
						GetButton<ButtonUnit>( this, buttonCount++ )->setButton( pUnit );
				}
			}

			// remove any extra buttons
			cullChildren( buttonCount );
		}
	}

}

//----------------------------------------------------------------------------
//EOF
