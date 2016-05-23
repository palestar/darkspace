/*
	LayoutCargo.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef LAYOUTCARGO_H
#define LAYOUTCARGO_H

#include "Gui3d/WindowLayout.h"

//----------------------------------------------------------------------------

class LayoutCargo : public WindowLayout
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
						LayoutCargo();
	// NodeInterfaceClient interface
	void				onActivate();
	void				onDeactivate();		
	void				onUpdate( float t );
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
