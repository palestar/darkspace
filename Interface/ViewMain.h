/*
	ViewMain.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef VIEWMAIN_H
#define VIEWMAIN_H

#include "System/Messages.h"
#include "Gui3d/WindowView.h"

//----------------------------------------------------------------------------

class ViewMain : public WindowView::View
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef Reference< ViewMain >	Ref;

	// Construction
						ViewMain();
	// View interface
	void				onActivate();
	void				onDeactivate();
	void				onUpdate( float t );
	bool				onMessage( const Message & msg );

	// {{BEGIN_DATA
	WindowButton *				m_pButtonPlay;
	WindowButton *				m_pButtonModerate;
	NodeWindow *				m_pObserveWindow;
	WindowText *				m_pUser;
	WindowText *				m_pMOTD;
	// END_DATA}}

	// {{BEGIN_MSG
	bool				onButtonOptions(const Message & msg);
	bool				onButtonModerate(const Message & msg);
	bool				onButtonObserve(const Message & msg);
	bool				onButtonCredits(const Message & msg);
	bool				onButtonExit(const Message & msg);
	bool				onButtonPlay(const Message & msg);
	// END_MSG}}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
