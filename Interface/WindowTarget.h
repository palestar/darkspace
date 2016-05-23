/*
	WindowTarget.h

	This window object is used to render the players current target object.
	(c)2000 Palestar, Richard Lyle
*/

#ifndef WINDOW_TARGET_H
#define WINDOW_TARGET_H

#include "Gui3d/NodeWindow.h"
#include "World/Noun.h"

//----------------------------------------------------------------------------

class WindowTarget : public NodeWindow
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	WindowTarget();
	// NodeWindow interface
	void				onRender( RenderContext & context,
							const RectInt & window );		// client render, window is in screen coordinates
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
