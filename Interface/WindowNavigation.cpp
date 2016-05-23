/*
	WindowNavigation.cpp
	(c)2004 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Display/PrimitiveSetTransform.h"
#include "Display/PrimitiveTriangleFan.h"
#include "Display/PrimitiveLineStrip.h"
#include "System/Keyboard.h"
#include "DarkSpace/NounBody.h"
#include "World/NounZoneTarget.h"
#include "DarkSpace/NounNebula.h"
#include "DarkSpace/NounProjectile.h"
#include "DarkSpace/NounField.h"
#include "DarkSpace/Constants.h"
#include "DarkSpace/VerbUseGadget.h"
#include "DarkSpace/NounStar.h"
#include "DarkSpace/NounAsteroid.h"
#include "DarkSpace/NounJumpGate.h"
#include "DarkSpace/ShipPlatform.h"
#include "DarkSpace/TraitOrbit.h"
#include "Interface/WindowTarget.h"
#include "Interface/WindowNavigation.h"
#include "Interface/ViewGame.h"

#include <stdio.h>

//----------------------------------------------------------------------------

const float				MIN_CAMERA_ZOOM = 500.0f;
const float				MAX_CAMERA_ZOOM = 1.0f;
const Vector3			ZERO_N( 0, 0, 0 );
const float				FRONT_PLANE = 1.0f;
const float				BACK_PLANE = 5000000.0f;

static Constant			LEFT_TARGET_TIME( "LEFT_TARGET_TIME", 0.5f );
static Constant			NAVIGATION_DOLLY_RATE( "NAVIGATION_DOLLY_RATE", 500.0f );
static Constant			NAVIGATION_MIN_PITCH( "NAVIGATION_MIN_PITCH", 0.0f );
static Constant			NAVIGATION_MAX_PITCH( "NAVIGATION_MAX_PITCH", PI / 2 );
static Constant			NAVIGATION_ZOOM_DECAY( "NAVIGATION_ZOOM_DECAY", 3.0f );
static Constant			NAVIGATION_ZOOM_SPEED( "NAVIGATION_ZOOM_SPEED", 10.0f );
static Constant			NAVIGATION_PITCH_SPEED( "NAVIGATION_PITCH_SPEED", PI / 512 );
static Constant			NAVIGATION_YAW_SPEED( "NAVIGATION_YAW_SPEED", PI / 256 );
static Constant			NAVIGATION_UPDATE_INTELLIGENCE( "NAVIGATION_UPDATE_INTELLIGENCE", 5.0f );
static Constant			NAVIGATION_MAX_SYSTEM_SIZE( "NAVIGATION_MAX_SYSTEM_SIZE", 250000.0f );
static Constant			NAVIGATION_ICON_SIZE( "NAVIGATION_CONTACT_ICON_SIZE", 0.01f );
static Constant			NAVIGATION_CURSOR_SIZE( "NAVIGATION_CURSOR_SIZE", 0.02f );
static Constant			NAVIGATION_DIAMOND_SIZE( "NAVIGATION_DIAMOND_SIZE", 4.0f );

static Constant			NAVIGATION_HUD_FRONT_PLANE( "NAVIGATION_HUD_FRONT_PLANE", 0.1f );
static Constant			NAVIGATION_HUD_BACK_PLANE( "NAVIGATION_HUD_BACK_PLANE", 100.0f );
static Constant			NAVIGATION_BOTTOM_INSET( "NAVIGATION_BOTTOM_INSET", 160.0f );
static Constant			NAVIGATION_CONTACT_INSET( "NAVIGATION_CONTACT_INSET", 8 );

static Constant			NAVIGATION_START_ZOOM( "NAVIGATION_START_ZOOM", 0.65f );
static Constant			NAVIGATION_ENEMY_DETECTED_RADIUS( "NAVIGATION_ENEMY_DETECTED_RADIUS", 0.01f );

//----------------------------------------------------------------------------

Vector3					WindowNavigation::sm_vCameraTarget = Vector3(0,0,0);
bool					WindowNavigation::sm_bStickyCursorTarget = false;

template<class T>
inline T sqr( T number )
{
	return number * number;
}

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( WindowNavigation, NodeWindow );
REGISTER_FACTORY_KEY( WindowNavigation, 4633423203667851400 );

WindowNavigation::WindowNavigation() : m_nSelectMode( SELECT_ALL ), 
	m_nShowFlags( SHOW_GRID|SHOW_PLANETS|SHOW_ORBITS|SHOW_SHIPS|SHOW_ENEMY|SHOW_LINKS ),
	m_bLeftDown( false ), 
	m_fLeftDownTime( 0.0f ),
	m_bRightDown( false ), 
	m_fRightDownTime( 0.0f ),
	m_vCameraZoom( MAX_CAMERA_ZOOM / 2, 0, 0 ), 
	m_fCameraYaw( PI / 4 ), 
	m_fCameraPitch( PI / 4),
	m_vCursorPosition( 0, 0, 0 ), 
	m_pCursorPosition( 256, 256 ), 
	m_fTargetTime( 0.0f ), 
	m_pDoc( NULL ), 
	m_fViewDistance( 1.0f ), 
	m_fUniverseSize( 1.0f ), 
	m_fSystemSize( 1.0f ), 
	m_fUpdateIntelligence( 0.0f )
{
	m_vCameraZoom.x = NAVIGATION_START_ZOOM;
}

//----------------------------------------------------------------------------

const float UNIVERSE_SIZE_OFFSET = 1.25f;

void WindowNavigation::onActivate()
{
	//{{BEGIN_DATA_INIT
	//END_DATA_INIT}}

	// get some pointers
	m_pDoc = WidgetCast<GameDocument>( document() );
	ASSERT( m_pDoc );
	// gather initial intelligence
	gatherIntelligence();

	NounShip * pShip = m_pDoc->ship();
	if ( pShip != NULL )
		sm_vCameraTarget = pShip->worldPosition();

	if ( m_nSelectMode != SELECT_SPAWN )
	{
		Settings settings( "Options" );
		m_nShowFlags = settings.get( "navFlags", SHOW_GRID|SHOW_PLANETS|SHOW_ORBITS|SHOW_SHIPS|SHOW_ENEMY|SHOW_LINKS );
	}
}

void WindowNavigation::onDeactivate()
{
	if ( m_bRightDown || m_bLeftDown )
	{
		setCursorState( POINTER );
		m_bRightDown = m_bLeftDown = false;
	}

	m_Grid = NULL;
	m_Objects.release();
	m_Objectives.release();

	if ( m_nSelectMode != SELECT_SPAWN )
	{
		Settings settings( "Options" );
		settings.put( "navFlags", m_nShowFlags );
	}
}

void WindowNavigation::onHide()
{
	NodeWindow::onHide();

	if ( m_bRightDown || m_bLeftDown )
	{
		setCursorState( POINTER );
		m_bRightDown = m_bLeftDown = false;
	}
}

const float CAMERA_SNAP_TIME = 1.0f;					// how long does it take for the camera to snap fully into the navigation frame/position
const float MAX_TIME = 1.0f / 20.0f;

void WindowNavigation::onUpdate( float t )
{
	m_fTargetTime += t;
	if ( m_bRightDown )
		m_fRightDownTime += t;
	if ( m_bLeftDown )
		m_fLeftDownTime += t;

	// zoom the camera
	m_vCameraZoom.y -= (m_vCameraZoom.y * NAVIGATION_ZOOM_DECAY) * t;
	m_vCameraZoom.y += m_vCameraZoom.z * t;

	// make the camera move faster when further out, make it slower in close
	float speed = (m_fUniverseSize * m_vCameraZoom.x) / m_fUniverseSize;
	// don't allow time jumps, since a pause while caching textures may cause t to be very high
	float time = Min( t, MAX_TIME );	
	m_vCameraZoom.x = Clamp( m_vCameraZoom.x + (m_vCameraZoom.y * speed * time ), 
		MIN_CAMERA_ZOOM / m_fUniverseSize, MAX_CAMERA_ZOOM );

	float scale = m_fUniverseSize * m_vCameraZoom.x;

	// calculate the camera frame and position
	Matrix33 cameraFrame( true );
	cameraFrame.rotate( m_fCameraPitch, m_fCameraYaw, 0 );
	Vector3 cameraPosition( sm_vCameraTarget - (cameraFrame.k * scale) );

	GameDocument::s_fCamera += t;
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
	}
	else
	{
		// animation is over, snap directly to the desired frame/position
		GameDocument::s_mCamera = cameraFrame;
		GameDocument::s_vCamera = cameraPosition;
	}

	// update the intellience update timer
	m_fUpdateIntelligence += t;
	if ( m_fUpdateIntelligence > NAVIGATION_UPDATE_INTELLIGENCE || m_pDoc->context() != m_pContext )
		gatherIntelligence();
}

bool WindowNavigation::onMessage( const Message & msg )
{
	//{{BEGIN_MSG_MAP
	//END_MSG_MAP}}

	switch( msg.message )
	{
	case HM_MOUSEWHEEL:
		if ( int( msg.wparam ) > 0 )
			m_vCameraZoom.y += -(NAVIGATION_ZOOM_SPEED / 10.0f);
		else if ( int( msg.wparam ) < 0 )
			m_vCameraZoom.y += (NAVIGATION_ZOOM_SPEED / 10.0f);
		return true;
	case HM_KEYDOWN:
		switch( msg.wparam )
		{
		case 'X':
			beginZoom( false );
			return true;
		case 'Z':
			beginZoom( true );
			return true;
		}
		return false;
	case HM_KEYUP:
		switch( msg.wparam )
		{
		case 'X':
		case 'Z':
			endZoom();
			return true;
		}
		return false;
	}

	return NodeWindow::onMessage( msg );
}

//----------------------------------------------------------------------------

void WindowNavigation::onRender( RenderContext & context, const RectInt & w )
{
	if ( m_pDoc == NULL )
		return;

	GameContext * pContext = m_pDoc->context();
	ASSERT( pContext );
	DisplayDevice * pDisplay = context.device();
	ASSERT( pDisplay );

	bool bBlink = fmod( context.time(), 1.0f ) > 0.5f;

	// get the players ship, note may be NULL
	NounShip * pShip = m_pDoc->ship();
	// get the local players factionId
	int nFactionId = m_pDoc->factionId();

	// get material pointers
	Material * pContactBox = WidgetCast<Material>( resource( "CONTACTBOX" ) );
	ASSERT( pContactBox );
	Material * pContactLock = WidgetCast<Material>( resource( "CONTACTLOCK" ) );
	ASSERT( pContactLock );

	pContactBox->setLightEnable( false );
	pContactLock->setLightEnable( false );

	Font * pFont = windowStyle()->font();
	ASSERT( pFont );

	//----------------------------------------------------------------------------

	// get the camera frame and position
	Matrix33 cameraFrame( GameDocument::s_mCamera );
	Vector3	cameraPosition( GameDocument::s_vCamera );

	// end the previous scene
	context.endScene();

	RenderContext::SaveState state( context );
	context.setProjection( context.window(), context.fov(), FRONT_PLANE, BACK_PLANE );
	context.setPosition( cameraPosition );
	context.setFrame( cameraFrame );
	context.setTime( pContext->tick() * TICK_DURATION_S );
	context.setShadowFocus( context.worldToView( sm_vCameraTarget ), SHADOW_MAP_FOCUS );

	// unproject the cursor position
	Vector3 cursorRay( m_pCursorPosition.x, m_pCursorPosition.y, 1.0f );
	context.unProject( cursorRay );
	// transform to world
	cursorRay = cameraFrame % cursorRay;

	// intersect with a flat-plane along the XZ axis
	Vector3 planeIntersect;
	if ( Plane( Vector3(0,1,0), 0.0f ).intersect( cameraPosition, cursorRay, planeIntersect ) )
		m_vCursorPosition = planeIntersect;

	//----------------------------------------------------------------------------

	// render the real universe
	pContext->render( context, Matrix33::IDENTITY, Vector3::ZERO ); 

	//----------------------------------------------------------------------------

	float scale = m_fUniverseSize * m_vCameraZoom.x;
	const float rScale = 1.0f / scale;		// get the recipical

	Matrix33 hudFrame( cameraFrame * rScale );
	Vector3 hudPosition( cameraPosition * rScale );

	context.setProjection( context.window(), context.fov(), NAVIGATION_HUD_FRONT_PLANE, NAVIGATION_HUD_BACK_PLANE );
	context.setPosition( Vector3::ZERO );
	context.setFrame( Matrix33::IDENTITY );
	context.beginScene();

	// set the transform for the navigational overlay
	context.pushTransform( ~hudFrame, cameraFrame * (-hudPosition) );

	//----------------------------------------------------------------------------

	// while navigation display is activating, the alpha of the hud fades in over time
	float hudAlpha = GameDocument::s_fCamera < CAMERA_SNAP_TIME ? (GameDocument::s_fCamera / CAMERA_SNAP_TIME) : 1.0f;
	// create an alpha material
	PrimitiveMaterial::Ref pAlphaMaterial = PrimitiveMaterial::create( pDisplay, PrimitiveMaterial::ALPHA, false );
	// push the alpha material
	context.push( pAlphaMaterial );

	//----------------------------------------------------------------------------

	// render the grid 
	if( m_nShowFlags & SHOW_GRID )
	{
		// rebuild the grid if needed
		if (! m_Grid.valid() )
			buildGrid( pDisplay );

		// render the grid
		if ( m_Grid.valid() )
			context.push( m_Grid );
	}

	//----------------------------------------------------------------------------

	// get the frame to display the map
	Noun *		bestTarget = sm_bStickyCursorTarget ? m_pDoc->cursorTarget() : NULL;
	int			bestTargetRange = 5000;

	RectInt	contactBox( context.window() );
	contactBox.inset( NAVIGATION_CONTACT_INSET );
	contactBox.bottom -= NAVIGATION_BOTTOM_INSET;		// don't display over interface at the bottom

	// render the real objects
	m_Objects.reset();
	while( m_Objects.valid() )
	{
		Noun * pContact = *m_Objects;
		m_Objects.next();

		if ( pContact == NULL )
			continue;		// invalid contact pointer
		
		// get the world position
		Vector3 worldPosition( pContact->worldPosition() );
		Vector3 delta( worldPosition - cameraPosition );
		// get the distance from the camera to the contact
		float contactDistance = delta.magnitude();
		// get the viewspace position
		Vector3 viewPosition( hudFrame * delta );

		// clip to the viewspace
		Vector3 contactPosition( viewPosition );
		context.clip( contactPosition );			// clip to the view fulstrum
		context.project( contactPosition );		// project into screen space

		// get the 2D position of the contact
		PointInt contactPoint( contactPosition.x, contactPosition.y );

		// get the contact factionId
		int nContactFactionId = pContact->factionId();
		// determine if this object is visible in the navigation screen currently
		bool isVisible = context.sphereVisible( viewPosition, pContact->radius() * rScale );
		bool isBody = WidgetCast<NounBody>( pContact ) != NULL;
		bool isPlanet = isBody && WidgetCast<NounPlanet>( pContact ) != NULL;
		bool isShip = isBody && WidgetCast<NounShip>( pContact ) != NULL;
		bool isPlatform = isShip && WidgetCast<ShipPlatform>( pContact ) != NULL;
		bool isStar = isBody && WidgetCast<NounStar>( pContact ) != NULL;
		bool isGate = isBody && WidgetCast<NounJumpGate>( pContact ) != NULL;
		bool isFriend = nContactFactionId == nFactionId;
		bool isEnemy = !isFriend && nContactFactionId != 0;
		bool isProjectile = !isBody && WidgetCast<NounProjectile>( pContact ) != NULL;
		bool isSpawn = (isGate || isPlanet) && m_pDoc->validateSpawn( pContact );
		bool isCloaked = isShip && ((NounShip *)pContact)->testFlags( NounShip::FLAG_CLOAKED ) && isEnemy;

		TraitOrbit * pOrbit = FindTrait<TraitOrbit>( pContact );
		bool inOrbit = pOrbit != NULL && pOrbit->orbiting() != NULL;
		bool isPrimaryStar = isStar && !inOrbit;

		bool isCursorTarget = pContact == m_pDoc->cursorTarget();
		bool isTarget = pContact == m_pDoc->target();
		bool isOrderTarget = pShip != NULL ? pContact == pShip->orderTarget() : false;

		bool showLabel = false;
		bool showDistance = false;

		bool isFiltered = false;
		if ( isPlanet && (m_nShowFlags & SHOW_PLANETS) == 0 )
			isFiltered = true;
		if ( isShip && (m_nShowFlags & SHOW_SHIPS) == 0 )
			isFiltered = true;
		if ( isShip && !isPlatform && pContact->userId() == 0 && (m_nShowFlags & SHOW_AI) == 0 )
			isFiltered = true;
		if ( isSpawn && (m_nShowFlags & SHOW_SPAWN ) != 0 )
			isFiltered = false;
		if ( isFiltered )
			continue;		// filtered, skip this object


		bool isSelectable = false;

		// get the color of the contact
		Color contactColor( isBody ? isFriend ? GREEN : isEnemy ? RED : YELLOW : GREY ); 
		if ( isShip )
			contactColor += GREY;

		float contactAlpha = (isPrimaryStar || isCursorTarget || isTarget || isOrderTarget || isShip) ? 1.0f : 
			contactDistance < m_fSystemSize ? (1.0f - (contactDistance / m_fSystemSize)) : 0.0f;
		// fade in contact displays
		contactColor.a = (contactAlpha * hudAlpha) * 255;

		if ( isOrderTarget || isCursorTarget || isTarget )
			contactColor += GREY;
		if ( isSpawn && (m_nShowFlags & SHOW_SPAWN) != 0 )
			contactColor = WHITE;

		//----------------------------------------------------------------------------

		// is the object visible and cloaked, if not don't bother rendering the icon
		if ( isVisible && contactColor.a > 0 && !isCloaked )
		{
			Matrix33 contactFrame( ~pContact->frame() );
			contactFrame.j = Vector3(0,1,0);
			contactFrame.orthoNormalizeXZ();

			float contactSize = NAVIGATION_ICON_SIZE / rScale; 
			
			PrimitiveTriangleFanDL::Ref pTriangleFan = PrimitiveTriangleFanDL::create( pDisplay, 4 );
			VertexL * pVertex = pTriangleFan->lock();
			pVertex[0] = VertexL( worldPosition + (contactFrame * Vector3( -contactSize, 0, -contactSize )), ZERO_N, contactColor, 0, 1 );
			pVertex[1] = VertexL( worldPosition + (contactFrame * Vector3( -contactSize, 0, contactSize)), ZERO_N, contactColor, 1, 1 );
			pVertex[2] = VertexL( worldPosition + (contactFrame * Vector3( contactSize, 0, contactSize)), ZERO_N, contactColor, 1, 0 );
			pVertex[3] = VertexL( worldPosition + (contactFrame * Vector3( contactSize, 0, -contactSize)), ZERO_N, contactColor, 0, 0 );
			pTriangleFan->unlock();

			context.push( getContactIcon( pContact ) );
			context.push( pTriangleFan );

			if ( isStar )
				showLabel = true;			// always show the names of all stars
			else if ( scale < m_fSystemSize )
				showLabel = true;			// show the names of planets and ships when we are zoom in and visible

			isSelectable = true;

			context.push( pAlphaMaterial );
		}

		//// place a white diamond around all spawn points
		//if ( isSpawn && (m_nShowFlags & SHOW_SPAWN) != 0 )
		//	PrimitiveLineStripDTL::createPushDiamond( pDisplay, contactPoint, 8, WHITE );

		// show cursor target
		if ( isCursorTarget || isTarget || isOrderTarget )
		{
			Color cursorColor( WHITE );
			cursorColor.a = hudAlpha * 255;

			if ( isVisible )
			{
				Matrix33 cursorFrame( true );
				cursorFrame.rotate( 0, context.time() * PI, 0 );

				float cursorSize = NAVIGATION_CURSOR_SIZE / rScale;

				PrimitiveTriangleFanDL::Ref pTriangleFan = PrimitiveTriangleFanDL::create( pDisplay, 4 );
				VertexL * pVertex = pTriangleFan->lock();
				pVertex[0] = VertexL( worldPosition + (cursorFrame * Vector3( -cursorSize, 0, -cursorSize )), ZERO_N, cursorColor, 0, 1 );
				pVertex[1] = VertexL( worldPosition + (cursorFrame * Vector3( -cursorSize, 0, cursorSize)), ZERO_N, cursorColor, 1, 1 );
				pVertex[2] = VertexL( worldPosition + (cursorFrame * Vector3( cursorSize, 0, cursorSize)), ZERO_N, cursorColor, 1, 0 );
				pVertex[3] = VertexL( worldPosition + (cursorFrame * Vector3( cursorSize, 0, -cursorSize)), ZERO_N, cursorColor, 0, 0 );
				pTriangleFan->unlock();

				if ( isTarget )
					context.push( pContactLock );
				else
					context.push( pContactBox );
				context.push( pTriangleFan );

				context.push( pAlphaMaterial );
			}
			else
			{
				// show indicator on the side of the screen
				PrimitiveLineStripDTL::createPushDiamond( pDisplay, contactPoint, NAVIGATION_DIAMOND_SIZE, cursorColor );
			}

			showLabel = true;
			showDistance = true;
			isSelectable = true;
		}

		if ( showLabel )
		{
			// show order indicator
			CharString label;
			if ( isOrderTarget && pShip != NULL )
				label = CharString().format("%s %s", NounShip::orderText( pShip->order() ), pContact->displayName( false ) );
			else
				label = pContact->displayName( false );

			if ( showDistance && pShip != NULL )
				label += CharString().format("\n%.1f gu", (pShip->worldPosition() - pContact->worldPosition()).magnitude() );

			// show the name of the target
			SizeInt labelSize( pFont->size(label) );

			PointInt labelPos( contactPoint );
			if ( labelPos.m_X > ((contactBox.m_Left + contactBox.m_Right) >> 1) )
				labelPos.m_X -= labelSize.width;
			if ( labelPos.m_Y > ((contactBox.m_Top + contactBox.m_Bottom) >> 1) )
				labelPos.m_Y -= (labelSize.height << 1);

			Font::push( pDisplay, pFont, labelPos, label, contactColor );

			context.push( pAlphaMaterial );
		}

		// draw line to current target
		if ( isTarget && pShip != NULL )
		{
			// draw line from current ship position to target position, check for collision... 
			// if we will collide then blink the line and make it red
			bool collide = false;

			Vector3 origin( pShip->worldPosition() );
			Vector3 destination( pContact->worldPosition() );
			Vector3 delta( destination - origin );

			float distance = delta.magnitude();
			//delta.normalize();

			// check for collision
			Array< GameContext::NounCollision > check;
			if ( pContext->proximityCheck( origin, distance, check ) )
			{
				Noun *			pCollide = NULL;
				float			maxDistance = distance * distance;
				float			collideDistance = maxDistance;
				Vector3			collidePosition;

				for(int i=0;i<check.size();i++)
				{
					Noun * pCheck = check[i].pNoun;

					if ( pCheck == pShip || pCheck == pContact ||
						WidgetCast<NounBody>( pCheck ) == NULL || 
						WidgetCast<NounNebula>( pCheck ) ||
						WidgetCast<NounShip>( pCheck ) ||
						WidgetCast<NounField>( pCheck ) )
					{
						// ignore object
						continue;
					}

					// object is close, final check
					Vector3 intersect;
					if ( pCheck->intersect( pShip->radius(), origin, delta, intersect ) )
					{
						collide = true;

						// draw box around object to indicate possible collision
						Color boxColor( bBlink ? RED : BLACK );
						boxColor.a = hudAlpha * boxColor.a;

						Vector3		position( pCheck->worldPosition() + Vector3( 0, 10, 0 ) );
						float		radius = pCheck->radius() * 2;

						static VertexL box[ 5 ];
						box[ 0 ] = VertexL( position + (Vector3( -1, 0, -1 ) * radius), ZERO_N, boxColor, 0, 0 );
						box[ 1 ] = VertexL( position + (Vector3( 1, 0, -1 ) * radius), ZERO_N, boxColor, 0, 0 );
						box[ 2 ] = VertexL( position + (Vector3( 1, 0, 1 ) * radius), ZERO_N, boxColor, 0, 0 );
						box[ 3 ] = VertexL( position + (Vector3( -1, 0, 1 ) * radius), ZERO_N, boxColor, 0, 0 );
						box[ 4 ] = box[ 0 ];

						PrimitiveLineStripDL::push( pDisplay, 5, box );
					}
				}
			}

			Color lineColor( WHITE );
			if ( collide )
				lineColor = ((pContext->tick() % 20) < 15) ? RED : BLACK;
			lineColor.a = hudAlpha * 255;

			static VertexL line[ 2 ];
			line[ 0 ] = VertexL( origin, ZERO_N, lineColor, 0, 0 );
			line[ 1 ] = VertexL( destination, ZERO_N, lineColor, 0, 0 );
			PrimitiveLineStripDL::push( pDisplay, 2, line );
		}

		// show ship orders
		if ( isShip && isFriend && contactColor.a > 0 && (m_nShowFlags & SHOW_ORDERS) != 0 )
		{
			if ( ((NounShip *)pContact)->order() != NounShip::NOORDER  )
			{
				NounGame * pOrderTarget = WidgetCast<NounGame>( ((NounShip *)pContact)->orderTarget() );

				// make sure the target is detected.. objects that aren't getting updated,
				// will not clear their order target when they lose dection
				if( pOrderTarget != NULL && pOrderTarget->isDetected( nFactionId ) )
				{
					Color lineColor( NounShip::orderColor( ((NounShip *)pContact)->order() ) );
					lineColor.a = hudAlpha * 128;

					Vector3 offset( 0, -3, 0 );		// offset in the Y axis, so the line doesn't overlap a jump line
					Vector3 origin( worldPosition + offset );
					Vector3 destination( pOrderTarget->worldPosition() + offset );

					static VertexL line[ 2 ];
					line[ 0 ] = VertexL( origin, ZERO_N, lineColor, 0, 0 );
					line[ 1 ] = VertexL( destination, ZERO_N, lineColor, 0, 0 );
					PrimitiveLineStripDL::push( pDisplay, 2, line );
				}
			}
		}

		// show enemy detected indicators
		if ( isPlanet && isFriend && (m_nShowFlags & SHOW_ENEMY) != 0 )
		{
			NounPlanet * pPlanet = (NounPlanet *)pContact;
			if ( (pPlanet->flags() & NounPlanet::FLAG_ENEMY) != 0 )
			{
				float fRadius = NAVIGATION_ENEMY_DETECTED_RADIUS * scale;

				Vector3 vWorld( pPlanet->worldPosition() );
				Vector3 vView( hudFrame * (vWorld - cameraPosition) );
				if ( context.sphereVisible( vView, fRadius ) )
				{
					float			fAngle = 0.0f;
					const int		CIRCLE_SEGMENTS = 32;
					const float		SEGMENT_PI = (PI * 2) / CIRCLE_SEGMENTS;
					static VertexL	lineStrip[ CIRCLE_SEGMENTS + 1 ];

					Color cDiffuse( bBlink ? RED : DARK_RED );
					int i;
					for(i=0;i<CIRCLE_SEGMENTS;i++)
					{
						Vector3 d( sin(fAngle), 0, cos(fAngle) );
						lineStrip[i].position = vWorld + (d * fRadius);
						lineStrip[i].diffuse = cDiffuse;
						lineStrip[i].normal = ZERO_N;
						
						fAngle += SEGMENT_PI;
					}
					lineStrip[i] = lineStrip[0];

					PrimitiveLineStripDL::push( pDisplay, CIRCLE_SEGMENTS + 1, lineStrip );
				}
			}
		}

		// show orbit paths
		if ( inOrbit && contactColor.a > 0 && (m_nShowFlags & SHOW_ORBITS) != 0 )
		{
			const int		CIRCLE_SEGMENTS = 32;
			const float		SEGMENT_PI = (PI * 2) / CIRCLE_SEGMENTS;
			const int		SEGMENT_ALPHA = 64 / CIRCLE_SEGMENTS;

			static VertexL	
							lineStrip[ CIRCLE_SEGMENTS + 1 ];

			ASSERT( pOrbit );
			Noun * pOrbiting = pOrbit->orbiting();

			Vector3	worldPosition( pOrbiting->worldPosition() );
			float angle = pOrbit->orbitPosition();
			float radius = pOrbit->orbitDistance();
			
			Vector3 viewPosition( hudFrame * (worldPosition - cameraPosition) );
			if ( context.sphereVisible( viewPosition, radius * rScale ) )
			{
				int i;
				for(i=0;i<CIRCLE_SEGMENTS;i++)
				{
					Vector3 d( sin(angle), 0, cos(angle) );
					lineStrip[i].position = worldPosition + (d * radius);
					lineStrip[i].diffuse = contactColor;
					lineStrip[i].normal = ZERO_N;
					
					angle += SEGMENT_PI;
				}
				lineStrip[i] = lineStrip[0];

				PrimitiveMaterial::push( pDisplay, pAlphaMaterial );
				PrimitiveLineStripDL::push( pDisplay, CIRCLE_SEGMENTS + 1, lineStrip );
			}
		}

		// show jumpgate links
		if ( isGate && (m_nShowFlags & SHOW_LINKS) != 0 )
		{
			// inverse of hud alpha, get brighter as the player zooms away from the current system
			const Color JUMP_LINK_COLOR( 80, 80, 155, 128 );
			const Color TRANSFER_JUMP_LINK_COLOR( 128, 80, 0, 128 );

			NounJumpGate * pGate = (NounJumpGate *)pContact;
			Noun * pDestination = pGate->destinationNoun();

			if ( pDestination != NULL )
			{
				Color color( pGate->context() == pDestination->context() ? JUMP_LINK_COLOR : TRANSFER_JUMP_LINK_COLOR );
				Vector3 origin( pGate->worldPosition() );
				Vector3 destination( pDestination->worldPosition() );

				static VertexL line[ 2 ];
				line[ 0 ] = VertexL( origin, ZERO_N, color, 0, 0 );
				line[ 1 ] = VertexL( destination, ZERO_N, color, 0, 0 );

				PrimitiveLineStripDL::push( pDisplay, 2, line );
			}
		}
		
		// show trade routes
		if ( (m_nShowFlags & SHOW_TRADE) != 0 && isPlanet && isTarget && contactColor.a > 0 )
		{
			NounPlanet * pPlanet = (NounPlanet *)pContact;
			Vector3 origin( pPlanet->worldPosition() );

			Color routeColor( CYAN );
			routeColor.a = contactColor.a;

			// show all trade routes for the planet
			for(int i=0;i<pPlanet->tradeRoutes();i++)
			{
				NounPlanet * pDestination = pPlanet->tradeRoute( i );
				if ( pDestination != NULL )
				{
					Vector3 destination( pDestination->worldPosition() );
					Vector3 delta( destination - origin );
					float distance = delta.magnitude();

					Vector3 viewPosition( hudFrame * (origin - cameraPosition) );
					if ( context.sphereVisible( viewPosition, distance * rScale ) )
					{
						static VertexL line[ 2 ];
						line[ 0 ] = VertexL( origin, ZERO_N, routeColor, 0, 0 );
						line[ 1 ] = VertexL( destination, ZERO_N, routeColor * 0.5f, 0, 0 );

						PrimitiveLineStripDL::push( pDisplay, 2, line );
					}
				}
			}
		}

		if ( m_nSelectMode == SELECT_SPAWN )
			isSelectable = isSpawn;		// in spawn mode, only spawn points can be selected!

		// check for best target
		if ( isSelectable )
		{
			int contactRange = sqr(contactPoint.m_X - m_pCursorPosition.m_X) + sqr(contactPoint.m_Y - m_pCursorPosition.m_Y);
			if ( contactRange < bestTargetRange )
			{
				bestTarget = pContact;
				bestTargetRange = contactRange;
			}
		}
	}

	if ( Keyboard::ctrlDown() )
	{
		if ( WidgetCast<NounZoneTarget>( m_pDoc->cursorTarget() ) == NULL )
		{
			m_pManualTarget = new NounZoneTarget( m_vCursorPosition, pContext );
			m_pDoc->setCursorTarget( m_pManualTarget );

			gatherIntelligence();
		}
		else
			((NounZoneTarget *)m_pDoc->cursorTarget())->setPosition( m_vCursorPosition, pContext );
	}
	else
		m_pDoc->setCursorTarget( bestTarget );

	context.endScene();
}

bool WindowNavigation::onCursorMove( const RectInt & window, 
						const PointInt & position,		// window and position are local coordinates
						const PointInt & delta )
{
	if ( m_bLeftDown )
	{
		setCursorState( HIDDEN );

		// calculate the scroll rate based on the distance from the target
		float scrollRate = m_vCameraZoom.x * NAVIGATION_DOLLY_RATE;

		Matrix33 scrollFrame( true );
		scrollFrame.rotate( 0, m_fCameraYaw, 0 );

		sm_vCameraTarget -= (scrollFrame.i * delta.m_X) * scrollRate;
		sm_vCameraTarget += (scrollFrame.k * delta.m_Y) * scrollRate;
		sm_vCameraTarget.x = Clamp( sm_vCameraTarget.x, -m_fUniverseSize, m_fUniverseSize );
		sm_vCameraTarget.z = Clamp( sm_vCameraTarget.z, -m_fUniverseSize, m_fUniverseSize );

		return true;
	}
	if ( m_bRightDown )
	{
		setCursorState( HIDDEN );

		m_fCameraYaw += NAVIGATION_YAW_SPEED * delta.m_X;
		m_fCameraPitch = Clamp<float>( m_fCameraPitch + (NAVIGATION_PITCH_SPEED * delta.m_Y), 
			NAVIGATION_MIN_PITCH, NAVIGATION_MAX_PITCH );

		return true;
	}

	if ( m_nSelectMode == SELECT_ALL )
		ViewGame::sm_pViewGame->updateAction( true );
	else
		setCursorState( ::USER1 );

	m_pCursorPosition = windowToScreen( position );
	sm_bStickyCursorTarget = false;

	return true;
}

bool WindowNavigation::onLeftDown( const RectInt & window, 
						const PointInt & position )
{
	if ( window.inRect( position ) )
	{
		m_bLeftDown = true;
		m_fLeftDownTime = 0.0f;
		return true;
	}

	return false;
}

bool WindowNavigation::onLeftUp( const RectInt & window, 
						const PointInt & position )
{
	if ( m_bLeftDown )
	{
		m_bLeftDown = false;

		if ( m_nSelectMode == SELECT_SPAWN 
			&& m_pDoc->cursorTarget() == NULL )
		{
			return true;	// don't allow them to deselect their current spawn to nothing!
		}

		if ( m_pDoc->cursorTarget() != NULL 
			&& m_pDoc->target() == m_pDoc->cursorTarget() 
			&& m_fTargetTime < 1.0f )
		{
			// user double clicked on their target, center view on target..
			sm_vCameraTarget = m_pDoc->target()->worldPosition();
			return true;
		}

		// make sure click was within left click time...
		if ( m_fLeftDownTime < LEFT_TARGET_TIME )
		{
			m_pDoc->setTarget( m_pDoc->cursorTarget() );
			if ( m_nSelectMode != SELECT_SPAWN )
				ViewGame::sm_pViewGame->updateAction( true );		// update actions after target change..
			m_fTargetTime = 0.0f;
		}
	}

	return true;
}

bool WindowNavigation::onRightDown( const RectInt & window,
						const PointInt & position )
{
	m_bRightDown = true;
	m_fRightDownTime = 0.0f;
	return true;
}

bool WindowNavigation::onRightUp( const RectInt & window,
						const PointInt & position )
{
	m_bRightDown = false;
	if ( m_fRightDownTime < 0.5f && m_nSelectMode != SELECT_SPAWN )
		ViewGame::sm_pViewGame->doAction( true );

	return true;
}

//----------------------------------------------------------------------------

void WindowNavigation::setSelectMode( SelectMode nMode )
{
	m_nSelectMode = nMode;
}

void WindowNavigation::setShowFlags( dword nFlags )
{
	m_nShowFlags = nFlags;
}

void WindowNavigation::flipShowFlag( dword nFlag )
{
	if ( m_nShowFlags & nFlag )
		m_nShowFlags &= ~nFlag;
	else
		m_nShowFlags |= nFlag;
}

void  WindowNavigation::onCenter()
{
	if ( m_pDoc->target() != NULL )
		sm_vCameraTarget = m_pDoc->target()->worldPosition();
	else if ( m_pDoc->ship() != NULL )
		sm_vCameraTarget = m_pDoc->ship()->worldPosition();
	else
		sm_vCameraTarget = Vector3(0,0,0);		// no target, no ship, center of the universe then...
}

void WindowNavigation::beginZoom( bool bZoomIn )
{
	if ( bZoomIn )
		m_vCameraZoom.z = -NAVIGATION_ZOOM_SPEED;
	else
		m_vCameraZoom.z = NAVIGATION_ZOOM_SPEED;
}

void WindowNavigation::endZoom()
{
	m_vCameraZoom.z = 0.0f;
}

void WindowNavigation::onFocus()
{
	if ( m_pDoc->target() != NULL )
		sm_vCameraTarget = m_pDoc->target()->worldPosition();
	else
		sm_vCameraTarget = m_pDoc->ship()->worldPosition();

	m_vCameraZoom.x = MIN_CAMERA_ZOOM / m_fUniverseSize;
	m_vCameraZoom.y = m_vCameraZoom.z = 0.0f;
}

void WindowNavigation::onFull()
{
	m_vCameraZoom.x = MAX_CAMERA_ZOOM;
	m_vCameraZoom.y = m_vCameraZoom.z = 0.0f;
}

//----------------------------------------------------------------------------

void WindowNavigation::gatherIntelligence()
{
	m_fUpdateIntelligence = 0;
	m_pContext = m_pDoc->context();
	if (! m_pContext )
		return;

	// release the previous object list
	m_Objects.release();
	// early out if we don't have a valid team yet..
	if (! m_pDoc->isTeamValid() )
		return;

	// get the bounds of the universe
	BoxHull bounds( m_pContext->worldExtents() );
	// get the real radius
	float radius = bounds.radius();
	// get the universe hull
	m_fUniverseSize = radius * UNIVERSE_SIZE_OFFSET;
	// get the system size
	m_fSystemSize = Min<float>( radius, NAVIGATION_MAX_SYSTEM_SIZE );

	// push all detected objects
	pushDetected( m_pContext );
	// push proxy objects
	for(int k=0;k<m_pContext->worldCount();++k)
		pushDetected( m_pContext->world(k) );

	if ( WidgetCast<NounTarget>( m_pDoc->target() ) )
		m_Objects.push( m_pDoc->target() );
	if ( WidgetCast<NounTarget>( m_pDoc->cursorTarget() ) )
		m_Objects.push( m_pDoc->cursorTarget() );
	
	// force grid to rebuilt
	m_Grid = NULL;
}

void WindowNavigation::pushDetected( WorldContext * pWorld )
{
	for(int z=0;z<pWorld->zoneCount();++z)
	{
		NodeZone * pZone = pWorld->zone( z );
		for(int i=0;i<pZone->childCount();++i)
		{
			NounGame * pNoun = WidgetCast<NounGame>( pZone->child(i) );
			if ( pNoun != NULL && pNoun->isDetected( m_pDoc->factionId() ) )
				m_Objects.push( pNoun );
		}
	}
}

Color WindowNavigation::getControlColor( const Vector3 & position )
{
	// find closest planet!
	NounPlanet * pBest = NULL;
	float		bestRange = 1000000.0f;

	m_Objects.reset();
	while( m_Objects.valid() )
	{
		Noun * pContact = *m_Objects;
		m_Objects.next();

		if ( WidgetCast<NounPlanet>( pContact ) )
		{
			float range = (pContact->worldPosition() - position).magnitude();
			if ( range < bestRange )
			{
				bestRange = range;
				pBest = (NounPlanet *)pContact;
			}
		}
	}

	if ( pBest != NULL )
	{
		int nFactionId = m_pDoc->factionId();
		int nBestFactionId = pBest->factionId();
		if ( nBestFactionId == nFactionId )
			return Color( 0, 255, 0, 64 );		// friendly
		else if ( nBestFactionId != 0 )
			return Color( 255, 0, 0, 64 );		// enemy
	}

	// neutral, default to black
	return Color( 0, 0, 0, 0 );
}

Material * WindowNavigation::getContactIcon( Noun * pContact )
{
	Material * pMaterial = findContactIcon( pContact );
	if ( pMaterial != NULL )
		pMaterial->setLightEnable( false );

	return pMaterial;
}

Material * WindowNavigation::findContactIcon( Noun * pContact )
{
	if ( WidgetCast<NounShip>( pContact ) )
		return WidgetCast<Material>( resource( NounShip::typeText( ((NounShip *)pContact)->type() ) ) );
	if ( WidgetCast<NounPlanet>( pContact ) )
		return WidgetCast<Material>( resource( "PLANET" ) );
	if ( WidgetCast<NounStar>( pContact ) )
		return WidgetCast<Material>( resource( "STAR" ) );
	if ( WidgetCast<NounNebula>( pContact ) )
		return WidgetCast<Material>( resource( "NEBULA" ) );
	if ( WidgetCast<NounAsteroid>( pContact ) )
		return WidgetCast<Material>( resource( "ASTEROID" ) );
	if ( WidgetCast<NounTarget>( pContact ) )
		return WidgetCast<Material>( resource( "TARGET" ) );
	if ( WidgetCast<NounJumpGate>( pContact ) )
		return WidgetCast<Material>( resource( "JUMPGATE" ) );
	if ( WidgetCast<NounProjectile>( pContact ) )
		return WidgetCast<Material>( resource( "PROJECTILE" ) );

	return WidgetCast<Material>( resource( "UNKNOWN" ) );
}

void WindowNavigation::buildGrid( DisplayDevice * pDisplay )
{
	// release any previous grid
	m_Grid = NULL;

	// build a triangle list which shows the zones of control
	float universeSize = m_fUniverseSize * 2;

	const float		SECTOR_SIZE = UNIVERSE_SECTOR / 2.0f;
	const int		GRID_STEPS = Min( (universeSize / SECTOR_SIZE) + 1, 10.0f ); 
	const float		GRID_CENTER = universeSize / 2.0f;
	const float		GRID_STEP = universeSize / GRID_STEPS;
	const float		GRID_HALF_STEP = GRID_STEP / 2.0f;
	const int		GRID_TRIANGLES = (GRID_STEPS * GRID_STEPS) * 4;

	if ( GRID_TRIANGLES > 0 )
	{
		TriangleL *		pTriangles = new TriangleL[ GRID_TRIANGLES ];
		TriangleL *		pTriangle = pTriangles;

		for(int z=0;z<GRID_STEPS;z++)
			for(int x=0;x<GRID_STEPS;x++)
			{
				// get the grid block center
				Vector3 v0( (GRID_STEP * x) - GRID_CENTER, 0, (GRID_STEP * z) - GRID_CENTER );

				// get the corner positions
				Vector3 v1( v0.x - GRID_HALF_STEP, 0, v0.z - GRID_HALF_STEP );
				Vector3 v2( v0.x + GRID_HALF_STEP, 0, v0.z - GRID_HALF_STEP );
				Vector3 v3( v0.x + GRID_HALF_STEP, 0, v0.z + GRID_HALF_STEP );
				Vector3 v4( v0.x - GRID_HALF_STEP, 0, v0.z + GRID_HALF_STEP );

				// get the vertex colors
				Color c0( getControlColor( v0 ) );
				Color c1( getControlColor( v1 ) );
				Color c2( getControlColor( v2 ) );
				Color c3( getControlColor( v3 ) );
				Color c4( getControlColor( v4 ) );

				pTriangle->v[ 0 ] = VertexL( v0, ZERO_N, c0, 0.5f, 0.5f );
				pTriangle->v[ 1 ] = VertexL( v2, ZERO_N, c2, 1, 0 );
				pTriangle->v[ 2 ] = VertexL( v1, ZERO_N, c1, 0, 0 );
				pTriangle++;
				
				pTriangle->v[ 0 ] = VertexL( v0, ZERO_N, c0, 0.5f, 0.5f );
				pTriangle->v[ 1 ] = VertexL( v3, ZERO_N, c3, 1, 1 );
				pTriangle->v[ 2 ] = VertexL( v2, ZERO_N, c2, 1, 0 );
				pTriangle++;

				pTriangle->v[ 0 ] = VertexL( v0, ZERO_N, c0, 0.5f, 0.5f );
				pTriangle->v[ 1 ] = VertexL( v4, ZERO_N, c4, 0, 1 );
				pTriangle->v[ 2 ] = VertexL( v3, ZERO_N, c3, 1, 1 );
				pTriangle++;
				
				pTriangle->v[ 0 ] = VertexL( v0, ZERO_N, c0, 0.5f, 0.5f );
				pTriangle->v[ 1 ] = VertexL( v1, ZERO_N, c1, 0, 0 );
				pTriangle->v[ 2 ] = VertexL( v4, ZERO_N, c4, 0, 1 );
				pTriangle++;
			}

		m_Grid = PrimitiveTriangleListL::create( pDisplay, GRID_TRIANGLES, pTriangles );
		delete pTriangles;
	}
}

//----------------------------------------------------------------------------
//EOF
