/*
	ButtonUnload.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "DarkSpace/VerbUnload.h"
#include "DarkSpace/NounUnit.h"
#include "DarkSpace/CargoGadget.h"
#include "Interface/GameDocument.h"
#include "Interface/ButtonUnload.h"

//----------------------------------------------------------------------------

ButtonUnload::ButtonUnload()
{
	m_Flags |= NOCLIP;
	m_Style = HAS_BACK | EFFECT_HIGHLIGHT | EFFECT_FADEIN | SMOOTH_LL | USER1;
	m_Alpha = 0.75f;
	m_HotKey = 'U';

	setWindow( RectInt( PointInt(0,0), SizeInt( 32, 24 ) ) );
	setEnable( false );
}

//----------------------------------------------------------------------------

void ButtonUnload::onRender( RenderContext & context, const RectInt & window )
{
	WindowButton::onRender( context, window );

	if ( enabled() )
	{
		DisplayDevice * pDisplay = context.display();
		ASSERT( pDisplay );

		Noun * pCargo = m_Cargo;
		if ( WidgetCast<NounCargo>( pCargo ) )
		{
			Font * pFont = windowStyle()->font();
			ASSERT( pFont );

			WideString sQuantity;
			sQuantity.format( STR("%d"), ((NounCargo *)pCargo)->quantity() );

			Font::push( context.display(), pFont, PointInt( window.m_Right - pFont->size( sQuantity ).width, window.top ), 
				sQuantity, WHITE );

			if ( ((NounCargo *)pCargo)->quantity() <= 0 )
				destroy();
		}
		else if ( WidgetCast<NounUnit>( pCargo ) )
		{
			NounUnit * pUnit = (NounUnit *)pCargo;

			// display damage bar
			if ( pUnit->damage() > 0 )
			{
				float damage = 1.0f - (pUnit->damage() / pUnit->maxDamage());
				RectInt bar( window.m_Left, window.m_Bottom + 1, 
					window.m_Right - (window.width() * (1.0f - damage)), window.m_Bottom + 3 );
				RectFloat barUV(0,0,1,1);

				Color barColor( 255 * (1.0f - damage), 255 * damage,0,255 );
				PrimitiveMaterial::push( pDisplay, PrimitiveMaterial::NONE );
				PrimitiveWindow::push( pDisplay, bar, barUV, barColor );
			}
		}
	}
}

bool ButtonUnload::trapKey() const
{
	return true;
}

void ButtonUnload::onButtonUp()
{
	WindowButton::onButtonUp();

	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc && pDoc->ship() );

	pDoc->ship()->verbUnload( m_Target, m_Cargo );
}

void ButtonUnload::setButton( Noun * pTarget, Noun * pCargo )
{
	ASSERT( pTarget && pCargo );

	m_Target = pTarget;
	m_Cargo = pCargo;

	setIcon( pCargo->icon() );
	setTip( CharString().format( "Unload %s onto %s", pCargo->name(), pTarget->name() ) );
	setName( pCargo->name() );
	setEnable( true );
}

//----------------------------------------------------------------------------
//EOF
