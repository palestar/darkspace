/*
	LayoutPlanets.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Interface/GameDocument.h"
#include "Interface/WindowSeperator.h"
#include "Interface/ButtonContact.h"
#include "Interface/LayoutPlanets.h"

//----------------------------------------------------------------------------

Constant UPDATE_PLANETS_LAYOUT( "UPDATE_PLANETS_LAYOUT", 5.0f );
Constant PLANET_CONTACT_RANGE( "PLANET_CONTACT_RANGE", 1000000.0f );		// get planets how far out from the focus position
Constant MAX_PLANET_CONTACTS( "MAX_PLANET_CONTACTS", 21 );

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( LayoutPlanets, WindowLayout );
REGISTER_FACTORY_KEY( LayoutPlanets, 4543197922338591223 );

LayoutPlanets::LayoutPlanets()
{}

//----------------------------------------------------------------------------

void LayoutPlanets::onActivate()
{
	WindowLayout::onActivate();
	detachAllNodes();
}

void LayoutPlanets::onDeactivate()
{
	WindowLayout::onDeactivate();
	m_Focus = NULL;
	detachAllNodes();
}

void LayoutPlanets::onUpdate( float t )
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

		Noun * pFocus = pDoc->focus();
		if ( m_Focus != pFocus || activeTime() > UPDATE_PLANETS_LAYOUT )
		{
			int buttonCount = 0;

			setActiveTime( 0 );

			m_Focus = pFocus;
			if ( pFocus != NULL )
			{
				GameContext * pContext = pDoc->context();
				ASSERT( pContext );

				// update the planet list
				Array< NounPlanet * > planets;

				Array< GameContext::NounCollision > nouns;
				if ( pContext->proximityCheck( pShip->worldPosition(), PLANET_CONTACT_RANGE, nouns, CLASS_KEY(NounPlanet) ) )
				{
					for(int i=0;i<nouns.size();i++)
					{
						NounPlanet * pPlanet = WidgetCast<NounPlanet>( nouns[i].pNoun );
						if ( pPlanet != NULL && pPlanet->isDetected( myFactionID ) && pPlanet->maxStructures() > 0 )
							planets.push( pPlanet );
					}
				}

				// trim planets down if it exceeds our maximum number of contacts
				CullContacts<NounPlanet>( pFocus, MAX_PLANET_CONTACTS, planets );
				// update/add contact buttons
				for(int i=0;i<planets.size();i++)
					GetButton<ButtonContact>( this, buttonCount++ )->setButton( planets[i] );
			}

			// remove any extra buttons
			cullChildren( buttonCount );
		}
	}

}

//----------------------------------------------------------------------------
//EOF
