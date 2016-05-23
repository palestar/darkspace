/*
	ViewSelectTeam.cpp
	(c)2000 Palestar, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Debug/Trace.h"
#include "Interface/GameDocument.h"
#include "DarkSpace/NounJumpGate.h"
#include "Interface/WindowObserve.h"
#include "Interface/ViewSelectTeam.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( ViewSelectTeam, WindowView::View );
REGISTER_FACTORY_KEY( ViewSelectTeam, 4096660768430088832 );

ViewSelectTeam::ViewSelectTeam()
{
	// Construct your view class
}

//----------------------------------------------------------------------------

void ViewSelectTeam::onActivate()
{
	//{{BEGIN_DATA_INIT
	m_pObserveWindow = WidgetCast<NodeWindow>( window()->findNode( "ObserveWindow" ) );
	m_pButtonOkay = WidgetCast<WindowButton>( window()->findNode( "ButtonOkay" ) );
	m_pTeamList = WidgetCast<WindowList>( window()->findNode( "TeamList" ) );
	m_pTeamDescription = WidgetCast<WindowText>( window()->findNode( "TeamDescription" ) );
	//END_DATA_INIT}}

	GameDocument * pDoc = (GameDocument *)document();
	if (! pDoc )
		return;
	GameProfile * pProfile = pDoc->profile();
	if (! pProfile )
	{
		pDoc->setScene( "Main" );
		return;
	}

	// if team is already valid, move onto spawn selection
	if ( pDoc->isTeamValid() )
		pDoc->setScene( "SelectShip" );

	m_SelectedTeam = -1;

	((WindowObserve *)m_pObserveWindow)->setCameraDistance( 5.0f );

	// initialize the team list
	m_pTeamList->removeAll();
	m_pTeamList->addColumn( -1, Color(255,255,255,255) );

	GameContext * pContext = pDoc->context();
	ASSERT( pContext );

	// get my clan
	dword myClan = pProfile->clanId();

	// is or is we not a moderator
	bool bCheckJoin = !(pProfile->isModerator() || pProfile->isAdmin() || pProfile->isDeveloper());

	m_pTeamList->removeAllItems();
	for(int i=0;i<pContext->teamCount();i++)
	{
		const GameContext::Team & team = pContext->teamN( i );
		if (! team.pTemplates.valid() )
			continue;		// skip team, no ships
		if ( bCheckJoin && (team.flags & GameContext::TF_NOJOIN) != 0 )
			continue;		// join is disabled for this team

		if ( team.clanId == 0 || team.clanId == myClan )
			m_pTeamList->addItem( CharString().format(" <color;ff00ffff>%s</color>", team.name ) );
		else
			m_pTeamList->addItem( CharString().format("<color;60006060>%s</color>", team.name ) );

		// push team into array, since the team list may change over time while we are selecting
		m_Teams.push( team );
	}

	// set the team description
	m_pTeamDescription->setText( "Please select a team" );
	// hide the okay button until a valid team is selected
	m_pButtonOkay->setEnable( false );

	pDoc->context()->scriptCall( "onFleetSelect()" );

	// auto-select if there is only 1 team
	if ( m_Teams.size() == 1 )
	{
		int nTeamId = m_Teams[ 0 ].nTeamId;

		((GameDocument *)document())->client()->selectTeam( nTeamId );
		pDoc->context()->scriptCall( CharString().format("onFleetSelected( %d )", nTeamId) );
	}

	// make cursor visible
	setCursorState( POINTER );
}

void ViewSelectTeam::onDeactivate()
{
	// called before this view is destroyed
	m_Teams.release();
}

void ViewSelectTeam::onUpdate( float t )
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );

	// check if team is valid, move onto spawn selection if so
	if ( pDoc->isTeamValid() )
		pDoc->setScene( "SelectShip" );
}

bool ViewSelectTeam::onMessage( const Message & msg )
{
	//{{BEGIN_MSG_MAP
	MESSAGE_MAP( WL_SELECT, 3739586744, onSelectTeam);
	MESSAGE_MAP( WB_BUTTONUP, 1293570276, onButtonCancel);
	MESSAGE_MAP( WB_BUTTONUP, 1507228051, onButtonOkay);
	//END_MSG_MAP}}

	return false;
}

void ViewSelectTeam::onDocumentUpdate()
{
	// document data has changed, update this view if needed
}

//----------------------------------------------------------------------------


bool ViewSelectTeam::onButtonOkay(const Message & msg)
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );

	int nTeamId = m_Teams[ m_SelectedTeam ].nTeamId;
	((GameDocument *)document())->client()->selectTeam( nTeamId );

	pDoc->context()->scriptCall( CharString().format("onFleetSelected( %d )", nTeamId) );

	return true;
}

bool ViewSelectTeam::onButtonCancel(const Message & msg)
{
	document()->setScene( "Main" );
	return true;
}

bool ViewSelectTeam::onSelectTeam(const Message & msg)
{
	m_SelectedTeam = msg.lparam;
	if ( m_SelectedTeam < 0 )
	{
		((WindowObserve *)m_pObserveWindow)->setTargetMode( WindowObserve::AUTO );

		m_pTeamDescription->setText( "Please select a team" );
		m_pButtonOkay->setEnable( false );
		return true;
	}

	// update the team description window
	GameContext::Team & team = m_Teams[ m_SelectedTeam ];
	m_pTeamDescription->setText( CharString().format("<large><color;ffffff>%s</large> - %s\n\n%s</color>", 
		team.name, team.longName, team.about ) );

	// enable the okay button
	m_pButtonOkay->setEnable( true );

	// find a jump gate for this team, set that as our target
	GameContext * pContext = ((GameDocument *)document())->context();
	ASSERT( pContext );

	if ( pContext->activeZone() != NULL )
	{
		BaseNode * pRoot = pContext->activeZone();
		for(int i=0;i<pRoot->childCount();i++)
		{
			NounJumpGate * pJumpGate = WidgetCast<NounJumpGate>( pRoot->child(i) );
			if ( pJumpGate != NULL )
			{
				if ( pJumpGate->factionId() == team.factionId )
				{
					((WindowObserve *)m_pObserveWindow)->setTargetMode( WindowObserve::LOCKED );
					((WindowObserve *)m_pObserveWindow)->setCameraMode( WindowObserve::CHASE );
					((WindowObserve *)m_pObserveWindow)->setTarget( pJumpGate );
					break;			
				}
			}
		}
	}

	return true;
}

