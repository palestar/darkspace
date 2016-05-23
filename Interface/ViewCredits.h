/*
	ViewCredits.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef VIEWCREDITS_H
#define VIEWCREDITS_H

#include "System/Messages.h"
#include "Gui3d/WindowView.h"

//----------------------------------------------------------------------------

class ViewCredits : public WindowView::View
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef Reference< ViewCredits >	Ref;

	// Construction
						ViewCredits();
	// View interface
	void				onActivate();
	void				onDeactivate();
	void				onUpdate( float t );
	bool				onMessage( const Message & msg );
	void				onDocumentUpdate();

	void				onRender( RenderContext & context, 
							const RectInt & window );
	bool				onCursorMove( const RectInt & window, 
							const PointInt & position,		// window and position are local coordinates
							const PointInt & delta );
	bool				onLeftDown( const RectInt & window, 
							const PointInt & position );
	bool				onLeftUp( const RectInt & window, 
							const PointInt & position );
	bool				onRightDown( const RectInt & window,
							const PointInt & position );
	bool				onRightUp( const RectInt & window,
							const PointInt & position );

	// {{BEGIN_DATA
	// END_DATA}}

	// {{BEGIN_MSG
	bool				onButtonOkay(const Message & msg);
	// END_MSG}}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
