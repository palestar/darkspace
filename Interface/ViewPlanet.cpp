/*
	ViewPlanet.cpp
	(c)2000 Palestar, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Display/PrimitiveMaterial.h"
#include "Display/PrimitiveLineStrip.h"
#include "Display/PrimitiveSetTransform.h"
#include "Display/PrimitiveTriangleFan.h"
#include "Math/Helpers.h"
#include "DarkSpace/VerbLockPlanet.h"
#include "DarkSpace/VerbBuildUnit.h"
#include "DarkSpace/VerbTradeRoute.h"
#include "Interface/ViewPlanet.h"
#include "Interface/ViewGame.h"
#include "DarkSpace/NounPlanet.h"
#include "DarkSpace/NounStructure.h"
#include "DarkSpace/NounDropTarget.h"
#include "DarkSpace/NounUnit.h"
#include "DarkSpace/CargoGadget.h"
#include "Interface/GameDocument.h"
#include "Interface/ButtonContact.h"
#include "Interface/ButtonStructure.h"
#include "Interface/ButtonUnit.h"
#include "Interface/ButtonBuildUnit.h"
#include "Interface/ViewTactical.h"

//----------------------------------------------------------------------------

static Constant			UPDATE_PLANET_CONTACTS( "UPDATE_PLANET_CONTACTS", 1.0f );
static Constant			LEFT_TARGET_TIME( "LEFT_TARGET_TIME", 0.5f );

const float				CAMERA_SNAP_TIME = 1.0f;					// how long does it take for the camera to snap fully into the tactical frame/position
const float				CAMERA_PITCH_RATE = PI / 512;
const float				CAMERA_YAW_RATE = PI / 256;
const float				MIN_CAMERA_PITCH = -PI / 16;
const float				MAX_CAMERA_PITCH = PI / 2;

const float				FRONT_PLANE = 1.0f;
const float				BACK_PLANE = 5000000.0f;

const float				CONTACT_CIRCLE_RADIUS = 0.40f;
const Color				CONTACT_CIRCLE_COLOR( 88,224,72,128 );

const float				HUD_Z = 2.0f;
const Vector3			HUD_N(0,0,-1);
const Color				HUD_D(255,255,255,200);
const Plane				HUD_PLANE( HUD_N, HUD_Z );

const int				CONTACT_INSET = 64;
const int				ARROW_SIZE = 4;
const int				PROJECTILE_SIZE = 6;
const int				DROP_TARGET_SIZE = 4;
const int				GROUND_TARGET_SIZE = 8;
const int				CURSOR_DIAMOND_SIZE = 10;
const int				TEXT_OFFSET = 12;
const RectInt			CONTACT_RECT( -8,-8,8,8 );
const RectInt			CONTACT_UNIT_RECT(-8,-8,8,8 );
const RectInt			CURSOR_RECT( -9,-9,9,9 );
const RectInt			TARGET_RECT( -9,-9,9,9 );
const RectFloat			CONTACT_BOX_UV(0,0,1,1);
const RectFloat			CONTACT_RIGHT_UV(0,0,1,1);
const RectFloat			CONTACT_LEFT_UV(1,1,0,0);
const Vector3			ZERO_N( 0, 0, 0 );

const int				CONTACT_RECTICLE_HSIZE = 8;
const int				CONTACT_RECTICLE_VSIZE = 4;
const int				CURSOR_RECTICLE_HSIZE = 12;
const int				CURSOR_RECTICLE_VSIZE = 6;

template<class T>
inline T sqr( T number )
{
	return number * number;
}

const float UPDATE_CHATTER_RATE = 2.0f;
const float CHATTER_VOLUME = 0.25f;
const int	CHATTER_COUNT = 10;
const float TARGET_LOCK_RATE = 0.75f;
const float CAMERA_TARGET_YAW = PI;

//----------------------------------------------------------------------------

template<class T>
T * GetChild( NodeWindow * pWindow, int i )
{
	if ( i < pWindow->childCount() )
		return WidgetCast<T>( pWindow->child(i) );

	return NULL;
}

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( ViewPlanet, WindowView::View );
REGISTER_FACTORY_KEY( ViewPlanet, 4341034859765828113 );

ViewPlanet::ViewPlanet() :
	m_bLeftDown( false ),
	m_fLeftDownTime( 0.0f ),
	m_bRightDown( false ),
	m_fRightDownTime( 0.0f )
{
	m_CameraYaw = PI / 4;
	m_CameraPitch = PI / 4;
	m_CursorPosition2 = PointInt( 256, 256 );
}

//----------------------------------------------------------------------------

void ViewPlanet::onActivate()
{
	//{{BEGIN_DATA_INIT
	//END_DATA_INIT}}

	m_Update = 1.0f;
	m_Planet = NULL;
}

void ViewPlanet::onDeactivate()
{
	onHide();

	m_Planet = NULL;
	m_Contacts.release();
	m_DropTargets.release();
}

//----------------------------------------------------------------------------

void ViewPlanet::onHide()
{
	if ( m_bLeftDown || m_bRightDown )
	{
		setCursorState( POINTER );
		m_bLeftDown = m_bRightDown = false;
	}
}

void ViewPlanet::onUpdate( float t )
{
	if ( m_bLeftDown )
		m_fLeftDownTime += t;
	if ( m_bRightDown )
		m_fRightDownTime += t;

	GameDocument * pDoc = (GameDocument *)document();
	if (! pDoc )
		return;
	GameContext * pContext = pDoc->context();
	if (! pContext )
		return;
	NounShip * pShip = pDoc->ship();
	if (! pShip )
		return;

	// resolve the current planet from the current target
	NounPlanet * pPlanet = pDoc->planetTarget();

	// get closest planet in ship contact list
	if ( pPlanet == NULL )
	{
		float bestDistance = 2500000.0f;

		for(int i=0;i<pShip->contactCount();i++)
		{
			Noun * pContact = pShip->contact( i );
			if ( WidgetCast<NounPlanet>( pContact ) )
			{
				NounPlanet * pCheck = (NounPlanet *)pContact;
				
				float distance = (pShip->worldPosition() - pCheck->worldPosition()).magnitude();
				if ( distance < bestDistance )
				{
					pPlanet = pCheck;
					bestDistance = distance;
				}
			}
		}

		// set our target no matter if pPlanet is NULL or not...
		pDoc->setTarget( pPlanet );
	}

	if ( m_Planet != pPlanet )
	{
		// set the new planet, may be NULL
		m_Planet = pPlanet;
		// release all the contacts and drop targets
		m_Contacts.release();
		m_DropTargets.release();
		// reset the camera time
		GameDocument::s_fCamera = 0.0f;

		if ( m_Planet.valid() )
		{
			// set the current focus on the planet
			pDoc->setFocus( pPlanet );
			// rebuild the drop target list
			updateDropTargets();
			// update our contacts
			updateContacts();
		}
		else
		{
			// no planet is selected, move focus back to players ship
			pDoc->setFocus( pShip );
		}
	}

	// update the planet contacts on a regular basis
	if ( m_Planet.valid() )
	{
		m_UpdateContacts += t;
		if ( (m_UpdateContacts > UPDATE_PLANET_CONTACTS || m_bLeftDown || m_bRightDown) && GameDocument::s_fCamera >= CAMERA_SNAP_TIME )
			updateContacts();
	}
}

bool ViewPlanet::onMessage( const Message & msg )
{
	//{{BEGIN_MSG_MAP
	MESSAGE_MAP( WB_BUTTONUP, 3265800966, onButtonResearchAll);
	MESSAGE_MAP( WB_BUTTONUP, 1475929700, onButtonStopResearch);
	//END_MSG_MAP}}

	return false;
}


void ViewPlanet::onRender( RenderContext & context, const RectInt & w )
{
	GameDocument * pDoc = (GameDocument *)document();
	if (! pDoc )
		return;
	GameContext * pContext = pDoc->context();
	if (! pContext )
		return;
	DisplayDevice * pDisplay = context.display();
	if (! pDisplay )
		return;
	Font * pFont = window()->windowStyle()->font();
	if (! pFont )
		return;
	NounShip * pShip = pDoc->ship();
	if (! pShip )
		return;

	Noun * pTarget = pDoc->target();
	Noun * pCursorTarget = pDoc->cursorTarget();

	// update the active zone
	pContext->setActiveZone( GameDocument::s_vCamera );

	// update the camera
	updateCamera( context.elapsed() );		

	// get the camera frame and position
	Matrix33 cameraFrame( GameDocument::s_mCamera );
	Vector3 cameraPosition( GameDocument::s_vCamera );

	// end any previous scene so we can begin a new one..
	context.endScene();		
	// create our render context
	RenderContext::SaveState state( context );
	context.setProjection( context.window(), context.fov(), FRONT_PLANE, BACK_PLANE );
	context.setTime( pContext->tick() * TICK_DURATION_S );
	context.setPosition( cameraPosition );
	context.setFrame( cameraFrame );
	context.beginScene();
	// set the ambient light
	pDisplay->setAmbient( UNIVERSE_AMBIENT );

	// render the universe
	pContext->render( context, Matrix33::IDENTITY, Vector3::ZERO );

	if (! m_Planet.valid() )
		return;

	// hide drop targets
	bool hideDrop = GameDocument::s_fCamera < CAMERA_SNAP_TIME;

	// render the contacts
	Noun::Ref		bestTarget; 
	int				bestTargetRange = 5000;

	RectInt renderBox( context.window() );
	RectInt	contactBox( renderBox );
	renderBox.inset( 5 );
	contactBox.inset( CONTACT_INSET );
	contactBox.bottom -= 160;

	PrimitiveMaterial::Ref pAlphaMaterial = PrimitiveMaterial::create( pDisplay, PrimitiveMaterial::ALPHA, false );
	context.push( pAlphaMaterial );

	context.pushWorldTransform( m_Planet->worldFrame(), m_Planet->worldPosition() );

	for(int i=0;i<m_Contacts.size();i++)
	{
		Noun * pContact = m_Contacts[i];
		if ( pContact == NULL )
			continue;

		// get the world position of the contact
		Vector3 contactWorldPosition( pContact->worldPosition() );
		// get the viewspace position of the target
		Vector3 contactPosition( cameraFrame * (contactWorldPosition - cameraPosition ) );
		// clip to the viewspace
		context.clip( contactPosition );
		// project into screen space
		context.project( contactPosition );

		// get the contact 2D position
		PointInt contactPoint( contactPosition.x, contactPosition.y );

		bool isDropTarget = false;
		bool isTarget = pContact == pTarget;
		bool isCursorTarget = pContact == pCursorTarget;

		// draw the contact indicators
		if ( WidgetCast<NounDropTarget>( pContact) )
		{
			if (! renderBox.inRect( contactPoint ) )
				continue;
			if ( hideDrop )
				continue;

			Color contactColor( YELLOW );
			contactColor.a = 64;

			// draw the drop target diamond
			PrimitiveLineStripDTL::createPushDiamond( pDisplay, contactPoint, DROP_TARGET_SIZE, contactColor );
			isDropTarget = true;
		}
		else if ( WidgetCast<NounStructure>( pContact ) )
		{
			NounStructure * pStructure = (NounStructure *)pContact;

			Color contactColor( pStructure->color() );
			if ( !pStructure->active() && ((pDoc->tick() % 10) < 6) )
				contactColor.a = 128;		// inactive structures are blinking
			if ( isTarget || isCursorTarget )
				contactColor += GREY;

			ViewTactical::renderRecticle( pDisplay, contactPoint, CONTACT_RECTICLE_HSIZE, CONTACT_RECTICLE_VSIZE, contactColor );
		}
		else
		{
			bool isFriend = pShip->isFriend( pContact );
			bool isEnemy = pShip->isEnemy( pContact );

			Color contactColor( isFriend ? GREEN : isEnemy ? RED : YELLOW );
			if ( isTarget || isCursorTarget )
				contactColor += GREY;

			ViewTactical::renderRecticle( pDisplay, contactPoint, CONTACT_RECTICLE_HSIZE, CONTACT_RECTICLE_VSIZE, contactColor );
		}


		// show brackets around current cursor target
		if ( isCursorTarget )
			ViewTactical::renderRecticle( pDisplay, contactPoint, CURSOR_RECTICLE_HSIZE, CURSOR_RECTICLE_VSIZE, GREY );
		// show current target
		if ( isTarget )
			ViewTactical::renderRecticle( pDisplay, contactPoint, CURSOR_RECTICLE_HSIZE, CURSOR_RECTICLE_VSIZE, WHITE );

		if ( (isCursorTarget || isTarget ) && !isDropTarget )
		{
			String targetInfo( pContact->name() );

			PointInt targetPos( contactPoint );
			if ( (targetPos.x - TEXT_OFFSET) > ((contactBox.left + contactBox.right) >> 1) )
			{
				SizeInt infoSize( pFont->size(targetInfo) );
				targetPos.x -= (infoSize.width + TEXT_OFFSET);
			}
			else
				targetPos.x += TEXT_OFFSET;


			Font::push( pDisplay, pFont, targetPos, targetInfo, WHITE );
			PrimitiveMaterial::push( pDisplay, pAlphaMaterial );
		}

		// check for best target
		int contactRange = sqr(contactPoint.m_X - m_CursorPosition2.m_X) + sqr(contactPoint.m_Y - m_CursorPosition2.m_Y);
		if ( contactRange < bestTargetRange )
		{
			bestTarget = pContact;
			bestTargetRange = contactRange;
		}
	}

	// set the best target
	pDoc->setCursorTarget( bestTarget );

	// if a enemy object is near, render a red box along the edge of the window..
	if ( ViewGame::sm_pViewGame != NULL )
		ViewGame::sm_pViewGame->renderViewGame( context, w );
}

bool ViewPlanet::onCursorMove( const RectInt & window, 
						const PointInt & position,		// window and position are local coordinates
						const PointInt & delta )
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );

	if ( m_bLeftDown || m_bRightDown )
	{
		setCursorState( HIDDEN );

		m_CameraYaw += CAMERA_YAW_RATE * delta.m_X;
		m_CameraPitch = Clamp( m_CameraPitch + (CAMERA_PITCH_RATE * delta.m_Y), 
			MIN_CAMERA_PITCH, MAX_CAMERA_PITCH );

		return true;
	}

	ViewGame::sm_pViewGame->updateAction( true );
	m_CursorPosition2 = position;
	return false;
}

bool ViewPlanet::onLeftDown( const RectInt & window, const PointInt & position )
{
	m_bLeftDown = true;
	m_fLeftDownTime = 0.0f;

	return true;
}

bool ViewPlanet::onLeftUp( const RectInt & window, 	const PointInt & position )
{
	m_bLeftDown = false;

	GameDocument * pDoc = (GameDocument *)document();
	if (! pDoc )
		return false;

	if ( pDoc->cursorTarget() != NULL )
		pDoc->setTarget( pDoc->cursorTarget() );
	else
		pDoc->setTarget( m_Planet );
	ViewGame::sm_pViewGame->updateAction( true );
	m_CameraLockTime = 1.0f;

	return true;
}

bool ViewPlanet::onRightDown( const RectInt & window, const PointInt & position )
{
	m_bRightDown = true;
	m_fRightDownTime = 0.0f;

	return true;
}

bool ViewPlanet::onRightUp( const RectInt & window, const PointInt & position )
{
	m_bRightDown = false;
	m_CameraLockTime = 1.0f;

	if ( m_fRightDownTime < 0.5f )
		ViewGame::sm_pViewGame->doAction( true );

	return true;
}

//----------------------------------------------------------------------------

bool ViewPlanet::onButtonStopResearch(const Message & msg)
{
	return true;
}

bool ViewPlanet::onButtonResearchAll(const Message & msg)
{
	return true;
}

//----------------------------------------------------------------------------

void ViewPlanet::updateDropTargets()
{
	if (! m_Planet.valid() )
		return;
	GameContext * pContext = ((GameDocument *)document())->context();
	if (! pContext )
		return;

	// rebuild the drop target list
	m_DropTargets.allocate( m_Planet->hexCount() );
	for(int i=0;i<m_Planet->hexCount();i++)
		m_DropTargets[ i ] = new NounDropTarget( m_Planet, i );
}

void ViewPlanet::updateContacts()
{
	if (! m_Planet.valid() )
		return;
	NounShip * pShip = ((GameDocument *)document())->ship();
	if (! pShip )
		return;

	int myFactionId = pShip->factionId();

	m_Contacts.release();
	m_Contacts.push( m_Planet.pointer() );

	// get the direction from which we're viewing the planet
	Vector3 delta( m_Planet->worldFrame() * (GameDocument::s_vCamera - m_Planet->worldPosition()) );
	delta.normalize();

	std::set< int > UsedHexes;

	// add contact indicators for objects we can currently see
	for(int i=0;i<m_Planet->childCount();i++)
	{
		NounGame * pChild = WidgetCast<NounGame>( m_Planet->child(i) );
		if ( !pChild || (pChild->position() | delta) < 0.0f )
			continue;		// wrong side of the planet
		if (! pChild->isDetected( myFactionId ) )
			continue;		// isn't detected..
		m_Contacts.push( pChild );

		if ( WidgetCast<NounStructure>( pChild ) )
			UsedHexes.insert( ((NounStructure *)pChild)->hex() );
		else if ( WidgetCast<NounUnit>( pChild ) )
			UsedHexes.insert( ((NounUnit *)pChild)->hex() );
	}
	
	// in some cases, updateDropTargets() might not get called before updateContacts().
	if ( m_DropTargets.size() == m_Planet->hexCount() )
	{
		for(int i=0;i<m_Planet->hexCount();++i)
		{
			const NounPlanet::Hex & hex = m_Planet->hex( i );
			if ( (hex.position | delta) < 0.0f )
				continue;		// wrong side of the planet
			if ( UsedHexes.find( i ) != UsedHexes.end() )
				continue;		// hex is occupied, don't show drop target..
			
			ASSERT( m_DropTargets[i]->hex() == i );
			m_Contacts.push( m_DropTargets[ i ].pointer() );
		}
	}

	m_UpdateContacts = 0.0f;
}

void ViewPlanet::updateCamera( float t )
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );

	if ( m_Planet.valid() && !m_bLeftDown && !m_bRightDown )
	{
		// rotate the camera frame if a child of the planet is selected as the target
		Noun * pTarget = pDoc->target();

		bool isChild = (pTarget != NULL && pTarget->parent() == m_Planet.pointer()) || 
			(WidgetCast<NounDropTarget>( pTarget ) && ((NounDropTarget *)pTarget)->planet() == m_Planet);

		if ( isChild )
		{
			Vector3 targetDirection( m_Planet->worldPosition() - pTarget->worldPosition() );
			targetDirection.normalize();

			// if this is a new target, set the time so the camera spins towards the child object
			if ( m_CameraLock != pTarget )
			{
				m_CameraLock = pTarget;
				m_CameraLockTime = 1.0f;
			}

			// get the angle towards the target
			float targetYaw = atan2( targetDirection.x, targetDirection.z );
			if ( m_CameraLockTime > 0.0f )
			{
				m_CameraLockTime -= t;

				// normalize the camera yaw 
				float delta = AngleDelta( NormalizeAngle( m_CameraYaw ), targetYaw );
				delta -= (delta * m_CameraLockTime );

				m_CameraYaw += delta;
			}
			else
				m_CameraYaw = targetYaw;

			Matrix33 cameraFrame( true );
			cameraFrame.rotate( m_CameraPitch, m_CameraYaw, 0 );

			float dot = cameraFrame.j | targetDirection;
			if ( fabs( dot ) > 0.25f )
			{
				if ( dot > 0.0f )
					dot -= 0.25f;
				else
					dot += 0.25f;

				// pitch the camera
				m_CameraPitch = Clamp( m_CameraPitch - (dot * (PI * t)), MIN_CAMERA_PITCH, MAX_CAMERA_PITCH);
			}
		}
	}

	Noun * pFocus = m_Planet;
	if ( pFocus == NULL )
		pFocus = pDoc->ship();
	ASSERT( pFocus );

	Matrix33 cameraFrame( true );
	cameraFrame.rotate( m_CameraPitch, m_CameraYaw, 0 );
	Vector3 cameraTarget( pFocus->worldPosition() );
	Vector3 cameraPosition( cameraTarget - (cameraFrame.k * (pFocus->radius() * 4.0f) ) );

	if ( GameDocument::s_fCamera < CAMERA_SNAP_TIME )
	{
		float d = GameDocument::s_fCamera / CAMERA_SNAP_TIME;

		// move the camera
		Vector3 deltaPosition( cameraPosition - GameDocument::s_vCamera );
		GameDocument::s_vCamera += deltaPosition * d;

		// update the camera frame
		GameDocument::s_mCamera.i += (cameraFrame.i - GameDocument::s_mCamera.i) * d;
		GameDocument::s_mCamera.j += (cameraFrame.j - GameDocument::s_mCamera.j) * d;
		GameDocument::s_mCamera.k += (cameraFrame.k - GameDocument::s_mCamera.k) * d;
		GameDocument::s_mCamera.orthoNormalizeXY();

		/*
		// update contacts while camera is moving quickly
		if ( m_Planet.valid() )
			updateContacts();
		*/
	}
	else
	{
		// animation is over, snap directly to the desired frame/position
		GameDocument::s_mCamera = cameraFrame;
		GameDocument::s_vCamera = cameraPosition;
	}
}

//----------------------------------------------------------------------------
// EOF


