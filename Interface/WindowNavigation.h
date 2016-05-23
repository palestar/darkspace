/*
	WindowNavigation.h

	This window class displays a map of the universe, it is used by ViewNavigation and ViewSelectShip
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef WINDOWNAVIGATION_H
#define WINDOWNAVIGATION_H

#include "Gui3d/NodeWindow.h"
#include "World/Noun.h"
#include "World/NounZoneTarget.h"
#include "Interface/GameDocument.h"


class ViewGame;

//----------------------------------------------------------------------------

class WindowNavigation : public NodeWindow
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	enum SelectMode
	{
		SELECT_ALL,
		SELECT_SPAWN,
	};
	enum ShowFlags
	{
		SHOW_GRID		= 0x1,
		SHOW_PLANETS	= 0x2,
		SHOW_ORBITS		= 0x4,
		SHOW_SHIPS		= 0x8,
		SHOW_ORDERS		= 0x10,
		SHOW_TRADE		= 0x20,
		SHOW_SPAWN		= 0x40,
		SHOW_ENEMY		= 0x80,
		SHOW_LINKS		= 0x100,
		SHOW_AI			= 0x200,
	};

	// Construction
	WindowNavigation();

	// NodeInterfaceClient interface
	void				onActivate();
	void				onDeactivate();
	bool				onMessage( const Message & msg );
	// NodeWindow interface
	void				onHide();
	void				onUpdate( float t );				// update this window
	void				onRender( RenderContext & context,
							const RectInt & window );		// client render, window is in screen coordinates
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

	// Accessors
	SelectMode			selectMode() const;
	dword				showFlags() const;
	
	// Mutators
	void				setSelectMode( SelectMode nMode );
	void				setShowFlags( dword nFlags );
	void				flipShowFlag( dword nFlag );

	void				onCenter();							// centers the map on the current target, if none then the players ship

	void				beginZoom( bool bZoomIn );			// begin zooming in or out
	void				endZoom();
	void				onFocus();							// zooms all the way in on the current target
	void				onFull();							// zooms out all the way

	// static
	static Vector3		sm_vCameraTarget;
	static bool			sm_bStickyCursorTarget;				// if true, then cursor target will stay unless something is closer

protected:
	// Data
	SelectMode			m_nSelectMode;		// current window mode
	dword				m_nShowFlags;		// display flags

	GameDocument *		m_pDoc;
	float				m_fUniverseSize;	// size of the entire universe
	float				m_fSystemSize;		// size of the current system

	PrimitiveTriangleListL::Ref
						m_Grid;				// the grid

	PointInt			m_pCursorPosition;	// current cursor position
	Vector3				m_vCursorPosition;	// 3d position of the cursor
	float				m_fCameraYaw;
	float				m_fCameraPitch;

	bool				m_bLeftDown;		// moving camera
	float				m_fLeftDownTime;	// how long has the left mouse been down
	bool				m_bRightDown;		// rotating camera
	float				m_fRightDownTime;	// how long has the right mouse
	Vector3				m_vCameraZoom;		// zoom of the camera

	float				m_fUpdateIntelligence;
	GameContext::Ref	m_pContext;			// our context
	Queue< Noun::wRef >	
						m_Objects;			// all object that are displayed
	Array< Noun::wRef >	
						m_Objectives;		// list of objectives

	NounZoneTarget::Ref	m_pManualTarget;

	Noun::wRef			m_pLastTarget;
	float				m_fViewDistance;
	float				m_fTargetTime;

	// Mutators
	void				gatherIntelligence();
	void				pushDetected( WorldContext * pWorld );
	Color				getControlColor( const Vector3 & position );
	Material *			getContactIcon( Noun * pContact );
	Material *			findContactIcon( Noun * pContact );
	void				buildGrid( DisplayDevice * pDisplay );

};

//----------------------------------------------------------------------------

inline WindowNavigation::SelectMode WindowNavigation::selectMode() const
{
	return m_nSelectMode;
}

inline dword WindowNavigation::showFlags() const
{
	return m_nShowFlags;
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
