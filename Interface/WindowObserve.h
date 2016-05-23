/*
	WindowObserve.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef WINDOWOBSERVE_H
#define WINDOWOBSERVE_H

#include "Gui3d/NodeWindow.h"
#include "World/Noun.h"



//----------------------------------------------------------------------------

class WindowObserve : public NodeWindow
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	enum CameraMode
	{
		FOLLOW,			// follow target
		FLYBY,			// lead target
		ISOMETRIC,		// isometric mode
		CHASE,			// chase mode
	};
	enum TargetMode
	{
		LOCKED,			// locked on to specified target
		AUTO,			// automatically find interesting targets
		AUTO_TEAM,		// find other targets on the same team
		AUTO_BATTLE,	// automatically find battles between ships
		AUTO_PLANET,	// automatically view different planets
		AUTO_SHIP,	
	};
	
	// Construction
						WindowObserve();
	// NodeInterfaceClient interface
	void				onActivate();
	void				onDeactivate();
	bool				onMessage( const Message & msg );
	// NodeWindow interface
	void				onUpdate( float t );				// update this window
	void				onRender( RenderContext & context,
							const RectInt & window );		// client render, window is in screen coordinates
	bool				onCursorMove( const RectInt & window, 
							const PointInt & position,		// window and position are local coordinates
							const PointInt & delta );
	bool				onRightDown( const RectInt & window,
							const PointInt & position );
	bool				onRightUp( const RectInt & window,
							const PointInt & position );

	// Accessors
	float				lingerTime() const;
	CameraMode			cameraMode() const;
	float				cameraDistance() const;					// camera distance, actual distance = (target radius * distance)

	TargetMode			targetMode() const;
	Noun *				target() const;

	// Mutators
	void				setCameraMode( CameraMode mode );		// set camera mode
	void				setCameraDistance( float distance );	// set distance from target
	void				setTargetMode( TargetMode mode );
	void				setTarget( Noun * pTarget );			// set the camera target
	void				setLingerTime( float time );			// how long to view a target in auto mode

	void				nextTarget();
	void				prevTarget();

private:
	float				m_AutoTime;								// time until we switch to a new target 
	float				m_LingerTime;							// how long to view a target in auto mode

	CameraMode			m_CameraMode;
	float				m_CameraDistance;
	bool				m_CameraControl;

	TargetMode			m_TargetMode;							// set target mode
	Noun::wRef			m_CameraTarget;
	Array< Noun::wRef >	m_Targets;								// sorted array of all objects

	void				updateTargets();
	void				findNextTarget();

};

IMPLEMENT_RAW_STREAMING( WindowObserve::CameraMode );
IMPLEMENT_RAW_STREAMING( WindowObserve::TargetMode );

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
