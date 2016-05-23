/*
	TextPlanet.h

	This text window displays details on the currently targeted planet
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef TEXTPLANET_H
#define TEXTPLANET_H

#include "Gui3d/WindowText.h"



//----------------------------------------------------------------------------

class TextPlanet : public WindowText
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	TextPlanet();
	// NodeInterfaceClient interface
	void				onActivate();
	void				onUpdate( float t );

private:
	// Data
	NounPlanet::wRef	m_Planet;
};

//----------------------------------------------------------------------------




#endif

//----------------------------------------------------------------------------
//EOF
