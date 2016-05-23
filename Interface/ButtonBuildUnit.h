/*
	ButtonBuildUnit.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef BUTTONBUILDUNIT_H
#define BUTTONBUILDUNIT_H

#include "Gui3D/WindowButton.h"
#include "DarkSpace/NounUnit.h"
#include "DarkSpace/StructureBarracks.h"



//----------------------------------------------------------------------------

class ButtonBuildUnit : public WindowButton
{
public:
	// Construction
	ButtonBuildUnit();

	// NodeWindow interface
	void				onUpdate( float t );
	void				onRender( RenderContext & context,
							const RectInt & window );		// client render, window is in screen coordinates
	// WindowButton interface
	void				onButtonUp();
	// Mutators
	void				setButton( StructureBarracks * pFactory, NounUnit * pUnit );

private:
	StructureBarracks::wRef	
						m_Barracks;
	NounUnit::wRef		m_Unit;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
