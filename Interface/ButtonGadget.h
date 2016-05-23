/*
	ButtonGadget.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef BUTTON_GADGET_H
#define BUTTON_GADGET_H

#include "Gui3d/WindowButton.h"
#include "DarkSpace/NounGadget.h"

//----------------------------------------------------------------------------

class ButtonGadget : public WindowButton
{
public:
	// Types
	typedef Reference< ButtonGadget >	Ref;

	// Construction
	ButtonGadget();

	// NodeInterfaceClient interface
	bool			onMessage( const Message & msg );
	// NodeWindow interface
	void			onUpdate( float t );
	void			onRender( RenderContext & context,
						const RectInt & window );		// client render, window is in screen coordinates
	bool			onCursorMove( const RectInt & window, const PointInt & position, const PointInt & delta );
	// WindowButton interface
	bool			trapKey() const;
	void			onButtonUp();						// called on left button release
	void			onMenuUp();							// called on right button release
	// Accessors
	NounGadget *	gadget() const;
	dword			group() const;

	// Mutators
	void			setGadget( NounGadget * pGadget );

protected:
	// Data
	NounGadget::wRef
					m_Gadget;
	Color			m_IconColor;
	bool			m_bCursorOver;
};

//----------------------------------------------------------------------------

inline NounGadget * ButtonGadget::gadget() const
{
	return m_Gadget;
}

inline dword ButtonGadget::group() const
{
	return m_Gadget->group();
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
