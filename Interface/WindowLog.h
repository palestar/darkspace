/*
	WindowLog.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef WINDOWLOG_H
#define WINDOWLOG_H

#include "Gui3d/WindowText.h"

//----------------------------------------------------------------------------

class WindowLog : public WindowText
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
						WindowLog();
	// NodeInterfaceClient interface
	void				onUpdate( float t );
private:
	// Data
	int					m_LastChatCount;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
