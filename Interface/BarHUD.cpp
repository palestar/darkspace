/*
	BarHUD.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Interface/BarHUD.h"
#include "Display/PrimitiveWindow.h"
#include "Display/PrimitiveMaterial.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( BarHUD, WindowBar );
REGISTER_FACTORY_KEY( BarHUD, 4212062304193304997 );

BarHUD::BarHUD()
{
	m_HeadColor1 = Color( 255, 0, 0 );
	m_HeadColor2 = Color( 255, 255, 0 );
	m_TailColor1 = Color( 255, 0, 0 );
	m_TailColor2 = Color( 0, 0, 255 );
}

//----------------------------------------------------------------------------

void BarHUD::onRender( RenderContext & context, const RectInt & window )
{
	const RectFloat windowUV(0,0,1,1);

	Color headColor( m_HeadColor1 );
	headColor.iterpolate( m_HeadColor2, m_Position );
	Color tailColor( m_TailColor1 );
	tailColor.iterpolate( m_TailColor2, m_Position );

	PrimitiveMaterial::push( context.display(), PrimitiveMaterial::ALPHA );

	// horizontal bar
	int		ticks = window.width() / TICK_WIDTH;
	float	perTick = 1.0f / ticks;
	float	p = 0.0f;
	int		x = 0;

	for(int i=0;i<ticks;i++)
	{
		Color tickColor( headColor );
		tickColor.iterpolate( tailColor, p );

		if ( p > m_Position )
			tickColor.alpha = LOW_ALPHA;
		else if ( (p + perTick) > m_Position )
			tickColor.alpha = LOW_ALPHA + (((m_Position - p) / perTick) * (HIGH_ALPHA - LOW_ALPHA));
		else
			tickColor.alpha = HIGH_ALPHA;

		PrimitiveWindow::push( context.display(), RectInt( window.left + x, window.top, window.left + x + TICK_SIZE, window.bottom),
			windowUV, tickColor );

		x += TICK_WIDTH;
		p += perTick;
	}
}


//----------------------------------------------------------------------------
//EOF
