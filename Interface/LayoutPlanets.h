/*
	LayoutPlanets.h

	This window displays the closest friendly planets to the players current focus position
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef LAYOUTPLANETS_H
#define LAYOUTPLANETS_H

#include "Gui3d/WindowLayout.h"



//----------------------------------------------------------------------------

class LayoutPlanets : public WindowLayout
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	LayoutPlanets();

	// NodeInterfaceClient interface
	void				onActivate();
	void				onDeactivate();		
	void				onUpdate( float t );

private:
	// Data
	Noun::wRef			m_Focus;
};

//----------------------------------------------------------------------------




#endif

//----------------------------------------------------------------------------
//EOF
