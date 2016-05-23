/*
	SliderHUD.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef SLIDERHUD_H
#define SLIDERHUD_H

#include "Gui3d/WindowSlider.h"

//----------------------------------------------------------------------------

class SliderHUD : public WindowSlider
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	enum Constants
	{
		TICK_SIZE	= 3,		
		TICK_SPACE	= 1,
		TICK_WIDTH	= TICK_SIZE + TICK_SPACE,
		TICK_INDENT	= 2,
		MARKER_WIDTH	= 4,
	};

	// Construction
						SliderHUD();
	// NodeWindow interface
	void				onRender( RenderContext & context,
							const RectInt & window );		// client render, window is in screen coordinates
	// Accessors
	float				position2() const;
	// Mutators
	void				setPosition2( float position );

private:
	float				m_Position2;
};

//----------------------------------------------------------------------------

inline float SliderHUD::position2() const
{
	return m_Position2;
}

inline void SliderHUD::setPosition2( float position )
{
	m_Position2 = position;
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
