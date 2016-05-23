/*
	ViewDeath.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef VIEWDEATH_H
#define VIEWDEATH_H

#include "System/Messages.h"
#include "Gui3d/WindowView.h"



//----------------------------------------------------------------------------

class ViewDeath : public WindowView::View
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef Reference< ViewDeath >	Ref;

	// Construction
						ViewDeath();

	// View interface
	void				onActivate();
	void				onDeactivate();
	void				onUpdate( float t );
	bool				onMessage( const Message & msg );
	void				onDocumentUpdate();

	void				onRender( RenderContext & context, 
							const RectInt & window );

	// {{BEGIN_DATA
	NodeWindow *				m_pObserveWindow;
	WindowText *				m_pTextMessage;
	// END_DATA}}

	// {{BEGIN_MSG
	bool				onButtonExit(const Message & msg);
	bool				onButtonOkay(const Message & msg);
	// END_MSG}}

private:
	float				m_CameraYaw;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
