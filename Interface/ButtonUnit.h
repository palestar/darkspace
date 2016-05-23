/*
	ButtonUnit.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef BUTTONUNIT_H
#define BUTTONUNIT_H

#include "Gui3D/WindowButton.h"
#include "DarkSpace/NounUnit.h"

//----------------------------------------------------------------------------

class ButtonUnit : public WindowButton
{
public:
	// Construction
	ButtonUnit();
	// Node Window interface
	void				onUpdate( float t );
	void				onRender( RenderContext & context,
							const RectInt & window );
	// WindowButton interface
	void				onButtonUp();
	// Mutators
	void				setButton( NounUnit * pUnit );

private:
	NounUnit::wRef		m_Unit;
	Material::Link		m_Icon;
};

//----------------------------------------------------------------------------

#endif

//----------------------------------------------------------------------------
//EOF
