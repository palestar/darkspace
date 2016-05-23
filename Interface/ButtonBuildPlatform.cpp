/*
	ButtonBuildPlatform.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "DarkSpace/VerbBuildPlatform.h"
#include "Interface/GameDocument.h"
#include "DarkSpace/Constants.h"
#include "ButtonBuildPlatform.h"

//----------------------------------------------------------------------------

ButtonBuildPlatform::ButtonBuildPlatform()
{
	m_Flags |= NOCLIP;
	m_Style = HAS_BACK | EFFECT_HIGHLIGHT | EFFECT_FADEIN | SMOOTH_LL | LOCK_ICON_SIZE;
	m_Alpha = 0.75f;

	setWindow( RectInt( PointInt(0,0), SizeInt( 32, 24 ) ) );
	setEnable( false );
}

//----------------------------------------------------------------------------

void ButtonBuildPlatform::onUpdate( float t )
{
	WindowButton::onUpdate( t );

	if ( enabled() )
	{
		GameDocument * pDoc = (GameDocument *)document();
		ASSERT( pDoc );

		// update the icon color
		setIconColor( VerbBuildPlatform::canBuild( pDoc->ship(), m_Target, m_Build ) ? WHITE : RED );
	}
}

void ButtonBuildPlatform::onRender( RenderContext & context, const RectInt & window )
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
			eta.format( "%d:%2.2d", m_Build->buildTime() / 60, m_Build->buildTime() % 60);

			SizeInt etaSize( pFont->size( eta ) );
			PointInt etaPos( window.right - etaSize.width, window.bottom - etaSize.height );
			Font::push( context.display(), pFont, etaPos, eta, WHITE );
		}
	}
}

void ButtonBuildPlatform::onButtonUp()
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );

	if ( VerbBuildPlatform::canBuild( pDoc->ship(), m_Target, m_Build ) )
		Verb::Ref( new VerbBuildPlatform( pDoc->ship(), m_Target, m_Build ) );

	WindowButton::onButtonUp();
}

void ButtonBuildPlatform::setButton( NounZoneTarget * pTarget, ShipPlatform * pBuild )
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc && pTarget && pBuild );

	m_Target = pTarget;
	m_Build = pBuild;

	// set the icon for the structure
	setIcon( pBuild->icon() );

	// update tool tip
	CharString sTip;
	sTip += CharString().format("<color;ffffff>Build %s</color>\n%s\n\n", 
		pBuild->nounContext()->name(), pBuild->nounContext()->description() );

	sTip += "Requires:\n";
	if ( pBuild->buildCost() > 0 )
	{
		int cost = pDoc->ship()->resourceCount() - pBuild->buildCost();
		sTip += CharString().format("<color;%s>%d Resources</color>\n", 
			(cost >= 0 ? "ffffff" : "8080ff"),  pBuild->buildCost() );
	}

	int nPlatformCount = pDoc->context()->factionPlatforms(pDoc->ship()->factionId());
	sTip += CharString().format("<color;%s>Platforms: %d/%d</color>\n", 
		(nPlatformCount >= MAX_PLATFORMS_PER_FACTION ? "8080ff" : "ffffff"),  nPlatformCount,(int)MAX_PLATFORMS_PER_FACTION );


	sTip += CharString().format("\n<color;ffffff>Time: %d:%2.2d</color>", 
		pBuild->buildTime() / 60, pBuild->buildTime() % 60 );

	setTip( sTip );

	setName( pBuild->name() );
	setEnable( true );
}

//----------------------------------------------------------------------------
//EOF
