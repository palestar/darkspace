/*
	LayoutPlanetUnits.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef LAYOUTPLANETUNITS_H
#define LAYOUTPLANETUNITS_H

#include "Gui3d/WindowLayout.h"



//----------------------------------------------------------------------------

class LayoutPlanetUnits : public WindowLayout
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	LayoutPlanetUnits();

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
