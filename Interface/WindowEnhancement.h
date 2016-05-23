/*
	WindowEnhancement.h

	(c)2008 Palestar, Jeff Everett
*/

#ifndef WINDOW_ENHANCEMENT_H
#define WINDOW_ENHANCEMENT_H

#include "Gui3d/NodeWindow.h"
#include "World/Noun.h"
#include "Gui3d/WindowButton.h"
#include "Interface/ButtonEnhancement.h"
#include "DarkSpace/NounGame.h"
#include "DarkSpace/NounEnhancement.h"


class NounShip;
class WindowText;
//----------------------------------------------------------------------------

class WindowEnhancement : public NodeWindow
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	WindowEnhancement();

	// NodeWindow interface
	void				onUpdate( float t );
	void				onActivate();

	void				setSelected(ButtonEnhancement *a_pEnhancement);
	ButtonEnhancement * getSelected() const;

private:
	ButtonEnhancement *	m_pEnhancementButtons[ NounEnhancement::MAX_ENHANCEMENTS_PER_SHIP ];
	ButtonEnhancement *	m_pSelectedButton;
	WindowButton *		m_pRemoveEnhancementButton;
	WindowButton *		m_pScrapEnhancementButton;

	WindowText *		m_pWindowText;
};

inline ButtonEnhancement* WindowEnhancement::getSelected() const
{
	return m_pSelectedButton;
}

inline void WindowEnhancement::setSelected(ButtonEnhancement *a_pEnhancement)
{
	m_pSelectedButton = a_pEnhancement;
}

//----------------------------------------------------------------------------


#endif
//----------------------------------------------------------------------------
// EOF
