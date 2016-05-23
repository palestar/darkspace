/*
	ViewEndGame.cpp
	(c)2000 Palestar, Richard Lyle
*/

#include "Interface/ViewEndGame.h"
#include "Interface/GameDocument.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( ViewEndGame, WindowView::View );
REGISTER_FACTORY_KEY( ViewEndGame, 4150830807013315388 );

ViewEndGame::ViewEndGame()
{
	// Construct your view class
}

//----------------------------------------------------------------------------

void ViewEndGame::onActivate()
{
	//{{BEGIN_DATA_INIT
	m_pMessage = WidgetCast<WindowText>( window()->findNode( "Message" ) );
	//END_DATA_INIT}}

	// make sure our cursor is visible
	setCursorState( POINTER );
}

void ViewEndGame::onDeactivate()
{
	// called before this view is destroyed
}

void ViewEndGame::onUpdate( float t )
{
	// implement view updating
}

bool ViewEndGame::onMessage( const Message & msg )
{
	//{{BEGIN_MSG_MAP
	MESSAGE_MAP( WB_BUTTONUP, 1480153139, onButtonQuit);
	MESSAGE_MAP( WB_BUTTONUP, 1199820260, onButtonContinue);
	//END_MSG_MAP}}

	return false;
}

void ViewEndGame::onDocumentUpdate()
{
	// document data has changed, update this view if needed
}

//----------------------------------------------------------------------------


bool ViewEndGame::onButtonContinue(const Message & msg)
{
	((GameDocument *)document())->setScene( "ConnectServer" );
	return true;
}

bool ViewEndGame::onButtonQuit(const Message & msg)
{
	window()->postMessage( IC_EXIT, 0, 0 );
	return true;
}

