/*
	ButtonContact.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef BUTTONCONTACT_H
#define BUTTONCONTACT_H

#include "Gui3d/WindowButton.h"
#include "World/Noun.h"
#include "DarkSpace/GameContext.h"

//----------------------------------------------------------------------------

class ButtonContact : public WindowButton
{
public:
	// Types
	typedef GameContext::ObjectiveType ObjectiveType;

	// Construction
	ButtonContact();
	ButtonContact( NodeWindow * pParent, Noun * pContact );

	// NodeInterfaceClient interface
	bool			onMessage( const Message & msg );
	// NodeWindow interface
	void			onUpdate( float t );
	void			onRender( RenderContext & context,
						const RectInt & window );		// client render, window is in screen coordinates
	bool			onCursorMove( const RectInt & window, const PointInt & position, const PointInt & delta );
	// WindowButton interface
	void			onButtonDown();	
	void			onButtonUp();	
	void			onMenuUp();
	// Accessors
	Noun *			noun() const;
	Material *		icon() const;
	bool			isObjective() const;

	// Mutators
	void			setButton( Noun * pContact );
	void			setIcon( Material * pIcon );

protected:
	// Data
	Noun::wRef		m_Noun;								// team member ship
	Material::Link	m_Icon;								// icon
	bool			m_IsObjective;						// is this object an objective
	ObjectiveType	m_Objective;
	bool			m_bGroupLeader;
	bool			m_bGroupPending;
};

//----------------------------------------------------------------------------

inline Noun * ButtonContact::noun() const
{
	return m_Noun;
}

inline Material * ButtonContact::icon() const
{
	return m_Icon;
}

inline bool ButtonContact::isObjective() const
{
	return m_IsObjective;
}

//----------------------------------------------------------------------------

#endif

//----------------------------------------------------------------------------
//EOF
