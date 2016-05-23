/*
	ViewRedirect.cpp
	(c)2000 Palestar, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Interface/GameDocument.h"
#include "DarkSpace/Constants.h"
#include "Interface/ViewRedirect.h"

//----------------------------------------------------------------------------

const float SHIP_VELOCITY = 30.0f;
const float FLYBY_TIME = 20.0f;
const float MID_TIME = 2.0f;
const float EXIT_TIME = 4.0f;
const Color JUMPSPACE_AMBIENT( 200, 20, 20 );

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( ViewRedirect, WindowView::View );
REGISTER_FACTORY_KEY( ViewRedirect, 4326662962320237761 );

ViewRedirect::ViewRedirect()
{}

//----------------------------------------------------------------------------

void ViewRedirect::onActivate()
{
	//{{BEGIN_DATA_INIT
	//END_DATA_INIT}}

	GameDocument * pDoc = (GameDocument *)document();
	if ( pDoc != NULL )
		initializeScene();
}

void ViewRedirect::onDeactivate()
{
	// called before this view is destroyed
}

void ViewRedirect::onUpdate( float t )
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc != NULL );

	m_Time += t;
	m_ShipPosition += m_ShipVelocity * t;
	m_CameraFrame = Matrix33( m_ShipPosition - m_CameraPosition );

	// if it's taking too long, then reinitialize the scene and do another flyby
	if ( m_Time > FLYBY_TIME )
		initializeScene();

	if ( !pDoc->client()->isServerTransfer() && pDoc->isShipValid() )
		pDoc->setScene( "Game" );			// ship is already valid, move into the game
}

bool ViewRedirect::onMessage( const Message & msg )
{
	//{{BEGIN_MSG_MAP
	//END_MSG_MAP}}

	return false;
}

void ViewRedirect::onDocumentUpdate()
{
	// document data has changed, update this view if needed
}

void ViewRedirect::onRender( RenderContext & context, const RectInt & window )
{
	// get some pointers
	GameDocument * pDoc = (GameDocument *)document();
	if ( pDoc == NULL )
		return;

	DisplayDevice * pDisplay = context.display();
	ASSERT( pDisplay );

	context.endScene();

	// create our render context
	context.setWindow( RectInt().setInvalid() );
	context.setPosition( m_CameraPosition );
	context.setFrame( m_CameraFrame );
	context.beginScene();

	// push a directional light
	pDisplay->addDirectionalLight( 0, WHITE, Vector3( -1, -1, -1 ).normalize() );

	// render the scenery
	if ( m_Scenery.valid() )
	{
		m_Scenery->render( context, Matrix33::IDENTITY, Vector3::ZERO );
	}

	// render the ship
	if ( m_Ship.valid() )
	{
		m_Ship->render( context, Matrix33::IDENTITY, Vector3::ZERO );
	}

	context.endScene();
}

//----------------------------------------------------------------------------

bool ViewRedirect::initializeScene()
{
	GameDocument * pDoc = (GameDocument *)document();
	if (! pDoc )
		return false;

	m_Ship = pDoc->ship();
	if (! m_Ship )
		return false;
	NodeZone * pZone = pDoc->context()->activeZone();
	if (! pZone )
		return false;
    m_Scenery = pZone->scenery();

	ASSERT( m_Scenery.valid() && m_Ship.valid() );

	float offset = m_Ship->radius();

	m_Time = 0.0f;
	m_CameraPosition = Vector3( -offset, -offset, 0 );
	m_ShipVelocity = Vector3( 0, 0, SHIP_VELOCITY );
	m_ShipPosition = m_ShipVelocity * -MID_TIME;
	m_CameraFrame = Matrix33( m_ShipPosition - m_CameraPosition );

	return true;
}


