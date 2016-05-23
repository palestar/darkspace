/*
	ViewDisconnected.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef VIEWDISCONNECTED_H
#define VIEWDISCONNECTED_H

#include "System/Messages.h"
#include "Gui3d/WindowView.h"



//----------------------------------------------------------------------------

class ViewDisconnected : public WindowView::View
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef Reference< ViewDisconnected >	Ref;

	// Construction
						ViewDisconnected();
	// View interface
	void				onActivate();
	void				onDeactivate();
	void				onUpdate( float t );
	bool				onMessage( const Message & msg );
	void				onDocumentUpdate();

	// {{BEGIN_DATA
	WindowButton *				m_pButtonReconnect;
	WindowText *				m_pDisconnectMessage;
	// END_DATA}}

	// {{BEGIN_MSG
	bool				onButtonReconnect(const Message & msg);
	bool				onButtonCancel(const Message & msg);
	// END_MSG}}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
