/*
	ViewConnectServer.cpp
	(c)2000 Palestar, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Gui3d/InterfaceContext.h"
#include "Render3d/NodeSound.h"
#include "Render3d/NodeLight.h"
#include "DarkSpace/NounStar.h"
#include "Interface/GameDocument.h"
#include "Interface/ViewConnectServer.h"

const float PRECACHE_DELAY = 5.0f;

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( ViewConnectServer, WindowView::View );
REGISTER_FACTORY_KEY( ViewConnectServer, 4137065806977686835 );

ViewConnectServer::ViewConnectServer() : m_fPreCacheTime( 0.0f ), m_bCacheInitialized( false ), m_bLoginPending( false )
{
	// Construct your view class
}

//----------------------------------------------------------------------------

void ViewConnectServer::onActivate()
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );

	//{{BEGIN_DATA_INIT
	m_pProgress = WidgetCast<WindowBar>( window()->findNode( "Progress" ) );
	m_pStatus = WidgetCast<WindowText>( window()->findNode( "Status" ) );
	//END_DATA_INIT}}

	// called when this view is created
	m_pStatus->setText( CharString().format("Connected to %s...\nLoading Objects...", pDoc->client()->address() ) );
	m_pProgress->setPosition( 0.0f );

	m_bCacheInitialized = false;
	m_bLoginPending = false;
	m_fPreCacheTime = 0.0f;
}

void ViewConnectServer::onDeactivate()
{}

void ViewConnectServer::onUpdate( float t )
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );
	GameClient * pClient = pDoc->client();
	ASSERT( pClient );

	if (! pClient->connected() )
	{
		pDoc->setGameState( GameDocument::GS_DISCONNECTED );
		return;
	}
	
	// we delay precaching to give the splash screen some time to load and appear to the user, so they aren't looking
	// at a black screen for a while.
	m_fPreCacheTime += t;
	if ( m_fPreCacheTime > PRECACHE_DELAY )
	{
		if (! pClient->loggedIn() )
		{
			if (! m_bLoginPending )
			{
				// send login message now..
				pClient->login( pDoc->sessionId() );
				m_bLoginPending = true;
			}
		}
		else if (! m_bCacheInitialized )
		{
			GameContext * pContext = pDoc->context();
			ASSERT( pContext );

			m_PreCache.release();
			m_PreCacheIndex = 0;

			// pre-cache all universe objects (i.e. planets, stars, moons, etc..)
			SortedArray< WidgetKey > cacheOnce;
			for(int z=0;z<pContext->zoneCount();z++)
			{
				NodeZone * pRoot = pContext->zone( z );
				for(int i=0;i<pRoot->childCount();i++)
				{
					BaseNode * pChild = pRoot->child(i);
					if ( WidgetCast<NounPlanet>( pChild ) || WidgetCast<NounStar>( pChild ) )
					{
						NounContext * pNounContext = ((Noun *)pChild)->nounContext();
						if ( pNounContext == NULL )
							continue;
						if ( cacheOnce.find( pNounContext->key() ) >= 0 )
							continue;
						cacheOnce.insert( pNounContext->key() );

						m_PreCache.push( (Noun *)pRoot->child(i) );
					}
				}
			}

			if ( m_PreCache.size() > 0 )
				m_bCacheInitialized = true;
		}
		else if ( m_PreCacheIndex < m_PreCache.size() )
		{
			BaseNode * pLoad = m_PreCache[ m_PreCacheIndex++ ];
			ASSERT( pLoad );

			m_pProgress->setPosition( float( m_PreCacheIndex ) / m_PreCache.size() );

			//TRACE( CharString().format("Loading %s", pLoad->name()) );

			// disable sound nodes
			NodeSound::sm_bDisable = true;
			NodeLight::sm_bDisable = true;

			DisplayDevice * pDisplay = context()->display();
			ASSERT( pDisplay );

			// create a render context
			RenderContext renderContext;
			renderContext.setDisplay( pDisplay );
			renderContext.setAudio( context()->audio() );
			renderContext.setProjection( RectInt( 0, 0, 100, 100 ), PI / 4, 1.0f, 32768.0f );
			renderContext.setPosition( Vector3( 0.0f, 0.0f, -100.0f ) );
			renderContext.beginScene();
			// render the object directly in front of the camera
			pLoad->render( renderContext, Matrix33::IDENTITY, Vector3::ZERO ); 
			// clear the display primitives, never actually render them...
			renderContext.abortScene();

			// renable sound nodes
			NodeSound::sm_bDisable = false;
			NodeLight::sm_bDisable = false;
		}
		else
		{
			GameDocument * pDoc = (GameDocument *)document();
			if ( pDoc->isConnected() )
				pDoc->setGameState( GameDocument::GS_MAIN );
			else
				pDoc->setGameState( GameDocument::GS_DISCONNECTED );

			// release the array
			m_PreCache.release();
		}
	}
}

bool ViewConnectServer::onMessage( const Message & msg )
{
	//{{BEGIN_MSG_MAP
	//END_MSG_MAP}}

	switch( msg.message )
	{
	case HM_KEYDOWN:
		switch( msg.wparam )
		{
		case HK_ESCAPE:
			// abort return to select server screen
			((GameDocument *)document())->client()->close();
			((GameDocument *)document())->setGameState( GameDocument::GS_LOBBY );
			return true;
		case HK_SPACE:
			// skip precache, and jump to main screen
			//if ( ((GameDocument *)document())->client()->loggedIn() )
			//{
			//	((GameDocument *)document())->setScene( "Main" );
			//	m_PreCache.release();
			//}
			return true;
		}
	}

	return false;
}

void ViewConnectServer::onDocumentUpdate()
{
	// document data has changed, update this view if needed
}

//----------------------------------------------------------------------------


