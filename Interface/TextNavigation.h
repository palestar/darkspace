/*
	TextNavigation.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef TEXTNAVIGATION_H
#define TEXTNAVIGATION_H

#include "Gui3d/WindowText.h"



//----------------------------------------------------------------------------

class TextNavigation : public WindowText
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	TextNavigation();
	// NodeInterfaceClient interface
	void				onActivate();
	void				onUpdate( float t );

private:
	// Data
	float				m_fUpdateText;
};

//----------------------------------------------------------------------------




#endif

//----------------------------------------------------------------------------
//EOF
