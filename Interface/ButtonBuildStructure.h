/*
	ButtonBuildStructure.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef BUTTONBUILDSTRUCTURE_H
#define BUTTONBUILDSTRUCTURE_H

#include "Gui3D/WindowButton.h"
#include "DarkSpace/NounDropTarget.h"
#include "DarkSpace/NounStructure.h"

//----------------------------------------------------------------------------

class ButtonBuildStructure : public WindowButton
{
public:
	// Construction
	ButtonBuildStructure();

	// NodeWindow interface
	void				onUpdate( float t );
	void				onRender( RenderContext & context,
							const RectInt & window );		// client render, window is in screen coordinates
	// WindowButton interface
	void				onButtonUp();
	// Mutators
	bool				setButton( Noun * pTarget, NounStructure * pBuild );

private:
	Noun::Ref			m_Target;
	NounStructure::wRef	m_Build;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
