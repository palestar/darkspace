/*
	ViewObserve.cpp
	(c)2000 Palestar, Richard Lyle
*/

#include "Interface/GameDocument.h"
#include "Interface/ViewObserve.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( ViewObserve, WindowView::View );
REGISTER_FACTORY_KEY( ViewObserve, 4252863914088943197 );

ViewObserve::ViewObserve()
{}

//----------------------------------------------------------------------------

void ViewObserve::onActivate()
{
	//{{BEGIN_DATA_INIT
	m_pTargetInfo = WidgetCast<WindowText>( window()->findNode( "TargetInfo" ) );
	m_pButtonChase = WidgetCast<WindowButton>( window()->findNode( "ButtonChase" ) );
	m_pButtonIso = WidgetCast<WindowButton>( window()->findNode( "ButtonIso" ) );
	m_pButtonFlyby = WidgetCast<WindowButton>( window()->findNode( "ButtonFlyby" ) );
	m_pButtonFollow = WidgetCast<WindowButton>( window()->findNode( "ButtonFollow" ) );
	m_pObserveWindow = WidgetCast<WindowObserve>( window()->findNode( "ObserveWindow" ) );
	m_pButtonLock = WidgetCast<WindowButton>( window()->findNode( "ButtonLock" ) );
	m_pButtonPlanets = WidgetCast<WindowButton>( window()->findNode( "ButtonPlanets" ) );
	m_pButtonBattle = WidgetCast<WindowButton>( window()->findNode( "ButtonBattle" ) );
	m_pButtonAuto = WidgetCast<WindowButton>( window()->findNode( "ButtonAuto" ) );
	//END_DATA_INIT}}

	// called when this view is created
	m_pObserveWindow->setLingerTime( 30.0f );
	m_pObserveWindow->setCameraDistance( 10.0f );
}

void ViewObserve::onDeactivate()
{
	// called before this view is destroyed
}

void ViewObserve::onUpdate( float t )
{
	m_pButtonChase->setAlpha( Clamp( m_pButtonChase->alpha() + ((m_pObserveWindow->cameraMode() == WindowObserve::CHASE) ? t : -t ), 0.35f, 1.0f ) );
	m_pButtonIso->setAlpha( Clamp( m_pButtonIso->alpha() + ((m_pObserveWindow->cameraMode() == WindowObserve::ISOMETRIC) ? t : -t ), 0.35f, 1.0f ) );
	m_pButtonFlyby->setAlpha( Clamp( m_pButtonFlyby->alpha() + ((m_pObserveWindow->cameraMode() == WindowObserve::FLYBY) ? t : -t ), 0.35f, 1.0f ) );
	m_pButtonFollow->setAlpha( Clamp( m_pButtonFollow->alpha() + ((m_pObserveWindow->cameraMode() == WindowObserve::FOLLOW) ? t : -t ), 0.35f, 1.0f ) );

	m_pButtonLock->setAlpha( Clamp( m_pButtonLock->alpha() + ((m_pObserveWindow->targetMode() == WindowObserve::LOCKED) ? t : -t ), 0.35f, 1.0f ) );
	m_pButtonPlanets->setAlpha( Clamp( m_pButtonPlanets->alpha() + ((m_pObserveWindow->targetMode() == WindowObserve::AUTO_PLANET) ? t : -t) , 0.35f, 1.0f ) );
	m_pButtonBattle->setAlpha( Clamp( m_pButtonBattle->alpha() + ((m_pObserveWindow->targetMode() == WindowObserve::AUTO_BATTLE) ? t : -t) , 0.35f, 1.0f ) );
	m_pButtonAuto->setAlpha( Clamp( m_pButtonAuto->alpha() + ((m_pObserveWindow->targetMode() == WindowObserve::AUTO) ? t : -t) , 0.35f, 1.0f ) );

	if ( m_pObserveWindow->target() != NULL )
	{
		m_pTargetInfo->showWindow();
		m_pTargetInfo->setText( m_pObserveWindow->target()->name() );
	}
	else
		m_pTargetInfo->hideWindow();
}

bool ViewObserve::onMessage( const Message & msg )
{
	//{{BEGIN_MSG_MAP
	MESSAGE_MAP( WB_BUTTONUP, 1479207027, onButtonPrev);
	MESSAGE_MAP( WB_BUTTONUP, 1505827891, onButtonNext);
	MESSAGE_MAP( WB_BUTTONUP, 1508144083, onButtonLock);
	MESSAGE_MAP( WB_BUTTONUP, 587141835, onButtonChase);
	MESSAGE_MAP( WB_BUTTONUP, 2597179989, onButtonIso);
	MESSAGE_MAP( WB_BUTTONUP, 682841419, onButtonFlyby);
	MESSAGE_MAP( WB_BUTTONUP, 289274757, onButtonFollow);
	MESSAGE_MAP( WB_BUTTONUP, 4254758514, onButtonPlanets);
	MESSAGE_MAP( WB_BUTTONUP, 212989380, onButtonBattle);
	MESSAGE_MAP( WB_BUTTONUP, 1496919891, onButtonAuto);
	MESSAGE_MAP( WB_BUTTONUP, 1501550643, onButtonExit);
	//END_MSG_MAP}}

	switch( msg.message )
	{
	case HM_MOUSEWHEEL:
		if ( int( msg.wparam ) > 0 )
			m_pObserveWindow->setCameraDistance( m_pObserveWindow->cameraDistance() - 1.0f );
		else
			m_pObserveWindow->setCameraDistance( m_pObserveWindow->cameraDistance() + 1.0f );
		return true;
	case HM_KEYDOWN:
		switch( msg.wparam )
		{
		case 'Z':
			m_pObserveWindow->setCameraDistance( m_pObserveWindow->cameraDistance() - 1.0f );
			return true;
		case 'X':
			m_pObserveWindow->setCameraDistance( m_pObserveWindow->cameraDistance() + 1.0f );
			return true;
		}
		break;
	}

	return false;
}

void ViewObserve::onDocumentUpdate()
{
	// document data has changed, update this view if needed
}

//----------------------------------------------------------------------------


bool ViewObserve::onButtonExit(const Message & msg)
{
	document()->setScene( "Main" );
	return true;
}

bool ViewObserve::onButtonAuto(const Message & msg)
{
	m_pObserveWindow->setTargetMode( WindowObserve::AUTO );
	return true;
}

bool ViewObserve::onButtonBattle(const Message & msg)
{
	m_pObserveWindow->setTargetMode( WindowObserve::AUTO_BATTLE );
	return true;
}

bool ViewObserve::onButtonPlanets(const Message & msg)
{
	m_pObserveWindow->setTargetMode( WindowObserve::AUTO_PLANET );
	return true;
}

bool ViewObserve::onButtonLock(const Message & msg)
{
	m_pObserveWindow->setTargetMode( WindowObserve::LOCKED );
	return true;
}


bool ViewObserve::onButtonFollow(const Message & msg)
{
	m_pObserveWindow->setCameraMode( WindowObserve::FOLLOW );
	return true;
}

bool ViewObserve::onButtonFlyby(const Message & msg)
{
	m_pObserveWindow->setCameraMode( WindowObserve::FLYBY );
	return true;
}

bool ViewObserve::onButtonIso(const Message & msg)
{
	m_pObserveWindow->setCameraMode( WindowObserve::ISOMETRIC );
	return true;
}

bool ViewObserve::onButtonChase(const Message & msg)
{
	m_pObserveWindow->setCameraMode( WindowObserve::CHASE );
	return true;
}

bool ViewObserve::onButtonNext(const Message & msg)
{
	m_pObserveWindow->nextTarget();
	return true;
}

bool ViewObserve::onButtonPrev(const Message & msg)
{
	m_pObserveWindow->prevTarget();
	return true;
}

