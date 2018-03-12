/*
	ViewNavigation.cpp
	(c)2000 Palestar, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Display/PrimitiveSetTransform.h"
#include "Display/PrimitiveTriangleFan.h"
#include "Display/PrimitiveLineStrip.h"
#include "System/Keyboard.h"
#include "DarkSpace/NounBody.h"
#include "World/NounZoneTarget.h"
#include "DarkSpace/NounNebula.h"
#include "DarkSpace/NounProjectile.h"
#include "DarkSpace/Constants.h"
#include "DarkSpace/VerbUseGadget.h"
#include "DarkSpace/NounStar.h"
#include "DarkSpace/NounAsteroid.h"
#include "DarkSpace/NounJumpGate.h"
#include "Interface/WindowTarget.h"
#include "Interface/ViewGame.h"
#include "Interface/ViewNavigation.h"

#include <stdio.h>

//----------------------------------------------------------------------------

template<class T>
inline T sqr( T number )
{
	return number * number;
}

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( ViewNavigation, WindowView::View );
REGISTER_FACTORY_KEY( ViewNavigation, 4096754072893440324 );

ViewNavigation::ViewNavigation() : m_pDoc( NULL )
{}

//----------------------------------------------------------------------------

const float UNIVERSE_SIZE_OFFSET = 1.25f;

void ViewNavigation::onActivate()
{
	//{{BEGIN_DATA_INIT
	DATA_MAP( m_pButtonAI, WindowButton, "ButtonAI" );
	DATA_MAP( m_pContactLayout, LayoutContacts, "ContactLayout" );
	DATA_MAP( m_pNavigationWindow, WindowNavigation, "NavigationWindow" );
	DATA_MAP( m_pButtonJump, WindowButton, "ButtonJump" );
	DATA_MAP( m_pButtonWormhole, WindowButton, "ButtonWormhole" );
	DATA_MAP( m_pInformation, WindowText, "Information" );
	DATA_MAP( m_pButtonOrders, WindowButton, "ButtonOrders" );
	DATA_MAP( m_pButtonShips, WindowButton, "ButtonShips" );
	DATA_MAP( m_pButtonPlanets, WindowButton, "ButtonPlanets" );
	DATA_MAP( m_pButtonGrid, WindowButton, "ButtonGrid" );
	DATA_MAP( m_pButtonOrbits, WindowButton, "ButtonOrbits" );
	//END_DATA_INIT}}

	// get some pointers
	m_pDoc = WidgetCast<GameDocument>( document() );
	ASSERT( m_pDoc );
}

void ViewNavigation::onDeactivate()
{}

void ViewNavigation::onHide()
{}

const float CAMERA_SNAP_TIME = 1.0f;					// how long does it take for the camera to snap fully into the navigation frame/position
const float MAX_TIME = 1.0f / 20.0f;

void ViewNavigation::onUpdate( float t )
{
	NounShip * pShip = m_pDoc->ship();
	if (! pShip )
		return;

	// show/hide the jump button
	GadgetJumpDrive * pJumpDrive = pShip->jumpDrive();
	m_pButtonJump->showWindow( pJumpDrive != NULL && 
		pJumpDrive->usable( m_pDoc->target(), false ) );
		
	// show/hide the gate button
	GadgetJumpGate * pJumpGate = pShip->jumpGate();
	m_pButtonWormhole->showWindow( pJumpGate != NULL && 
		pJumpGate->usable( m_pDoc->target(), false ) );		

	m_pButtonAI->setIconColor( m_pContactLayout->showAI() ? WHITE : GREY );

	// update the filter buttons
	m_pButtonOrbits->setIconColor( (m_pNavigationWindow->showFlags() & WindowNavigation::SHOW_ORBITS) ? WHITE : GREY );
	m_pButtonGrid->setIconColor( (m_pNavigationWindow->showFlags() & WindowNavigation::SHOW_GRID) ? WHITE : GREY );
	m_pButtonPlanets->setIconColor( (m_pNavigationWindow->showFlags() & WindowNavigation::SHOW_PLANETS) ? WHITE : GREY );
	m_pButtonShips->setIconColor( (m_pNavigationWindow->showFlags() & WindowNavigation::SHOW_SHIPS) ? WHITE : GREY );
	m_pButtonOrders->setIconColor( (m_pNavigationWindow->showFlags() & WindowNavigation::SHOW_ORDERS) ? WHITE : GREY );
}

bool ViewNavigation::onMessage( const Message & msg )
{
	//{{BEGIN_MSG_MAP
	MESSAGE_MAP( WB_BUTTONUP, 2899725725, onButtonAI);
	MESSAGE_MAP( WB_BUTTONUP, 1502169267, onButtonJump);
	MESSAGE_MAP( WB_BUTTONUP, 1972583907, onButtonWormhole);
	MESSAGE_MAP( WB_BUTTONDOWN, 1807290119, onButtonOrders);
	MESSAGE_MAP( WB_BUTTONDOWN, 50011147, onButtonShips);
	MESSAGE_MAP( WB_BUTTONDOWN, 4254758514, onButtonPlanets);
	MESSAGE_MAP( WB_BUTTONDOWN, 1497976627, onButtonFull);
	MESSAGE_MAP( WB_BUTTONDOWN, 680396811, onButtonFocus);
	MESSAGE_MAP( WB_BUTTONDOWN, 1158796068, onButtonCenter);
	MESSAGE_MAP( WB_BUTTONUP, 565146306, onButtonZoomOutEnd);
	MESSAGE_MAP( WB_BUTTONDOWN, 565146306, onButtonZoomOutBegin);
	MESSAGE_MAP( WB_BUTTONUP, 286089378, onButtonZoomInEnd);
	MESSAGE_MAP( WB_BUTTONDOWN, 286089378, onButtonZoomInBegin);
	MESSAGE_MAP( WB_BUTTONDOWN, 1499126323, onButtonGrid);
	MESSAGE_MAP( WB_BUTTONDOWN, 1809782535, onButtonOrbits);
	MESSAGE_MAP( WB_BUTTONDOWN, 1354953777, onButtonCameraAngleReset);
	//END_MSG_MAP}}

	return false;
}

void ViewNavigation::onRender( RenderContext & context, const RectInt & w )
{
	// if a enemy object is near, render a red box along the edge of the window..
	if ( ViewGame::sm_pViewGame != NULL )
		ViewGame::sm_pViewGame->renderViewGame( context, w );
}

//----------------------------------------------------------------------------

bool ViewNavigation::onButtonOrbits(const Message & msg)
{
	m_pNavigationWindow->flipShowFlag( WindowNavigation::SHOW_ORBITS );
	return true;
}

bool ViewNavigation::onButtonGrid(const Message & msg)
{
	m_pNavigationWindow->flipShowFlag( WindowNavigation::SHOW_GRID );
	return true;
}

bool ViewNavigation::onButtonPlanets(const Message & msg)
{
	m_pNavigationWindow->flipShowFlag( WindowNavigation::SHOW_PLANETS );
	return true;
}

bool ViewNavigation::onButtonShips(const Message & msg)
{
	m_pNavigationWindow->flipShowFlag( WindowNavigation::SHOW_SHIPS );
	return true;
}

bool ViewNavigation::onButtonOrders(const Message & msg)
{
	m_pNavigationWindow->flipShowFlag( WindowNavigation::SHOW_ORDERS );
	return true;
}


bool ViewNavigation::onButtonZoomInBegin(const Message & msg)
{
	m_pNavigationWindow->beginZoom( true );
	return true;
}

bool ViewNavigation::onButtonZoomInEnd(const Message & msg)
{
	m_pNavigationWindow->endZoom();
	return true;
}

bool ViewNavigation::onButtonZoomOutBegin(const Message & msg)
{
	m_pNavigationWindow->beginZoom( false );
	return true;
}

bool ViewNavigation::onButtonZoomOutEnd(const Message & msg)
{
	m_pNavigationWindow->endZoom();
	return true;
}

bool ViewNavigation::onButtonCenter(const Message & msg)
{
	m_pNavigationWindow->onCenter();
	return true;
}

bool ViewNavigation::onButtonFocus(const Message & msg)
{
	m_pNavigationWindow->onFocus();
	return true;
}

bool ViewNavigation::onButtonFull(const Message & msg)
{
	m_pNavigationWindow->onFull();
	return true;
}

bool ViewNavigation::onButtonJump(const Message & msg)
{
	if ( m_pDoc->ship() != NULL && m_pDoc->ship()->jumpDrive() != NULL && m_pDoc->target() != NULL )
		m_pDoc->ship()->useGadget( m_pDoc->ship()->jumpDrive(), m_pDoc->target(), false );
	return true;
}

bool ViewNavigation::onButtonWormhole(const Message & msg)
{
	if ( m_pDoc->ship() != NULL && m_pDoc->ship()->jumpGate() != NULL && m_pDoc->target() != NULL )
		m_pDoc->ship()->useGadget( m_pDoc->ship()->jumpGate(), m_pDoc->target(), false );
	return true;
}

bool ViewNavigation::onButtonAI(const Message & msg)
{
	m_pContactLayout->setShowAI( !m_pContactLayout->showAI() );
	if ( m_pContactLayout->showAI() )
		m_pNavigationWindow->setShowFlags( m_pNavigationWindow->showFlags() | WindowNavigation::SHOW_AI	);
	else
		m_pNavigationWindow->setShowFlags( m_pNavigationWindow->showFlags() & ~WindowNavigation::SHOW_AI );

	return true;
}

bool ViewNavigation::onButtonCameraAngleReset(const Message & msg)
{
	m_pNavigationWindow->onCameraAngleReset();
	return true;
}
