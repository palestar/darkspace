/*
	ButtonBuildPlatform.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef BUTTONBUILDPLATFORM_H
#define BUTTONBUILDPLATFORM_H

#include "Gui3D/WindowButton.h"
#include "DarkSpace/NounDropTarget.h"
#include "DarkSpace/ShipPlatform.h"

//----------------------------------------------------------------------------

class ButtonBuildPlatform : public WindowButton
{
public:
	// Construction
	ButtonBuildPlatform();

	// NodeWindow interface
	void				onUpdate( float t );
	void				onRender( RenderContext & context,
							const RectInt & window );		// client render, window is in screen coordinates
	// WindowButton interface
	void				onButtonUp();
	// Mutators
	void				setButton( NounZoneTarget * pTarget, ShipPlatform * pBuild );

private:
	NounZoneTarget::Ref	m_Target;
	ShipPlatform::wRef	m_Build;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
