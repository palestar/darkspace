/*
	WindowTarget.cpp

	This window object is used to display / track an object in the game
	(c)2000 Palestar, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Render3d/NodeLight.h"
#include "Render3d/NodeSound.h"
#include "Interface/WindowTarget.h"
#include "Interface/GameDocument.h"
#include "DarkSpace/NounUnit.h"
#include "DarkSpace/NounCargo.h"
#include "DarkSpace/NounStructure.h"
#include "DarkSpace/NounDropTarget.h"
#include "DarkSpace/NounBeacon.h"

static Constant WINDOW_TARGET_SCALE( "WINDOW_TARGET_SCALE", 2.0f );
static Constant WINDOW_TARGET_GADGET_SCALE( "WINDOW_TARGET_GADGET_SCALE", 2.0f );

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( WindowTarget, NodeWindow );
REGISTER_FACTORY_KEY( WindowTarget, 4109377257530988770 );

WindowTarget::WindowTarget()
{}

//----------------------------------------------------------------------------

void WindowTarget::onRender( RenderContext & context, const RectInt & window )
{
	GameDocument * pDoc = (GameDocument *)document();
	if ( pDoc != NULL )
	{
		if ( pDoc->target() != NULL && pDoc->ship() != NULL )
		{
			// end the previoue scene
			context.endScene();
			// save our state
			RenderContext::SaveState state( context );

			Noun * pTarget = pDoc->target();
			if ( WidgetCast<NounDropTarget>( pTarget ) )
				pTarget = ((NounDropTarget *)pTarget)->planet();

			float fRadius = pTarget->hull().radius() * WINDOW_TARGET_SCALE;

			bool bUseWorldFrame = true;
			// calculate the view direction from the players ship
			Vector3 direction;
			if ( WidgetCast<NounGadget>( pTarget ) 
				|| WidgetCast<NounUnit>( pTarget ) 
				|| WidgetCast<NounStructure>( pTarget ) 
				|| WidgetCast<NounCargo>( pTarget ) )
			{
				direction = Vector3(cos(m_ActiveTime),1,sin(m_ActiveTime) );
				fRadius *= WINDOW_TARGET_GADGET_SCALE;
				bUseWorldFrame = false;
			}
			else if ( pDoc->ship() != pTarget )
				direction = pDoc->ship()->worldPosition() - pTarget->worldPosition();
			else
				direction = Vector3(1,1,1);
			direction.normalize();
			

			context.setPosition( direction * fRadius );
			context.setFrame( Matrix33( -direction ) );
			context.setTime( pDoc->tick() * TICK_DURATION_S );
			context.setProjection( window, context.fov(), 0.1f, 10000.0f );

			DisplayDevice * pDisplay = context.display();
			ASSERT( pDisplay );

			context.beginScene();
			pDisplay->setAmbient( WHITE );

			// disable lights
			NodeLight::sm_bDisable = true;
			NodeSound::sm_bDisable = true;
			if ( WidgetCast<NounGadget>( pTarget ) )
				NounGadget::sm_bRenderGadgets = true;

			// clear the z-buffer, so objects aren't clipped by objects in the world.
			pDisplay->clearZ();

			// render the target object
			pTarget->render( context, bUseWorldFrame ? pTarget->worldFrame() : Matrix33::IDENTITY, Vector3::ZERO );
			context.endScene();

			// enable lights
			NodeLight::sm_bDisable = false;
			NodeSound::sm_bDisable = false;
			NounGadget::sm_bRenderGadgets = false;

			// restore the ambient light
			pDisplay->setAmbient( UNIVERSE_AMBIENT );
			// restart the scene
			context.beginScene();
		}
	}
}

//----------------------------------------------------------------------------
// EOF
