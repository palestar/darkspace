/*
	LayoutUnitBuild.h
	
	This window displays the interface for all units that can be built by the current target
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef LAYOUTUNITBUILD_H
#define LAYOUTUNITBUILD_H

#include "Gui3d/WindowLayout.h"



//----------------------------------------------------------------------------

class LayoutUnitBuild : public WindowLayout
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	LayoutUnitBuild();

	// NodeInterfaceClient interface
	void				onActivate();
	void				onDeactivate();		
	void				onUpdate( float t );

private:
	// Data
	Noun::wRef			m_Target;
};

//----------------------------------------------------------------------------




#endif

//----------------------------------------------------------------------------
//EOF
