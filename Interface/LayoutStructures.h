/*
	LayoutStructures.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef LAYOUTSTRUCTURES_H
#define LAYOUTSTRUCTURES_H

#include "Gui3d/WindowLayout.h"



//----------------------------------------------------------------------------

class LayoutStructures : public WindowLayout
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	LayoutStructures();

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

