/**
	@file WindowShipSelection.h
	@brief This window will render the current ship selection.
	
	(c)2009 Palestar Inc
	@author Richard Lyle @date 9/17/2010 12:23:18 PM
*/

#ifndef WINDOWSHIPSELECTION_H
#define WINDOWSHIPSELECTION_H

#include "Render3D/NodeSpin.h"
#include "Gui3d/NodeWindow.h"
#include "DarkSpace/NounShip.h"

//---------------------------------------------------------------------------------------------------

class WindowShipSelection : public NodeWindow
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef Reference< WindowShipSelection >		Ref;

	// Construction
	WindowShipSelection();
	// NodeWindow interface
	void				onUpdate( float t );
	void				onRender( RenderContext & context,
							const RectInt & window );		// client render, window is in screen coordinates

	void				setSelectedShip( NounShip * pShip );

protected:
	//! Data
	NodeSpin::Ref		m_ShipSpin;
	float				m_ShipRadius;
	float				m_ShipZoom;
};

#endif

//---------------------------------------------------------------------------------------------------
//EOF
