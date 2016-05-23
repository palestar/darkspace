/*
	WindowObserve.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Interface/WindowObserve.h"
#include "Interface/GameDocument.h"
#include "Gui3d/InterfaceContext.h"

//---------------------------------------------------------------------------------------------------

const float				FRONT_PLANE = 1.0f;
const float				BACK_PLANE = 5000000.0f;

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( WindowObserve, NodeWindow );
REGISTER_FACTORY_KEY( WindowObserve, 4216916891316462975 );

WindowObserve::WindowObserve()
{
	m_AutoTime = 0;
	m_LingerTime = 30.0f;
	m_CameraMode = ISOMETRIC;
	m_CameraDistance = 4.0f;
	m_CameraControl = false;
	m_TargetMode = AUTO;
}

//----------------------------------------------------------------------------

void WindowObserve::onActivate()
{
	GameDocument * pDoc = (GameDocument *)document();

	NodeWindow::onActivate();
	m_CameraControl = false;

	// if we have a current ship, automatically target and lock onto that ship..
	if ( pDoc->ship() != NULL )
	{
		setTarget( ((GameDocument *)document())->ship() );
		setTargetMode( WindowObserve::LOCKED );
		setCameraMode( WindowObserve::CHASE );
	}
}

void WindowObserve::onDeactivate()
{
	NodeWindow::onDeactivate();

	if ( m_CameraControl )
	{
		setCursorState( POINTER );
		m_CameraControl = false;
	}
}

bool WindowObserve::onMessage( const Message & msg )
{
	switch( msg.message )
	{
	case HM_MOUSEWHEEL:
		if ( int( msg.wparam ) > 0 )
			m_CameraDistance = Max<float>( m_CameraDistance - 1, 1 );
		else
			m_CameraDistance = Min<float>( m_CameraDistance + 1, 16 );
		return true;
	case HM_KEYDOWN:
		switch( msg.wparam )
		{
		case 'Z':
			m_CameraDistance = Max<float>( m_CameraDistance - 1, 0 );
			return true;
		case 'X':
			m_CameraDistance = Min<float>( m_CameraDistance + 1, 16 );
			return true;
		}
		return false;
	}

	return NodeWindow::onMessage( msg );
}

//----------------------------------------------------------------------------

const float TETHER_SLACK = 10.0f;
const float CAMERA_GLUE_RATE = 1.0f / 60.0f;
const float CAMERA_VELOCITY = 2000.0f;

void WindowObserve::onUpdate( float t )
{
	GameContext * pContext = ((GameDocument *)document())->context();
	if ( pContext != NULL )
	{
		if ( m_TargetMode != LOCKED )
		{
			m_AutoTime -= t;
			if ( m_AutoTime < 0.0f )
			{
				m_AutoTime = m_LingerTime;
				
				// change the camera mode
				if ( m_CameraMode == CHASE )
					m_CameraMode = FOLLOW;
				else if ( m_CameraMode == FOLLOW )
					m_CameraMode = FLYBY;
				else if ( m_CameraMode == FLYBY )
					m_CameraMode = ISOMETRIC;
				else
					m_CameraMode = CHASE;

				// find new camera target
				findNextTarget();

				// if target is ship, then use the chase camera
				if ( WidgetCast<NounShip>( m_CameraTarget.pointer() ) )
					m_CameraMode = CHASE;
			}
		}

		if ( m_CameraTarget.valid() )
		{
			float cameraZoom = m_CameraDistance * m_CameraTarget->radius();
			if (! m_CameraControl )
			{
				Vector3 destPosition( GameDocument::s_vCamera );
				Matrix33 destFrame( GameDocument::s_mCamera );

				switch( m_CameraMode )
				{
				case FOLLOW:
					{
						Vector3 targetPosition( m_CameraTarget->worldPosition() );
						Matrix33 targetFrame( m_CameraTarget->worldFrame() );

						destFrame = Matrix33( targetFrame.k );
						destPosition = targetPosition - (targetFrame.k * cameraZoom);
						destPosition.y += m_CameraTarget->radius() / 2.0f;
					}
					break;
				case FLYBY:
					{
						Vector3 targetPosition( m_CameraTarget->worldPosition() );
						Matrix33 targetFrame( m_CameraTarget->worldFrame() );

						destPosition = targetPosition + (targetFrame.k * cameraZoom);
						//destPosition += targetFrame.i * m_CameraTarget->radius();
						destPosition.y += m_CameraTarget->radius() / 2.0f;

						destFrame = Matrix33( (targetPosition - destPosition).normalize() );
					}
					break;
				case ISOMETRIC:
					{
						Vector3 targetPosition( m_CameraTarget->worldPosition() );

						destFrame = Matrix33( Vector3( 1, -1, 1 ).normalize() );
						destPosition = targetPosition - (destFrame.k * cameraZoom);
					}
					break;
				case CHASE:
					{
						Vector3 targetPosition( m_CameraTarget->worldPosition() );
						Matrix33 targetFrame( m_CameraTarget->worldFrame() );

						Vector3 view( targetPosition - GameDocument::s_vCamera );
						view.normalize();

						destPosition = targetPosition - (targetFrame.k * cameraZoom);
						destPosition.y += m_CameraTarget->radius();


						destFrame = Matrix33( view );
					}
					break;
				}

				float fTime = Clamp( t, 0.0f, 1.0f );
				// move the camera
				Vector3 deltaPosition( destPosition - GameDocument::s_vCamera );
				float positionGlue = Clamp( deltaPosition.magnitude() / TETHER_SLACK, 0.0f, 1.0f ) * fTime;
				GameDocument::s_vCamera += deltaPosition * positionGlue;

				// update the camera frame
				GameDocument::s_mCamera.i += (destFrame.i - GameDocument::s_mCamera.i) * fTime;
				GameDocument::s_mCamera.j += (destFrame.j - GameDocument::s_mCamera.j) * fTime;
				GameDocument::s_mCamera.k += (destFrame.k - GameDocument::s_mCamera.k) * fTime;
				GameDocument::s_mCamera.orthoNormalizeXY();
			}
			else
			{
				GameDocument::s_vCamera = m_CameraTarget->worldPosition() - (GameDocument::s_mCamera.k * cameraZoom );
			}
		}
	}
}

void WindowObserve::onRender( RenderContext & context, const RectInt & window )
{
	if ( document() != NULL )
	{
		GameContext * pContext = ((GameDocument *)document())->context();
		if ( pContext != NULL )
		{
			DisplayDevice * pDisplay = context.display();
			ASSERT( pDisplay );

			// end the previous scene
			context.endScene();

			// update the active zone
			pContext->setActiveZone( GameDocument::s_vCamera );

			RenderContext::SaveState state( context );
			context.setPosition( GameDocument::s_vCamera );
			context.setFrame( GameDocument::s_mCamera );
			context.setProjection( window, context.fov(), FRONT_PLANE, BACK_PLANE );
			context.setTime( pContext->tick() * TICK_DURATION_S );
			context.setShadowFocus( m_CameraTarget.valid() 
				? context.worldToView( m_CameraTarget->worldPosition() ) : Vector3( 0, 0, 1 ), SHADOW_MAP_FOCUS );

			// set the ambient light
			pDisplay->setAmbient( UNIVERSE_AMBIENT );
			// render the universe
			pContext->render( context, Matrix33::IDENTITY, Vector3::ZERO ); 

			context.beginScene();
		}
	}
}

const float CAMERA_PITCH_RATE = PI / 512;
const float CAMERA_YAW_RATE = PI / 256;

bool WindowObserve::onCursorMove( const RectInt & window, 
						const PointInt & position,
						const PointInt & delta )
{
	if ( m_CameraControl )
	{
		GameDocument::s_mCamera.rotate( CAMERA_PITCH_RATE * delta.y, CAMERA_YAW_RATE * delta.x, 0 );
		return true;
	}

	return NodeWindow::onCursorMove( window, position, delta );
}

bool WindowObserve::onRightDown( const RectInt & window,
						const PointInt & position )
{
	if ( window.inRect( position ) )
	{
		m_CameraControl = true;
		context()->lockFocus( this );
		setCursorState( HIDDEN );

		return true;
	}

	return NodeWindow::onRightDown( window, position );
}

bool WindowObserve::onRightUp( const RectInt & window,
						const PointInt & position )
{
	if ( m_CameraControl )
	{
		m_CameraControl = false;
		context()->unlockFocus( this );
		setCursorState( POINTER );

		return true;
	}

	return NodeWindow::onRightUp( window, position );
}

//----------------------------------------------------------------------------

float WindowObserve::lingerTime() const
{
	return m_LingerTime;
}

WindowObserve::CameraMode  WindowObserve::cameraMode() const
{
	return m_CameraMode;
}

float WindowObserve::cameraDistance() const
{
	return m_CameraDistance;
}

WindowObserve::TargetMode  WindowObserve::targetMode() const
{
	return m_TargetMode;
}

Noun *  WindowObserve::target() const
{
	return m_CameraTarget;
}

//----------------------------------------------------------------------------

void WindowObserve::setCameraMode( CameraMode mode )
{
	m_CameraMode = mode;
}

void WindowObserve::setCameraDistance( float distance )
{
	m_CameraDistance = distance;
}

void WindowObserve::setTargetMode( TargetMode mode )
{
	m_TargetMode = mode;

}

void WindowObserve::setTarget( Noun * pTarget )
{
	m_CameraTarget = pTarget;
	((GameDocument *)document())->setFocus( m_CameraTarget );
}

void WindowObserve::setLingerTime( float time )
{
	m_LingerTime = time;
}

void WindowObserve::nextTarget()
{
	updateTargets();
	if ( m_Targets.size() == 0 )
		return;

	Noun * pCurrent = target();
	for(int i=0;i<m_Targets.size();i++)
		if ( pCurrent == m_Targets[i] )
		{
			setTarget( m_Targets[ (i + 1) % m_Targets.size() ] );
			return;
		}

	setTarget( m_Targets[ 0 ] );
}

void WindowObserve::prevTarget()
{
	updateTargets();
	if ( m_Targets.size() == 0 )
		return;

	Noun * pCurrent = target();
	for(int i=1;i<m_Targets.size();i++)
		if ( pCurrent == m_Targets[i] )
		{
			setTarget( m_Targets[ i - 1 ] );
			return;
		}

	setTarget( m_Targets[ m_Targets.size() - 1 ] );
}

//----------------------------------------------------------------------------

void WindowObserve::updateTargets()
{
	m_Targets.release();

	GameContext * pContext = ((GameDocument *)document())->context();
	ASSERT( pContext );

	for( int j=0;j<pContext->zoneCount();++j)
	{
		BaseNode * pRoot = pContext->zone( j );
		if (! pRoot )
			continue;

		for(int i=0;i<pRoot->childCount();i++)
		{
			NounBody * pBody = WidgetCast<NounBody>( pRoot->child(i) );
			if ( pBody == NULL )
				continue;		// not a body object


			bool inserted = false;
			for(int k=0;k<m_Targets.size() && !inserted;k++)
			{
				Noun * pTarget = m_Targets[k];
				if( !pTarget )
					continue;

				if ( _stricmp( pBody->name(), pTarget->name() ) < 0 )
				{
					m_Targets.insert( k, pBody );
					inserted = true;
				}
			}

			if ( !inserted )
				m_Targets.push( pBody );
		}
	}
}

void WindowObserve::findNextTarget()
{
	ASSERT( m_TargetMode != LOCKED );

	updateTargets();
	if ( m_Targets.size() == 0 )
		return;

	GameContext * pContext = ((GameDocument *)document())->context();
	BaseNode * pRoot = pContext->activeZone();
	if ( pRoot == NULL )
		return;

	bool objectFound = false;
	bool nextObject = m_CameraTarget.valid() ? false : true;

	Noun * pFirstObject = NULL;
	for(int i=0;i<m_Targets.size();i++)
	{
		Noun * pNoun = m_Targets[ i ];
		
		bool valid = false;
		switch( m_TargetMode )
		{
		case AUTO:			// automatically find interesting targets
			if ( WidgetCast<NounShip>( pNoun ) != NULL || WidgetCast<NounPlanet>( pNoun ) != NULL )
				valid = true;
			break;
		case AUTO_TEAM:		// find other targets on the same team
			if ( ((NounBody *)pNoun)->teamId() == ((GameDocument *)document())->teamId() )
				valid = true;
			break;
		case AUTO_BATTLE:	// automatically find battles between ships
			if ( WidgetCast<NounShip>( pNoun ) != NULL )
			{
				NounShip * pShip = (NounShip *)pNoun;

				for(int j=0;j<m_Targets.size();j++)
				{
					Noun * pTarget = m_Targets[j];
					if ( j != i && WidgetCast<NounShip>( pTarget ) )
						if ( pShip->isEnemy( (NounShip *)pTarget ) )
							if ( (pShip->worldPosition() - ((NounShip *)pTarget)->worldPosition()).magnitude() < 1000.0f )
							{
								// another ship is within range
								valid = true;
							}
				}
			}
			break;
		case AUTO_PLANET:	// automatically view different planets
			if ( WidgetCast<NounPlanet>( pNoun ) != NULL )
				valid = true;
			break;
		case AUTO_SHIP:
			if ( WidgetCast<NounShip>( pNoun ) != NULL )
				valid = true;
			break;
		}
		
		if (! valid )
			continue;		// not a valid object

		if ( nextObject )
		{
			setTarget( (Noun *)pNoun );
			objectFound = true;
			break;
		}
		if ( pFirstObject == NULL )
			pFirstObject = (Noun *)pNoun;
		if ( pNoun == m_CameraTarget )
			nextObject = true;
	}

	if (! objectFound )
		setTarget( pFirstObject );
}

//----------------------------------------------------------------------------
//EOF
