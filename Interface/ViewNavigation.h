/*
	ViewNavigation.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef VIEWNAVIGATION_H
#define VIEWNAVIGATION_H

#include "System/Messages.h"
#include "Gui3d/WindowView.h"
#include "Display/PrimitiveTriangleList.h"
#include "World/Noun.h"
#include "DarkSpace/GameContext.h"
#include "Interface/WindowNavigation.h"
#include "Interface/GameDocument.h"
#include "Interface/LayoutContacts.h"

//----------------------------------------------------------------------------

class ViewNavigation : public WindowView::View
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef Reference< ViewNavigation >	Ref;
	typedef NounShip::Order				Order;

	// Construction
	ViewNavigation();

	// View interface
	void				onActivate();
	void				onDeactivate();
	void				onHide();
	void				onUpdate( float t );
	bool				onMessage( const Message & msg );
	void				onRender( RenderContext & context, const RectInt & w );

	// {{BEGIN_DATA
	WindowButton *		m_pButtonAI;
	LayoutContacts *	m_pContactLayout;
	WindowNavigation *	m_pNavigationWindow;
	WindowButton *		m_pButtonJump;
	WindowButton *		m_pButtonWormhole;
	WindowText *		m_pInformation;
	WindowButton *		m_pButtonOrders;
	WindowButton *		m_pButtonShips;
	WindowButton *		m_pButtonPlanets;
	WindowButton *		m_pButtonGrid;
	WindowButton *		m_pButtonOrbits;
	// END_DATA}}

	// {{BEGIN_MSG
	bool				onButtonAI(const Message & msg);
	bool				onButtonOrders(const Message & msg);
	bool				onButtonShips(const Message & msg);
	bool				onButtonPlanets(const Message & msg);
	bool				onButtonJump(const Message & msg);
	bool				onButtonWormhole(const Message & msg);
	bool				onButtonFull(const Message & msg);
	bool				onButtonFocus(const Message & msg);
	bool				onButtonCenter(const Message & msg);
	bool				onButtonZoomOutEnd(const Message & msg);
	bool				onButtonZoomOutBegin(const Message & msg);
	bool				onButtonZoomInEnd(const Message & msg);
	bool				onButtonZoomInBegin(const Message & msg);
	bool				onButtonGrid(const Message & msg);
	bool				onButtonOrbits(const Message & msg);
	bool				onButtonCameraAngleReset(const Message & msg);
	// END_MSG}}

private:
	// Data
	GameDocument *		m_pDoc;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
