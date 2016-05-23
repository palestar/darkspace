/*
	ViewRedirect.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef VIEWREDIRECT_H
#define VIEWREDIRECT_H

#include "System/Messages.h"
#include "Gui3d/WindowView.h"



//----------------------------------------------------------------------------

class ViewRedirect : public WindowView::View
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef Reference< ViewRedirect >	Ref;

	// Construction
						ViewRedirect();
	// View interface
	void				onActivate();
	void				onDeactivate();
	void				onUpdate( float t );
	bool				onMessage( const Message & msg );
	void				onDocumentUpdate();

	void				onRender( RenderContext & context, 
							const RectInt & window );
	// {{BEGIN_DATA
	// END_DATA}}

	// {{BEGIN_MSG
	// END_MSG}}

	Scene::Ref			m_Scenery;
	Noun::Ref			m_Ship;
	float				m_Time;
	Vector3				m_CameraPosition;
	Matrix33			m_CameraFrame;
	Vector3				m_ShipPosition;
	Vector3				m_ShipVelocity;
	bool				m_InJump;

	bool				initializeScene();
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
