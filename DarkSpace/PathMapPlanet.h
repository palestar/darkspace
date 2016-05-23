/*
	PathMapPlanet.h

	PathMap implementation for planets
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef PATHMAPPLANET_H
#define PATHMAPPLANET_H

#include "Render3D/PathMap.h"

//----------------------------------------------------------------------------

class PathMapPlanet : public PathMap
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// BaseNode interface
	void			onAttached();
};



#endif

//----------------------------------------------------------------------------
//EOF
