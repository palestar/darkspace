/*
	ButtonCargo.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef BUTTON_CARGO_H
#define BUTTON_CARGO_H

#include "Gui3d/WindowButton.h"
#include "World/Noun.h"



//----------------------------------------------------------------------------

class ButtonCargo : public WindowButton
{
public:
	// Construction
	ButtonCargo();

	// NodeWindow interface
	void			onUpdate( float t );
	void			onRender( RenderContext & context,
						const RectInt & window );		// client render, window is in screen coordinates
	bool			onCursorMove( const RectInt & window, const PointInt & position, const PointInt & delta );
	// WindowButton interface
	bool			trapKey() const;
	void			onButtonUp();						// called on button release, action
	void			onMenuUp();
	// Accessors
	Noun *			cargo() const;
	// Mutators
	void			setCargo( Noun * pCargo );
	// Helpers
	Material *		cargoIcon( Noun * pCargo );

protected:
	// Data
	Noun::wRef		m_Cargo;
};

//----------------------------------------------------------------------------

inline Noun * ButtonCargo::cargo() const
{
	return m_Cargo;
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
