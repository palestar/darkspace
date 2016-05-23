/*
	ButtonUnload.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef BUTTONUNLOAD_H
#define BUTTONUNLOAD_H

#include "Gui3D/WindowButton.h"
#include "World/Noun.h"



//----------------------------------------------------------------------------

class ButtonUnload : public WindowButton
{
public:
	// Construction
	ButtonUnload();

	// NodeWindow interface
	void				onRender( RenderContext & context,
							const RectInt & window );		// client render, window is in screen coordinates
	// WindowButton interface
	bool				trapKey() const;					// trap keystroke
	void				onButtonUp();
	// Mutators
	void				setButton( Noun * pTarget, Noun * pCargo );

private:
	Noun::Ref			m_Target;
	Noun::wRef			m_Cargo;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
