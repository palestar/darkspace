/*
	TextPlanet.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Interface/GameDocument.h"
#include "Interface/TextPlanet.h"
#include "Standard/StringBuffer.h"

//----------------------------------------------------------------------------

Constant UPDATE_PLANET_TEXT( "UPDATE_PLANET_TEXT", 5.0f );

static const char * MoralText( float moral )
{
	if ( moral > 0.95f )
		return "High";
	if ( moral > 0.60f )
		return "Okay";
	return "Low";
}

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( TextPlanet, WindowText );
REGISTER_FACTORY_KEY( TextPlanet, 4543230490597685161 );

TextPlanet::TextPlanet()
{}

//----------------------------------------------------------------------------

void TextPlanet::onActivate()
{
	WindowText::onActivate();
	setText( "" );
}

void TextPlanet::onUpdate( float t )
{
	WindowText::onUpdate( t );

	if ( visible() )
	{
		GameDocument * pDoc = WidgetCast<GameDocument>( document() );
		ASSERT( pDoc );
		NounShip * pShip = pDoc->ship();
		if (! pShip )
			return;

		NounPlanet * pPlanet = pDoc->planetTarget();
		if ( m_Planet != pPlanet || activeTime() > UPDATE_PLANET_TEXT )
		{
			setActiveTime( 0 );
			if ( pPlanet != NULL )
			{
				// update the planet status
				CharString sStatus;
				sStatus += CharString().format("<b;l>%s</b;/l>\n%s\n", pPlanet->name(), pPlanet->teamName() );
				sStatus += CharString().format("Control:<color;ffffff;X;150>%.1f%%</color>\n", (pPlanet->control() / pPlanet->maxControl()) * 100.0f );
				sStatus += CharString().format("Allegiance:<color;ffffff;X;150>%d</color>\n", pPlanet->allegiance() );
				sStatus += CharString().format("Allegiance Progress:<color;ffffff;X;150>%d / %d</color>\n", pPlanet->allegianceXP(), pPlanet->allegianceXPRequired() );
				sStatus += CharString().format("Population:<color;ffffff;X;150>%d</color>\n", ((int)pPlanet->population()) );
				sStatus += CharString().format("<X;25>Habitat:<color;ffffff;X;150>%d</color>\n", pPlanet->habitat() );
				sStatus += CharString().format("Technology:<color;ffffff;x;150>%.0f</color>\n", floor( pPlanet->technology() ) );
				sStatus += CharString().format("<X;25>Research:<color;ffffff;X;150>%d</color>\n", pPlanet->research() );

				int workers = pPlanet->workersAvailable();
				int workersNeeded = pPlanet->workersNeeded();

				sStatus += CharString().format("Workers:<color;%s;X;150>%d / %d / %c%d</color>\n", 
					(pPlanet->isWorkerShortage() ? "8080ff" : "ffffff"), 
					workers, workersNeeded, (workers >= workersNeeded ? '+' : '-'), abs(workers - workersNeeded) );

				sStatus += CharString().format("<X;25>Morale:<color;ffffff;X;150>%s</color>\n", MoralText( pPlanet->moral() ) );
				sStatus += CharString().format("Value:<color;ffffff;X;150>%s credits</color>\n", FormatNumber<char,int>( pPlanet->value() ) );
				//sStatus += CharString().format("<X;25>Credits:<color;ffffff;X;150>%s credits</color>\n", FormatNumber<char,int>( pPlanet->credits() ) );
				//sStatus += CharString().format("<X;25>Income:<color;ffffff;X;150>%s credits</color>\n", FormatNumber<char,int>( pPlanet->income() ) );
				sStatus += CharString().format("Govern Rank:<color;ffffff;X;150>%s</color>\n", GameProfile::rankText( pPlanet->lockRank() ) );

				int foodNeeded = pPlanet->foodNeeded();
				int food = pPlanet->food();

				sStatus += CharString().format("Food:<color;%s;X;150>%d / %d / %c%d</color>\n",
					(foodNeeded > food ? "8080ff" : "ffffff"), food, foodNeeded, 
					(food >= foodNeeded ? '+' : '-'), abs(food - foodNeeded) );

				int powerNeeded = pPlanet->powerNeeded();
				int power = pPlanet->powerAvailable();

				sStatus += CharString().format("Power:<color;%s;X;150>%d / %d / %c%d</color>\n",
					(powerNeeded > power ? "8080ff" : "ffffff"), power, powerNeeded, 
					(power >= powerNeeded ? '+' : '-'), abs(power - powerNeeded) );

				//status += "\n";
				sStatus += CharString().format("Mining:<color;ffffff;X;150>%d</color>\n", pPlanet->mining() );
				sStatus += CharString().format("Production:<color;ffffff;X;150>%d</color>\n", pPlanet->production() );
				sStatus += CharString().format("Resources:<color;ffffff;X;150>%s</color>\n", FormatNumber<char,int>( pPlanet->resources() ) );
				sStatus += "\n";

				// display planet bits
				for(int i=0;i<32;i++)
				{
					dword nBit = 1 << i;
					if ( pPlanet->flags() & nBit )
						sStatus += CharString().format("%s\n", pPlanet->flagName( nBit ) );
				}

				// update the window text
				setText( sStatus );
			}
			else
				setText( "" );		// no planet targeted
		}
	}
}

//----------------------------------------------------------------------------
//EOF
