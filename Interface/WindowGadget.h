/*
	WindowGadget.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef WINDOW_TARGET_H
#define WINDOW_TARGET_H

#include "Gui3d/NodeWindow.h"
#include "World/Noun.h"



//----------------------------------------------------------------------------

class WindowGadget : public NodeWindow
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
						WindowGadget();
	// NodeWindow interface
	void				onRender( RenderContext & context,
							const RectInt & window );		// client render, window is in screen coordinates
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
