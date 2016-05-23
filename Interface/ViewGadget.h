/*
	ViewGadget.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef VIEWGADGET_H
#define VIEWGADGET_H

#include "System/Messages.h"
#include "Gui3d/WindowView.h"



//----------------------------------------------------------------------------

class ViewGadget : public WindowView::View
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef Reference< ViewGadget >	Ref;

	// Construction
	ViewGadget();
	virtual	~ViewGadget();

	// View interface
	void				onActivate();
	void				onDeactivate();
	void				onUpdate( float t );
	bool				onMessage( const Message & msg );
	void				onDocumentUpdate();

	// {{BEGIN_DATA
	NodeWindow *				m_pGadgetRender;
	WindowText *				m_pAutoText;
	WindowText *				m_pRepairText;
	NodeWindow *				m_pAutoPanel;
	NodeWindow *				m_pRepairPanel;
	WindowText *				m_pGadgetDescription;
	// END_DATA}}

	// {{BEGIN_MSG
	bool				onButtonAutoAttack(const Message & msg);
	bool				onButtonAutoDefend(const Message & msg);
	bool				onButtonAutoOff(const Message & msg);
	bool				onButtonRepairQueue(const Message & msg);
	bool				onButtonRepairLast(const Message & msg);
	bool				onButtonRepairNow(const Message & msg);
	bool				onButtonClose(const Message & msg);
	// END_MSG}}
};

//----------------------------------------------------------------------------

#endif

//----------------------------------------------------------------------------
// EOF
