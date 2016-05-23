/*
	LayoutMessages.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef LAYOUTMESSAGES_H
#define LAYOUTMESSAGES_H

#include "Gui3d/WindowLayout.h"



//----------------------------------------------------------------------------

class LayoutMessages : public WindowLayout
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
						LayoutMessages();
	// NodeInterfaceClient interface
	void				onActivate();
	void				onDeactivate();
	void				onUpdate( float t );

private:
	// Data
	int					m_LastChatCount;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
