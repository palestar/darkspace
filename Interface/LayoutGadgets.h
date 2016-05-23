/*
	LayoutGadgets.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef LAYOUTGADGETS_H
#define LAYOUTGADGETS_H

#include "Gui3d/WindowLayout.h"



//----------------------------------------------------------------------------

class LayoutGadgets : public WindowLayout
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
						LayoutGadgets();
	// NodeInterfaceClient interface
	void				onActivate();
	void				onDeactivate();							
	void				onUpdate( float t );
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
