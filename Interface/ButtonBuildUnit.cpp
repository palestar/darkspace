/*
	ButtonBuildUnit.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Interface/ButtonBuildUnit.h"
#include "DarkSpace/VerbBuildUnit.h"
#include "Interface/GameDocument.h"

//----------------------------------------------------------------------------

ButtonBuildUnit::ButtonBuildUnit()
{
	m_Flags |= NOCLIP;
	m_Style = HAS_BACK | EFFECT_HIGHLIGHT | EFFECT_FADEIN | SMOOTH_LL | LOCK_ICON_SIZE;
	m_Alpha = 0.75f;

	setWindow( RectInt( PointInt(0,0), SizeInt( 32, 24 ) ) );
	setEnable( false );
}

//----------------------------------------------------------------------------

void ButtonBuildUnit::onUpdate( float t )
{
	WindowButton::onUpdate( t );

	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );

	setIconColor( VerbBuildUnit::canBuild( m_Barracks, pDoc->ship(), m_Unit ) ? WHITE : RED );
}

void ButtonBuildUnit::onRender( RenderContext & context, const RectInt & window )
{
	WindowButton::onRender( context, window );

	if ( enabled() && m_Unit.valid() )
	{
		DisplayDevice * pDisplay = context.display();
		ASSERT( pDisplay );

		Font * pFont = windowStyle()->font();
		if ( pFont != NULL )
		{
			// draw the eta to build this gadget
			WideString eta;
			eta.format( "%d:%2.2d", m_Unit->buildTime() / 60, m_Unit->buildTime() % 60);

			SizeInt etaSize( pFont->size( eta ) );
			PointInt etaPos( window.right - etaSize.width, window.bottom - etaSize.height );
			Font::push( context.display(), pFont, etaPos, eta, WHITE );
		}
	}
}

void ButtonBuildUnit::onButtonUp()
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );

	// add item to queue
	if ( VerbBuildUnit::canBuild( m_Barracks, pDoc->ship(), m_Unit ) )
		Verb::Ref( new VerbBuildUnit( m_Barracks, pDoc->ship(), m_Unit ) );

	WindowButton::onButtonUp();
}

void ButtonBuildUnit::setButton( StructureBarracks * pBarracks, NounUnit * pUnit )
{
	ASSERT( pBarracks && pUnit );

	m_Barracks = pBarracks;
	m_Unit = pUnit;

	// set the icon
	setIcon( pUnit->icon() );

	// update tool tip
	CharString tip;
	tip += CharString().format("<color;ffffff>Build %s</color>\n%s\n\n", 
		pUnit->nounContext()->name(), pUnit->nounContext()->description() );

	NounPlanet * pPlanet = pBarracks->planet();
	ASSERT( pPlanet );

	if ( pUnit->buildTechnology() > 0 )
	{
		tip += CharString().format("<color;%s>Technology %d\n", 
			pPlanet->technology() < pUnit->buildTechnology() ? "8080ff" : "ffffff",
			pUnit->buildTechnology() );
	}

	if ( pUnit->buildCost() > 0 )
	{
		int available = pPlanet->resources() - pUnit->buildCost();
		tip += CharString().format("<color;%s>%d Resources\n", available < 0 ? "8080ff" : "ffffff", pUnit->buildCost() );
	}

	tip += CharString().format("\n<color;ffffff>Time: %d:%2.2d</color>", 
		pUnit->buildTime() / 60, 
		pUnit->buildTime() % 60 );

	setTip( tip );
	setEnable( true );
}

//----------------------------------------------------------------------------
//EOF
