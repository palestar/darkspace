/**
	@file WindowShipSelection.cpp

	(c)2009 Palestar Inc
	@author Richard Lyle @date 9/17/2010 12:27:15 PM
*/

#include "Debug/Assert.h"
#include "WindowShipSelection.h"
#include "GameDocument.h"

#include "Display/PrimitiveSetTransform.h"
#include "Display/PrimitiveLineList.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( WindowShipSelection, NodeWindow );

WindowShipSelection::WindowShipSelection()
{
	// create the spin node
	m_ShipSpin = new NodeSpin;
	m_ShipSpin->setSpin( 0, 0, 0 );
	m_ShipRadius = 5.0f;
	m_ShipZoom = 5.0f;
}

//----------------------------------------------------------------------------

void WindowShipSelection::onUpdate( float t )
{
	NodeWindow::onUpdate( t );

	if ( m_ShipSpin.valid() && m_ShipSpin->childCount() > 0 )
		m_ShipRadius = m_ShipSpin->child(0)->hull().radius() * 3.0f;
	// implement view updating
	m_ShipZoom += (m_ShipRadius - m_ShipZoom) * Clamp( t, 0.0f, 1.0f / 15.0f );
}

const float CAMERA_PITCH = PI / 4;
const float CAMERA_YAW_RATE = PI / 32;

void WindowShipSelection::onRender( RenderContext & context, const RectInt & window )
{
	// render this view, window is in screen space
	GameDocument * pDoc = (GameDocument *)document();
	if ( pDoc == NULL )
		return;
	if (! m_ShipSpin.valid() )
		return;

	// get the display device
	DisplayDevice * pDisplay = context.device();
	ASSERT( pDisplay );

	// end any previous scene
	context.endScene();

	// render the selected ship
	RenderContext::SaveState state( context );

	// calculate the camera frame and position
	Matrix33 cameraFrame( true );
	cameraFrame.rotate( CAMERA_PITCH, CAMERA_YAW_RATE * context.time(), 0 );
	Vector3 cameraPosition( m_ShipSpin->position() - (cameraFrame.k * m_ShipZoom) );

	context.setPosition( cameraPosition );
	context.setFrame( cameraFrame );

	// set the window to render the ship inside
	RectInt renderWindow( window );
	renderWindow.inset( windowStyle()->borderSize() );
	context.setWindow( renderWindow );
	context.beginScene();

	pDisplay->addDirectionalLight( 0, WHITE, Vector3( 1, 1, 1).normalize() );
	pDisplay->addDirectionalLight( 0 ,WHITE, Vector3( -1, -1, -1).normalize() );

	// render the ship 
	context.render( m_ShipSpin );

	// render a grid below the ship
	Vector3 n( 0, 1, 0 );

	const int		GRID_STEPS = 15;
	const float		GRID_SIZE = 30.0f;
	const float		GRID_CENTER = GRID_SIZE / 2.0f;
	const float		GRID_STEP = GRID_SIZE / GRID_STEPS;
	const int		GRID_LINES = GRID_STEPS + GRID_STEPS + 2;

	Line gridLines[ GRID_LINES ];
	for(int x=0;x<=GRID_STEPS;x++)
	{
		float dx = (GRID_STEP * x) - GRID_CENTER;

		gridLines[ x ] = Line( 
			Vertex( Vector3( dx, 0, -GRID_CENTER ), n, 0, 0 ),
			Vertex( Vector3( dx, 0, GRID_CENTER ), n, 0, 0 ) );
	}
	for(int z=0;z<=GRID_STEPS;z++)
	{
		float dz = (GRID_STEP * z) - GRID_CENTER;

		gridLines[ GRID_STEPS + 1 + z ] = Line( 
			Vertex( Vector3( -GRID_CENTER, 0, dz), n, 0, 0 ),
			Vertex( Vector3( GRID_CENTER, 0, dz), n, 0, 0 ) );
	}

	// push a material of the proper color
	Material::push( context, Color(0,224,40,255), true, PrimitiveMaterial::ADDITIVE );
	PrimitiveSetTransform::push( pDisplay, Matrix33::IDENTITY, Vector3::ZERO );
	PrimitiveLineList::push( pDisplay, GRID_LINES, gridLines );

	// end this scene
	context.endScene();

	// restart the previous scene
	context.beginScene();
	// restore the ambient light
	pDisplay->setAmbient( UNIVERSE_AMBIENT );
}

//---------------------------------------------------------------------------------------------------

void WindowShipSelection::setSelectedShip( NounShip * pShip )
{
	if ( pShip != NULL )
	{
		// create a copy of the ship to show the player
		NounShip * pCopy = (NounShip *)pShip->copy();
		pCopy->setPosition( Vector3( true ) );
		pCopy->setSyncronized( true );		// so it will render

		m_ShipSpin->detachAllNodes();
		m_ShipSpin->setPosition( Vector3( 0,0,0 ) );
		m_ShipSpin->attachNode( pCopy );
		m_ShipRadius = pShip->radius() * 3.0f;
	}
	else
		m_ShipSpin->detachAllNodes();
}

//---------------------------------------------------------------------------------------------------
//EOF
