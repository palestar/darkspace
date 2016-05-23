/*
	ViewModerate.cpp
	(c)2000 Palestar, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Standard/Time.h"
#include "Interface/ViewModerate.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( ViewModerate, WindowView::View );
REGISTER_FACTORY_KEY( ViewModerate, 4420985700173175596 );

ViewModerate::ViewModerate() : m_UpdateTime( 0 )
{
	// Construct your view class
}

//----------------------------------------------------------------------------

void ViewModerate::onActivate()
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );

	// turn on moderator mode
	pDoc->client()->setModerate( true );
	// get current server info
	pDoc->client()->sendServerStatus();

	m_UpdateTime = 0.0f;
	m_nLastUpdate = 0;
	m_nLastBytesRecv = 0;
	m_nLastBytesSent = 0;

	//{{BEGIN_DATA_INIT
	m_pTextStatus = WidgetCast<WindowText>( window()->findNode( "TextStatus" ) );
	m_pChatEdit = WidgetCast<WindowEdit>( window()->findNode( "ChatEdit" ) );
	m_pChatEditWindow = WidgetCast<NodeWindow>( window()->findNode( "ChatEditWindow" ) );
	m_pListObjects = WidgetCast<WindowList>( window()->findNode( "ListObjects" ) );
	m_pListServers = WidgetCast<WindowList>( window()->findNode( "ListServers" ) );
	m_pListClients = WidgetCast<WindowList>( window()->findNode( "ListClients" ) );
	m_pObserveWindow = WidgetCast<WindowObserve>( window()->findNode( "ObserveWindow" ) );
	//END_DATA_INIT}}

	m_pChatEditWindow->hideWindow();
	m_pObserveWindow->setTargetMode( WindowObserve::LOCKED );
	m_pObserveWindow->setCameraMode( WindowObserve::CHASE );

	m_pListClients->addColumn( 150, WHITE );	// name
	m_pListClients->addColumn( 100, WHITE );	// user id
	m_pListClients->addColumn( 100, WHITE );	// latency
	m_pListClients->addColumn( 100, WHITE );	// IP

	m_pListServers->addColumn( 300, WHITE );	// name
	m_pListServers->addColumn( 100, WHITE );	// clients

	updateLists();
}

void ViewModerate::onDeactivate()
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );

	// turn off moderator mode
	pDoc->client()->setModerate( false );
	// clear lists
	m_pListClients->removeAll();
	m_pListServers->removeAll();
}

void ViewModerate::onUpdate( float t )
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );

	m_pChatEditWindow->setVisible( m_pChatEdit->editing() );

	m_UpdateTime += t;
	if ( m_UpdateTime > 10.0f )
	{
		m_UpdateTime = 0.0f;

		// get updated server information
		pDoc->client()->sendServerStatus();
		// update the lists
		updateLists();
	}
}

bool ViewModerate::onMessage( const Message & msg )
{
	//{{BEGIN_MSG_MAP
	MESSAGE_MAP( WL_SELECT, 928967688, onListObjects);
	MESSAGE_MAP( WB_BUTTONUP, 2992269568, onButtonRefresh);
	MESSAGE_MAP( WB_BUTTONUP, 1501550643, onButtonExit);
	MESSAGE_MAP( WE_EDITDONE, 3966988240, onChatEdit);
	MESSAGE_MAP( WB_BUTTONUP, 597305866, onButtonConnect);
	MESSAGE_MAP( WB_BUTTONUP, 2596805237, onButtonBan);
	MESSAGE_MAP( WB_BUTTONUP, 1503097811, onButtonKick);
	//END_MSG_MAP}}

	switch( msg.message )
	{
	case HM_CHAR:
		switch( msg.wparam )
		{
		case HK_RETURN:
			m_pChatEditWindow->showWindow();
			m_pChatEdit->onBeginEdit();
			return true;
		}
		break;
	}

	return false;
}

//----------------------------------------------------------------------------


bool ViewModerate::onButtonKick(const Message & msg)
{
	int selected = m_pListClients->selected();
	if ( selected < 0 )
		return false;

	int i = m_pListClients->sortedItem( selected );
	if ( i >= 0 && i < m_Status.clients.size() )
	{
		m_pChatEdit->setText( CharString().format("/kick @%u ", m_Status.clients[i].userId) );
		m_pChatEdit->onBeginEdit();
	}

	return true;
}

bool ViewModerate::onButtonBan(const Message & msg)
{
	int selected = m_pListClients->selected();
	if ( selected < 0 )
		return false;

	int i = m_pListClients->sortedItem( selected );
	if ( i >= 0 && i < m_Status.clients.size() )
	{
		m_pChatEdit->setText( CharString().format("/ban @%u ", m_Status.clients[i].userId) );
		m_pChatEdit->onBeginEdit();
	}

	return true;
}

bool ViewModerate::onButtonConnect(const Message & msg)
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );

	int selected = m_pListServers->selected();
	if ( selected < 0 )
		return false;

	int i = m_pListServers->sortedItem( selected );
	if ( i >= 0 && i < m_Servers.size() )
	{
		// connect to the new server
		pDoc->sendChat( CharString().format("/connect %u", m_Servers[i].id) );
		// turn moderator mode on for the new server
		pDoc->client()->setModerate( true );
		// update the lists
		updateLists();
	}

	return true;
}

bool ViewModerate::onChatEdit(const Message & msg)
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );

	CharString send( m_pChatEdit->text() );
	if ( send.length() > 0 )
	{
		pDoc->sendChat( send );
		m_pChatEdit->setText( "" );
	}
	return true;
}

bool ViewModerate::onButtonExit(const Message & msg)
{
	document()->setScene( "Main" );
	return true;
}

bool ViewModerate::onButtonRefresh(const Message & msg)
{
	updateLists();
	return true;
}

bool ViewModerate::onListObjects(const Message & msg)
{
	int selected = m_pListObjects->selected();
	if ( selected < 0 )
		return false;

	int i = m_pListObjects->sortedItem( selected );
	if ( i >= 0 && i < m_Objects.size() )
		m_pObserveWindow->setTarget( m_Objects[i] );

	return true;
}


//----------------------------------------------------------------------------

void ViewModerate::updateLists()
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );

	// clear the lists

	// get the latest server status
	m_Status = pDoc->client()->serverStatus();

	dword nSentBPS = 0;
	dword nRecvBPS = 0;

	dword nCurrentTime = Time::milliseconds();
	dword nElapsed = nCurrentTime - m_nLastUpdate;
	if ( nElapsed > 0 )
	{
		nSentBPS = ((m_Status.bytesSent - m_nLastBytesSent) * 1000) / nElapsed;
		nRecvBPS = ((m_Status.bytesRecv - m_nLastBytesRecv) * 1000) / nElapsed;
	}

	m_nLastUpdate = Time::milliseconds();
	m_nLastBytesRecv = m_Status.bytesRecv;
	m_nLastBytesSent = m_Status.bytesSent;


	CharString statusText;
	statusText += CharString().format( "<b>%s</b>\nAddress: %s : %u\n\n", m_Status.name, m_Status.address, m_Status.port );
	if ( m_Status.upTime > 0 )
	{
		statusText += CharString().format("Uptime: %2.2u:%2.2u:%2.2u\n", m_Status.upTime / 3600, (m_Status.upTime / 60) % 60, m_Status.upTime % 60);
		statusText += CharString().format("Bytes Received: %u / %u bps\n", m_Status.bytesRecv, nRecvBPS );
		statusText += CharString().format("Bytes Sent: %u / %u bps\n", m_Status.bytesSent, nSentBPS );
	}
	statusText += CharString().format("Clients: %u / %u\n", m_Status.clients.size(), m_Status.maxClients );

	m_pTextStatus->setText( statusText );

	int selected = m_pListClients->selected();
	int highlighted = m_pListClients->highlighted();

	m_pListClients->removeAllItems();
	for(int i=0;i<m_Status.clients.size();i++)
	{
		GameClient::ClientStatus & client = m_Status.clients[ i ];

		int item = m_pListClients->addItem( client.name );
		m_pListClients->setItem( item, 1, CharString().format("%u", client.userId) );
		m_pListClients->setItem( item, 2, CharString().format("%u ms", client.latency) );
		m_pListClients->setItem( item, 3, client.address );
	}
	
	m_pListClients->setSelected( selected );
	m_pListClients->setHighlighted( selected );

	// get the latest server list
	MetaClient * pMetaClient = pDoc->client()->metaClient();
	ASSERT( pMetaClient );

	if ( pMetaClient->connected() )
	{
		if ( pMetaClient->getServers( "", pMetaClient->gameId(), MetaClient::GAME_SERVER, m_Servers) > 0 )
		{
			selected = m_pListServers->selected();
			highlighted = m_pListServers->highlighted();

			m_pListServers->removeAllItems();
			for(int i=0;i<m_Servers.size();i++)
			{
				int item = m_pListServers->addItem( m_Servers[i].name );
				m_pListServers->setItem( item, 1, CharString().format("%u / %u", m_Servers[i].clients, m_Servers[i].maxClients) );
			}

			m_pListServers->setSelected( selected );
			m_pListServers->setHighlighted( selected );
		}
	}

	m_Objects.release();

	selected = m_pListObjects->selected();
	highlighted = m_pListObjects->highlighted();
	m_pListObjects->removeAllItems();

	GameContext * pContext = pDoc->context();
	for(int z=0;z<pContext->zoneCount();z++)
	{
		NodeZone * pZone = pContext->zone( z );
		for(int i=0;i<pZone->childCount();i++)
		{
			Noun * pNoun = WidgetCast<Noun>( pZone->child(i) );
			if ( pNoun != NULL )
				m_Objects.push( pNoun );
		}
	}

	for(int i=0;i<m_Objects.size();i++)
		m_pListObjects->addItem( m_Objects[i]->name() );

	m_pListObjects->setSelected( selected );
	m_pListObjects->setHighlighted( selected );
}

