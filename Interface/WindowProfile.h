/*
	WindowProfile.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef WINDOWPROFILE_H
#define WINDOWPROFILE_H

#include "Gui3d/WindowText.h"

//----------------------------------------------------------------------------

class WindowProfile : public WindowText
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
						WindowProfile();
	// NodeWindow interface
	void				onUpdate( float t );
	bool				onRightDown( const RectInt & window,
							const PointInt & position );
};


//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
