/*
	WindowMission.h
	(c)2009 Palestar Inc, Jeff Everett

	Window for displaying missions
*/

#ifndef WINDOWMISSION_H
#define WINDOWMISSION_H

#include "Gui3d/NodeWindow.h"

class WindowMission : public NodeWindow
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	WindowMission();

	// NodeInterfaceClient interface
	void				onActivate();
	void				onUpdate( float t );
};



#endif