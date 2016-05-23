/*
	ViewDisconnected.cpp
	(c)2000 Palestar, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Interface/GameDocument.h"
#include "Interface/ViewDisconnected.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( ViewDisconnected, WindowView::View );
REGISTER_FACTORY_KEY( ViewDisconnected, 4096748061456171227 );

ViewDisconnected::ViewDisconnected()
{
	// Construct your view class
}

//----------------------------------------------------------------------------

void ViewDisconnected::onActivate()
{
	//{{BEGIN_DATA_INIT
	DATA_MAP( m_pButtonReconnect, WindowButton, "ButtonReconnect" );
	DATA_MAP( m_pDisconnectMessage, WindowText, "DisconnectMessage" );
	//END_DATA_INIT}}

	GameClient * pClient = ((GameDocument *)document())->client();
	if ( pClient->isBanned() )
		m_pDisconnectMessage->setText( "You have been banned from the server for a ROC violation." );
	else
		m_pDisconnectMessage->setText( "Connection to the server has been lost, please check your firewall and internet connection." );

	// restore the normal cursor in case they were rotating their view when disconnected
	setCursorState( POINTER );
}

void ViewDisconnected::onDeactivate()
{
	// called before this view is destroyed
}

void ViewDisconnected::onUpdate( float t )
{
	// implement view updating
	GameClient * pClient = ((GameDocument *)document())->client();
	// make the reconnect button visible only if they haven't been banned..
	m_pButtonReconnect->setVisible( !pClient->isBanned() );
}

bool ViewDisconnected::onMessage( const Message & msg )
{
	//{{BEGIN_MSG_MAP
	MESSAGE_MAP( WB_BUTTONUP, 1047757926, onButtonReconnect);
	MESSAGE_MAP( WB_BUTTONUP, 1293570276, onButtonCancel);
	//END_MSG_MAP}}

	return false;
}

void ViewDisconnected::onDocumentUpdate()
{
	// document data has changed, update this view if needed
}

//----------------------------------------------------------------------------


bool ViewDisconnected::onButtonCancel(const Message & msg)
{
	window()->postMessage( IC_EXIT, 0, 0 );
	return true;
}

bool ViewDisconnected::onButtonReconnect(const Message & msg)
{
	GameClient * pClient = ((GameDocument *)document())->client();
	if (! pClient->isBanned() )
	{
		// unlock while reconnecting, otherwise we will dead-lock
		pClient->unlock();
		// reconnect without the client locked
		if ( pClient->reconnect() )
			document()->setScene( "ConnectServer" );
		// relock the client before proceeding
		pClient->lock();
	}

	return true;
}

