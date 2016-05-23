/*
	ButtonBuildStructure.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "DarkSpace/VerbBuildStructure.h"
#include "Interface/GameDocument.h"

#include "ButtonBuildStructure.h"

//----------------------------------------------------------------------------

ButtonBuildStructure::ButtonBuildStructure()
{
	m_Flags |= NOCLIP;
	m_Style = HAS_BACK | EFFECT_HIGHLIGHT | EFFECT_FADEIN | SMOOTH_LL | LOCK_ICON_SIZE;
	m_Alpha = 0.75f;

	setWindow( RectInt( PointInt(0,0), SizeInt( 32, 24 ) ) );
	setEnable( false );
}

//----------------------------------------------------------------------------

void ButtonBuildStructure::onUpdate( float t )
{
	WindowButton::onUpdate( t );

	if ( enabled() )
	{
		GameDocument * pDoc = (GameDocument *)document();
		ASSERT( pDoc );

		// update the icon color
		setIconColor( VerbBuildStructure::canBuild( pDoc->ship(), m_Target, m_Build ) ? WHITE : RED );
	}
}

void ButtonBuildStructure::onRender( RenderContext & context, const RectInt & window )
{
	WindowButton::onRender( context, window );

	if ( enabled() && m_Build.valid() )
	{
		DisplayDevice * pDisplay = context.display();
		ASSERT( pDisplay );

		// draw the eta to build this structure
		Font * pFont = windowStyle()->font();
		if ( pFont != NULL )
		{
			WideString eta;
			GameDocument * pDoc = (GameDocument *)document();

			if ( pDoc != NULL )
			{
				NounShip::Ref pShip = pDoc->ship();
				if ( pShip.valid() )
				{
					int buildTime = m_Build->buildTime()* pShip->calculateModifier(MT_BUILD_SPEED,true);
					eta.format( "%d:%2.2d", buildTime / 60, buildTime % 60);

					SizeInt etaSize( pFont->size( eta ) );
					PointInt etaPos( window.right - etaSize.width, window.bottom - etaSize.height );
					Font::push( context.display(), pFont, etaPos, eta, WHITE );
				}
			}
		}
	}
}

void ButtonBuildStructure::onButtonUp()
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );

	if ( VerbBuildStructure::canBuild( pDoc->ship(), m_Target, m_Build ) )
		Verb::Ref( new VerbBuildStructure( pDoc->ship(), m_Target, m_Build ) );

	WindowButton::onButtonUp();
}

bool ButtonBuildStructure::setButton( Noun * pTarget, NounStructure * pBuild )
{
	if ( !pTarget || !pBuild )
		return false;

	NounDropTarget * pDrop = WidgetCast<NounDropTarget>( pTarget );
	NounStructure * pUpgrade = WidgetCast<NounStructure>( pTarget );
	if (! pDrop && !pUpgrade )
		return false;
	NounPlanet * pPlanet = pDrop != NULL ? pDrop->planet() : pUpgrade->planet();

	m_Target = pTarget;
	m_Build = pBuild;

	// set the icon for the structure
	setIcon( pBuild->icon() );

	// update tool tip
	CharString sTip;
	sTip += CharString().format("<color;ffffff>Build %s</color>\n%s\n\n", 
		pBuild->nounContext()->name(), pBuild->nounContext()->description() );

	sTip += "Requires:\n";

	if ( pBuild->buildTechnology() > 0 )
	{
		sTip += CharString().format("<color;%s>%d Technology\n", pPlanet->technology() < pBuild->buildTechnology() ? "8080ff" : "ffffff",
			pBuild->buildTechnology() );
	}
	if ( pBuild->workers() > 0 )
	{
		int workers = (pPlanet->workersAvailable() - pPlanet->workersNeeded()) - pBuild->workers();
		sTip += CharString().format("<color;%s>%d Workers</color>\n", (workers >= 0 ? "ffffff" : "8080ff"), pBuild->workers() );
	}
	if ( pBuild->power() < 0 )
	{
		int power = (pPlanet->powerAvailable() - pPlanet->powerNeeded()) + pBuild->power();
		sTip += CharString().format("<color;%s>%d Power</color>\n", (power >= 0 ? "ffffff" : "8080ff"), -pBuild->power() );
	}
	if ( pBuild->buildCost() > 0 )
	{
		int cost = pPlanet->resources() - pBuild->buildCost();
		sTip += CharString().format("<color;%s>%d Resources</color>\n", (cost >= 0 ? "ffffff" : "8080ff"),  pBuild->buildCost() );
	}

	if ( pBuild->buildFlags() != 0 )
	{
		dword nFlags = pBuild->buildFlags();
		for(int i=0;i<32;i++)
		{
			dword nBit = 1 << i;
			if ( (nFlags & nBit) != 0 )
			{
				sTip += CharString().format("<color;%s>%s\n", 
					(pPlanet->flags() & nBit) ? "ffffff" : "8080ff", NounPlanet::flagName( nBit ) );
			}
		}
	}

	sTip += CharString().format("\n<color;ffffff>Time: %d:%2.2d</color>", pBuild->buildTime() / 60, pBuild->buildTime() % 60 );

	setTip( sTip );

	setName( pBuild->name() );
	setEnable( true );

	return true;
}

//----------------------------------------------------------------------------
//EOF
