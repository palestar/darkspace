/*
	ButtonRefit.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef BUTTONREFIT_H
#define BUTTONREFIT_H

#include "Gui3D/WindowButton.h"
#include "DarkSpace/NounGadget.h"
#include "DarkSpace/CargoGadget.h"



//----------------------------------------------------------------------------

class ButtonRefit : public WindowButton
{
public:
	// Construction
	ButtonRefit();

	// NodeWindow interface
	virtual void		onUpdate( float t );
	virtual void		onRender( RenderContext & context,
							const RectInt & window );		// client render, window is in screen coordinates
	// WindowButton interface
	virtual void		onButtonUp();
	// Mutators
	void				setButton( NounGadget * pGadget, NounGadget * pNewGadget );

private:
	NounGadget::wRef	m_pGadget;
	NounGadget::wRef	m_pNewGadget;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
