/*
	ButtonUnit.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Interface/ButtonUnit.h"
#include "Interface/GameDocument.h"

//----------------------------------------------------------------------------

ButtonUnit::ButtonUnit()
{
	m_Flags |= NOCLIP;
	m_Style = HAS_BACK | EFFECT_HIGHLIGHT | EFFECT_FADEIN | SMOOTH_LL;
	m_Alpha = 0.75f;

	setWindow( RectInt( PointInt(0,0), SizeInt( 32, 24 ) ) );
	setEnable( false );
}

//----------------------------------------------------------------------------

void ButtonUnit::onUpdate( float t )
{
	WindowButton::onUpdate( t );
}

void ButtonUnit::onRender( RenderContext & context, const RectInt & window )
{
	WindowButton::onRender( context, window );

	if ( enabled() && m_Unit.valid() )
	{
		GameDocument * pDoc = (GameDocument *)document();
		ASSERT( pDoc );
		NounShip * pShip = pDoc->ship();
		if (! pShip )
			return;
		DisplayDevice * pDisplay = context.display();
		ASSERT( pDisplay );

		// draw the gadget icon
		if ( m_Icon.valid() )
		{
			RectInt iconBox( PointInt( window.left, window.top ), SizeInt( 32, 16 ) );
			RectFloat iconUV(0,0,1,1);

			Material::push( context, m_Icon );
			PrimitiveWindow::push( pDisplay, iconBox, iconUV, 
				pShip->isFriend( m_Unit ) ? GREEN : pShip->isEnemy( m_Unit ) ? RED : YELLOW );
		}

		// draw the unit health on the button
		Font * pFont = windowStyle()->font();
		ASSERT( pFont );

		WideString sHealth;
		sHealth.format( STR("%d%%"), 100 - ((m_Unit->damage() * 100) / m_Unit->maxDamage()) );

		SizeInt szHealth( pFont->size( sHealth ) );
		PointInt ptHealth( window.right - szHealth.width, window.bottom - szHealth.height );
		Font::push( context.display(), pFont, ptHealth, sHealth, WHITE );

		// display damage bar
		if ( m_Unit->damage() > 0 )
		{
			float damage = 1.0f - (m_Unit->damage() / m_Unit->maxDamage());
			RectInt bar( window.m_Left, window.m_Bottom + 1, 
				window.m_Right - (window.width() * (1.0f - damage)), window.m_Bottom + 3 );
			RectFloat barUV(0,0,1,1);

			Color barColor( 255 * (1.0f - damage), 255 * damage,0,255 );
			PrimitiveMaterial::push( pDisplay, PrimitiveMaterial::NONE );
			PrimitiveWindow::push( pDisplay, bar, barUV, barColor );
		}

		// display blinking border if this unit is the current target
		if ( pDoc->target() == m_Unit && (pDoc->tick() % 10) < 6)
			renderGlow( context );
	}
}

void ButtonUnit::onButtonUp()
{
	WindowButton::onButtonUp();

	if ( enabled() && m_Unit.valid() )
		((GameDocument *)document())->setTarget( m_Unit );
}

void ButtonUnit::setButton( NounUnit * pUnit )
{
	m_Unit = pUnit;
	if ( m_Unit.valid() )
	{
		m_Icon = pUnit->icon();

		setTip( CharString().format("%s\n%s", pUnit->name(), pUnit->status()) );
		setEnable( true );
	}
	else
		setEnable( false );
}

//----------------------------------------------------------------------------
//EOF
