/*
	ViewSelectSpawn.cpp
	(c)2000 Palestar, Richard Lyle
*/

#define INTERFACE_DLL
#include "Debug/Assert.h"
#include "Interface/ViewSelectSpawn.h"
#include "Interface/GameDocument.h"
#include "Interface/WindowObserve.h"
#include "Game/NounJumpGate.h"

//----------------------------------------------------------------------------

Constant UPDATE_SPAWN_TIME( "UPDATE_SPAWN_TIME", 5.0f );

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( ViewSelectSpawn, WindowView::View );
REGISTER_FACTORY_KEY( ViewSelectSpawn, 4493036934062758419 );

ViewSelectSpawn::ViewSelectSpawn() : m_UpdateSpawnList( 0.0f )
{}

//----------------------------------------------------------------------------

void ViewSelectSpawn::onActivate()
{
	//{{BEGIN_DATA_INIT
	m_pListSpawn = WidgetCast<WindowList>( window()->findNode( "ListSpawn" ) );
	m_pChatEdit = WidgetCast<WindowEdit>( window()->findNode( "ChatEdit" ) );
	m_pChatEditWindow = WidgetCast<NodeWindow>( window()->findNode( "ChatEditWindow" ) );
	m_pTextSpawn = WidgetCast<WindowText>( window()->findNode( "TextSpawn" ) );
	m_pButtonOkay = WidgetCast<WindowButton>( window()->findNode( "ButtonOkay" ) );
	m_pObserveWindow = WidgetCast<NodeWindow>( window()->findNode( "ObserveWindow" ) );
	//END_DATA_INIT}}

	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );

	// make sure the fleetId is valid
	if (! pDoc->isFleetValid() )
		pDoc->setScene( "SelectTeam" );

	m_Spawn.release();
	m_pListSpawn->removeAll();
	m_pListSpawn->addColumn( -1, Color(255,255,255,255) );

	updateSpawnList();
	m_UpdateSpawnList = UPDATE_SPAWN_TIME;

	m_pTextSpawn->setText( "" );
	m_pButtonOkay->setEnable( false );
	m_pChatEditWindow->hideWindow();

	// inform script we are entering the game
	pDoc->context()->scriptCall( "onSpawnSelect()" );

	// use previous spawn target if any
	if ( pDoc->spawnTarget() )
	{
		// select previous spawn target
		onSelectSpawn( pDoc->spawnTarget() );
	}
	else if ( m_Spawn.size() == 1 )
	{
		// select the one and only spawn point
		onSelectSpawn( m_Spawn[ 0 ] );
		// move onto the next view
		selectSpawn();
	}
}

void ViewSelectSpawn::onDeactivate()
{
	m_Spawn.release();
}

void ViewSelectSpawn::onUpdate( float t )
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );

	// validate the spawn point, it may be lost while we are selecting a ship so check it every update!
	if (! pDoc->isFleetValid() )
		pDoc->setScene( "SelectTeam" );

	// show/hide chat edit window
	m_pChatEditWindow->setVisible( m_pChatEdit->editing() );

	m_UpdateSpawnList -= t;
	if ( m_UpdateSpawnList < 0.0f )
	{
		// update the spawn list now
		updateSpawnList();
		m_UpdateSpawnList = UPDATE_SPAWN_TIME;
	}
}

bool ViewSelectSpawn::onMessage( const Message & msg )
{
	//{{BEGIN_MSG_MAP
	MESSAGE_MAP( WL_SELECT, 2737315405, onListSpawn);
	MESSAGE_MAP( WE_EDITDONE, 3966988240, onChatEdit);
	MESSAGE_MAP( WB_BUTTONUP, 1293570276, onButtonCancel);
	MESSAGE_MAP( WB_BUTTONUP, 1507228051, onButtonOkay);
	//END_MSG_MAP}}

	switch( msg.message )
	{
	case HM_CHAR:
		switch( msg.wparam )
		{
		case HK_RETURN:
			m_pChatEditWindow->showWindow();
			m_pChatEdit->onBeginEdit();
			return true;
		}
		break;
	}

	return false;
}

void ViewSelectSpawn::onDocumentUpdate()
{
	// document data has changed, update this view if needed
}

//----------------------------------------------------------------------------


bool ViewSelectSpawn::onButtonOkay(const Message & msg)
{
	selectSpawn();
	return true;
}

bool ViewSelectSpawn::onButtonCancel(const Message & msg)
{
	document()->setScene( "Main" );
	return true;
}

bool ViewSelectSpawn::onChatEdit(const Message & msg)
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );

	String send( m_pChatEdit->text() );
	if ( send.length() > 0 )
	{
		pDoc->sendChat( send );
		m_pChatEdit->setText( "" );
	}
	return true;
}

bool ViewSelectSpawn::onListSpawn(const Message & msg)
{
	int selectedSpawn = msg.lparam;
	if ( selectedSpawn >= 0 && selectedSpawn < m_Spawn.size() )
		onSelectSpawn( m_Spawn[ selectedSpawn ] );
	else
		onSelectSpawn( NULL );

	return true;
}

//----------------------------------------------------------------------------

void ViewSelectSpawn::onSelectSpawn( Noun * pSpawn )
{
	if ( pSpawn != NULL && m_Spawn.search( pSpawn ) >= 0 )
	{
		m_rSelectedSpawn = pSpawn;

		// are we de-selecting the spawn position
		if ( m_rSelectedSpawn.valid() )
		{
			m_pButtonOkay->setEnable( true );

			if ( WidgetCast<NounPlanet>( pSpawn ) )
			{
				NounPlanet * pPlanet = (NounPlanet *)pSpawn;

				String info;
				info += String().format("<color;ffffff;Bold>%s</Bold>\n\n", pPlanet->name() );
				info += String().format("Star System: <X;100>%s\n", pPlanet->starSystem() );
				info += String().format("Fleet: <X;100>%s\n", pPlanet->fleetName() );
				info += String().format("Population: <X;100>%d\n", ((int)pPlanet->population()) );
				info += String().format("Technology: <X;100>%.0f\n", pPlanet->technology() );
				info += String().format("Units: <X;100>%d\n", pPlanet->unitCount() );
				info += String().format("Structures: <X;100>%d\n", pPlanet->structureCount() );
				
				info += "\nResources:\n";
				for(int i=0;i<RESOURCE_TYPE_LAST;i++)
					info += String().format("%s: <X;100>%d\n", CargoResource::typeName( (ResourceType)i ), 
						pPlanet->resources( (ResourceType)i ) );

				m_pTextSpawn->setText( info );
			}
			else if ( WidgetCast<NounJumpGate>( pSpawn ) )
			{
				NounJumpGate * pGate = (NounJumpGate *)pSpawn;

				String info;
				info += String().format("<color;ffffff;Bold>%s</Bold>\n\n", pGate->name() );
				info += String().format("Star System: <X;100>%s\n", pGate->starSystem() );
				info += String().format("Fleet: <X;100>%s\n", pGate->fleetName() );

				m_pTextSpawn->setText( info );
			}

			((WindowObserve *)m_pObserveWindow)->setTargetMode( WindowObserve::LOCKED );
			((WindowObserve *)m_pObserveWindow)->setCameraMode( WindowObserve::CHASE );
			((WindowObserve *)m_pObserveWindow)->setTarget( pSpawn );
		}
		else
		{
			m_pButtonOkay->setEnable( false );
			m_pTextSpawn->setText( "" );
		}
	}
	else
		m_rSelectedSpawn = NULL;
}

void ViewSelectSpawn::selectSpawn()
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );

	// set the spawn target
	pDoc->setSpawnTarget( m_rSelectedSpawn );
	// change the scene if it's a valid target
	if ( pDoc->isSpawnValid() )
		pDoc->setScene( "SelectShip" );

	pDoc->context()->scriptCall( String().format("onSpawnSelected( %s )", m_rSelectedSpawn.key().string()) );
}

void ViewSelectSpawn::updateSpawnList()
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );
	UniverseContext * pContext = pDoc->context();
	ASSERT( pContext );

	int myFaction = pDoc->factionId();
	int highlighted = m_pListSpawn->highlighted();
	int selected = m_pListSpawn->selected();

	// reset the update time
	m_UpdateSpawnList = 1.0f;
	// remove all items from the list
	m_pListSpawn->removeAllItems();
	// release our spawn list
	m_Spawn.release();

	// make a list of all spawn positions in alpha order
	for(int i=0;i<pContext->zoneCount();i++)
	{
		NodeZone * pZone = pContext->zone( i );
		for(int j=0;j<pZone->childCount();j++)
		{
			Noun * pSpawn = NULL;

			BaseNode * pChild = pZone->child(j);
			if ( dynamic_cast<NounPlanet *>( pChild ) )
			{
				NounPlanet * pPlanet = (NounPlanet *)pChild;
				if ( pPlanet->factionId() == myFaction && (pPlanet->flags() & NounPlanet::FLAG_HAS_SHIPYARD) != 0 )
					pSpawn = pPlanet;
			}
			else if ( dynamic_cast<NounJumpGate *>( pChild ) )
			{
				NounJumpGate * pGate = (NounJumpGate *)pChild;
				if ( pGate->factionId() == myFaction )
					pSpawn = pGate;
			}

			if ( pSpawn != NULL )
			{
				// add item to list
				m_pListSpawn->addItem( pSpawn->name() );
				// add to internal list
				m_Spawn.push( pSpawn );
			}
		}
	}

	// restore the currently highlighted item
	m_pListSpawn->setHighlighted( highlighted );
	m_pListSpawn->setSelected( selected );
}

//----------------------------------------------------------------------------
// EOF
