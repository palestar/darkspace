/*
	ViewEndGame.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef VIEWENDGAME_H
#define VIEWENDGAME_H

#include "System/Messages.h"
#include "Gui3d/WindowView.h"



//----------------------------------------------------------------------------

class ViewEndGame : public WindowView::View
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef Reference< ViewEndGame >	Ref;

	// Construction
						ViewEndGame();
	// View interface
	void				onActivate();
	void				onDeactivate();
	void				onUpdate( float t );
	bool				onMessage( const Message & msg );
	void				onDocumentUpdate();

	// {{BEGIN_DATA
	WindowText *				m_pMessage;
	// END_DATA}}

	// {{BEGIN_MSG
	bool				onButtonQuit(const Message & msg);
	bool				onButtonContinue(const Message & msg);
	// END_MSG}}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
