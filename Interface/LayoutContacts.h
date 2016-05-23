/*
	LayoutContacts.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef LAYOUT_CONTACTS_H
#define LAYOUT_CONTACTS_H

#include "Gui3d/WindowLayout.h"
#include "DarkSpace/GameContext.h"



//----------------------------------------------------------------------------

class LayoutContacts : public WindowLayout
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	LayoutContacts();

	// NodeInterfaceClient interface
	void				onActivate();
	void				onDeactivate();		
	void				onUpdate( float t );
	// WindowLayout interface
	void				doLayout();
	// Accessors
	bool				showAI() const;
	// Mutators
	void				setShowAI( bool bShowAI );
	void				sortButtons();

protected:
	//! Returns true if object is a navigation contact
	bool				isNavigatonContact( Noun * pNoun );

	// Data
	GameContext::Ref	m_pContext;
	float				m_Update;
	bool				m_bShowAI;
};

//----------------------------------------------------------------------------

inline bool LayoutContacts::showAI() const
{
	return m_bShowAI;
}

#endif

//----------------------------------------------------------------------------
//EOF
