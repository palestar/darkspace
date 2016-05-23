/*
	LayoutPlanetGadgets.h

	This window displays all the gadgets found on the current planetary target that can be bought
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef LAYOUTPLANETGADGETS_H
#define LAYOUTPLANETGADGETS_H

#include "Gui3d/WindowLayout.h"



//----------------------------------------------------------------------------

class LayoutPlanetGadgets : public WindowLayout
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	LayoutPlanetGadgets();

	// NodeInterfaceClient interface
	void				onActivate();
	void				onDeactivate();		
	void				onUpdate( float t );

private:
	// Data
	NounPlanet::wRef	m_Planet;
};

//----------------------------------------------------------------------------




#endif

//----------------------------------------------------------------------------
//EOF
