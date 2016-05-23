/*
	LayoutGroup.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef LAYOUTGROUP_H
#define LAYOUTGROUP_H

#include "Gui3d/WindowLayout.h"
#include "World/Noun.h"

#include <list>

//----------------------------------------------------------------------------

class LayoutGroup : public WindowLayout
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
						LayoutGroup();
	// NodeInterfaceClient interface
	void				onActivate();
	void				onDeactivate();		
	void				onUpdate( float t );
	// Helpers
	void				keepButtons( int n );		// keep only the first n buttons, remove extra

private:
	// Data
	float				m_Update;
	std::list< Noun::Ref >
						m_DummyObjects;
};

//----------------------------------------------------------------------------




#endif

//----------------------------------------------------------------------------
//EOF
