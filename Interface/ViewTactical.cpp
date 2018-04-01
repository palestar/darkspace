/*
	ViewTactical.cpp
	(c)2000 Palestar, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Audio/Sound.h"
#include "System/Keyboard.h"
#include "Standard/Settings.h"
#include "Math/Helpers.h"
#include "Math/Quat.h"
#include "Display/PrimitiveSetTransform.h"
#include "Display/PrimitiveLineStrip.h"
#include "Display/PrimitiveTriangleFan.h"
#include "DarkSpace/VerbDistress.h"
#include "DarkSpace/NounBody.h"
#include "World/NounZoneTarget.h"
#include "DarkSpace/NounProjectile.h"
#include "DarkSpace/NounJumpGate.h"
#include "DarkSpace/VerbOrderShip.h"
#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetWeapon.h"
#include "DarkSpace/GadgetBeamWeapon.h"
#include "DarkSpace/GadgetShield.h"
#include "DarkSpace/VerbSelfDestruct.h"
#include "DarkSpace/Resource.h"
#include "DarkSpace/CargoEnhancement.h"
#include "DarkSpace/NounEnhancement.h"
#include "DarkSpace/VerbLoad.h"

#include "Interface/GameDocument.h"
#include "Interface/ViewGame.h"
#include "Interface/ButtonEnhancement.h"
#include "Interface/WindowEnhancement.h"
#include "Interface/ViewTactical.h"


//----------------------------------------------------------------------------

const float		ZOOM_SCALE[] = 
{
	2.0f,
	4.0f,
	8.0f,
	16.0f,
	32.0f,
	64.0f,
	128.0f,
	256.0f,
	512.0f,
	1024.0f,
	2048.0f,
};
const int		ZOOM_LEVELS = sizeof(ZOOM_SCALE) / sizeof(ZOOM_SCALE[0]);
const int		DISTRESS_MESSAGE_DELAY = TICKS_PER_SECOND * 5;

const float		PIX2 = PI * 2;

static Constant		DROP_TARGET_RANGE( "DROP_TARGET_RANGE", 500.0f );

Matrix33		ViewTactical::sm_CameraFrame;		// destination camera frame
Vector3			ViewTactical::sm_CameraPosition;
Vector3			ViewTactical::sm_AdjustPosition;

static Constant		LEFT_TARGET_TIME( "LEFT_TARGET_TIME", 0.5f );
static Constant		SET_HEADING_RATE( "SET_HEADING_RATE", PI / 3 );
static Constant		SET_VELOCITY_RATE( "SET_VELOCITY_RATE", 10.0f );
static Constant		SET_YAW_RATE( "SET_YAW_RATE", 2.0f );
static Constant		TACTICAL_UPDATE_BUTTONS( "TACTICAL_UPDATE_BUTTONS", 2.0f );
static Constant		TACTICAL_CONTACT_UPDATE( "TACTICAL_CONTACT_UPDATE", 1.0f );
static Constant		TACTICAL_SCANNER_RANGE( "TACTICAL_SCANNER_RANGE", 5000.0f );
static Constant		SHIELD_ARMOR_LAYER( "SHIELD_ARMOR_LAYER", 50000.0f );

// copied from NounPlanet.cpp
static Constant		PLANET_SCANNER_RANGE( "PLANET_SCANNER_RANGE", 2000.0f );

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( ViewTactical, WindowView::View );
REGISTER_FACTORY_KEY( ViewTactical, 4096692533678357889 );

ViewTactical::ViewTactical() :
	m_bLeftDown( false ),
	m_fLeftDownTime( 0.0f ),
	m_bRightDown( false ),
	m_fRightDownTime( 0.0f )
{
#ifdef _DEBUG
	Settings settings( "ClientD" );
#else
	Settings settings( "Client" );
#endif

	m_CursorPosition2 = PointInt( 0,0 );
	sm_CameraFrame = Matrix33( true );
	sm_CameraPosition = Vector3( 0.0f );
	m_CameraYaw = PI / 4;
	m_CameraPitch = PI / 4;
	m_LastCameraMode = m_CameraMode = TARGET;
	m_CameraZoom = 50.0f;
	m_CameraZoomLevel = 3;

	m_fYaw = m_fYawV = 0.0f;
	YawStatePressed m_yawState = YawStatePressed::None;
	m_SetHeading = m_SetHeadingV = 0.0f;
	m_SetVelocity = m_SetVelocityV = 0.0f;
	m_BeginControl = false;
	m_UpdateControl = false;
	m_fUpdateContacts = 0.0f;
	m_TargetTime = 0.0f;
	m_ScannerRange = TACTICAL_SCANNER_RANGE;
	m_Adjust = false;

	m_bYawControl = true;
	m_bShowShipStatus = true;
	m_bShowFriendlyStatus = true;
	m_bShowTurnStatus = true;
	m_bShowContacts = true;

	m_SelfDestruct = false;
	m_SelfDestructConfirm = false;
	m_SelfDestructTime = 0;

	m_fUpdateButtons = 0;

	m_pDoc = NULL;
}

//----------------------------------------------------------------------------

void ViewTactical::onActivate()
{
	//{{BEGIN_DATA_INIT
	DATA_MAP( m_pDetectedStatus, WindowButton, "DetectedStatus" );
	m_pButtonPointDefense = WidgetCast<WindowButton>( window()->findNode( "ButtonPointDefense" ) );
	m_pButtonShields = WidgetCast<WindowButton>( window()->findNode( "ButtonShields" ) );
	m_pHeadingText = WidgetCast<WindowText>( window()->findNode( "HeadingText" ) );
	m_pHeadingBar = WidgetCast<SliderHUD>( window()->findNode( "HeadingBar" ) );
	m_pCargoButtons = WidgetCast<NodeWindow>( window()->findNode( "CargoButtons" ) );
	m_pGadgetLayout = WidgetCast<NodeWindow>( window()->findNode( "GadgetLayout" ) );
	m_pVelocityText = WidgetCast<WindowText>( window()->findNode( "VelocityText" ) );
	m_pVelocityBar = WidgetCast<SliderHUD>( window()->findNode( "VelocityBar" ) );
	//END_DATA_INIT}}

	// grab the enhancement buttons and hide them


#ifdef _DEBUG
	Settings settings( "ClientD" );
#else
	Settings settings( "Client" );
#endif

	// load settings
	m_bYawControl = settings.get("YawControl", 1 ) != 0;
	m_bShowShipStatus = settings.get("ShowShipStatus", 1 ) != 0;
	m_bShowFriendlyStatus = settings.get("ShowFriendlyStatus", 1 ) != 0;
	m_bShowTurnStatus = settings.get("ShowTurnStatus", 1 ) != 0;
	m_bShowContacts = settings.get("ShowContacts", 1 ) != 0;

	// get some pointers
	m_pDoc = WidgetCast<GameDocument>( document() );
	if (! m_pDoc )
		return;
	NounShip * pShip = m_pDoc->ship();
	if (! pShip )
		return;

	// set the times for various updates
	m_UpdateChatter = 0.0f;
	m_UseGadgetTick = 0;
	m_bShowContacts = true;
	m_Contacts.release();
	m_nDistressTick = m_pDoc->tick() + DISTRESS_MESSAGE_DELAY;

	const Color DARK_GREY( 80, 80, 80, 255 );

	// setup the velocity bar
	m_pVelocityBar->setBaseColor( DARK_GREY );	
	m_pVelocityBar->setBarColor( DARK_YELLOW );		
	m_pHeadingBar->setBaseColor( DARK_GREY );
	m_pHeadingBar->setBarColor( DARK_GREEN );

	m_fYaw = pShip->yaw();
	m_fYawV = 0.0f;
	m_SetHeading = pShip->heading();
	m_SetHeadingV = 0.0f;
	m_SetVelocity = pShip->velocity();
	m_SetVelocityV = 0.0f;
	m_Adjust = false;

	m_SelfDestruct = false;
	m_SelfDestructConfirm = false;
	m_SelfDestructTime = 0;

	// hide detected indicator on activation..
	m_pDetectedStatus->setVisible( false );
}

void ViewTactical::onDeactivate()
{
#ifdef _DEBUG
	Settings settings( "ClientD" );
#else
	Settings settings( "Client" );
#endif

	onHide();

	// save settings
	settings.put("YawControl", m_bYawControl );
	settings.put("ShowShipStatus", m_bShowShipStatus );
	settings.put("ShowFriendlyStatus", m_bShowFriendlyStatus );
	settings.put("ShowTurnStatus", m_bShowTurnStatus );
	settings.put("ShowContacts", m_bShowContacts );

	m_CameraLock = NULL;
	m_Orbiting = NULL;
	m_Contacts.release();
	m_OldContacts.release();
	m_pSelfDestructMessage = NULL;
}

const float SCROLL_LOW_MARGIN	= 0.05f;
const float SCROLL_HIGH_MARGIN	= 0.95f;
const float CAMERA_DOLLY_RATE	= 10.0f;				// how fast does the camera scroll
const float MIN_CAMERA_PITCH = -PI / 16;
const float MAX_CAMERA_PITCH = PI / 2;
const float MIN_CAMERA_ZOOM = 10.0f;
const float MAX_CAMERA_ZOOM = 200.0f;
const float CONTACT_RADIUS = 5000.0f;
const float DECAY_RATE = 3.0f;
const int	CONTROL_LAG = 5;
const float CAMERA_SNAP_TIME = 1.0f;					// how long does it take for the camera to snap fully into the tactical frame/position

void ViewTactical::onUpdate( float t )
{
	GameContext * pContext = m_pDoc->context();
	ASSERT( pContext );

	NounShip * pShip = m_pDoc->ship();
	if (! pShip )
		return;

	Noun * pFocus = m_pDoc->focus();
	if (! pFocus )
		return;

	Noun * pTarget = m_pDoc->target();

	//----------------------------------------------------------------------------

	float targetZoom = Max( pFocus->radius(), 1.0f) * ZOOM_SCALE[ m_CameraZoomLevel ];
	if ( targetZoom > 6000 )
		targetZoom = 6000;
	ASSERT( !_isnan( targetZoom ) );
	m_CameraZoom += (targetZoom - m_CameraZoom) * t;
	ASSERT( !_isnan( m_CameraZoom ) );

	// reset the lock time any time the current target changes
	if ( m_CameraLock != pTarget )
	{
		m_CameraLock = pTarget;
		m_CameraLockTime = 0.0f;
	}
	m_CameraLockTime += t;

	if ( m_bLeftDown )
		m_fLeftDownTime += t;
	if ( m_bRightDown )
		m_fRightDownTime += t;

	// calculate the snap strength, 1.0f is full snap, 0.0f is no snap
	float snap = Min( m_CameraLockTime, CAMERA_SNAP_TIME ) / CAMERA_SNAP_TIME;

	// destination camera frame
	switch( m_CameraMode )
	{
	case FREE:		// free camera mode
		{
			Vector3	cameraTarget( pFocus->worldPosition() );
			sm_CameraFrame = Matrix33( m_CameraPitch, m_CameraYaw, 0 );
			sm_CameraPosition = cameraTarget - (sm_CameraFrame.k * m_CameraZoom);

			m_CameraLock = NULL;
		}
		break;
	case TARGET:	// this mode keeps the focus and target object always in view, keeping the focus at the center of the screen
		{
			Vector3 focusPosition( pFocus->worldPosition() );

			if ( pTarget != NULL && pTarget->zoneParent() != pFocus )
			{
				Vector3 targetPosition( pTarget->worldPosition() );
				Vector3 targetDirection( targetPosition - focusPosition );
				targetDirection.normalize();

				if ( pTarget->parent() == pFocus || (WidgetCast<NounDropTarget>( pTarget ) && ((NounDropTarget *)pTarget)->planet() == pFocus) )
					targetDirection = -targetDirection;

				// get the yaw towards the target
				float targetYaw = atan2( targetDirection.x, targetDirection.z );
				float yawDelta = AngleDelta( NormalizeAngle( m_CameraYaw), targetYaw );
				m_CameraYaw += yawDelta * snap;

				float targetPitch = -asin( targetDirection.y ) + (PI / 16);
				float pitchDelta = AngleDelta( NormalizeAngle( m_CameraPitch), targetPitch );
				m_CameraPitch += pitchDelta * snap;
			}

			sm_CameraFrame = Matrix33( m_CameraPitch, m_CameraYaw, 0 );
			sm_CameraPosition = focusPosition - (sm_CameraFrame.k * m_CameraZoom);
		}
		break;
	case TACTICAL:	// this camera mode keeps the focus and target objects always in view, the camera will zoom in and out as needed	
		{
			Vector3 focusPosition( pFocus->worldPosition() );
			if ( pTarget != NULL && pTarget != pFocus )
			{
				Vector3 targetPosition( pTarget->worldPosition() );
				Vector3 targetDirection( targetPosition - focusPosition );

				float viewingArea = m_CameraZoom * 0.5f;
				float targetDistance = targetDirection.magnitude();
				if ( targetDistance < 1.0f )
					targetDistance = 1.0f;
				else if ( targetDistance > viewingArea )
					targetDistance = viewingArea;
				
				// normalize the target direction
				targetDirection.normalize();

				// pitch camera almost overhead
				float targetPitch = PI / 4;
				float pitchDelta = AngleDelta( NormalizeAngle( m_CameraPitch), targetPitch );
				m_CameraPitch += pitchDelta * snap;
				
				// rotate the camera towards the target
				float targetYaw = atan2( targetDirection.x, targetDirection.z );
				float yawDelta = AngleDelta( NormalizeAngle( m_CameraYaw), targetYaw );
				m_CameraYaw += yawDelta * snap;

				sm_CameraFrame = Matrix33( m_CameraPitch, m_CameraYaw, 0 );
				
				// position the camera between the target and the focus point
				Vector3 midPoint( (focusPosition + ((targetDirection * targetDistance) * 0.5f)) - (sm_CameraFrame.k * m_CameraZoom) );
				Vector3 deltaPosition( midPoint - sm_CameraPosition );
				sm_CameraPosition += deltaPosition * snap;
			}
			else
			{
				float targetPitch = PI / 4;
				float pitchDelta = AngleDelta( NormalizeAngle( m_CameraPitch), targetPitch );
				m_CameraPitch += pitchDelta * t;

				sm_CameraFrame = Matrix33( m_CameraPitch, m_CameraYaw, 0 );

				Vector3 midPoint( focusPosition - (sm_CameraFrame.k * m_CameraZoom) );
				Vector3 deltaPosition( midPoint - sm_CameraPosition );
				sm_CameraPosition += deltaPosition * snap;
			}
		}
		break;
	case FOLLOW:	// this camera mode follows the focus object
		{
			Matrix33 focusFrame( pFocus->frame() );

			float targetYaw = atan2( focusFrame.k.x, focusFrame.k.z );
			float yawDelta = AngleDelta( NormalizeAngle( m_CameraYaw), targetYaw );
			m_CameraYaw += yawDelta * t;

			float targetPitch = PI / 16;
			float pitchDelta = AngleDelta( NormalizeAngle( m_CameraPitch), targetPitch );
			m_CameraPitch += pitchDelta * t;

			Vector3	cameraTarget( pFocus->worldPosition() );
			sm_CameraFrame = Matrix33( m_CameraPitch, m_CameraYaw, 0 );
			sm_CameraPosition = cameraTarget - (sm_CameraFrame.k * m_CameraZoom);
		}
		break;
	}

	if ( GameDocument::s_fCamera < CAMERA_SNAP_TIME )
	{
		// if ship is moving, keep it from jumping around while the camera is animating
		if ( m_Adjust )
		{
			Vector3 position( pFocus->worldPosition() );
			Vector3 adjust( position - sm_AdjustPosition );
			sm_AdjustPosition = position;

			GameDocument::s_vCamera += adjust;
		}
		else
		{
			sm_AdjustPosition = pFocus->worldPosition();
			m_Adjust = true;
		}

		float d = GameDocument::s_fCamera / CAMERA_SNAP_TIME;
		// move the camera
		Vector3 deltaPosition( sm_CameraPosition - GameDocument::s_vCamera );
		GameDocument::s_vCamera += deltaPosition * d;

		// update the camera frame
		GameDocument::s_mCamera.i += (sm_CameraFrame.i - GameDocument::s_mCamera.i) * d;
		GameDocument::s_mCamera.j += (sm_CameraFrame.j - GameDocument::s_mCamera.j) * d;
		GameDocument::s_mCamera.k += (sm_CameraFrame.k - GameDocument::s_mCamera.k) * d;
		GameDocument::s_mCamera.orthoNormalizeXY();

	}
	else
	{
		// animation is over, snap directly to the desired frame/position
		GameDocument::s_mCamera = sm_CameraFrame;
		GameDocument::s_vCamera = sm_CameraPosition;
		m_Adjust = false;
	}

	//----------------------------------------------------------------------------

	if (! m_BeginControl )
	{
		// hide the velocity and heading bar when in jumping or under autopilot
		if ( pShip->command() != NounShip::NOCOMMAND ||
			(pShip->jumpDrive() != NULL && (pShip->jumpDrive()->engaged() || pShip->jumpDrive()->jumping())) )
		{
			// ship is under AI control, make controls match the AI
			m_SetVelocity = pShip->velocity();
			m_SetVelocityV = 0.0f;
			m_SetHeading = pShip->heading();
			m_SetHeadingV = 0.0f;
			m_fYaw = pShip->yaw();
			m_fYawV = 0.0f;
		}
	}

	if ( m_bYawControl )
	{
		// ship is being control by yaw only
		if ( m_fYawV != 0.0f )
		{
			m_fYaw += m_fYawV * t;
			if ( m_fYaw > 1.0f )
			{
				m_fYaw = 1.0f;
				m_fYawV = 0.0f;
				m_UpdateControl = true;
			}
			else if ( m_fYaw < -1.0f )
			{
				m_fYaw = -1.0f;
				m_fYawV = 0.0f;
				m_UpdateControl = true;
			}
		}
	}
	else
	{
		// ship is being controlled by absolute heading
		if ( m_SetHeadingV != 0.0f )
			m_SetHeading = NormalizeAngle( m_SetHeading + (m_SetHeadingV * t) );
	}

	if ( m_SetVelocityV != 0.0f )
	{
		// velocity is being changed
		m_SetVelocity += m_SetVelocityV * t;
		if ( m_SetVelocity > pShip->maxVelocity() )
		{
			m_SetVelocity = pShip->maxVelocity();
			m_SetVelocityV = 0.0f;
			m_UpdateControl = true;
		}
		else if ( m_SetVelocity < 0.0f )
		{
			m_SetVelocity = 0.0f;
			m_SetVelocityV = 0.0f;
			m_UpdateControl = true;
		}
	}

	if ( m_UpdateControl )
	{
		// clear any command, if the ship has one
		if ( pShip->command() != NounShip::NOCOMMAND )
		{
			pShip->verbCommand( NounShip::NOCOMMAND, NULL );
			// set the command locally now, otherwise the code above will change the m_SetVelocity values
			pShip->setCommand( NounShip::NOCOMMAND, NULL );
		}

		if ( m_bYawControl )
			pShip->controlYawVelocity( m_fYaw, m_SetVelocity );
		else
			pShip->controlHeadingVelocity( m_SetHeading, m_SetVelocity );

		m_UpdateControl = false;
		m_BeginControl = false;
	}

	if (! m_pVelocityBar->sliding() )
		m_pVelocityBar->setPosition( m_SetVelocity / pShip->maxVelocity() );

	float normalizedVelocity = Clamp( pShip->velocity(), 0.0f, pShip->maxVelocity() );
	m_pVelocityBar->setPosition2( normalizedVelocity / pShip->maxVelocity() );
	m_pVelocityText->setText( CharString().format( "Velocity: %.1f / %.1f", normalizedVelocity, m_SetVelocity) );
	
	if ( m_bYawControl )
	{
		if (! m_pHeadingBar->sliding() )
			m_pHeadingBar->setPosition( 0.5f + (m_fYaw * 0.5f) );

		float fNormHeading = NormalizeAngle( pShip->heading() );		
		m_pHeadingBar->setPosition2( (fNormHeading + PI) / PIX2 );
		m_pHeadingText->setText( CharString().format("Heading: %.0f / %.0f", RadToDeg( fNormHeading ), RadToDeg( pShip->yaw() * pShip->maxYaw() ) ) );
	}
	else
	{
		float fNormSetHeading = NormalizeAngle( m_SetHeading );
		if (! m_pHeadingBar->sliding() )
			m_pHeadingBar->setPosition( ((fNormSetHeading + PI) / PIX2 ) );

		float fNormHeading = NormalizeAngle( pShip->heading() );		
		m_pHeadingBar->setPosition2( (fNormHeading + PI) / PIX2 );
		m_pHeadingText->setText( CharString().format("Heading: %.0f / %.0f", RadToDeg( fNormHeading ), RadToDeg( fNormSetHeading ) ) );
	}

	// update chatter timer, the sound is actually played in render()
	m_UpdateChatter += t;
	// update the targeting animation
	m_TargetTime += t;

	if ( m_Orbiting.pointer() != pShip->orbiting() )
	{
		// update our orbiting pointer
		m_Orbiting = pShip->orbiting();
		if ( pShip->inOrbit() )
			m_SetVelocity = m_SetVelocityV = 0.0f;
	}

	//----------------------------------------------------------------------------

	// update the list of clickable objects
	m_fUpdateContacts += t;
	if ( m_fUpdateContacts > TACTICAL_CONTACT_UPDATE )
		updateContacts();

	// handle self-destruct
	if ( m_SelfDestruct && m_SelfDestructConfirm && pContext->tick() >= m_SelfDestructTime)
	{
		Verb::Ref( new VerbSelfDestruct( m_pDoc->ship() ) );			
		m_SelfDestruct = m_SelfDestructConfirm = false;
	}


	m_fUpdateButtons += t;
	if ( m_fUpdateButtons > TACTICAL_UPDATE_BUTTONS )
	{
		m_fUpdateButtons = 0;

		// show shield/PD toggle button only if ship has shields/beam weapons
		bool hasShields = false;
		bool hasAutoDefense = false;
		for(int i=0;i<pShip->childCount();i++)
		{
			if ( WidgetCast<GadgetShield>( pShip->child(i) ) )
				hasShields = true;
			else if ( WidgetCast<GadgetBeamWeapon>( pShip->child(i) ) && !hasAutoDefense )
				hasAutoDefense = WidgetCast<GadgetBeamWeapon>( pShip->child(i) )->allowPointDefense();
		}
		
		m_pButtonShields->showWindow( hasShields );
		m_pButtonPointDefense->showWindow( hasAutoDefense );
	}

	// update detection indicator.. button alpha is used to indicate how hidden our ship is, the closer to 1.0
	// in alpha the better we are hidden, as we get closer to being seen the button alpha is faded to 0.0
	if ( (pShip->detectionFlags() & ~(1 << pShip->factionId())) == 0 )
	{
		// we are currently undetected by any other faction than our own, show the button and set it's alpha
		// based on how close we are to being detected by nearby contacts that we can currently detect.

		float fBestDetection = 1.0f;
		for(int i=0;i<pShip->contactCount();++i)
		{
			NounGame * pContact = WidgetCast<NounGame>( pShip->contact(i) );
			if (! pContact || pShip->isFriend( pContact ) )
				continue;		// skip null and friendly contacts..

			float fDistance = (pShip->worldPosition() - pContact->worldPosition()).magnitude();
			float fDetectRange = 0.0f;

			if ( WidgetCast<NounShip>( pContact ) )
			{
				NounShip * pEnemyShip = (NounShip *)pContact;
				float fVisibleRange = (pShip->radius() * pShip->visibility()) * pEnemyShip->view();
				float fSensorRange = pShip->signature() * pEnemyShip->sensor();
				fDetectRange = Max( fVisibleRange, fSensorRange );
			}
			else if ( WidgetCast<NounPlanet>( pContact ) )
			{
				NounPlanet * pEnemyPlanet = (NounPlanet *)pContact;
				float fVisibleRange = (pShip->radius() * pShip->visibility()) * pEnemyPlanet->view();
				float fSensorRange = pShip->signature() * pEnemyPlanet->sensor();

				fDetectRange = Max( fVisibleRange, fSensorRange );
				if (fDetectRange > PLANET_SCANNER_RANGE )
					fDetectRange = PLANET_SCANNER_RANGE;			// planets have a hard max detection range, ships go all the way out to 5k
			}
			
			float fFadeDistance = fDetectRange * 0.1f;					// we want the indicator to fade for the last 10% of the range
			float fOuterRange = fDetectRange + fFadeDistance;
			if ( fDistance < fOuterRange )
			{
				if ( fDistance > fDetectRange )
				{
					float fDetect = Clamp( (fDistance - fDetectRange) / fFadeDistance, 0.0f, 1.0f );
					if ( fDetect < fBestDetection )
						fBestDetection = fDetect;
				}
				else
					fBestDetection = -1.0f;		// stealth is 0, ship should be detected..
			}
		}

		if ( fBestDetection >= 0.0f )
		{
			m_pDetectedStatus->setTip( CharString().format( "Stealth at %d%%", (int)(fBestDetection * 100) ) );
			m_pDetectedStatus->setAlpha( (fBestDetection * 0.75f) + 0.25f );
			m_pDetectedStatus->setVisible( true  );
		}
		else
			m_pDetectedStatus->setVisible( false );		// detected, hide the indicator..
	}
	else
	{
		// we are detected, hide the detected indicator button..
		m_pDetectedStatus->setVisible( false );
	}
}

const float CAMERA_PITCH_RATE = PI / 1024;
const float CAMERA_YAW_RATE = PI / 1024;
const float CAMERA_ZOOM_RATE = 250.0f;
const float ORBIT_MASS = 100.0f;			// a body must be atleast this mass for a ship to orbit

static bool FOG_ENABLE = false;

bool ViewTactical::onMessage( const Message & msg )
{
	GameContext * pContext = m_pDoc->context();
	ASSERT( pContext );

	if ( m_pDoc == NULL || m_pDoc->ship() == NULL )
		return false;

	//{{BEGIN_MSG_MAP
	MESSAGE_MAP( WB_BUTTONUP, 2084942820, onButtonSelfDestruct);
	MESSAGE_MAP( WB_BUTTONUP, 3106761400, onButtonSendSupply);
	MESSAGE_MAP( WB_BUTTONUP, 2084942692, onButtonSendDistress);
	MESSAGE_MAP( WB_BUTTONUP, 3979261547, onButtonShields);
	MESSAGE_MAP( WB_BUTTONUP, 3962397129, onButtonPointDefense);
	MESSAGE_MAP( WS_END_SLIDE, 3648580978, onHeadingBar);
	MESSAGE_MAP( WS_BEGIN_SLIDE, 3648580978, onHeadingBar);
	MESSAGE_MAP( WS_END_SLIDE, 3350053706, onVelocityBar);
	MESSAGE_MAP( WS_BEGIN_SLIDE, 3350053706, onVelocityBar);
	//END_MSG_MAP}}

	switch( msg.message )
	{
	case HM_MOUSEWHEEL:
		if ( int( msg.wparam ) > 0 )
			m_CameraZoomLevel = Max( m_CameraZoomLevel - 1, 0 );
		else
			m_CameraZoomLevel = Min( m_CameraZoomLevel + 1, ZOOM_LEVELS - 1 );
		return true;
	case HM_SYSKEYDOWN:
		switch( msg.wparam )
		{
#ifdef _DEBUG
		case 'J':
			if ( m_pDoc->target() != NULL )
			{
				Vector3 newPosition( m_pDoc->target()->worldPosition() );
				newPosition.x += m_pDoc->ship()->radius() + m_pDoc->target()->radius() + 75.0f;
				
				m_pDoc->ship()->setPosition( newPosition );
			}
			return true;
		case 'U':	// damage gadgets
			{
				for(int i=0;i<m_pDoc->ship()->childCount();i++)
					if ( WidgetCast<NounGadget>( m_pDoc->ship()->child(i) ) )
						((NounGadget *)m_pDoc->ship()->child(i))->inflictDamage( 0, NULL, 10, DAMAGE_EMP, Vector3( 0.0f) );
			}
			return true;
#endif
		case 'Y':
			m_bYawControl = !m_bYawControl;
			m_pDoc->pushChat( CharString().format("<color;ffffff>Helm: Yaw control %s...</color>", m_bYawControl ? "ENABLED" : "DISABLED" ) );
			break;
		case 'C':
			m_bShowContacts = !m_bShowContacts;
			return true;
		case 'F':
			FOG_ENABLE = !FOG_ENABLE;
			return true;
		case 'R':
			m_pDoc->setTarget( m_pDoc->ship() );
			return true;			
		}
		break;
	case HM_KEYDOWN:
		switch( msg.wparam )
		{
		case 'X':	
			m_CameraZoomLevel = Min( m_CameraZoomLevel + 1, ZOOM_LEVELS - 1 );
			return true;
		case 'Z':
			m_CameraZoomLevel = Max( m_CameraZoomLevel - 1, 0 );
			return true;
		case HK_TAB:	// next camera view
			nextCameraMode();
			return true;
		case 191:		// "/" character on keyboard
			if ( m_bShowShipStatus && m_bShowTurnStatus && m_bShowFriendlyStatus )
			{
				m_bShowShipStatus = false;
				m_bShowFriendlyStatus = false;
				m_bShowTurnStatus = false;
				m_pDoc->pushChat( "<color;ffffff>Tactical: Tactical HUD, Helm Assistance, and ATIL disabled...</color>" );
			}
			else if ( m_bShowTurnStatus )
			{
				m_bShowFriendlyStatus = true;
				m_pDoc->pushChat( "<color;ffffff>Tactical: Allied Tactical Information Link engaged...</color>" );
			}				
			else if ( m_bShowShipStatus )
			{
				m_bShowTurnStatus = true;
				m_pDoc->pushChat( "<color;ffffff>Tactical: Helm Assistance engaged...</color>" );
			}					
			else 
			{
				m_bShowShipStatus = true;
				m_pDoc->pushChat( "<color;ffffff>Tactical: Tactical HUD engaged...</color>" );
			}
			return true;
		case HK_UP:
		case 'W':
			if ( m_SetVelocity < m_pDoc->ship()->maxVelocity() )
			{
				m_BeginControl = true;
				m_SetVelocityV = SET_VELOCITY_RATE;
			}
			return true;
		case HK_DOWN:
		case 'Q':
			if ( m_SetVelocity > 0.0f )
			{
				m_BeginControl = true;
				m_SetVelocityV = -SET_VELOCITY_RATE;
			}
			return true;
		case 'S':
			if ( m_bYawControl )
			{
				m_fYaw = m_fYawV = 0.0f;
				m_BeginControl = true;
				m_UpdateControl = true;
			}
			else
			{
				m_SetHeadingV = 0.0f;
				m_SetHeading = m_pDoc->ship()->heading();
				m_BeginControl = true;
				m_UpdateControl = true;
			}
			return true;
		case HK_LEFT:
			if ( m_bYawControl )
			{
				if ( m_yawState != YawStatePressed::Left && m_yawState != YawStatePressed::Both )
				{
					if (m_yawState == YawStatePressed::Right)
						m_yawState = YawStatePressed::Both;
					else
						m_yawState = YawStatePressed::Left;

					m_BeginControl = true;
					m_UpdateControl = true;
					m_fYaw = -1.0f;
				}
			}
			else
			{
				m_BeginControl = true;
				m_SetHeadingV = -SET_HEADING_RATE;
			}
			return true;
		case 'A':
			if ( m_bYawControl )
			{
				if ( m_yawState != YawStatePressed::Left && m_yawState != YawStatePressed::Both )
				{
					m_BeginControl = true;
					if (m_yawState == YawStatePressed::Right)
						m_yawState = YawStatePressed::Both;
					else
						m_yawState = YawStatePressed::Left;

					m_fYawV = -SET_YAW_RATE;
					if ( m_fYaw > 0.0f )
						m_fYaw = 0.0f;
				}
			}
			else
			{
				m_BeginControl = true;
				m_SetHeadingV = -SET_HEADING_RATE;
			}
			return true;
		case HK_RIGHT:
			if ( m_bYawControl )
			{
				if ( m_yawState != YawStatePressed::Right && m_yawState != YawStatePressed::Both )
				{
					if (m_yawState == YawStatePressed::Left)
						m_yawState = YawStatePressed::Both;
					else
						m_yawState = YawStatePressed::Right;

					m_BeginControl = true;
					m_UpdateControl = true;
					m_fYaw = 1.0f;
				}
			}
			else
			{
				m_BeginControl = true;
				m_SetHeadingV = SET_HEADING_RATE;
			}
			return true;
		case 'D':
			if ( m_bYawControl )
			{
				if ( m_yawState != YawStatePressed::Right && m_yawState != YawStatePressed::Both )
				{
					m_BeginControl = true;
					if (m_yawState == YawStatePressed::Left)
						m_yawState = YawStatePressed::Both;
					else
						m_yawState = YawStatePressed::Right;

					m_fYawV = SET_YAW_RATE;
					if ( m_fYaw < 0.0f )
						m_fYaw = 0.0f;
				}
			}
			else
			{
				m_BeginControl = true;
				m_SetHeadingV = SET_HEADING_RATE;
			}
			return true;
		case 219:	// '[' decrease scanner range
			if ( m_ScannerRange > 1000.0f )
				m_ScannerRange -= 1000.0f;
			return true;
		case 221:	// ']' increase scanner range
			if ( m_ScannerRange < 100000.0f )
				m_ScannerRange += 1000.0f;
			return true;
		case HK_SHIFT:
			ViewGame::sm_pViewGame->updateAction( false );
			break;
		}
		return false;

	case HM_KEYUP:
		switch( msg.wparam & 0xff )
		{
		case HK_SHIFT:
			ViewGame::sm_pViewGame->updateAction( false );
			return true;
		case 'X':
		case 'Z':
		case HK_TAB:
			return true;
		case HK_UP:
		case 'W':
		case HK_DOWN:
		case 'Q':
			if ( m_SetVelocityV != 0.0f )
			{
				m_SetVelocityV = 0.0f;
				m_UpdateControl = true;
			}
			return true;
		case HK_LEFT:
			if (m_yawState == YawStatePressed::Both) {
				m_yawState = YawStatePressed::Right;
				m_fYaw = 1.0f;
				m_UpdateControl = true;
			}
			else if (m_yawState == YawStatePressed::Left) {
				m_yawState = YawStatePressed::None;
				m_fYaw = m_fYawV = 0.0f;
				m_UpdateControl = true;
			}
			return true;
		case HK_RIGHT:
			if (m_yawState == YawStatePressed::Both) {
				m_yawState = YawStatePressed::Left;
				m_fYaw = -1.0f;
				m_UpdateControl = true;
			}
			else if (m_yawState == YawStatePressed::Right) {
				m_yawState = YawStatePressed::None;
				m_fYaw = m_fYawV = 0.0f;
				m_UpdateControl = true;
			}
			return true;
		case 'A':	
			if (m_bYawControl) {
				if (m_yawState == YawStatePressed::Both) {
					m_yawState = YawStatePressed::Right;
					m_fYawV = 1.0f;
					m_UpdateControl = true;
				}
				else if (m_yawState == YawStatePressed::Left) {
					m_yawState = YawStatePressed::None;
					m_fYawV = 0.0f;
					m_UpdateControl = true;
				}
			}
			else
			{
				if ( m_SetHeadingV != 0.0f )
				{
					m_SetHeadingV = 0.0f;
					m_UpdateControl = true;
				}
			}
			return true;
		case 'D':
			if ( m_bYawControl )
			{
				if (m_yawState == YawStatePressed::Both) {
					m_yawState = YawStatePressed::Left;
					m_fYawV = -1.0f;
					m_UpdateControl = true;
				}
				else if (m_yawState == YawStatePressed::Right) {
					m_yawState = YawStatePressed::None;
					m_fYawV = 0.0f;
					m_UpdateControl = true;
				}
			}
			else
			{
				if ( m_SetHeadingV != 0.0f )
				{
					m_SetHeadingV = 0.0f;
					m_UpdateControl = true;
				}
			}
			return true;
		}
		return false;
	}

	return false;
}

void ViewTactical::onDocumentUpdate()
{
	// document data has changed, update this view if needed
}

void ViewTactical::onHide()
{
	if ( m_bRightDown || m_bLeftDown )
	{
		setCursorState( POINTER );
		m_bRightDown = m_bLeftDown = false;
	}

	//stop selfdestruct abuse
	m_SelfDestruct = false;
	m_SelfDestructConfirm = false;
	m_SelfDestructTime = 0;
}

//----------------------------------------------------------------------------

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
const int				PROJECTILE_HSIZE = 4;
const int				PROJECTILE_VSIZE = 2;
const int				DROP_TARGET_SIZE = 4;

const int				CONTACT_RECTICLE_HSIZE = 8;
const int				CONTACT_RECTICLE_VSIZE = 4;
const int				CURSOR_RECTICLE_HSIZE = 12;
const int				CURSOR_RECTICLE_VSIZE = 6;

const int				TEXT_OFFSET = 12;
const RectInt			CONTACT_RECT( -8,-8,8,8 );
const Vector3			ZERO_N( 0, 0, 0 );

const Color				RECTICLE_BLACK(0,0,0,0);

template<class T>
inline T sqr( T number )
{
	return number * number;
}

const float UPDATE_CHATTER_RATE = 2.0f;
const float CHATTER_VOLUME = 0.35f;
const int	CHATTER_COUNT = 10;
const float TARGET_LOCK_RATE = 0.75f;
const float CAMERA_TARGET_YAW = PI;

void ViewTactical::onRender( RenderContext & context, const RectInt & w )
{
	if ( m_pDoc == NULL )
		return;
	NounShip::Ref pShip = m_pDoc->ship();
	if (! pShip )
		return;
	Noun::Ref pFocus = m_pDoc->focus();
	if (! pFocus )
		return;

	Font * pFont = window()->windowStyle()->font();
	ASSERT( pFont );
	GameContext * pContext = m_pDoc->context();
	ASSERT( pContext );
	DisplayDevice * pDisplay = context.display();
	ASSERT( pDisplay );

	// get our target pointers
	Noun * pTarget = m_pDoc->target();
	Noun * pCursorTarget = m_pDoc->cursorTarget();
	NounGadget * pGadgetTarget = m_pDoc->gadgetTarget();
	// update the active zone  
	pContext->setActiveZone( GameDocument::s_vCamera );
	// turn on the star field
	pContext->setStarFieldActive( true );
	// get the camera frame and position
	Matrix33 cameraFrame( GameDocument::s_mCamera );
	Vector3 cameraPosition( GameDocument::s_vCamera );

	// send previous scene
	context.endScene();

	// create our render context
	RenderContext::SaveState state( context );
	context.setPosition( cameraPosition );
	context.setFrame( cameraFrame );
	context.setProjection( context.window(), context.fov(), FRONT_PLANE, BACK_PLANE );
	context.setTime( pContext->tick() * TICK_DURATION_S );

	context.setShadowFocus( pFocus.valid() ? 
		context.worldToView( pFocus->worldPosition() ) : Vector3( 0, 0, 1 ), SHADOW_MAP_FOCUS );

	// unproject the cursor position
	Vector3 cursorRay( m_CursorPosition2.x, m_CursorPosition2.y, 1.0f );
	context.unProject( cursorRay );
	// transform to world
	cursorRay = ~cameraFrame * cursorRay;

	// intersect with a flat-plane along the XZ axis
	Vector3 planeIntersect;
	if ( Plane( Vector3(0,1,0), 0.0f ).intersect( cameraPosition, cursorRay, planeIntersect ) )
		m_CursorPosition = planeIntersect;

	// set the ambient light
	pDisplay->setAmbient( UNIVERSE_AMBIENT );

	// render the universe, this will call beginScene() on our RenderContext object..
	pContext->render( context, Matrix33::IDENTITY, Vector3::ZERO );

	// start the new scene for the HUD items
	context.beginScene();

	// turn off the star field
	pContext->setStarFieldActive( false );

	bool isShipAttached = pShip->parent() != NULL;
	bool bCanTargetSelf = VerbOrderShip::canOrder( pTarget, pShip );

	GadgetJumpDrive * pJump = pShip->jumpDrive();
	bool inJump = pJump != NULL ? pJump->engaged() || pJump->jumping() : false;

	if ( Keyboard::ctrlDown() )
	{
		// disable target mode, otherwise the camera spins while trying to set a manual waypoint
		m_LastCameraMode = m_CameraMode;
		m_CameraMode = FREE;

		if ( WidgetCast<NounZoneTarget>( m_pDoc->cursorTarget() ) == NULL )
		{
			m_pManualTarget = new NounZoneTarget( m_CursorPosition, pContext );
			m_pDoc->setCursorTarget( m_pManualTarget );

			m_fUpdateContacts = TACTICAL_CONTACT_UPDATE;		
		}
		else
			((NounZoneTarget *)m_pDoc->cursorTarget())->setPosition( m_CursorPosition, pContext );
	}

	PrimitiveMaterial::Ref pAlphaMaterial = PrimitiveMaterial::create( pDisplay, PrimitiveMaterial::ALPHA, false );
	pAlphaMaterial->setDoubleSided( true );
	PrimitiveMaterial::push( pDisplay, pAlphaMaterial );

	if ( m_bShowContacts )
	{
 		// render contact indicators
		Noun::Ref		bestTarget; 
		int				bestTargetRange = 5000;

		RectInt renderBox( context.window() );
		RectInt	contactBox( renderBox );
		renderBox.inset( 5 );
		contactBox.inset( CONTACT_INSET );
		contactBox.bottom -= 160;


		for(int i=0;i<m_Contacts.size();i++)
		{
			Noun * pContact = m_Contacts[i];
			if ( pContact == NULL )
				continue;
			if ( pContact == pShip && !bCanTargetSelf )
				continue;
			if ( WidgetCast<NounTarget>( pContact ) == NULL && !pContact->syncronized() )
				continue;		// skip non-synchronized contacts

			bool isOnPlanet = WidgetCast<NounPlanet>( pContact->parent() ) != NULL;
			bool isStructure = isOnPlanet && WidgetCast<NounStructure>( pContact );
			bool isNewContact = m_OldContacts.find( pContact->key() ) < 0;
			bool isBody = WidgetCast<NounBody>( pContact ) != NULL;
			bool isPlanet = isBody && WidgetCast<NounPlanet>( pContact ) != NULL;
			bool isShip = isBody && WidgetCast<NounShip>( pContact ) != NULL;
			bool isFriend = pShip->isFriend( pContact );
			bool isEnemy = pShip->isEnemy( pContact );
			bool isDropTarget = !isBody && WidgetCast<NounDropTarget>( pContact ) != NULL;
			bool isProjectile = !isBody && WidgetCast<NounProjectile>( pContact ) != NULL;

			bool isTarget = pContact == pTarget;
			bool isCursorTarget = pContact == pCursorTarget;
			bool isOrderTarget = pContact == pShip->orderTarget();
			bool isGadgetTarget = pContact == (Noun *)pGadgetTarget;
			bool isEnhancement = WidgetCast<CargoEnhancement>( pContact ) != NULL;

			// get the world position of the contact
			Vector3 contactWorldPosition( pContact->worldPosition() );
			// get the viewspace position of the target
			Vector3 contactPosition( cameraFrame * (contactWorldPosition - cameraPosition ) );
			// clip to the viewspace
			context.clip( contactPosition );
			// project into screen space
			context.project( contactPosition );

			// get the color of the contact
			Color contactColor( isOrderTarget ? WHITE : isFriend ? GREEN : isEnemy ? RED : YELLOW );

			if ( isStructure )
				contactColor = ((NounStructure *)pContact)->color();
			if ( isTarget || isCursorTarget || isGadgetTarget || isNewContact )
				contactColor += Color(128,128,128);

			RectInt clipBox( contactBox );
			if ( isFriend )
				clipBox.inset( 8 );
			else if ( isEnemy )
				clipBox.inset( -8 );

			// if the contact is an enhancement, color it nicely
			if(isEnhancement)
			{
				if(((CargoEnhancement *)pContact)->enhancement())
					contactColor = ((CargoEnhancement *)pContact)->enhancement()->color();
			}

			// convert to a point and check if the contact is inside the box
			PointInt contactPoint( contactPosition.x, contactPosition.y );
			if ( isDropTarget )
			{
				contactColor.a = 64;
				if ( renderBox.inRect( contactPoint ) )
					PrimitiveLineStripDTL::createPushDiamond( pDisplay, contactPoint, DROP_TARGET_SIZE, contactColor );
			}
			else 
			{
				if (! clipBox.inRect( contactPoint ) )
				{
					// contact is offscreen, show arrow on side of screen
					if ( contactPoint.m_X < clipBox.m_Left || contactPoint.m_X > clipBox.m_Right )
					{
						contactPoint.m_X = Clamp( contactPoint.m_X, clipBox.m_Left, clipBox.m_Right );
						contactPoint.m_Y = Clamp( contactPoint.m_Y, clipBox.m_Top, clipBox.m_Bottom );

						if ( contactPoint.m_X < clipBox.m_Right )
							renderLeftArrow( pDisplay, contactPoint, CONTACT_RECTICLE_HSIZE, CONTACT_RECTICLE_VSIZE, contactColor );
						else
							renderRightArrow( pDisplay, contactPoint, CONTACT_RECTICLE_HSIZE, CONTACT_RECTICLE_VSIZE, contactColor );
					}
					else
					{
						contactPoint.m_X = Clamp( contactPoint.m_X, clipBox.m_Left, clipBox.m_Right );
						contactPoint.m_Y = Clamp( contactPoint.m_Y, clipBox.m_Top, clipBox.m_Bottom );

						if ( contactPoint.y < clipBox.bottom )
							renderUpArrow( pDisplay, contactPoint, CONTACT_RECTICLE_HSIZE, CONTACT_RECTICLE_VSIZE, contactColor );
						else
							renderDownArrow( pDisplay, contactPoint, CONTACT_RECTICLE_HSIZE, CONTACT_RECTICLE_VSIZE, contactColor );
					}
				}
				else if ( isProjectile )
					renderRecticle( pDisplay, contactPoint, PROJECTILE_HSIZE, PROJECTILE_VSIZE, contactColor );
				else
					renderRecticle( pDisplay, contactPoint, CONTACT_RECTICLE_HSIZE, CONTACT_RECTICLE_VSIZE, contactColor );
			}

			// show brackets around current cursor target
			if ( isCursorTarget )
				renderRecticle( pDisplay, contactPoint, CURSOR_RECTICLE_HSIZE, CURSOR_RECTICLE_VSIZE, GREY );
			// show current target
			if ( isTarget )
				renderRecticle( pDisplay, contactPoint, CURSOR_RECTICLE_HSIZE, CURSOR_RECTICLE_VSIZE, WHITE );

			if ( (isCursorTarget || isTarget || isOrderTarget || isGadgetTarget || isNewContact) && !isDropTarget )
			{
				CharString targetInfo;
				
				if ( pShip->orderTarget() == pContact )
				{
					targetInfo = CharString().format("%s %s\n%.1f", 
						NounShip::orderText( pShip->order() ), 
						pContact->displayName( false ).cstr(),
						(pShip->worldPosition() - pContact->worldPosition()).magnitude() );
				}
				else
				{
					targetInfo = CharString().format( "%s\n%.1f", 
						pContact->displayName( false ).cstr(), 
						(pShip->worldPosition() - pContact->worldPosition()).magnitude() );
				}

				// display text information
				//CharString targetInfo("%s\n%.1f km", pContact->name(), (pShip->worldPosition() - pContact->worldPosition()).magnitude() );

				PointInt targetPos( contactPoint );
				if ( (targetPos.x - TEXT_OFFSET) > ((clipBox.left + clipBox.right) >> 1) )
				{
					SizeInt infoSize( pFont->size( targetInfo ) );
					targetPos.x -= (infoSize.width + TEXT_OFFSET);
				}
				else
					targetPos.x += TEXT_OFFSET;


				Font::push( pDisplay, pFont, targetPos, targetInfo, contactColor );

				// restore the alpha material
				PrimitiveMaterial::push( pDisplay, pAlphaMaterial );
			}


			// check for best target
			int contactRange = sqr(contactPoint.m_X - m_CursorPosition2.m_X) + sqr(contactPoint.m_Y - m_CursorPosition2.m_Y);
			if ( contactRange < bestTargetRange )
			{
				bestTarget = pContact;
				bestTargetRange = contactRange;
			}

			// show shield/armor status if contact is current target and ship
			if ( (isShip && m_bShowShipStatus && isTarget)
				|| (isShip && isFriend && m_bShowFriendlyStatus) )
			{
				renderShipStatus( context, (NounShip *)pContact );
			}
		}

		if (! Keyboard::ctrlDown() )
			m_pDoc->setCursorTarget( bestTarget );
	}

	// show shield/armor status
	if ( m_bShowShipStatus && isShipAttached )
		renderShipStatus( context, pShip );

	// show yaw settings
	if ( m_bShowTurnStatus && isShipAttached && !inJump )
		renderTurnStatus( context, pShip );
	
	// show weapon/beam firing arc
	if ( pGadgetTarget != NULL )
		renderGadgetTarget( context, pShip, pGadgetTarget );

	// show self destruct message
	if ( m_SelfDestruct && m_SelfDestructConfirm )
	{
		float eta = TICK_DURATION_S * (m_SelfDestructTime - m_pDoc->context()->tick() );
	
		if (! m_pSelfDestructMessage )
			m_pSelfDestructMessage = new MessageLine();
		m_pSelfDestructMessage->m_sText.format( "Self Destruct Initiated... %.1f s", eta );
	}
	else if ( m_pSelfDestructMessage )
		m_pSelfDestructMessage = NULL;

	// generate ambient chatter sounds
	if ( m_UpdateChatter > UPDATE_CHATTER_RATE )
	{
		CharString chatter;
		chatter.format( "CHAT%d", int( rand() % CHATTER_COUNT) );

		Sound * pSound = WidgetCast<Sound>( window()->resource( chatter, false ) );
		if ( pSound )
			pSound->play( context.audioDevice(), CHATTER_VOLUME, 0.0f, false );

		m_UpdateChatter = 0.0f;
	}

	if ( ViewGame::sm_pViewGame != NULL )
		ViewGame::sm_pViewGame->renderViewGame( context, w, pShip->damage() > 0 );

	context.endScene();
}

bool ViewTactical::onCursorMove( const RectInt & window, 
						const PointInt & position,		// window and position are local coordinates
						const PointInt & delta )
{
	if ( m_bLeftDown || m_bRightDown )
	{
		setCursorState( HIDDEN );

		m_CameraYaw += CAMERA_YAW_RATE * delta.m_X;
		m_CameraPitch = Clamp( m_CameraPitch + (CAMERA_PITCH_RATE * delta.m_Y), 
			MIN_CAMERA_PITCH, MAX_CAMERA_PITCH );

		// mouse movement keeps the camera in free mode
		m_LastCameraMode = FREE;
		return true;
	}

	ViewGame::sm_pViewGame->updateAction( false );
	m_CursorPosition2 = position;
	return true;		// capture the mouse movement
}

bool ViewTactical::onLeftDown( const RectInt & window, const PointInt & position )
{
	m_bLeftDown = true;
	m_fLeftDownTime = 0.0f;
	m_LastCameraMode = m_CameraMode;
	m_CameraMode = FREE;

	return true;
}

bool ViewTactical::onLeftUp( const RectInt & window, 
						const PointInt & position )
{
	m_bLeftDown = false;
	m_CameraMode = m_LastCameraMode;

	if ( m_fLeftDownTime < LEFT_TARGET_TIME )
	{
		// get the current cursor target
		Noun * pTarget = m_pDoc->cursorTarget();

		// set the current target
		if ( pTarget != NULL && m_CameraMode == FREE )
			m_CameraMode = TARGET;
		m_pDoc->setTarget( pTarget );
		m_TargetTime = 0.0f;

		// update context action after a target change
		ViewGame::sm_pViewGame->updateAction( false );
	}

	return true;
}

bool ViewTactical::onRightDown( const RectInt & window,
						const PointInt & position )
{
	m_fRightDownTime = 0.0f;
	m_bRightDown = true;
	m_LastCameraMode = m_CameraMode;
	m_CameraMode = FREE;

	return true;
}

bool ViewTactical::onRightUp( const RectInt & window,
						const PointInt & position )
{
	m_bRightDown = false;
	m_CameraMode = m_LastCameraMode;
	if ( m_fRightDownTime < 0.5f )
		ViewGame::sm_pViewGame->doAction( false );

	return true;
}

//----------------------------------------------------------------------------

bool ViewTactical::onVelocityBar(const Message & msg)
{
	float position = *((float *)&msg.wparam);
	m_SetVelocity = position * m_pDoc->ship()->maxVelocity();
	m_SetVelocityV = 0.0f;
	m_UpdateControl = true;
	m_BeginControl = true;
	return true;
}

bool ViewTactical::onHeadingBar(const Message & msg)
{
	float position = *((float *)&msg.wparam);
	
	if ( m_bYawControl )
	{
		m_fYaw = (position - 0.5f) * 2;
		m_fYawV = 0.0f;
	}
	else
	{
		m_SetHeading = (position - 0.5f) * PIX2;
		m_SetHeadingV = 0.0f;
	}

	m_UpdateControl = true;
	m_BeginControl = true;
	return true;
}

//----------------------------------------------------------------------------

void ViewTactical::setCameraMode( CameraMode mode, bool pushMessage /*= true*/ )
{
	ASSERT( mode >= FIRST_CAMERA_MODE && mode <= LAST_CAMERA_MODE );
	m_CameraMode = mode;

	// set the camera time to 0, so the camera zooms to the new mode
	GameDocument::s_fCamera = 0.0f;

	if ( pushMessage )
	{
		static char * CAMERA_MODE_TEXT[ LAST_CAMERA_MODE + 1 ] =
		{
			"FREE",
			"TARGET",
			"TACTICAL",
			"FOLLOW",
		};
		m_pDoc->pushChat( CharString().format("CAMERA: %s", CAMERA_MODE_TEXT[ m_CameraMode ]) );
	}
}

void ViewTactical::nextCameraMode()
{
	CameraMode next = (CameraMode)( m_CameraMode + 1 );
	if ( next > LAST_CAMERA_MODE )
		next = FIRST_CAMERA_MODE;

	setCameraMode( next );
}

// This function renders with the currently set material
void ViewTactical::renderShipStatus( RenderContext & context, NounShip * pShip )
{
	if (! pShip )
		return;

	DisplayDevice * pDisplay = context.display();
	ASSERT( pDisplay );

	static float ARC_ANGLE[ 4 ] =
	{
		-PI4,			// FRONT
		PI4,			// RIGHT
		PI + PI4,		// LEFT,
		PI - PI4,		// BACK,
	};
	static float ARC_LENGTH[ 4 ] =
	{
		PI2,			// FRONT
		PI2,			// RIGHT
		PI2,			// LEFT,
		PI2,			// BACK,
	};

	// set the transform
	context.pushTransform( pShip->worldFrame(), pShip->worldPosition() );
	for(int i=FACING_FRONT;i<=FACING_BACK;i++)
	{
		// get the status of this facing
		int armormax, armordamage, shieldmax, shielddamage;
		int layers = 0, armorlayers = 0, shieldlayers = 0;
		pShip->protection( (Facing)i, shieldmax, shielddamage, armormax, armordamage );
		
		float range = pShip->radius();
		
		// determine the number of layers to draw for this facing
		if( armormax > 0 )
		{
			armorlayers = (armordamage / SHIELD_ARMOR_LAYER) + 1;
			layers += armorlayers;
		}
		
		if( shieldmax > 0 )
		{
			shieldlayers = (shielddamage / SHIELD_ARMOR_LAYER) + 1;
			layers += shieldlayers;
		}
			
		for(int z = 0; z < layers; z++)
		{
			Color color(0,0,0);

			//start off with black and add the color
			if(z < armorlayers)
			{
				//draw the armor ring
				color = GREEN;
				color.iterpolate( RED, 1.0f - (float( armordamage ) / armormax) );
			}
			else
			{
				//draw the shield ring
				color = Color(57,215,232);
				Color shielddamaged(163,60,210); //shield fully damaged color
				color.iterpolate(shielddamaged, 1.0f - (float( shielddamage ) / shieldmax) );
			}
			float angle = ARC_ANGLE[ i ]; // + pShip->orientation().m_fYaw;
			float perimeter = ARC_LENGTH[ i ];

			const int		SEGMENTS = 10;
			const int		SEGMENT_MID = SEGMENTS / 2;
			const Vector3	N( 0.0f );

			PrimitiveLineStripDL::Ref lineStrip = PrimitiveLineStripDL::create( pDisplay, SEGMENTS + 1 );
			VertexL * pVertex = lineStrip->lock();

			float angleStep = perimeter / SEGMENTS;
			for(int k=0;k<=SEGMENTS;k++)
			{
				Color D( color );
					// set alpha for segment
				if ( k <= SEGMENT_MID )
					D.a = (float( k ) / SEGMENT_MID) * 128;
				else 
					D.a = (float( SEGMENTS - k ) / SEGMENT_MID) * 128;
				pVertex[k].position = Vector3( sin( angle ) * range, 0, cos( angle ) * range );
				pVertex[k].normal = N;
				pVertex[k].diffuse = D;

				angle += angleStep;
			}
			lineStrip->unlock();

			// push the line strip
			PrimitiveLineStripDL::push( pDisplay, lineStrip );

			// increase range from ship for next layer
			range += 0.2f;

		}//end for loop for layers
	}	
}

void ViewTactical::renderTurnStatus( RenderContext & context, NounShip * pShip )
{
	float fTurn = m_bYawControl ? (m_fYaw * pShip->maxYaw()) : 
		AngleDelta( NormalizeAngle( pShip->heading() ), NormalizeAngle( m_SetHeading ) );

	if ( fTurn != 0.0f )
	{
		const float		RADIUS = pShip->radius() * 2.0f;
		const int		SEGMENTS = 8;
		const Color		D( 0,190,20,64 );
		const Vector3	N( 0.0f );

		PrimitiveTriangleFanDL::Ref triangleFan = PrimitiveTriangleFanDL::create( context.display(), SEGMENTS + 1 );
		VertexL * pVertex = triangleFan->lock();

		pVertex[0].position = Vector3( 0.0f );
		pVertex[0].normal = N;
		pVertex[0].diffuse = D;

		float angleStep = fTurn / (SEGMENTS - 1);
		float angle = 0.0f; //pShip->heading();
		if ( fTurn < 0.0f )
		{
			angle += fTurn;
			angleStep = -angleStep;
		}
		int i;
		for(i=1;i<SEGMENTS;i++)
		{
			pVertex[i].position = Vector3( sin( angle ) * RADIUS, 0, cos( angle ) * RADIUS );
			pVertex[i].normal = N;
			pVertex[i].diffuse = D;

			angle += angleStep;
		}

		pVertex[i].position = Vector3( sin( angle ) * RADIUS, 0, cos( angle ) * RADIUS );
		pVertex[i].normal = N;
		pVertex[i].diffuse = D;

		triangleFan->unlock();

		// show turn indicator
		context.pushTransform( pShip->worldFrame(), pShip->worldPosition() );
		// push the line strip
		PrimitiveTriangleFanDL::push( context.display(), triangleFan );
	}
}

void ViewTactical::renderGadgetTarget( RenderContext & context, NounShip * pShip, NounGadget * pGadget )
{
	bool isWeapon = WidgetCast<GadgetWeapon>( pGadget ) != NULL;
	bool isBeamWeapon = WidgetCast<GadgetBeamWeapon>( pGadget ) != NULL;

	if ( isWeapon || isBeamWeapon )
	{
		float fRange = isWeapon ? ((GadgetWeapon *)pGadget)->maxRange() 
			: ((GadgetBeamWeapon *)pGadget)->maxRange();

		context.pushTransform( pShip->worldFrame(), pGadget->worldPosition() );	// NOTE: don't use the worldFrame() of the gadget, it might be rotated strangely..

		// render firing arcs for this weapon
		if ( pGadget->mountFlags() & NounGadget::FLAG_MOUNT_FORE )
			renderWeaponArc( context, pShip, FACING_FRONT, fRange );
		if ( pGadget->mountFlags() & NounGadget::FLAG_MOUNT_RIGHT )
			renderWeaponArc( context, pShip, FACING_RIGHT, fRange );
		if ( pGadget->mountFlags() & NounGadget::FLAG_MOUNT_LEFT )
			renderWeaponArc( context, pShip, FACING_LEFT, fRange );
		if ( pGadget->mountFlags() & NounGadget::FLAG_MOUNT_AFT )
			renderWeaponArc( context, pShip, FACING_BACK, fRange );
	}
}

void ViewTactical::renderWeaponArc( RenderContext & context, NounShip * pShip, Facing nFace, float fRange )
{
	// render the firing arc
	static float ARC_ANGLE[ 4 ] =
	{
		-PI4,			// FRONT
		PI4,			// RIGHT
		PI + PI4,		// LEFT,
		PI - PI4,		// BACK,
	};
	static float ARC_LENGTH[ 4 ] =
	{
		PI2,			// FRONT
		PI2,			// RIGHT
		PI2,			// LEFT,
		PI2,			// BACK,
	};

	const int		SEGMENTS = 10;
	const Color		D( 190,20,20,64 );
	const Vector3	N( 0.0f );

	PrimitiveTriangleFanDL::Ref pFan = PrimitiveTriangleFanDL::create( context.display(), SEGMENTS + 1 );
	VertexL * pVertex = pFan->lock();

	pVertex[0].position = Vector3( 0.0f );
	pVertex[0].normal = N;
	pVertex[0].diffuse = D;

	float fPerimeter = ARC_LENGTH[ nFace ];
	float fStep = fPerimeter / (SEGMENTS - 1);
	float fAngle = ARC_ANGLE[ nFace ]; // + pShip->heading();

	int i;
	for(i=1;i<SEGMENTS;i++)
	{
		pVertex[i].position = Vector3( sinf( fAngle ) * fRange, 0, cosf( fAngle ) * fRange );
		pVertex[i].normal = N;
		pVertex[i].diffuse = D;

		fAngle += fStep;
	}

	pVertex[i].position = Vector3( sinf( fAngle ) * fRange, 0, cosf( fAngle ) * fRange );
	pVertex[i].normal = N;
	pVertex[i].diffuse = D;

	pFan->unlock();

	PrimitiveTriangleFanDL::push( context.display(), pFan );
}

void ViewTactical::updateContacts()
{
	m_fUpdateContacts = 0.0f;

	GameContext * pContext = m_pDoc->context();
	ASSERT( pContext );
	NounShip::Ref pShip = m_pDoc->ship();
	if (! pShip.valid() )
		return;

	int myFactionId = pShip->factionId();

	Noun * pFocus = m_pDoc->focus();
	ASSERT( pFocus );

	// copy current contacts to old contacts
	m_OldContacts.release();
	for(int i=0;i<m_Contacts.size();i++)
		m_OldContacts.insert( m_Contacts[i].key() );
	
	// rebuild the contact list
	m_Contacts.release();

	// get target information
	Noun::Ref pTarget = m_pDoc->target();
	Noun::Ref pCursorTarget = m_pDoc->cursorTarget();
	Noun::Ref pOrderTarget = pShip->orderTarget();

	// add current targets to contacts by default
	if ( pShip->validateTarget( pTarget ) )
	{
		// always push our current target
		m_Contacts.push( pTarget.pointer() );
		// get the parent of our target
		Noun * pParent = NULL;
		if ( WidgetCast<NounDropTarget>( pTarget ) )
			pParent = ((NounDropTarget *)pTarget.pointer())->planet();
		else
			pParent = pTarget->zoneParent();

		// Only show targets based on the view if this target is a planet
		if ( WidgetCast<NounPlanet>( pParent ) )
		{
			NounPlanet * pPlanet = (NounPlanet *)pParent;

			// get the view direction of the planet in planet space
			Vector3 delta( pPlanet->worldFrame() * (GameDocument::s_vCamera - pPlanet->worldPosition()) );
			// get the distance viewed from
			float viewRange = delta.magnitude();
			// normalize the delta, so our dot test will work
			delta *= 1.0f / viewRange;

			List< int > UsedHexes;

			// add children of planet that are on the correct side and can be detected
			for(int k=0;k<pPlanet->childCount();k++)
			{
				NounGame * pChild = WidgetCast<NounGame>( pPlanet->child(k) );
				if ( !pChild || (pChild->position() | delta) < 0.0f )
					continue;		// wrong side of the planet
				if ( pChild->isDetected( myFactionId ) )
				{
					m_Contacts.push( pChild );

					if ( WidgetCast<NounStructure>( pChild ) )
						UsedHexes.insert( ((NounStructure *)pChild)->hex() );
					//else if ( WidgetCast<NounUnit>( pChild ) )
					//	UsedHexes.insert( ((NounUnit *)pChild)->hex() );
				}
			}

			if ( m_pPlanetTarget != pPlanet )
			{
				m_DropTargets.clear();
				for(int k=0;k<pPlanet->hexCount();k++)
				{
					const NounPlanet::Hex & hex = pPlanet->hex( k );
					m_DropTargets.push_back( new NounDropTarget( pPlanet, k ) );
				}

				m_pPlanetTarget = pPlanet;
			}

			// show planet drop targets only if within a certain distance...
			if ( (viewRange - pPlanet->radius()) < DROP_TARGET_RANGE )
			{
				// add empty hexes of the planet to the contacts list
				for( DropList::iterator iDropTarget = m_DropTargets.begin(); 
					iDropTarget != m_DropTargets.end(); ++iDropTarget )
				{
					NounDropTarget * pTarget = *iDropTarget;
					if ( (pTarget->position() | delta) < 0.0f )
						continue;		// wrong side of the planet
					if ( UsedHexes.find( pTarget->hex() ).valid() )
						continue;		// hex not empty
					m_Contacts.push( pTarget );
				}
			}
		}
		/*
		else if ( pParent != NULL )
		{
			// add any detected children of current target
			int myFleetId = pShip->teamId();
			for(int i=0;i<pParent->childCount();i++)
			{
				Noun * pChild = (Noun *)pParent->child(i);
				if ( pContext->isDetected( myFleetId, pChild ) )
					m_Contacts.push( pChild );
			}
		}
		*/
	}

	m_Contacts.push( pShip.pointer() );
	if ( pShip->validateTarget( pCursorTarget ) )
		m_Contacts.push( pCursorTarget.pointer() );
	if ( pShip->validateTarget( pOrderTarget ) )
		m_Contacts.push( pOrderTarget.pointer() );

	Array< GameContext::NounCollision > contacts;
	pContext->proximityCheck( pFocus->worldPosition(), m_ScannerRange, contacts );

	for(int i=0;i<contacts.size();i++)
	{
		NounGame * pContact = WidgetCast<NounGame>( contacts[i].pNoun );

		// skip these objects, they are already added
		if ( pTarget == pContact || pCursorTarget == pContact || pOrderTarget == pContact || pShip == pContact )
			continue;

		// can this object be detected
		if ( pContact->isDetected( myFactionId ) )
		{
			m_Contacts.push( pContact );

			if ( WidgetCast<NounProjectile>( pContact ) )				// is this contact a projectile
			{
				if ( ((NounProjectile *)pContact)->target() == pShip )		// does it have us targeted
					if ( m_OldContacts.find( pContact->key() ) < 0 )		// is this a new target
					{
						// play incoming missile sound
						window()->playSound( "INCOMINGMISSILE" );
					}
			}
			else if ( WidgetCast<NounShip>( pContact ) )
			{
				if ( m_OldContacts.find( pContact->key() ) < 0 && !pShip->isFriend( pContact ) )
					window()->playSound( "ENEMYDETECTED" );
					
			}
		}
	}
}

//----------------------------------------------------------------------------

void ViewTactical::renderRecticle( DisplayDevice * pDevice, const PointInt & center, int hsize, int vsize, Color diffuse )
{
	PrimitiveLineStripDTL::Ref primitive;
	pDevice->create( primitive );

	primitive->initialize( 5 );

	// setup the verticies
	VertexTL * pVertex = primitive->lock();
	pVertex[0] = VertexTL( Vector3( center.x, center.y - vsize, 0.0f ), 1.0f, diffuse, 0.0f, 0.0f );
	pVertex[1] = VertexTL( Vector3( center.x + hsize, center.y, 0.0f), 1.0f, diffuse, 0.0f, 0.0f );
	pVertex[2] = VertexTL( Vector3( center.x, center.y + vsize, 0.0f), 1.0f, diffuse, 0.0f, 0.0f );
	pVertex[3] = VertexTL( Vector3( center.x - hsize, center.y, 0.0f), 1.0f, diffuse, 0.0f, 0.0f );
	pVertex[4] = pVertex[0];
	primitive->unlock();

	pDevice->push( primitive );
}

void ViewTactical::renderLeftArrow( DisplayDevice * pDevice, const PointInt & center, int hsize, int vsize, Color diffuse )
{
	PrimitiveLineStripDTL::Ref primitive;
	pDevice->create( primitive );

	// initialize the primitive
	primitive->initialize( 4 );
	VertexTL * pVertex = primitive->lock();
	pVertex[0] = VertexTL( Vector3( center.x - hsize, center.y, 0.0f ), 1.0f, diffuse, 0.0f, 0.0f );
	pVertex[1] = VertexTL( Vector3( center.x, center.y + vsize, 0.0f), 1.0f, diffuse, 0.0f, 0.0f );
	pVertex[2] = VertexTL( Vector3( center.x, center.y - vsize, 0.0f), 1.0f, diffuse, 0.0f, 0.0f );
	pVertex[3] = pVertex[0];
	primitive->unlock();
	
	pDevice->push( primitive );
}

void ViewTactical::renderRightArrow( DisplayDevice * pDevice, const PointInt & center, int hsize, int vsize, Color diffuse )
{
	PrimitiveLineStripDTL::Ref primitive;
	pDevice->create( primitive );

	// initialize the primitive
	primitive->initialize( 4 );
	VertexTL * pVertex = primitive->lock();
	pVertex[0] = VertexTL( Vector3( center.x + hsize, center.y, 0.0f ), 1.0f, diffuse, 0.0f, 0.0f );
	pVertex[1] = VertexTL( Vector3( center.x, center.y + vsize, 0.0f), 1.0f, diffuse, 0.0f, 0.0f );
	pVertex[2] = VertexTL( Vector3( center.x, center.y - vsize, 0.0f), 1.0f, diffuse, 0.0f, 0.0f );
	pVertex[3] = pVertex[0];
	primitive->unlock();
	
	pDevice->push( primitive );
}

void ViewTactical::renderUpArrow( DisplayDevice * pDevice, const PointInt & center, int hsize, int vsize, Color diffuse )
{
	PrimitiveLineStripDTL::Ref primitive;
	pDevice->create( primitive );

	// initialize the primitive
	primitive->initialize( 4 );
	VertexTL * pVertex = primitive->lock();
	pVertex[0] = VertexTL( Vector3( center.x, center.y - vsize, 0.0f ), 1.0f, diffuse, 0.0f, 0.0f );
	pVertex[1] = VertexTL( Vector3( center.x + hsize, center.y, 0.0f), 1.0f, diffuse, 0.0f, 0.0f );
	pVertex[2] = VertexTL( Vector3( center.x - hsize, center.y, 0.0f), 1.0f, diffuse, 0.0f, 0.0f );
	pVertex[3] = pVertex[0];
	primitive->unlock();
	
	pDevice->push( primitive );
}

void ViewTactical::renderDownArrow( DisplayDevice * pDevice, const PointInt & center, int hsize, int vsize, Color diffuse )
{
	PrimitiveLineStripDTL::Ref primitive;
	pDevice->create( primitive );

	// initialize the primitive
	primitive->initialize( 4 );
	VertexTL * pVertex = primitive->lock();
	pVertex[0] = VertexTL( Vector3( center.x, center.y + vsize, 0.0f ), 1.0f, diffuse, 0.0f, 0.0f );
	pVertex[1] = VertexTL( Vector3( center.x + hsize, center.y, 0.0f), 1.0f, diffuse, 0.0f, 0.0f );
	pVertex[2] = VertexTL( Vector3( center.x - hsize, center.y, 0.0f), 1.0f, diffuse, 0.0f, 0.0f );
	pVertex[3] = pVertex[0];
	primitive->unlock();
	
	pDevice->push( primitive );
}

//----------------------------------------------------------------------------

bool ViewTactical::onButtonPointDefense(const Message & msg)
{
	NounShip * pShip = m_pDoc->ship();
	if ( pShip == NULL )
		return false;

	if ( pShip->togglePointDefense() )
		m_pDoc->pushChat("<color;ffffff>Point Defense ON...</color>");
	else
		m_pDoc->pushChat("<color;ffffff>Point Defense OFF...</color>");
	return true;
}

bool ViewTactical::onButtonShields(const Message & msg)
{
	NounShip * pShip = m_pDoc->ship();
	if ( pShip == NULL )
		return false;

	switch ( pShip->toggleShields() ) {
	case 0:
		m_pDoc->pushChat( "<color;ffffff>Shields OFF...</color>" );
		break;
	case 1:
		m_pDoc->pushChat( "<color;ffffff>Shields ON...</color>" );
		break;
	default:
		return true;

	}
	return true;
}

bool ViewTactical::onButtonSendDistress(const Message & msg)
{
	if ( m_pDoc->tick() >= m_nDistressTick )
	{
		m_nDistressTick = m_pDoc->tick() + DISTRESS_MESSAGE_DELAY;
		Verb::Ref( new VerbDistress( m_pDoc->ship(), VerbDistress::ATTACKED ) );
	}
	else
		m_pDoc->pushChat( "<color;ffffff>Comms: You cannot send another distress so soon after the other...</color>" );
	return true;
}

bool ViewTactical::onButtonSendSupply(const Message & msg)
{
	if ( m_pDoc->tick() >= m_nDistressTick )
	{
		m_nDistressTick = m_pDoc->tick() + DISTRESS_MESSAGE_DELAY;
		Verb::Ref( new VerbDistress( m_pDoc->ship(), VerbDistress::SUPPLY ) );
	}
	else
		m_pDoc->pushChat( "<color;ffffff>Comms: You cannot send another distress so soon after the other...</color>" );
	return true;
}

bool ViewTactical::onButtonSelfDestruct(const Message & msg)
{
	GameContext * pContext = m_pDoc->context();
	ASSERT( pContext != NULL );

	if ( m_SelfDestruct && m_SelfDestructConfirm )
	{
		m_pDoc->pushChat( CharString( "<color;ffffff>Tactical: Self Destruct Aborted ...</color>" ) );
		m_SelfDestruct = m_SelfDestructConfirm = false;
	}
	else if ( m_SelfDestruct )
	{
		m_pDoc->pushChat( CharString( "<color;ffffff>Command: Self Destruct Confirmed ...</color>" ) );
		m_SelfDestructConfirm = true;
		m_SelfDestructTime = pContext->tick() + (TICKS_PER_SECOND * 10);
	}
	else 
	{
		m_pDoc->pushChat( CharString( "<color;ffffff>Command: Confirm Self Destruct, execute command again ...</color>" ) );
		m_SelfDestruct = true;
		m_SelfDestructConfirm = false;
	}
	return true;
}