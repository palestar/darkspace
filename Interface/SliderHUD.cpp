/*
	SliderHUD.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Display/PrimitiveWindow.h"
#include "Display/PrimitiveMaterial.h"
#include "Display/PrimitiveLineStrip.h"
#include "Interface/SliderHUD.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( SliderHUD, WindowSlider );
REGISTER_FACTORY_KEY( SliderHUD, 4212081484604201331 );

SliderHUD::SliderHUD()
{
	m_Position2 = 0.0f;
}

//----------------------------------------------------------------------------

void SliderHUD::onRender( RenderContext & context, const RectInt & window )
{
	const RectFloat windowUV(0,0,1,1);

	PrimitiveMaterial::push( context.display(), PrimitiveMaterial::ALPHA );

	// horizontal bar
	int		ticks = window.width() / TICK_WIDTH;
	float	p = 0.0f;
	float	perTick = 1.0f / ticks;

	int		x = 0;
	for(int i=0;i<ticks;i++)
	{
		Color tickColor( m_BarColor );

		if ( p > m_Position2 )
			tickColor = m_BaseColor;
		else if ( (p + perTick) > m_Position2 )
			tickColor.iterpolate( m_BaseColor, 1.0f - ((m_Position2 - p) / perTick) );

		int indent = 0;
		if ( (i % 5) != 0 )
			indent = TICK_INDENT;

		PrimitiveWindow::push( context.display(), 
			RectInt( window.left + x, window.top + indent, window.left + x + TICK_SIZE, window.bottom - indent), 
			windowUV, tickColor );

		x += TICK_WIDTH;
		p += perTick;
	}

	// draw marker at current position
	PointInt markerCenter( window.left + ((m_Position / perTick) * TICK_WIDTH), (window.top + window.bottom) / 2 );
	int markerHeight = window.height() / 2;

	// get the marker color
	Color markerColor( m_BarColor * 2.0f );
	markerColor.m_A = 255;

	// draw lines on the outside of the beam
	VertexTL marker[ 4 ];
	marker[ 0 ] = VertexTL( Vector3( markerCenter.x, markerCenter.y - markerHeight, 0 ), 0.0f, markerColor, 0, 0 );
	marker[ 1 ] = VertexTL( Vector3( markerCenter.x + MARKER_WIDTH, markerCenter.y + markerHeight, 0), 0, markerColor, 0, 0 );
	marker[ 2 ] = VertexTL( Vector3( markerCenter.x - MARKER_WIDTH, markerCenter.y + markerHeight, 0), 0, markerColor, 0, 0 );
	marker[ 3 ] = marker[ 0 ];

	PrimitiveLineStripDTL::push( context.display(), 4, marker );
}


//----------------------------------------------------------------------------
//EOF
