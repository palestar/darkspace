/*
	ViewMain.cpp
	(c)2000 Palestar, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Standard/Settings.h"
#include "Interface/GameDocument.h"
#include "Interface/ViewMain.h"
#include "Interface/ViewOptions.h"
#include "Interface/WindowObserve.h"

#include <time.h>

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( ViewMain, WindowView::View );
REGISTER_FACTORY_KEY( ViewMain, 4097030728440558163 );

ViewMain::ViewMain()
{
	// Construct your view class
}

//----------------------------------------------------------------------------

void ViewMain::onActivate()
{
	GameDocument * pDoc = WidgetCast<GameDocument>( document() );
	ASSERT( pDoc );

	//{{BEGIN_DATA_INIT
	DATA_MAP( m_pButtonPlay, WindowButton, "ButtonPlay" );
	m_pButtonModerate = WidgetCast<WindowButton>( window()->findNode( "ButtonModerate" ) );
	m_pObserveWindow = WidgetCast<NodeWindow>( window()->findNode( "ObserveWindow" ) );
	m_pUser = WidgetCast<WindowText>( window()->findNode( "User" ) );
	m_pMOTD = WidgetCast<WindowText>( window()->findNode( "MOTD" ) );
	//END_DATA_INIT}}

	((WindowObserve *)m_pObserveWindow)->setTargetMode( WindowObserve::AUTO );
	((WindowObserve *)m_pObserveWindow)->setLingerTime( 15.0f );
	((WindowObserve *)m_pObserveWindow)->setCameraDistance( 10.0f );
	

	// show/hide moderate button if player is moderator
	m_pButtonModerate->showWindow( pDoc->client()->canModerate() );
	// make cursor visible
	setCursorState( POINTER );
}

void ViewMain::onDeactivate()
{
	// called before this view is destroyed
}

void ViewMain::onUpdate( float t )
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );
	GameContext * pContext = pDoc->context();
	ASSERT( pContext );

	const GameClient::ServerStatus & status = pDoc->client()->serverStatus();

	// display the server information
	m_pMOTD->setText( CharString().format( "<color;ffffff;Bold><large>%s</large></Bold>\n\n%s\n%s</color>", 
			status.name, pContext->name(), pContext->description() ) );
	m_pButtonPlay->setEnable( pDoc->client()->loggedIn() );
}

bool ViewMain::onMessage( const Message & msg )
{
	//{{BEGIN_MSG_MAP
	MESSAGE_MAP( WB_BUTTONUP, 3622772365, onButtonOptions);
	MESSAGE_MAP( WB_BUTTONUP, 3828631526, onButtonModerate);
	MESSAGE_MAP( WB_BUTTONUP, 1495399497, onButtonObserve);
	MESSAGE_MAP( WB_BUTTONUP, 1966896877, onButtonCredits);
	MESSAGE_MAP( WB_BUTTONUP, 1501550643, onButtonExit);
	MESSAGE_MAP( WB_BUTTONUP, 1478883731, onButtonPlay);
	//END_MSG_MAP}}

	return false;
}

//----------------------------------------------------------------------------

bool ViewMain::onButtonPlay(const Message & msg)
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );

	if ( !pDoc->isTeamValid() )
		pDoc->setScene( "SelectTeam" );
	else if ( pDoc->isShipValid() )
		pDoc->setScene( "Game" );
	else if ( !pDoc->isShipValid() )
		pDoc->setScene( "SelectShip" );
	else
		pDoc->setScene( "Game" );

	return true;
}

bool ViewMain::onButtonExit(const Message & msg)
{
	window()->postMessage( IC_EXIT, 0, 0 );
	return true;
}

bool ViewMain::onButtonCredits(const Message & msg)
{
	document()->setScene( "Credits" );
	return true;
}

bool ViewMain::onButtonObserve(const Message & msg)
{
	document()->setScene( "Observe" );
	return true;
}

bool ViewMain::onButtonModerate(const Message & msg)
{
	document()->setScene( "Moderate" );
	return true;
}

bool ViewMain::onButtonOptions(const Message & msg)
{
	document()->setScene( "Options" );
	return true;
}

