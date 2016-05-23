/*
	WindowSeperator.h

	This window is used to seperate groups of objects in a layout window
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef WINDOWSEPERATOR_H
#define WINDOWSEPERATOR_H

#include "Gui3D/WindowText.h"
#include "Interface/InterfaceDll.h"

//----------------------------------------------------------------------------

class DLL WindowSeperator : public WindowText
{
public:
	// Construction
	WindowSeperator();
	// Mutators
	void			setSeperator( int size, const char * pText = NULL );
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
