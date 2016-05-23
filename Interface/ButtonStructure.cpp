/*
	ButtonStructure.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Interface/ButtonStructure.h"
#include "Interface/GameDocument.h"

//----------------------------------------------------------------------------

ButtonStructure::ButtonStructure()
{
	m_Flags |= NOCLIP;
	m_Style = HAS_BACK | EFFECT_HIGHLIGHT | EFFECT_FADEIN | SMOOTH_LL | LOCK_ICON_SIZE;
	m_Alpha = 0.75f;

	setWindow( RectInt( PointInt(0,0), SizeInt( 32, 24 ) ) );
	setEnable( false );
}

//----------------------------------------------------------------------------

void ButtonStructure::onUpdate( float t )
{
	WindowButton::onUpdate( t );
}

void ButtonStructure::onRender( RenderContext & context, const RectInt & window )
{
	WindowButton::onRender( context, window );

	if ( enabled() && m_Structure.valid() )
	{
		GameDocument * pDoc = (GameDocument *)document();
		ASSERT( pDoc );
		DisplayDevice * pDisplay = context.display();
		ASSERT( pDisplay );
		Font * pFont = windowStyle()->font();
		ASSERT( pFont );

		// show workers / power
		WideString status;
		status.format(STR("%d/%d"), m_Structure->workers(), m_Structure->power() );

		SizeInt statusSize( pFont->size( status ) );
		PointInt statusPos( window.right - statusSize.width, window.bottom - statusSize.height );

		Font::push( context.display(), pFont, statusPos, status, WHITE );

		// display damage bar
		if ( m_Structure->damage() > 0 )
		{
			float damage = 1.0f - (m_Structure->damage() / m_Structure->maxDamage());
			RectInt bar( window.m_Left, window.m_Bottom + 1, 
				window.m_Right - (window.width() * (1.0f - damage)), window.m_Bottom + 3 );
			RectFloat barUV(0,0,1,1);

			Color barColor( 255 * (1.0f - damage), 255 * damage,0,255 );
			PrimitiveMaterial::push( pDisplay, PrimitiveMaterial::NONE );
			PrimitiveWindow::push( pDisplay, bar, barUV, barColor );
		}

		// place blinking glow border if this structure is the current target
		if ( pDoc->target() == m_Structure && fmod( activeTime(), 1.0f ) < 0.5f )
			renderGlow( context );
	}
}

void ButtonStructure::onButtonUp()
{
	WindowButton::onButtonUp();

	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );

	// target the structure
	if ( m_Structure.valid() )
		pDoc->setTarget( m_Structure );
}

void ButtonStructure::setButton( NounStructure * pStructure )
{
	m_Structure = pStructure;
	if ( m_Structure.valid() )
	{
		setIcon( m_Structure->icon() );
		setIconColor( m_Structure->active() ? WHITE : RED );
		setTip( m_Structure->name() );
		setEnable( true );
	}
	else
	{
		setEnable( false );
	}
}

//----------------------------------------------------------------------------
//EOF
