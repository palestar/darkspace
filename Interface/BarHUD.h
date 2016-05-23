/*
	BarHUD.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef BARHUD_H
#define BARHUD_H

#include "Gui3d/WindowBar.h"



//----------------------------------------------------------------------------

class BarHUD : public WindowBar
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	enum Constants
	{
		TICK_SIZE	= 5,		
		TICK_SPACE	= 2,
		TICK_WIDTH	= TICK_SIZE + TICK_SPACE,

		HIGH_ALPHA	= 255,
		LOW_ALPHA	= 64,
	};

	// Construction
						BarHUD();
	// NodeWindow interface
	void				onRender( RenderContext & context,
							const RectInt & window );		// client render, window is in screen coordinates
	// Mutators
	void				setHeadColor( Color head1, Color head2 );
	void				setTailColor( Color tail1, Color tail2 );

protected:
	// Data
	Color				m_HeadColor1, m_HeadColor2;
	Color				m_TailColor1, m_TailColor2;
};

//----------------------------------------------------------------------------

inline void BarHUD::setHeadColor( Color head1, Color head2 )
{
	m_HeadColor1 = head1;
	m_HeadColor2 = head2;
}

inline void BarHUD::setTailColor( Color tail1, Color tail2 )
{
	m_TailColor1 = tail1;
	m_TailColor2 = tail2;
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
