/*
	ViewEngineering.cpp
	(c)2000 Palestar, Richard Lyle
*/

#define INTERFACE_DLL
#include "Debug/Assert.h"
#include "Math/Helpers.h"
#include "Display/PrimitiveMaterial.h"
#include "Display/PrimitiveWindow.h"
#include "Display/PrimitiveSetFill.h"
#include "Display/PrimitiveLineStrip.h"
#include "Render3d/NodeComplexMesh.h"
#include "Game/VerbRepairGadget.h"
#include "Game/VerbRefitGadget.h"
#include "Interface/GameDocument.h"
#include "Game/GadgetArmor.h"
#include "Game/GadgetShield.h"
#include "Game/GadgetWeapon.h"
#include "Game/GadgetBeamWeapon.h"
#include "Game/GadgetSlot.h"
#include "Interface/ButtonCargo.h"
#include "Interface/ViewGame.h"
#include "Interface/ViewEngineering.h"

//----------------------------------------------------------------------------

const Color			HULL_COLOR( 255, 0, 0, 255 );
const RectFloat		DAMAGE_BAR_UV( 0,0,0,0);

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( ViewEngineering, WindowView::View );
REGISTER_FACTORY_KEY( ViewEngineering, 4125641697038318513 );

ViewEngineering::ViewEngineering()
{
	m_Adjust = false;
}

//----------------------------------------------------------------------------

void ViewEngineering::onActivate()
{
	//{{BEGIN_DATA_INIT
	m_pRepairQueue = WidgetCast<NodeWindow>( window()->findNode( "RepairQueue" ) );
	m_pLayoutRepair = WidgetCast<NodeWindow>( window()->findNode( "LayoutRepair" ) );
	m_pShipStatus = WidgetCast<WindowText>( window()->findNode( "ShipStatus" ) );
	//END_DATA_INIT}}

	m_UpdateStatus = 1.0f;
	m_Adjust = false;

	m_pRepairQueue->detachAllNodes();
}

void ViewEngineering::onDeactivate()
{
	m_pRepairQueue->detachAllNodes();
}

const float CAMERA_SNAP_TIME = 1.0f;

void ViewEngineering::onUpdate( float t )
{
}

bool ViewEngineering::onMessage( const Message & msg )
{
	//{{BEGIN_MSG_MAP
	//END_MSG_MAP}}

	return false;
}

void ViewEngineering::onRender( RenderContext & context, const RectInt & window )
{
	GameDocument * pDoc = (GameDocument *)document();
	if ( pDoc == NULL )
		return;

	UniverseContext * pContext = pDoc->context();
	ASSERT( pContext );
	NounShip * pShip = pDoc->ship();
	ASSERT( pShip );

	// get the time elapsed
	float t = context.elapsed();
	// calculate the distance from the ship based upon it's radius
	float cameraDistance = pShip->radius() * 2.5f;
	// calculate the camera frame and position
	Matrix33 cameraFrame( pShip->frame() );
	cameraFrame.rotate( PI / 4, ViewGame::s_CameraTime * (PI / 4), 0 );
	Vector3	cameraPosition( pShip->worldPosition() - (cameraFrame.k * cameraDistance) );

	if ( ViewGame::s_CameraTime < CAMERA_SNAP_TIME )
	{
		// if ship is moving, keep it from jumping around while the camera is animating
		if ( m_Adjust )
		{
			Vector3 position( pShip->worldPosition() );
			Vector3 adjust( position - m_AdjustPosition );
			m_AdjustPosition = position;

			ViewGame::s_CameraPosition += adjust;
		}
		else
		{
			m_AdjustPosition = pShip->worldPosition();
			m_Adjust = true;
		}

		float d = ViewGame::s_CameraTime / CAMERA_SNAP_TIME;
		// move the camera
		Vector3 deltaPosition( cameraPosition - ViewGame::s_CameraPosition );
		ViewGame::s_CameraPosition += deltaPosition * d;

		// update the camera frame
		ViewGame::s_CameraFrame.i += (cameraFrame.i - ViewGame::s_CameraFrame.i) * d;
		ViewGame::s_CameraFrame.j += (cameraFrame.j - ViewGame::s_CameraFrame.j) * d;
		ViewGame::s_CameraFrame.k += (cameraFrame.k - ViewGame::s_CameraFrame.k) * d;
		ViewGame::s_CameraFrame.orthoNormalizeXY();
	}
	else
	{
		// animation is over, snap directly to the desired frame/position
		ViewGame::s_CameraFrame = cameraFrame;
		ViewGame::s_CameraPosition = cameraPosition;
		m_Adjust = false;
	}

	m_UpdateStatus += t;
	if ( m_UpdateStatus > 1.0f )
	{
		m_UpdateStatus = 0.0f;

		// update the ship status
		String shipStatus;
		shipStatus += String().format("Hull:<X;75;Color;ff00ffff>%d%%</Color>\n", int(pShip->damageRatioInv() * 100) );
		shipStatus += String().format("Energy:<X;75;Color;ff00ffff>%d / %d</Color>\n", pShip->energy(), pShip->maxEnergy() );
		shipStatus += String().format("Velocity:<X;75;Color;ff00ffff>%.1f / %.1f</Color>\n", pShip->velocity(), pShip->maxVelocity() );
		shipStatus += String().format("Signature:<X;75;Color;ff00ffff>%.1f</Color>\n", pShip->signature() );

		shipStatus += "\nSYSTEMS:\n";

		// display armor status
		for(int i=0;i<pShip->childCount();i++)
			if ( WidgetCast<GadgetArmor>( pShip->child(i) ) )
			{
				GadgetArmor * pArmor = (GadgetArmor *)pShip->child(i);
				shipStatus += String().format("%s<X;100>%d%%\n", 
					pArmor->nounContext()->name(), 
					(pArmor->armor() * 100 ) / pArmor->strength() );
			}
		// display shield status
		for(i=0;i<pShip->childCount();i++)
			if ( WidgetCast<GadgetShield>( pShip->child(i) ) )
			{
				GadgetShield * pShield = (GadgetShield *)pShip->child(i);
				shipStatus += String().format("%s<X;100>%d%%\n", 
					pShield->nounContext()->name(), 
					(pShield->charge() * 100 ) / pShield->maxCharge() );
			}

		for(i=0;i<pShip->childCount();i++)
			if ( WidgetCast<NounGadget>( pShip->child(i) ) )
			{
				NounGadget * pGadget = (NounGadget *)pShip->child(i);
				if ( WidgetCast<GadgetArmor>( pGadget ) )
					continue;
				if ( WidgetCast<GadgetShield>( pGadget ) )
					continue;
				
				shipStatus += String().format("%s<X;100>%d%%\n", 
					pGadget->nounContext()->name(), 
					int( pGadget->damageRatioInv() * 100 ) );
			}

		m_pShipStatus->setText( shipStatus );
	}
	
	if ( m_pLayoutRepair->visible() )
	{
		for(int i=0;i<pShip->repairCount();i++)
			GetButton<ButtonGadget>( m_pRepairQueue, i )->setGadget( pShip->repair( i ) );

		// remove excess buttons
		m_pRepairQueue->cullChildren( pShip->repairCount() );
	}

	//----------------------------------------------------------------------------

	// update the active zone
	pContext->setActiveZone( ViewGame::s_CameraPosition );

	// create our render context
	RenderContext engineeringContext( context );
	engineeringContext.setPosition( ViewGame::s_CameraPosition );
	engineeringContext.setFrame(  ViewGame::s_CameraFrame );
	engineeringContext.setTime( pContext->tick() * TICK_DURATION_S );

	//----------------------------------------------------------------------------

	// render the universe
	pContext->render( engineeringContext, 
		~ViewGame::s_CameraFrame, ViewGame::s_CameraFrame * (-ViewGame::s_CameraPosition) );
}


//----------------------------------------------------------------------------

ViewEngineering::ButtonGadget::ButtonGadget() 
{
	m_Flags |= NOCLIP;
	m_Style = HAS_BACK | EFFECT_HIGHLIGHT | EFFECT_FADEIN | SMOOTH_LL | LOCK_ICON_SIZE;
	m_Alpha = 0.75f;

	setWindow( RectInt( PointInt(0,0), SizeInt( 32, 24 ) ) );
	setEnable( false );
}


void ViewEngineering::ButtonGadget::onRender( RenderContext & context, const RectInt & window )
{
	WindowButton::onRender( context, window );

	if ( enabled() )
	{
		DisplayDevice * pDisplay = context.display();
		ASSERT( pDisplay );
		Font * pFont = windowStyle()->font();
		ASSERT( pFont );

		// display damage percentage
		String status;
		status.format( "%d%%", int( m_rGadget->damageRatioInv() * 100) );

		SizeInt stringSize( pFont->size( status ) );
		PointInt stringPos( window.m_Right - stringSize.width, window.top );
		Font::push( pDisplay, pFont, stringPos, status, YELLOW );

		// display the damage bar
		if ( m_rGadget->damage() > 0 )
		{
			if ( fmod( activeTime(), 1.0f ) < 0.5f )		// make the bar blink
			{
				float damage = m_rGadget->damageRatioInv();
				RectInt bar( window.m_Left, window.m_Bottom + 1, 
					window.m_Right - (window.width() * (1.0f - damage)), window.m_Bottom + 3 );

				Color barColor( 255 * (1.0f - damage), 255 * damage,0,255 );
				PrimitiveMaterial::push( pDisplay, PrimitiveMaterial::NONE );
				PrimitiveWindow::push( pDisplay, bar, DAMAGE_BAR_UV, barColor );
			}
		}

		// display white blinking box around gadget currently at the top of the queue
		NounShip * pShip = WidgetCast<NounShip>( m_rGadget->parentBody() );
		if ( pShip != NULL && pShip->repairCount() > 0 && pShip->repair( 0 ) == m_rGadget && fmod( activeTime(), 1.0f ) < 0.5f )
			renderGlow( context );
	}
}

void ViewEngineering::ButtonGadget::onButtonUp()
{
	// move this gadget to the top of the repair queue
	if ( enabled() )
		Verb::Ref( new VerbRepairGadget( m_rGadget, NounShip::NOW ) );	

	WindowButton::onButtonUp();
}

void ViewEngineering::ButtonGadget::setGadget( NounGadget * pGadget )
{
	m_rGadget = pGadget;

	setName( pGadget->name() );
	setIcon( pGadget->icon() );
	setTip( String().format("%s\n%d%%", pGadget->nounContext()->name(), int( pGadget->damageRatioInv() * 100) ) );
	setEnable( true );
}

//----------------------------------------------------------------------------
// EOF

