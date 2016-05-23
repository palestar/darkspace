/*
	ButtonRefit.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Interface/ButtonRefit.h"
#include "DarkSpace/VerbRefitGadget.h"
#include "DarkSpace/GadgetSlot.h"
#include "Interface/GameDocument.h"

//----------------------------------------------------------------------------

ButtonRefit::ButtonRefit() 
{
	m_Flags |= NOCLIP;
	m_Style = HAS_BACK | EFFECT_HIGHLIGHT | SMOOTH_LL;
	m_Alpha = 0.75f;

	setWindow( RectInt( PointInt(0,0), SizeInt( 32, 24 ) ) );
	setEnable( false );
}

//----------------------------------------------------------------------------

void ButtonRefit::onUpdate( float t )
{
	WindowButton::onUpdate( t );

	if (! m_pGadget )
		return;
	if (! m_pNewGadget )
		return;

	bool bCanRefit =  VerbRefitGadget::canRefit( m_pGadget, m_pNewGadget );
	NounShip * pShip = WidgetCast<NounShip>( m_pGadget->parent() );
	NounPlanet * pPlanet = pShip ? WidgetCast<NounPlanet>( pShip->orbiting() ) : NULL;

	bool bResources = (pPlanet != NULL && pPlanet->resources() >= m_pNewGadget->buildCost());
	bool bTechnology = (pPlanet != NULL && pPlanet->technology() >= m_pNewGadget->buildTechnology());
	bool bBlockade = (pPlanet != NULL && (pPlanet->flags() & NounPlanet::FLAG_BLOCKADE) != 0);

	CharString sTip;
	sTip += CharString().format("<color;ffffff>Refit with %s\n<color;808080>%s\n", m_pNewGadget->name(), m_pNewGadget->nounContext()->description() );
	sTip += CharString().format("\n<color;%s>%d Resources", bResources ? "ffffff" : "0000ff", m_pNewGadget->buildCost());
	sTip += CharString().format("\n<color;%s>%d Technology", bTechnology ? "ffffff" : "0000ff", m_pNewGadget->buildTechnology());
	if ( bBlockade )
		sTip += "\n<color;0000ff>BLOCKADE";

	dword nFlags = m_pNewGadget->buildFlags();
	for(int i=0;nFlags != 0 && i<32;i++)
	{
		dword nBit = 1 << i;
		if ( (nFlags & nBit) != 0 )
		{
			sTip += CharString().format("\n<color;%s>%s", 
				(pPlanet != NULL && (pPlanet->flags() & nBit)) ? "ffffff" : "0000ff", NounPlanet::flagName( nBit ) );
		}
	}

	//setIcon( m_pNewGadget->icon() );
	setTip( sTip );
	setName( m_pNewGadget->name() );
	setEnable( true );
	setIconColor( bCanRefit ? GREEN : RED );
}


void ButtonRefit::onRender( RenderContext & context, const RectInt & window )
{
	WindowButton::onRender( context, window );

	if ( m_pNewGadget.valid() )
	{
		// draw the gadget icon
		Material * pIcon = m_pNewGadget->icon();
		if ( pIcon != NULL )
		{
			const RectFloat WINDOW_UV(0,0,1,1);
			const RectInt ICONBOX( PointInt( window.left, window.top ), SizeInt( 32, 16 ) );

			Material::push( context, pIcon );
			PrimitiveWindow::push( context.display(), ICONBOX, WINDOW_UV, iconColor() );
		}
	}
}

void ButtonRefit::onButtonUp()
{
	WindowButton::onButtonUp();

	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );

	if ( VerbRefitGadget::canRefit( m_pGadget, m_pNewGadget ) )
		Verb::Ref( new VerbRefitGadget( m_pGadget, m_pNewGadget ) );
	else
		pDoc->client()->pushChat( CharString().format("<color;8080ff>Unable to refit %s!", m_pGadget->name()) );
}

void ButtonRefit::setButton( NounGadget * pGadget, NounGadget * pNewGadget )
{
	m_pGadget = pGadget;
	m_pNewGadget = pNewGadget;
	onUpdate( 0.0f );
}

//----------------------------------------------------------------------------
//EOF
