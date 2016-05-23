/*
	ButtonStructure.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef BUTTONSTRUCTURE_H
#define BUTTONSTRUCTURE_H

#include "Gui3D/WindowButton.h"
#include "DarkSpace/NounStructure.h"



//----------------------------------------------------------------------------

class ButtonStructure : public WindowButton
{
public:
	// Construction
	ButtonStructure();

	// Node Window interface
	void				onUpdate( float t );
	void				onRender( RenderContext & context,
							const RectInt & window );
	// WindowButton interface
	void				onButtonUp();
	// Mutators
	void				setButton( NounStructure * pStructure );

private:
	NounStructure::wRef	m_Structure;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
