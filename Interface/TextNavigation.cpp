/*
	TextNavigation.cpp
	(c)2004 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "DarkSpace/NounAsteroid.h"
#include "DarkSpace/NounJumpGate.h"
#include "DarkSpace/NounStar.h"
#include "Interface/GameDocument.h"
#include "Interface/TextNavigation.h"
#include "Standard/StringBuffer.h"

//----------------------------------------------------------------------------

static Constant UPDATE_NAV_TEXT( "UPDATE_NAV_TEXT", 5.0f );

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( TextNavigation, WindowText );
REGISTER_FACTORY_KEY( TextNavigation, 4633914674088028627 );

TextNavigation::TextNavigation()
{}

//----------------------------------------------------------------------------

void TextNavigation::onActivate()
{
	WindowText::onActivate();
	setText( "" );
}

void TextNavigation::onUpdate( float t )
{
	WindowText::onUpdate( t );

	if ( visible() && activeTime() > UPDATE_NAV_TEXT )
	{
		setActiveTime( 0 );

		GameDocument * pDoc = (GameDocument *)document();
		ASSERT( pDoc );

		CharString sText;
		if ( pDoc->isTeamValid() )
		{
			GameContext * pContext = pDoc->context();
			ASSERT( pContext );

			Hash< dword, Array<NounPlanet *> >	factionPlanets;
			Hash< dword, Array<NounShip *> >	factionShips;

			int	nStars = 0;
			int	nAsteroids = 0;
			int	nGates = 0;

			// get the players teamId
			int nFactionId = pDoc->factionId();
			// push all detected objects
			for(int z=0;z<pContext->zoneCount();z++)
			{
				NodeZone * pZone = pContext->zone( z );
				for(int i=0;i<pZone->childCount();i++)
				{
					NounGame * pNoun = WidgetCast<NounGame>( pZone->child(i) );
					if (! pNoun )
						continue;

					if ( pNoun->isDetected( nFactionId ) )
					{
						if ( WidgetCast<NounShip>( pNoun ) )
							factionShips[ pNoun->factionId() ].push( (NounShip *)pNoun );
						else if ( WidgetCast<NounPlanet>( pNoun ) )
							factionPlanets[ pNoun->factionId() ].push( (NounPlanet *)pNoun );
						else if ( WidgetCast<NounStar>( pNoun ) )
							nStars++;
						else if ( WidgetCast<NounAsteroid>( pNoun ) )
							nAsteroids++;
						else if ( WidgetCast<NounJumpGate>( pNoun ) )
							nGates++;
					}
				}
			}

			// update navigation text information
			const GameContext::Team & fleet = pContext->team( pDoc->teamId() );
			sText += CharString().format( "<b;l>%s</b;/l>:\n", fleet.name );

			Array< NounShip * > & ships = factionShips[ fleet.factionId ];

			sText += CharString().format("<x;10>Ships:<x;150>%d\n", ships.size() );
			for(int k=0;k<NounShip::TYPE_COUNT;k++)
			{
				// how many ships of this type
				int count = 0;
				for(int j=0;j<ships.size();j++)
					if ( ships[j]->type() == (NounShip::Type)k )
						count++;

				if ( count > 0 )
					sText += CharString().format( "<X;20>%s:<X;150>%d\n", NounShip::typeText( (NounShip::Type)k ), count );
			}

			if ( factionPlanets.find( fleet.factionId ).valid() )
			{
				Array< NounPlanet *> & planets = factionPlanets[ fleet.factionId ];
				sText += CharString().format("<X;10>Planets:<X;150>%d\n", planets.size() );

				int population = 0;
				int ports = 0;
				int depots = 0;
				int yards = 0;
				int units = 0;

				for(int j=0;j<planets.size();j++)
				{
					NounPlanet * pPlanet = planets[ j ];
					population += pPlanet->population();
					units += pPlanet->friendlyUnitCount();

					if ( pPlanet->flags() & NounPlanet::FLAG_HAS_DEPOT )
						depots++;
					if ( pPlanet->flags() & NounPlanet::FLAG_HAS_PORT )
						ports++;
					if ( pPlanet->flags() & NounPlanet::FLAG_HAS_SHIPYARD )
						yards++;
				}

				if ( population > 0 )
					sText += CharString().format( "<X;20>Population:<X;150>%d\n", population );
				if ( units > 0 )
					sText += CharString().format( "<X;20>Units:<X;150>%d\n", units );
				if ( ports > 0 )
					sText += CharString().format( "<X;20>Ports:<X;150>%d\n", ports );
				if ( depots > 0 )
					sText += CharString().format( "<X;20>Depots:<X;150>%d\n", depots );
				if ( yards > 0 )
					sText += CharString().format( "<X;20>Ship Yards:<X;150>%d\n", yards );
			}
		}

		setText( sText );
	}
}

//----------------------------------------------------------------------------
//EOF
