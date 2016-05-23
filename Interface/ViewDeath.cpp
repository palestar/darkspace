/*
	ViewDeath.cpp
	(c)2000 Palestar, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Interface/GameDocument.h"
#include "DarkSpace/Constants.h"
#include "Interface/ViewDeath.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( ViewDeath, WindowView::View );
REGISTER_FACTORY_KEY( ViewDeath, 4096729882495776647 );

ViewDeath::ViewDeath()
{
	// Construct your view class
}

//----------------------------------------------------------------------------

void ViewDeath::onActivate()
{
	//{{BEGIN_DATA_INIT
	m_pObserveWindow = WidgetCast<NodeWindow>( window()->findNode( "ObserveWindow" ) );
	m_pTextMessage = WidgetCast<WindowText>( window()->findNode( "TextMessage" ) );
	//END_DATA_INIT}}

	// restore the normal cursor in case they were rotating their view on death
	setCursorState( POINTER );
}

void ViewDeath::onDeactivate()
{
	// called before this view is destroyed
}

void ViewDeath::onUpdate( float t )
{
}

bool ViewDeath::onMessage( const Message & msg )
{
	//{{BEGIN_MSG_MAP
	MESSAGE_MAP( WB_BUTTONUP, 1501550643, onButtonExit);
	MESSAGE_MAP( WB_BUTTONUP, 1507228051, onButtonOkay);
	//END_MSG_MAP}}

	return false;
}

void ViewDeath::onDocumentUpdate()
{
	// document data has changed, update this view if needed
}

void ViewDeath::onRender( RenderContext & context, const RectInt & window )
{}

//----------------------------------------------------------------------------


bool ViewDeath::onButtonOkay(const Message & msg)
{
	document()->setScene( "SelectShip" );
	return true;
}

bool ViewDeath::onButtonExit(const Message & msg)
{
	document()->setScene( "Main" );
	return true;
}

