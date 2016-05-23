/*
	WindowHUD.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef WINDOWHUD_H
#define WINDOWHUD_H

#include "Gui3d/NodeWindow.h"



//----------------------------------------------------------------------------

class WindowHUD : public NodeWindow
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	enum Flags
	{
		SMOOTH_UL		= NodeWindow::USER1,			// smooth upper-left corner
		SMOOTH_UR		= NodeWindow::USER2,			// upper-right
		SMOOTH_LR		= NodeWindow::USER3,			// lower-right
		SMOOTH_LL		= NodeWindow::USER4,			// lower-left
		ANIM_BEAM		= NodeWindow::USER5,			// enable beam animation
		ANIM_BACK		= NodeWindow::USER6,			// animate background
		ANIM_FLARE		= NodeWindow::USER7,			// flare background color
	};

	// Construction
						WindowHUD();
	// NodeWindow interface
	void				onUpdate( float t );
	void				onRender( RenderContext & context,
							const RectInt & window );		// client render, window is in screen coordinates
private:

	// non-serialized
	float				m_ActionTime;
	PointFloat			m_MapOffset;
	PointFloat			m_MapVelocity;
	float				m_Flare;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
