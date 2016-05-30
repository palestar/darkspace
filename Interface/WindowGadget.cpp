/*
	WindowGadget.cpp
	(c)2000 Palestar, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Render3D/NodeLight.h"
#include "Render3D/NodeSound.h"
#include "Interface/WindowGadget.h"
#include "Interface/GameDocument.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( WindowGadget, NodeWindow );
REGISTER_FACTORY_KEY( WindowGadget, 4218809028462559824 );

WindowGadget::WindowGadget()
{}

//----------------------------------------------------------------------------

void WindowGadget::onRender( RenderContext & context, const RectInt & window )
{
	GameDocument * pDoc = (GameDocument *)document();
	if ( pDoc != NULL && pDoc->gadgetTarget() != NULL )
	{
		NounGadget * pTarget = pDoc->gadgetTarget();

		// get the hull of the object
		float scale = 1.0f / pTarget->hull().radius();
		// calculate the view direction from the players ship
		Vector3 view( cos(m_ActiveTime),-1,sin(m_ActiveTime) );
		view.normalize();
		
		Matrix33 cameraFrame( view );
		cameraFrame = cameraFrame * pTarget->frame();
		Vector3 cameraPosition( view * 4.0f );

		// end previous scene
		context.endScene();

		RenderContext::SaveState state( context );
		// set up new render context
		context.setPosition( cameraPosition );
		context.setFrame( cameraFrame );
		context.setWindow( window );
		context.setTime( pDoc->tick() * TICK_DURATION_S );

		DisplayDevice * pDisplay = context.display();
		ASSERT( pDisplay );

		// increase the ambient light so that the object is clearly visible
		pDisplay->setAmbient( WHITE );
		// remove all lights from the render device
		pDisplay->clearLights();

		// disable lights
		NodeLight::sm_bDisable = true;
		NodeSound::sm_bDisable = true;
		NounGadget::sm_bRenderGadgets = true;

		// render the target object
		pTarget->render( context, Matrix33::IDENTITY, Vector3::ZERO );

		// enable lights
		NodeLight::sm_bDisable = false;
		NodeSound::sm_bDisable = false;
		NounGadget::sm_bRenderGadgets = false;

		context.endScene();

		// restore the ambient light
		pDisplay->setAmbient( UNIVERSE_AMBIENT );
	}
}

//----------------------------------------------------------------------------
// EOF
