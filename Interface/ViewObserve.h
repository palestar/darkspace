/*
	ViewObserve.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef VIEWOBSERVE_H
#define VIEWOBSERVE_H

#include "System/Messages.h"
#include "Gui3d/WindowView.h"
#include "Interface/WindowObserve.h"



//----------------------------------------------------------------------------

class ViewObserve : public WindowView::View
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef Reference< ViewObserve >	Ref;

	// Construction
						ViewObserve();
	// View interface
	void				onActivate();
	void				onDeactivate();
	void				onUpdate( float t );
	bool				onMessage( const Message & msg );
	void				onDocumentUpdate();

	// {{BEGIN_DATA
	WindowText *		m_pTargetInfo;
	WindowButton *		m_pButtonChase;
	WindowButton *		m_pButtonIso;
	WindowButton *		m_pButtonFlyby;
	WindowButton *		m_pButtonFollow;
	WindowObserve *		m_pObserveWindow;
	WindowButton *		m_pButtonLock;
	WindowButton *		m_pButtonPlanets;
	WindowButton *		m_pButtonBattle;
	WindowButton *		m_pButtonAuto;
	// END_DATA}}

	// {{BEGIN_MSG
	bool				onButtonPrev(const Message & msg);
	bool				onButtonNext(const Message & msg);
	bool				onButtonLock(const Message & msg);
	bool				onButtonChase(const Message & msg);
	bool				onButtonIso(const Message & msg);
	bool				onButtonFlyby(const Message & msg);
	bool				onButtonFollow(const Message & msg);
	bool				onButtonPlanets(const Message & msg);
	bool				onButtonBattle(const Message & msg);
	bool				onButtonAuto(const Message & msg);
	bool				onButtonExit(const Message & msg);
	// END_MSG}}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
