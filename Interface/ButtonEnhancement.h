/*
	WindowEnhancement.h

	(c)2008 Palestar, Jeff Everett
*/

#ifndef BUTTON_ENHANCEMENT_H
#define BUTTON_ENHANCEMENT_H

#include "Gui3d/NodeWindow.h"
#include "World/Noun.h"
#include "Gui3d/WindowButton.h"

class NounShip;
class NounEnhancement;
class WindowEnhancement;
//----------------------------------------------------------------------------

class ButtonEnhancement : public WindowButton
{
public:
	DECLARE_WIDGET_CLASS();


	typedef WidgetReference< ButtonEnhancement >		wRef;


	// Construction
						ButtonEnhancement();
	// WindowButton interface
	void				onRender( RenderContext & context,const RectInt & window );		// client render, window is in screen coordinates
	void				onActivate();
	void				onButtonUp();						// called on button release, action


	NounEnhancement *	enhancement() const;
	void				setEnhancement(NounEnhancement *a_pEnhancement);

	WindowEnhancement *	enhancementWindow() const;
	void				setEnhancementWindow(WindowEnhancement *a_pEnhancement);

private:
	NounEnhancement *	m_pEnhancement;
	WindowEnhancement * m_pEnhancementWindow;

};

inline NounEnhancement * ButtonEnhancement::enhancement() const
{
	return m_pEnhancement;
}

inline void ButtonEnhancement::setEnhancement(NounEnhancement *a_pEnhancement)
{
	m_pEnhancement = a_pEnhancement;
}


inline WindowEnhancement * ButtonEnhancement::enhancementWindow() const
{
	return m_pEnhancementWindow;
}

inline void ButtonEnhancement::setEnhancementWindow(WindowEnhancement *a_pEnhancement)
{
	m_pEnhancementWindow = a_pEnhancement;
}

//----------------------------------------------------------------------------


#endif
//----------------------------------------------------------------------------
// EOF
