/*
	TextMission.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef TEXTMISSION_H
#define TEXTMISSION_H

#include "Gui3d/WindowText.h"

//----------------------------------------------------------------------------

class TextMission : public WindowText
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	TextMission();

	// NodeInterfaceClient interface
	void				onActivate();
	void				onUpdate( float t );
};

//----------------------------------------------------------------------------


#endif

//----------------------------------------------------------------------------
//EOF
