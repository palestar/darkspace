/*
	ViewLobby.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef VIEWLOBBY_H
#define VIEWLOBBY_H

#include "System/Messages.h"
#include "Gui3d/WindowView.h"



//----------------------------------------------------------------------------

class ViewLobby : public WindowView::View
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef Reference< ViewLobby >	Ref;

	// Construction
	ViewLobby();

	// View interface
	void				onActivate();
	void				onDeactivate();
	void				onUpdate( float t );
	bool				onMessage( const Message & msg );
	void				onDocumentUpdate();

	// {{BEGIN_DATA
	// END_DATA}}

	// {{BEGIN_MSG
	// END_MSG}}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
