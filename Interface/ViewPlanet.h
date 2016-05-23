/*
	ViewPlanet.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef VIEWPLANET_H
#define VIEWPLANET_H

#include "System/Messages.h"
#include "Gui3d/WindowView.h"
#include "DarkSpace/NounDropTarget.h"
#include "DarkSpace/GameContext.h"

//----------------------------------------------------------------------------

class ViewPlanet : public WindowView::View
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef Reference< ViewPlanet >	Ref;

	// Construction
	ViewPlanet();

	// View interface
	void				onActivate();
	void				onDeactivate();
	void				onHide();
	void				onUpdate( float t );
	bool				onMessage( const Message & msg );

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
	bool				onButtonResearchAll(const Message & msg);
	bool				onButtonStopResearch(const Message & msg);
	// END_MSG}}

	// Data
	float				m_Update;
	NounPlanet::wRef	m_Planet;
	
	PointInt			m_CursorPosition2;	// current cursor position
	bool				m_bLeftDown;
	float				m_fLeftDownTime;
	bool				m_bRightDown;
	float				m_fRightDownTime;

	float				m_CameraYaw;
	float				m_CameraPitch;

	Noun::wRef			m_CameraLock;		// camera is locked onto which object
	float				m_CameraLockTime;

	Array< Noun::wRef >	
						m_Contacts;			// current contacts
	float				m_UpdateContacts;
	Array< NounDropTarget::Ref >
						m_DropTargets;		// drop targets for our planet

	// Mutators
	void				updateDropTargets();
	void				updateContacts();
	void				updateCamera( float t );
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
