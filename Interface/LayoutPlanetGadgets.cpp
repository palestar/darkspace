/*
	LayoutPlanetGadgets.cpp

	This window displays all the gadgets on the current planet target that can be traded
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "DarkSpace/CargoGadget.h"
#include "Interface/GameDocument.h"
#include "Interface/LayoutPlanetGadgets.h"

//----------------------------------------------------------------------------

Constant UPDATE_PLANET_GADGETS_LAYOUT( "UPDATE_PLANET_GADGETS_LAYOUT", 5.0f );

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( LayoutPlanetGadgets, WindowLayout );
REGISTER_FACTORY_KEY( LayoutPlanetGadgets, 4542816757060693423 );

LayoutPlanetGadgets::LayoutPlanetGadgets()
{}

//----------------------------------------------------------------------------

void LayoutPlanetGadgets::onActivate()
{
	WindowLayout::onActivate();

	detachAllNodes();
}

void LayoutPlanetGadgets::onDeactivate()
{
	WindowLayout::onDeactivate();

	m_Planet = NULL;
	detachAllNodes();
}

void LayoutPlanetGadgets::onUpdate( float t )
{
	WindowLayout::onUpdate( t );

	if ( visible() )
	{
		GameDocument * pDoc = WidgetCast<GameDocument>( document() );
		ASSERT( pDoc );
		NounShip * pShip = pDoc->ship();
		if (! pShip )
			return;

		NounPlanet * pPlanet = pDoc->planetTarget();
		if ( m_Planet != pPlanet || activeTime() > UPDATE_PLANET_GADGETS_LAYOUT )
		{
			int buttonCount = 0;

			setActiveTime( 0 );

			m_Planet = pPlanet;
			//if ( m_Planet.valid() )
			//{
			//	for(int i=0;i<m_Planet->childCount();i++)
			//	{
			//		Noun * pChild = (Noun *)m_Planet->child(i);
			//		if ( dynamic_cast<StructurePort *>( pChild ) )
			//		{
			//			StructurePort * pPort = (StructurePort *)pChild;
			//			for(int j=0;j<pPort->childCount();j++)
			//			{
			//				CargoGadget * pCargo = dynamic_cast<CargoGadget *>( pPort->child(j) );
			//				if ( pCargo != NULL )
			//					GetButton<ButtonTrade>( this, buttonCount++ )->setButton( pPort, pShip, pCargo );
			//			}
			//		}
			//	}
			//}

			// remove any extra buttons
			cullChildren( buttonCount );
		}
	}
}

//----------------------------------------------------------------------------
//EOF
