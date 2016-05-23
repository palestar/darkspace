/*
	WindowUniverse.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef WINDOWUNIVERSE_H
#define WINDOWUNIVERSE_H

#include "Gui3d/NodeWindow.h"
#include "Game/GameServer.h"



//----------------------------------------------------------------------------

class WindowUniverse : public NodeWindow
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
						WindowUniverse();
	// NodeWindow interface
	void				onUpdate( float t );				// update this window
	void				onRender( RenderContext & context,
							const RectInt & window );		// client render, window is in screen coordinates
	bool				onCursorMove( const RectInt & window, 
							const PointInt & position,		// window and position are local coordinates
							const PointInt & delta );
	bool				onRightDown( const RectInt & window,
							const PointInt & position );
	bool				onLeftDown( const RectInt & window,
							const PointInt & position );

	int					renderPath( RenderContext & context,
							int src, int dst, Color color );	// render path, returns the next system from the src

private:
	float				m_Time;
	PointInt			m_CursorPosition;
	WidgetKey			m_Destination;
	WidgetKey			m_Highlighted;
	NounJumpGate::wRef	m_NextJump;

	float				m_Extents;
	Array< WidgetKey >	m_Context;
	Array< Color >		m_Colors;
	Array< String >		m_Name;								// system name
	Array< String >		m_Labels;
	Array< Vector3 >	m_Position;							// 3d positions
	Array< PointInt >	m_Position2;						// 2d positions
	Hash< int, Array< int > >	
						m_Links;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
