/*
	ViewSelectShip.cpp
	(c)2000 Palestar, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Standard/Time.h"
#include "Display/PrimitiveSetTransform.h"
#include "Display/PrimitiveLineList.h"
#include "Display/PrimitiveMaterial.h"
#include "Gui3d/WindowLayout.h"
#include "Gui3d/InterfaceContext.h"
#include "Interface/GameDocument.h"
#include "DarkSpace/GadgetWeapon.h"
#include "DarkSpace/GadgetBeamWeapon.h"
#include "DarkSpace/CargoEnhancement.h"
#include "DarkSpace/CargoGadget.h"
#include "DarkSpace/NounUnit.h"
#include "DarkSpace/NounJumpGate.h"
#include "DarkSpace/FieldEnhancement.h"
#include "DarkSpace/TraitDisplayName.h"
#include "DarkSpace/TierInfo.h"

#include "WindowObserve.h"
#include "ViewSelectShip.h"
#include "WindowShipSelection.h"

#include <stdio.h>

//----------------------------------------------------------------------------

static Constant		UPDATE_SHIP_LIST_TIME( "UPDATE_SHIP_LIST_TIME", 1.0f );

// imported from GameServer.cpp
DLL_IMPORT Constant STORAGE_REPAIR_PER_SECOND;
DLL_IMPORT Constant TRANSFER_SPAWN_COST;

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( ViewSelectShip, WindowView::View );
REGISTER_FACTORY_KEY( ViewSelectShip, 4096637078369204814 );

ViewSelectShip::ViewSelectShip() : m_bShipSelected( false ), m_nRank( 0 )
{
	m_FilterType = NounShip::UNKNOWN;
}

//----------------------------------------------------------------------------

void ViewSelectShip::onActivate()
{
	//{{BEGIN_DATA_INIT
	DATA_MAP( m_pButtonBuyCargo, WindowButton, "ButtonBuyCargo" );
	DATA_MAP( m_pItemDescription, WindowText, "ItemDescription" );
	DATA_MAP( m_pCreditsStatus, WindowText, "CreditsStatus" );
	DATA_MAP( m_pItemList, WindowList, "ItemList" );
	DATA_MAP( m_pBuyWindow, NodeWindow, "BuyWindow" );
	DATA_MAP( m_pTextLabel, WindowText, "Label" );
	DATA_MAP( m_pNavigationWindow, WindowNavigation, "NavigationWindow" );
	DATA_MAP( m_pTextShips, WindowText, "TextShips" );
	DATA_MAP( m_pTextCargo, WindowText, "TextCargo" );
	DATA_MAP( m_pButtonRemoveCargo, WindowButton, "ButtonRemoveCargo" );
	DATA_MAP( m_pCargoButtons, NodeWindow, "CargoButtons" );
	DATA_MAP( m_pButtonRemoveShip, WindowButton, "ButtonRemoveShip" );
	DATA_MAP( m_pSlotButtons, NodeWindow, "SlotButtons" );
	DATA_MAP( m_pLeftConsole, NodeWindow, "LeftConsole" );
	DATA_MAP( m_pButtonUnknown, WindowButton, "ButtonUnknown" );
	DATA_MAP( m_pButtonStation, WindowButton, "ButtonStation" );
	DATA_MAP( m_pButtonSupply, WindowButton, "ButtonSupply" );
	DATA_MAP( m_pButtonEngineer, WindowButton, "ButtonEngineer" );
	DATA_MAP( m_pButtonTransport, WindowButton, "ButtonUnknown" );
	DATA_MAP( m_pButtonUnknown, WindowButton, "ButtonTransport" );
	DATA_MAP( m_pButtonDread, WindowButton, "ButtonDread" );
	DATA_MAP( m_pButtonCruiser, WindowButton, "ButtonCruiser" );
	DATA_MAP( m_pButtonDestroyer, WindowButton, "ButtonDestroyer" );
	DATA_MAP( m_pButtonFrigate, WindowButton, "ButtonFrigate" );
	DATA_MAP( m_pButtonScout, WindowButton, "ButtonScout" );
	DATA_MAP( m_pShipButtons, NodeWindow, "ShipButtons" );
	DATA_MAP( m_pIncomingMessages, NodeWindow, "IncomingMessages" );
	DATA_MAP( m_pChatEdit, WindowEdit, "ChatEdit" );
	DATA_MAP( m_pChatEditWindow, NodeWindow, "ChatEditWindow" );
	DATA_MAP( m_pShipDescriptionFrame, NodeWindow, "ShipDescriptionFrame" );
	DATA_MAP( m_pShipRender, WindowShipSelection, "ShipRender" );
	DATA_MAP( m_pButtonOkay, WindowButton, "ButtonOkay" );
	DATA_MAP( m_pShipDescription, WindowText, "ShipDescription" );
	m_pMessageBoxCANCEL = WidgetCast<WindowButton>( window()->findNode( "MessageBoxCANCEL" ) );
	m_pMessageBoxOK = WidgetCast<WindowButton>( window()->findNode( "MessageBoxOK" ) );
	m_pMessageBox = WidgetCast<WindowText>( window()->findNode( "MessageBox" ) );
	DATA_MAP( m_pFilterEdit, WindowEdit, "FilterEdit" );
	DATA_MAP( m_pButtonTier3, WindowButton, "ButtonTier3" );
	DATA_MAP( m_pButtonTier2, WindowButton, "ButtonTier2" );
	DATA_MAP( m_pButtonTier1, WindowButton, "ButtonTier1" );	
	//END_DATA_INIT}}

	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );

	m_pLastSpawn = pDoc->lastSpawn();
	// hide message box
	m_pMessageBox->hideWindow();

	// make sure the teamId is valid
	if (! pDoc->isTeamValid() && !pDoc->client()->isServerTransfer() )
		pDoc->setScene( "SelectTeam" );		// team selection lost or invalid

	// select a spawn point if none is already selected, to make it more dummy proof!
	if (! m_pLastSpawn.valid() || m_pLastSpawn->context() != pDoc->context() )
	{
		GameContext * pContext = pDoc->context();
		ASSERT( pContext );

		Array< Noun * > spawns;
		for(int i=0;i<pContext->zoneCount();i++)
		{
			NodeZone * pZone = pContext->zone( i );
			for(int j=0;j<pZone->childCount();j++)
			{
				Noun * pNoun = WidgetCast<Noun>( pZone->child(j) );
				if (! pNoun )
					continue;

				if ( pDoc->validateSpawn( pNoun ) )
					spawns.push( pNoun );
			}
		}

		if ( spawns.size() > 0 )
			m_pLastSpawn = spawns[ rand() % spawns.size() ];
	}

	// show buy button only if server is persistant
	m_pButtonBuyCargo->setVisible( pDoc->client()->serverStatus().bPersistant );
	// request profile update
	pDoc->client()->sendProfile();
	// make our spawn the current target
	pDoc->setTarget( m_pLastSpawn );
	// put navigation window into spawn mode
	m_pNavigationWindow->onActivate();
	m_pNavigationWindow->setSelectMode( WindowNavigation::SELECT_SPAWN );
	m_pNavigationWindow->setShowFlags( WindowNavigation::SHOW_SPAWN|WindowNavigation::SHOW_ENEMY|
		WindowNavigation::SHOW_LINKS|WindowNavigation::SHOW_PLANETS|
		WindowNavigation::SHOW_ORBITS|WindowNavigation::SHOW_GRID );

	m_pNavigationWindow->onCenter();	// center the map on our current spawn position
	m_pNavigationWindow->onFull();

	m_pSelectedShip = NULL;
	m_SelectedCargo.release();

	m_UpdateShipList = UPDATE_SHIP_LIST_TIME;

	m_pCargoButtons->detachAllNodes();
	m_pSlotButtons->detachAllNodes();
	m_pShipButtons->detachAllNodes();

	m_pTextLabel->setText( "" );
	m_pShipDescription->setText( "" );
	m_pButtonOkay->setEnable( false );
	m_pChatEditWindow->hideWindow();
	m_pBuyWindow->hideWindow();
	
	// reset the camera time
	GameDocument::s_fCamera = 0.0f;
	// call the script
	pDoc->context()->scriptCall( "onShipSelect()" );
	// update the ship list on the first call to onUpdate()
	m_UpdateShipList = -1.0f;

	m_bRemoveShip = false;
	m_bRemoveCargo = false;

	// make cursor visible
	setCursorState( POINTER );
}

void ViewSelectShip::onDeactivate()
{
	// called before this view is destroyed
	m_pCargoButtons->detachAllNodes();
	m_pSlotButtons->detachAllNodes();
	m_pShipButtons->detachAllNodes();
}

void ViewSelectShip::onUpdate( float t )
{
	ASSERT( m_pShipButtons );

	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );
	GameClient * pClient = pDoc->client();
	ASSERT( pClient );
	GameContext * pContext = WidgetCast<GameContext>( pClient->context() );
	ASSERT( pContext );

	// validate the spawn point, it may be lost while we are selecting a ship so check it every update!
	if (! pDoc->isTeamValid() && !pDoc->client()->isServerTransfer() )
		pDoc->setScene( "SelectTeam" );		// team selection lost or invalid
	else if ( pDoc->isShipValid() )
		pDoc->setScene( "Game" );			// ship is already valid, move into the game

	// enable / disable the ok button...
	m_pButtonOkay->setEnable( m_bShipSelected && !pClient->inQueue() );

	// update the queue status..
	if ( pClient->inQueue() )
	{
		m_pTextLabel->setText( CharString().format("SERVER FULL, WAIT QUEUE POSITION IS %d of %d..", 
			pClient->queuePosition(), pClient->queueSize() ) );
	}
	else
		m_pTextLabel->setText( "" );

	// update the alpha of the filter buttons
	for(int i=0;i<NounShip::TYPE_COUNT;i++)
	{
		WindowButton * pButton = getFilterButton( (NounShip::Type)i );
		pButton->setIconColor( (m_FilterType == i || m_FilterType == NounShip::UNKNOWN) ? WHITE : GREY );
	}

	// show/hide chat edit window
	m_pChatEditWindow->setVisible( m_pChatEdit->editing() );

	// update the ship and cargo buttons
	m_UpdateShipList -= t;
	if ( m_UpdateShipList < 0.0f && pDoc->isTeamValid() )
	{
		m_UpdateShipList = UPDATE_SHIP_LIST_TIME;
		updateShipList();
	}

	// update our ship selection when our spawn target has change
	if ( pDoc->target() != m_pSelectedSpawn ||
		pDoc->rank() != m_nRank )
	{
		onSelectShip( m_pSelectedShip, pDoc->target() );
	}

	// update our credit amounts
	m_pCreditsStatus->setText( CharString().format("%s Credits", FormatNumber<char,int>( pClient->credits() ) ) );
	
	// display any message box at the front of the queue
	if (! m_pMessageBox->visible() && !m_DialogList.empty() )
	{
		m_pActiveDialog = m_DialogList.front();
		m_DialogList.pop_front();

		m_pMessageBox->setText( m_pActiveDialog->m_sMessage );

		switch( m_pActiveDialog->m_eType )
		{
		case MBT_OKCANCEL:			// ok & cancel buttons are displayed.
			m_pMessageBoxOK->setLabel( "OK" );
			m_pMessageBoxOK->setVisible( true );
			m_pMessageBoxCANCEL->setLabel( "CANCEL" );
			m_pMessageBoxCANCEL->setVisible( true );
			break;
		case MBT_OK:				// only the ok button is displayed.
			m_pMessageBoxOK->setLabel( "OK" );
			m_pMessageBoxOK->setVisible( true );
			m_pMessageBoxCANCEL->setVisible( false );
			break;
		case MBT_YESNO:				// yes and no buttons are displayed instead of ok & cancel.
			m_pMessageBoxOK->setLabel( "YES" );
			m_pMessageBoxOK->setVisible( true );
			m_pMessageBoxCANCEL->setLabel( "NO" );
			m_pMessageBoxCANCEL->setVisible( true );
			break;
		}

		m_pMessageBox->showWindow();
		m_pActiveDialog->onDispayed();
	}	
	
}

bool ViewSelectShip::onMessage( const Message & msg )
{
	//{{BEGIN_MSG_MAP
	MESSAGE_MAP( WB_BUTTONUP, 676493741, onButtonCancelBuy);
	MESSAGE_MAP( WB_BUTTONUP, 2380443940, onButtonBuyItem);
	MESSAGE_MAP( WB_BUTTONUP, 2836235633, onButtonBuyCargo);
	MESSAGE_MAP( WL_SELECT, 3739585841, onItemList);
	MESSAGE_MAP( WB_BUTTONUP, 36851163, onButtonRemoveCargo);
	MESSAGE_MAP( WB_BUTTONUP, 3642064065, onButtonRemoveShip);
	MESSAGE_MAP( WB_BUTTONUP, 1772059130, onButtonUnknown);
	MESSAGE_MAP( WB_BUTTONUP, 4231797196, onButtonStation);
	MESSAGE_MAP( WB_BUTTONUP, 1693709888, onButtonSupply);
	MESSAGE_MAP( WB_BUTTONUP, 1039534350, onButtonEngineer);
	MESSAGE_MAP( WB_BUTTONUP, 1167464310, onButtonTransport);
	MESSAGE_MAP( WB_BUTTONUP, 761092843, onButtonDread);
	MESSAGE_MAP( WB_BUTTONUP, 1440821965, onButtonCruiser);
	MESSAGE_MAP( WB_BUTTONUP, 3329186848, onButtonDestroyer);
	MESSAGE_MAP( WB_BUTTONUP, 1829270533, onButtonFrigate);
	MESSAGE_MAP( WB_BUTTONUP, 38275307, onButtonScout);
	MESSAGE_MAP( WE_EDITDONE, 3966988240, onChatEdit);
	MESSAGE_MAP( WB_BUTTONUP, 1293570276, onCancel);
	MESSAGE_MAP( WB_BUTTONUP, 1507228051, onOkay);
	MESSAGE_MAP( WB_BUTTONUP, 1947446322, onMessageBoxCANCEL);
	MESSAGE_MAP( WB_BUTTONUP, 857413197, onMessageBoxOK);
	MESSAGE_MAP( WE_EDITDONE, 3781312561, onFilterEdit);
	MESSAGE_MAP( WB_BUTTONUP, 216864779, onButtonTier3);
	MESSAGE_MAP( WB_BUTTONUP, 216864811, onButtonTier2);
	MESSAGE_MAP( WB_BUTTONUP, 216864843, onButtonTier1);
	MESSAGE_MAP( WB_BUTTONDOWN, 216864779, onButtonTier3);
	MESSAGE_MAP( WB_BUTTONDOWN, 216864811, onButtonTier2);
	MESSAGE_MAP( WB_BUTTONDOWN, 216864843, onButtonTier1);
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

void ViewSelectShip::onDocumentUpdate()
{
	// document data has changed, update this view if needed
}

void ViewSelectShip::onRender( RenderContext & context, const RectInt & window )
{}

//----------------------------------------------------------------------------

void ViewSelectShip::pushMessageBox(MessageBoxDialog * pDialog)
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT(pDoc);
	m_DialogList.push_back(pDialog);
	pDialog->m_pDoc = pDoc;
}

void ViewSelectShip::popMessageBox(MessageBoxDialog * pDialog)
{
	if (pDialog == m_pActiveDialog)
	{
		m_pMessageBox->hideWindow();
		m_pActiveDialog = NULL;
	}
	else
		m_DialogList.remove(pDialog);
}

//----------------------------------------------------------------------------


bool ViewSelectShip::onOkay(const Message & msg)
{
	GameDocument * pDoc = (GameDocument *)document();
	if (! pDoc )
		return false;
	GameClient * pClient = pDoc->client();
	if (! pClient )
		return false;
	Noun * pSpawnTarget = pDoc->target();
	if (! pSpawnTarget )
		return false;
	if (! m_pSelectedShip )
		return false;

	Array< WidgetKey > cargo;
	for(int i=0;i<m_SelectedCargo.size();i++)
	{
		Noun * pCargo = m_SelectedCargo[i];
		if ( pCargo != NULL )
			cargo.push( pCargo->key() );
	}

	pClient->selectSelf( pSpawnTarget->key(), m_pSelectedShip->key(), cargo );

	// callback into the script
	CharString sSelf = m_pSelectedShip->key().string();
	pDoc->context()->scriptCall( CharString().format("onShipSelected( %s )", sSelf) );

	// set the last selected spawn
	m_pLastSpawn = pSpawnTarget;
	pDoc->setLastSpawn( m_pLastSpawn );

	return true;
}

bool ViewSelectShip::onCancel(const Message & msg)
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );
	
	// if we are a moderator, then exiting ship selection automatically leaves our current team.
	if ( pDoc->isMod() || pDoc->isDev() || pDoc->isAdmin() )
		pDoc->client()->selectTeam( -1 );

	document()->setScene( "Main" );
	return true;
}

bool ViewSelectShip::onChatEdit(const Message & msg)
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );

	CharString send( m_pChatEdit->text() );
	if ( send.length() > 0 )
	{
		pDoc->sendChat( send );
		m_pChatEdit->setText( "" );
	}
	return true;
}

bool ViewSelectShip::onFilterEdit(const Message & msg)
{
	m_sFilter = m_pFilterEdit->text();
	m_UpdateShipList = 0;
	return true;
}

class ShipDeleteDialog : public MessageBoxDialog
{
public: //Construction
	ShipDeleteDialog(ViewSelectShip * view, NounShip * pSelectedShip) : m_pSelectedShip(pSelectedShip),
		MessageBoxDialog(MBT_YESNO,
		CharString().format("Selected Ship has '%i' enhancements, are you sure you wish to delete it?", pSelectedShip->enhancementCount()))
	{
		m_view = view;
	}

	virtual void onOK()
	{
		m_view->onRemoveShipDialogYes(m_pSelectedShip);
	}
	virtual void onCancel()
	{}

	NounShip::Ref		m_pSelectedShip;
	ViewSelectShip *	m_view;
};

void ViewSelectShip::onRemoveShipDialogYes(NounShip * pSelectedShip) 
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT(pDoc);
	Array< WidgetKey > remove;
	remove.push(pSelectedShip->key());

	pDoc->client()->removeStorage(remove);
	m_UpdateShipList = 0.0f;

	onSelectShip(NULL, NULL);
}

bool ViewSelectShip::onButtonRemoveShip(const Message & msg)
{
	if ( m_pSelectedShip.valid()&& m_pActiveDialog == NULL && m_DialogList.size() == 0 )
		pushMessageBox(new ShipDeleteDialog(this,m_pSelectedShip));

	return true;
}

bool ViewSelectShip::onButtonRemoveCargo(const Message & msg)
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );
	
	if ( m_bRemoveCargo )
	{
		Array< WidgetKey > remove;
		for(int i=0;i<m_SelectedCargo.size();i++)
			remove.push( m_SelectedCargo[i]->key() );

		pDoc->client()->removeStorage( remove );
		m_UpdateShipList = 0.0f;

		m_SelectedCargo.release();
		m_bRemoveCargo = false;
	}
	else
	{
		pDoc->pushChat( CharString( "<color;ffffff>Command: Confirm Cargo Removal, execute command again ...</color>" ) );
		m_bRemoveCargo = true;
	}
	return true;
}

bool ViewSelectShip::onButtonScout(const Message & msg)
{
	m_FilterType = NounShip::SCOUT;
	m_UpdateShipList = 0;
	return true;
}

bool ViewSelectShip::onButtonFrigate(const Message & msg)
{
	m_FilterType = NounShip::FRIGATE;
	m_UpdateShipList = 0;
	return true;
}

bool ViewSelectShip::onButtonDestroyer(const Message & msg)
{
	m_FilterType = NounShip::DESTROYER;
	m_UpdateShipList = 0;
	return true;
}

bool ViewSelectShip::onButtonCruiser(const Message & msg)
{
	m_FilterType = NounShip::CRUISER;
	m_UpdateShipList = 0;
	return true;
}

bool ViewSelectShip::onButtonDread(const Message & msg)
{
	m_FilterType = NounShip::DREAD;
	m_UpdateShipList = 0;
	return true;
}

bool ViewSelectShip::onButtonTransport(const Message & msg)
{
	m_FilterType = NounShip::TRANSPORT;
	m_UpdateShipList = 0;
	return true;
}

bool ViewSelectShip::onButtonEngineer(const Message & msg)
{
	m_FilterType = NounShip::ENGINEER;
	m_UpdateShipList = 0;
	return true;
}

bool ViewSelectShip::onButtonSupply(const Message & msg)
{
	m_FilterType = NounShip::SUPPLY;
	m_UpdateShipList = 0;
	return true;
}

bool ViewSelectShip::onButtonStation(const Message & msg)
{
	m_FilterType = NounShip::STATION;
	m_UpdateShipList = 0;
	return true;
}

bool ViewSelectShip::onButtonUnknown(const Message & msg)
{
	m_FilterType = NounShip::UNKNOWN;
	m_UpdateShipList = 0;
	return true;
}

bool ViewSelectShip::onButtonTier1(const Message & msg)
{
	m_UpdateShipList = 0;
	return true;
}

bool ViewSelectShip::onButtonTier2(const Message & msg)
{
	m_UpdateShipList = 0;
	return true;
}

bool ViewSelectShip::onButtonTier3(const Message & msg)
{
	m_UpdateShipList = 0;
	return true;
}

//----------------------------------------------------------------------------

void ViewSelectShip::onSelectShip( NounShip * pShip, Noun * pSpawn )
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );
	GameClient * pClient = pDoc->client();
	ASSERT( pClient );
	GameContext * pContext = pDoc->context();
	ASSERT( pContext );
	
	m_bRemoveShip = false;
	// by default disable the ok button
	m_bShipSelected = false;
	m_pButtonOkay->setEnable( false );

	// are we deselecting a ship
	if ( pShip == NULL )
	{
		m_pShipDescription->setText( "" );

		if ( m_pShipRender != NULL )
			m_pShipRender->setSelectedShip( NULL );
		m_pSelectedShip = NULL;
		return;
	}

	if ( pDoc->validateSpawn( pSpawn ) )
	{
		// set the spawn as the target
		pDoc->setTarget( pSpawn );
		// center the navigation window
		m_pNavigationWindow->onCenter();
	}

	// find the shipcontext for the selected ship, if not found then it's from the players stash
	NounTemplate * pShipContext = pContext->findNounTemplate( pShip );
	if (! pShipContext )
		return;

	m_bShipSelected = true;
	m_pButtonOkay->setEnable( m_bShipSelected && !pClient->inQueue() );

	m_pSelectedShip = pShip;
	m_pSelectedSpawn = pSpawn;

	// set the ship to render
	if ( m_pShipRender != NULL )
		m_pShipRender->setSelectedShip( pShip );

	// display ship systems
	Array<CharString> primary, weapons, fields;
	Array<int> primarynum, weaponsnum;
	for(int i=0;i<pShip->childCount();i++)
	{
		BaseNode * pChild = pShip->child(i);
        if ( WidgetCast<NounGadget>( pChild ) )
		{
			NounGadget * pGadget = (NounGadget *)pShip->child(i);
			if ( pGadget->type() >= NounGadget::FIRST_WEAPON && pGadget->type() <= NounGadget::LAST_WEAPON )
			{
				CharString temp = CharString().format(pGadget->nounContext()->name());
				int found = weapons.search(temp);
				if(found < 0)
				{
						weapons.push(temp);
						weaponsnum.push(1);
				}
				else
				{
					weaponsnum[found]++;
				}
			}
			else
			{
				CharString temp = CharString().format(pGadget->nounContext()->name());
				int found = primary.search(temp);
				if(found < 0)
				{
						primary.push(temp);
						primarynum.push(1);
				}
				else
				{
					primarynum[found]++;
				}
			}
		}
		else if ( WidgetCast<FieldEnhancement>( pChild ) )
			fields.push( pChild->name() );
	}
	
	CharString shipSystems;
	shipSystems += "<color;ff0000ff>PRIMARY SYSTEMS:</color>\n";
	for(int i = 0; i < primary.size(); i++)
	{
		shipSystems += CharString().format("<X;20>%s x%i\n", primary[i], primarynum[i] );
	}
	shipSystems += "<color;ff0000ff>WEAPON SYSTEMS:</color>\n";

	for(int i = 0; i < weapons.size(); i++)
	{
		shipSystems += CharString().format("<X;20>%s x%i\n", weapons[i], weaponsnum[i] );
	}

	if ( fields.size() > 0 )
	{
		shipSystems += "<color;ff0000ff>ENHANCEMENT BONUS:<color>\n";
		for(int i=0;i<fields.size();++i)
			shipSystems += CharString().format("<X;20>%s\n", fields[i] );
	}


	// show the requirements only if this is a new ship..
	CharString sRequire;

	// display required rank
	m_nRank = pDoc->rank();
	if(m_nRank < 0)
		m_nRank = 0;
	bool hasRank = m_nRank >= pShipContext->rank();

	sRequire += CharString().format( "<color;%s>Rank: %s</color>\n", hasRank ? "ffffffff" : "ff0000ff", 
		GameProfile::rankText( pShipContext->rank() ) );

	GameClient::Storage * pStorage = pClient->storage( pShip->key() );
	if ( pStorage != NULL )
	{
		int nShipFactionId = pStorage->nFactionId;
		int nMyFactionId = pDoc->factionId();

		// check the faction of the ship and the player, let them know if they can't spawn it as their current faction or not..
		if ( nMyFactionId != nShipFactionId )
			sRequire += "<color;ff0000ff>FACTION INVALID!</color>\n";

		// show hull damage if any...
		if ( pShip->damage() > 0 )
		{
			int nDamage = (int)(pShip->damageRatioInv() * 100);
			if ( pShip->isDestroyed() )
				nDamage = 5;		// if destroyed, it will spawn with alteast 5% hull..

			// account for repairs that will happen while ship is in storage..
			if ( pStorage->nTime != 0 )
			{
				dword nStorageTime = Time::seconds() - pStorage->nTime;
				float fPercent = Clamp<float>( STORAGE_REPAIR_PER_SECOND * nStorageTime, 0.0f, 1.0f );
				nDamage += (fPercent * 100);
			}

			if ( nDamage < 100 )
				sRequire += CharString().format( "<color;ff0000ff>HULL: %d%%</color>\n", nDamage );
		}

		Noun * pLocation = pStorage->pLocation;
		if ( pDoc->validateSpawn( pLocation ) )
		{
			bool bCanSpawn = pDoc->target() == pLocation && pDoc->validateSpawn( pLocation );
			sRequire += CharString().format("Docked: <color;%s>%s</color>\n", 
				bCanSpawn ? "ffffffff" : "ff0000ff", pLocation->name());

			NounPlanet * pPlanet = WidgetCast<NounPlanet>( pDoc->target() );
			if ( pPlanet != NULL && pPlanet != pLocation )
			{
				int nTransferCost = TRANSFER_SPAWN_COST * NounShip::buildCost( pShip );
				if ( nTransferCost > 0 )
				{
					sRequire += CharString().format("Transfer Cost: <color;%s>%s Resources\n", 
						nTransferCost <= pPlanet->resources() ? "ffffff" : "ff0000ff",
						FormatNumber<char,int>( nTransferCost ) );
				}
			}
		}
	}
	else if ( pShipContext != NULL )
	{
		// display required badges
		dword required = pShipContext->flags();
		dword badges = pDoc->badges();

		bool hasBadges = (required & badges) == required;

		sRequire += CharString().format( "<color;%s>Badges: ", hasBadges ? "ffffffff" : "ff0000ff" );
		if ( pShipContext->flags() != 0 )
		{
			for(int i=FIRST_BADGE;i<=LAST_BADGE;i++)
				if ( ((1 << i) & required) != 0 )
					sRequire += CharString().format("%s ", GameProfile::badgeText( i ) );

			sRequire += "</color>\n";
		}
		else
			sRequire += "</color>None\n";

		// spawn point may not be a planet
		NounPlanet * pPlanet = WidgetCast<NounPlanet>( pDoc->target() );
		if ( pPlanet != NULL )
		{
			// display build costs
			sRequire += "<color;ffffffff>Requirements:</color>\n";

			int nBuildCost = NounShip::buildCost( pShip );
			sRequire += CharString().format("<X;20;color;%s>%s Resources\n", 
				nBuildCost <= pPlanet->resources() ? "ffffff" : "ff0000ff",
				FormatNumber<char,int>( nBuildCost ) );

			int nTechnology = NounShip::buildTechnology( pShip );
			if ( nTechnology > 0 )
				sRequire += CharString().format("<X;20;color;%s>%d Technology\n", 
					pPlanet->technology() < nTechnology ? "ff0000ff" : "ffffffff", nTechnology );
			
			dword nFlags = NounShip::buildFlags( pShip );
			if ( nFlags != 0 )
			{
				for(int i=0;i<32;i++)
				{
					dword nBit = 1 << i;
					if ( (nFlags & nBit) != 0 )
					{
						sRequire += CharString().format("<X;20;color;%s>%s\n", 
							(pPlanet->flags() & nBit) ? "ffffffff" : "ff0000ff", NounPlanet::flagName( nBit ) );
					}
				}
			}

		}
		else if (! pShip->enableGateSpawn() )
			sRequire += "\n<color;ff0000ff>SHIPYARD REQUIRED!</color>\n";
	}
	
	CharString info;
	info += CharString().format("<b;l>%s</b;/l>\n", pShip->nounContext()->name() );
	if ( TierInfo * nTier = pShip->nounContext()->findData<TierInfo>( "TIER" ) )
		info += CharString().format("Tier: %i\n\n", nTier->m_nTier );
	info += CharString().format("%s\n", sRequire );
	info += "<color;ffffffff>";
	info += CharString().format("%s\n\n", pShip->nounContext()->description() );
	info += CharString().format("%s</color>\n", shipSystems );

	// show any enhancements that this ship has
	pShip->initializeEnhancements();		
	if(pShip->enhancementCount() > 0)
	{
		CharString enhancementInfo = CharString().format("<large><color;ffffff>Enhancements (%d / %d) </color></large>\n---------------\n",pShip->enhancementCount(),pShip->maxEnhancements());
		for(int i=0;i<pShip->childCount();i++)
		{
			NounEnhancement *pEnh = WidgetCast<NounEnhancement>(pShip->child(i));
			if(pEnh)
			{
				enhancementInfo += pEnh->status() + "\n";
			}
		}
		info += enhancementInfo;
	}
	// set the ship description
	m_pShipDescription->setText( info );
}

void ViewSelectShip::onSelectCargo( NounCargo * pCargo )
{
	m_bRemoveCargo = false;
	if ( m_SelectedCargo.search( pCargo ) < 0 )
		m_SelectedCargo.push( pCargo );
	else
		m_SelectedCargo.removeSearch( pCargo );
}


WindowButton * ViewSelectShip::getFilterButton( NounShip::Type type )
{
	switch( type )
	{
	case NounShip::SCOUT:
		return m_pButtonScout;
	case NounShip::FRIGATE:
		return m_pButtonFrigate;
	case NounShip::DESTROYER:
		return m_pButtonDestroyer;
	case NounShip::CRUISER:
		return m_pButtonCruiser;
	case NounShip::DREAD:
		return m_pButtonDread;
	case NounShip::ENGINEER:
		return m_pButtonEngineer;
	case NounShip::SUPPLY:
		return m_pButtonSupply;
	case NounShip::TRANSPORT:
		return m_pButtonTransport;
	case NounShip::STATION:
		return m_pButtonStation;
	}

	return m_pButtonUnknown;
}

static int SortCargo( NounCargo::Ref p1, NounCargo::Ref p2 )
{
	return strcmp( p1->name(), p2->name() );
}

bool ViewSelectShip::updateShipList()
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );
	GameClient * pClient = pDoc->client();
	ASSERT( pClient );
	GameContext * pContext = pClient->gameContext();
	if (! pContext )
		return false;

	m_ShipContextHash.release();

	// get the spawn target
	Noun * pSpawn = pDoc->target();
	// is the spawn valid
	bool bSpawnValid = pDoc->validateSpawn( pSpawn );

	// get the player badges
	dword badges = pDoc->badges();
	// get the player rank
	int rank = pDoc->rank();
	if(rank < 0)
		rank = 0;
	// check the player flags
	bool isAdmin = pDoc->isAdmin();

	// hide all filter buttons, show the buttons as we enumerate the ships
	for(int i=0;i<NounShip::TYPE_COUNT;i++)
		getFilterButton( (NounShip::Type)i )->hideWindow();
	// always show the unknown filter button
	getFilterButton( NounShip::UNKNOWN )->showWindow();

	// count the number of ships added
	int shipCount = 0;

	// get the collection object containing the ships from the fleet
	Collection::Link pTemplates = pContext->team( pDoc->teamId() ).pTemplates;
	if ( pTemplates.valid() )
	{
		// enumerate all the avialble ship types
		for(int i=0;i<pTemplates->resourceCount();i++)
		{
			// get the ship context
			NounTemplate::Ref pShipContext = WidgetCast<NounTemplate>( pTemplates->resource( i ) );
			if ( pShipContext.valid() )
			{
				NounShip * pShip = WidgetCast<NounShip>( pShipContext->noun() );
				if (! pShip )
					continue;
				// show the filter button for this ship type
				WindowButton * pFilterButton = getFilterButton( pShip->type() );
				ASSERT( pFilterButton );
				pFilterButton->showWindow();

				// apply the current filter if any
				if ( m_FilterType != NounShip::UNKNOWN && pShip->type() != m_FilterType )
					continue;		// filtered!
				if ( pShip->type() == NounShip::UNKNOWN && !isAdmin )
					continue;		// allow admins to select UNKNOWN ships only

				CharString name = pShip->nounContext()->name();
				
				if ( m_sFilter != "" && name.find( m_sFilter, 0, true ) < 0 )
					continue;

				if ( TierInfo * nTier = pShip->nounContext()->findData<TierInfo>( "TIER" ) )
				{
						// only filter if one of the buttons is down
						if ( m_pButtonTier1->buttonDown() || m_pButtonTier2->buttonDown() || m_pButtonTier3->buttonDown() )
							switch( nTier->m_nTier )
							{
								case 1:
									if ( !m_pButtonTier1->buttonDown() )
										continue;
									break;
								case 2:
									if ( !m_pButtonTier2->buttonDown() )
										continue;						
									break;
								case 3:
									if ( !m_pButtonTier3->buttonDown() )
										continue;						
									break;
								default:
									break;
							}
				}
					
				bool bCanBuild = false;
				bool bUseShipFlags = pDoc->client()->serverStatus().bUseShipFlags;
				bool bMatchesShipFlags = pDoc->client()->serverStatus().sShipFlags.find( NounShip::typeText( pShip->type() ), 0, true ) > -1;
				
				if ( bUseShipFlags && bMatchesShipFlags )
					bCanBuild = true;
				else
				{
					if ( WidgetCast<NounPlanet>( pSpawn ) )
					{
						NounPlanet * pPlanet = (NounPlanet *)pSpawn;

						bCanBuild = NounShip::buildTechnology( pShip ) <= pPlanet->technology() && 
							NounShip::buildCost( pShip ) <= pPlanet->resources();
					}
					else if ( WidgetCast<NounJumpGate>( pSpawn ) )
					{
						bCanBuild = pShip->enableGateSpawn();
					}
				}

				bool hasBadges = (pShipContext->flags() & badges) == pShipContext->flags();
				bool hasRank = rank >= pShipContext->rank();
				bool bAvailable = ( ( ( ( hasBadges && hasRank ) || isAdmin ) && bSpawnValid ) || ( bUseShipFlags && bMatchesShipFlags ) );

				// check for existing button, if found then update that button ... otherwise create a new button
				ShipButton * pButton = shipCount < m_pShipButtons->childCount() ? (ShipButton *)m_pShipButtons->child(shipCount) : NULL;
				if ( pButton != NULL )
					pButton->setButton( this, pShip, NULL, bAvailable, bCanBuild );						// update existing button
				else
					new ShipButton( m_pShipButtons, this, pShip, NULL, bAvailable, bCanBuild );			// create new button

				// hash the context
				m_ShipContextHash[ pShip->key() ] = pShipContext;

				shipCount++;
			}
		}
	}

	// remove extra buttons
	for(int i=shipCount;i<m_pShipButtons->childCount();i++)
		((ShipButton *)m_pShipButtons->child(i))->destroy();
	
	int cargoCount = 0;
	int slotCount = 0;

	Array< NounCargo::Ref > cargo;
	for(int i=0;i<pClient->storageCount();i++)
	{
		const GameClient::Storage & storage = pClient->storage( i );
		
		WidgetKey nStorageKey( pClient->storageKey( i ) );
		Noun::wRef pInstance( nStorageKey );
		if (! pInstance.valid() )
			pInstance = pClient->storageInstance( i );
			
		if ( storage.nType == CARGO_TYPE_SHIP )
		{
			NounShip * pShip = WidgetCast<NounShip>( pInstance.pointer() );
			if (! pShip )
				continue;		// invalid ship
			pShip->setKey( nStorageKey );		// make sure it has the correct ID so it can be removed if invalid
			
			NounTemplate * pNounTemplate = pContext->findNounTemplate( pShip );
			if (! pNounTemplate )
				continue;	// TODO: fix invalid ships with no context, don't display them for now
						

			Noun * pLocation = storage.pLocation;
			dword nShipFactionId = storage.nFactionId;
			int nMyFactionId = pDoc->factionId();
			
			// we don't want to display ships on other factions
			if ( nMyFactionId != nShipFactionId )
				continue;

			// items are only available if the players is of the same faction!
			bool bFactionValid = nShipFactionId == pDoc->factionId();
			bool bAvailable = bFactionValid && bSpawnValid;

			ShipButton * pButton = slotCount < m_pSlotButtons->childCount() ? (ShipButton *)m_pSlotButtons->child(slotCount) : NULL;
			if ( pButton != NULL )
				pButton->setButton( this, pShip, pLocation, bAvailable, true );						// update existing button
			else
				new ShipButton( m_pSlotButtons, this, pShip, pLocation, bAvailable, true );			// create new button
			slotCount++;
		}
		else if ( storage.nType == CARGO_TYPE_ITEM )
		{
			NounCargo * pCargo = WidgetCast<NounCargo>( pInstance );
			if (! pCargo )
				continue;

			pCargo->setKey( nStorageKey );		// make sure it has the correct ID so it can be removed if invalid
			cargo.push( pCargo );
		}
	}
	for(int i=slotCount;i<m_pSlotButtons->childCount();i++)
		((ShipButton *)m_pSlotButtons->child(i))->destroy();

	// sort cargo by name.. 
	cargo.qsort( SortCargo );
	// display cargo buttons
	for(int i=0;i<cargo.size();++i)
	{
		NounCargo * pCargo = cargo[i];

		CargoButton * pButton = cargoCount < m_pCargoButtons->childCount() ? (CargoButton *)m_pCargoButtons->child(cargoCount) : NULL;
		if ( pButton != NULL )
			pButton->setButton( this, pCargo, true );
		else
			new CargoButton( m_pCargoButtons, this, pCargo, true );
		cargoCount++;
	}
	for(int i=cargoCount;i<m_pCargoButtons->childCount();i++)
		((CargoButton *)m_pCargoButtons->child(i))->destroy();

	// relayout the buttons
	((WindowLayout *)m_pShipButtons)->doLayout();
	// invalidate the message queue
	context()->setMessageQueueInvalid();

	CharString sCargo;
	sCargo.format( "SELECT CARGO: %d Items, %d Selected", cargoCount, m_SelectedCargo.size() );
	if ( m_pSelectedShip.valid() )
		sCargo += CharString().format(", %d Available", m_pSelectedShip->availableCargo() );
	m_pTextCargo->setText( sCargo );

	return shipCount == 1 && slotCount == 0 && cargoCount == 0;
}

//----------------------------------------------------------------------------

ViewSelectShip::ShipButton::ShipButton( NodeWindow * pParent, ViewSelectShip * pView, NounShip * pShip, Noun * pSpawn, bool bAvailable, bool bCanBuild  ) 
{
	m_Flags |= NOCLIP;
	m_Style = HAS_BACK | EFFECT_HIGHLIGHT | EFFECT_FADEIN | SMOOTH_LL;
	m_Alpha = 0.35f;

	setContext( pParent->context() );
	pParent->attachNode( this );

	setWindowStyle( new WindowStyle( *windowStyle() ) );
	setWindow( RectInt( PointInt(0,0), SizeInt( 120, 18 ) ) );
	setButton( pView, pShip, pSpawn, bAvailable, bCanBuild );
}

void ViewSelectShip::ShipButton::onRender( RenderContext & context, const RectInt & window )
{
	WindowButton::onRender( context, window );

	if ( m_pView->m_pSelectedShip == m_pShip && fmod( m_ActiveTime, 1.0f ) < 0.6f )	
		renderGlow( context );
}

void ViewSelectShip::ShipButton::onButtonUp()
{
	WindowButton::onButtonUp();
	m_pView->onSelectShip( m_pShip, m_pSpawn );
}

Color ViewSelectShip::ShipButton::labelColor() const
{
	return m_bAvailable ? (m_bCanBuild ? WHITE : GREY) : RED;
}

void ViewSelectShip::ShipButton::setButton( ViewSelectShip * pView, NounShip * pShip, Noun * pSpawn, bool bAvailable, bool bCanBuild )
{
	m_pView = pView;
	m_pShip = pShip;
	m_pSpawn = pSpawn;
	m_bAvailable = bAvailable;
	m_bCanBuild = bCanBuild;

	// get the full name of the ship type
	CharString name;

	TraitDisplayName * pDisplayName = pShip->findTrait<TraitDisplayName>();
	if ( pDisplayName != NULL )
		name = pDisplayName->displayName();
	else
		name = pShip->nounContext()->name();

	// set the tool-tip
	setTip( name );

	// set the label, keep it under 15 characters
	name.left( 15 );
	setLabel( name );
}

//----------------------------------------------------------------------------

ViewSelectShip::CargoButton::CargoButton( NodeWindow * pParent, ViewSelectShip * pView, NounCargo * pCargo, bool available ) 
{
	m_Flags |= NOCLIP;
	m_Style = HAS_BACK | EFFECT_HIGHLIGHT | EFFECT_FADEIN | SMOOTH_LL;
	m_Alpha = 0.75f;

	setContext( pParent->context() );
	pParent->attachNode( this );

	setWindowStyle( new WindowStyle( *windowStyle() ) );
	setWindow( RectInt( PointInt(0,0), SizeInt( 32, 24 ) ) );
	setButton( pView, pCargo, available );
}

void ViewSelectShip::CargoButton::onUpdate( float t )
{
	WindowButton::onUpdate( t );
}

void ViewSelectShip::CargoButton::onRender( RenderContext & context, const RectInt & window )
{
	WindowButton::onRender( context, window );

	if ( m_pView->m_SelectedCargo.search( m_pCargo.pointer() ) >= 0 && fmod( m_ActiveTime, 1.0f ) < 0.6f )	
		renderGlow( context );

	// draw the quantity
	Font * pFont = windowStyle()->font();
	if ( pFont != NULL )
	{
		if ( WidgetCast<CargoEnhancement>( m_pCargo ) )
		{
			// show the durability if an enhancement..
			CargoEnhancement * pCargoEnh = (CargoEnhancement *)m_pCargo.pointer();

			int nMaxDamage = pCargoEnh->enhancement()->maxDamage();
			if ( nMaxDamage > 0 )
			{
				WideString sQuantity;
				sQuantity.format( "%d/%d", nMaxDamage - pCargoEnh->damage(), nMaxDamage );

				SizeInt stringSize( pFont->size( sQuantity ) );
				PointInt stringPos( window.m_Right - stringSize.width, window.top );
				Font::push( context.display(), pFont, stringPos, sQuantity, WHITE );
			}
		}
		else if ( m_pCargo )
		{
			// draw the quantity if cargo
			WideString sQuantity;
			sQuantity.format( "%d", m_pCargo->quantity() );

			SizeInt stringSize( pFont->size( sQuantity ) );
			PointInt stringPos( window.m_Right - stringSize.width, window.top );
			Font::push( context.display(), pFont, stringPos, sQuantity, WHITE );
		}
	}
}

void ViewSelectShip::CargoButton::onButtonUp()
{
	WindowButton::onButtonUp();
	m_pView->onSelectCargo( m_pCargo );
}

void ViewSelectShip::CargoButton::setButton( ViewSelectShip * pView, NounCargo * pCargo, bool available )
{
	m_pView = pView;
	m_pCargo = pCargo;

	if (WidgetCast<CargoEnhancement>( pCargo ) )
	{
		CargoEnhancement * pCargoEnhancement = (CargoEnhancement *)pCargo;
		setTip( pCargoEnhancement->status() );
		setIcon( pCargoEnhancement->enhancement()->icon() );
		setIconColor(  pCargoEnhancement->enhancement()->color() );
	}
	else
	{
		setTip( pCargo->name() );
		setIcon( pCargo->icon() );
		setIconColor( available ? WHITE : GREY );
	}
}

bool ViewSelectShip::onItemList(const Message & msg)
{
	GameDocument * pDoc = WidgetCast<GameDocument>( document() );
	if (! pDoc )
		return false;
	GameClient * pClient = pDoc->client();
	if (! pClient )
		return false;

	unsigned int nSelectedItem = msg.lparam;

	NounEnhancement::EnhancmentList & buyList = NounEnhancement::sm_BuyList;
	if ( nSelectedItem < buyList.size() )
	{
		NounEnhancement * pEnhancement = buyList[ nSelectedItem ];

		CharString sDesc( pEnhancement->status() );
		sDesc += CharString().format("\n\n<color;%s>%s Credits", 
			pClient->credits() >= pEnhancement->buyValue() ? "ffffffff" : "ff0000ff",
			FormatNumber<char,int>( pEnhancement->buyValue() ) );

		m_pItemDescription->setText( sDesc );
	}

	return true;
}

bool ViewSelectShip::onButtonBuyCargo(const Message & msg)
{
	GameDocument * pDoc = WidgetCast<GameDocument>( document() );
	if (! pDoc )
		return false;
	GameClient * pClient = pDoc->client();
	if (! pClient )
		return false;

	if (! m_pBuyWindow->visible() )
	{
		// initialize the list control with all items..
		m_pItemList->removeAll();

		int nWidth = m_pItemList->window().width() / 3;
		m_pItemList->addColumn( nWidth * 2, WHITE );		// column for credits
		m_pItemList->addColumn( nWidth, LAVENDER );			// item name..

		NounEnhancement::EnhancmentList & buyList = NounEnhancement::sm_BuyList;
		for(unsigned int i=0;i<buyList.size();++i)
		{
			NounEnhancement * pEnhancement = buyList[i];
			if (! pEnhancement )
				continue;

			int nItem = m_pItemList->addItem( pEnhancement->getName() );
			m_pItemList->setItem( nItem, 1, FormatNumber<char,int>( pEnhancement->buyValue() ) );
		}

		m_pBuyWindow->showWindow();
	}

	return true;
}

bool ViewSelectShip::onButtonBuyItem(const Message & msg)
{
	GameDocument * pDoc = WidgetCast<GameDocument>( document() );
	if (! pDoc )
		return false;
	GameClient * pClient = pDoc->client();
	if (! pClient )
		return false;

	unsigned int nSelected = m_pItemList->sortedItem( m_pItemList->selected() );
	NounEnhancement::EnhancmentList & buyList = NounEnhancement::sm_BuyList;

	if ( nSelected < buyList.size() )
	{
		NounEnhancement * pEnhancement = buyList[ nSelected ];
		if ( pEnhancement )
			pClient->buyEnhancement( pEnhancement->factory()->classKey() );
	}

	return true;
}

bool ViewSelectShip::onButtonCancelBuy(const Message & msg)
{
	m_pBuyWindow->hideWindow();
	return true;
}

bool ViewSelectShip::onMessageBoxOK(const Message & msg)
{
	m_pMessageBox->hideWindow();
	
	if ( m_pActiveDialog.valid() )
	{
		m_pActiveDialog->onOK();
		m_pActiveDialog = NULL;
	}
	else
	{
		WidgetCast<GameDocument>( document() )->context()->scriptCall( m_pMessageBoxOK->script() );
	}

	return true;
}

bool ViewSelectShip::onMessageBoxCANCEL(const Message & msg)
{
	m_pMessageBox->hideWindow();

	if ( m_pActiveDialog.valid() )
	{
		m_pActiveDialog->onCancel();
		m_pActiveDialog = NULL;
	}
	else
	{
		WidgetCast<GameDocument>( document() )->context()->scriptCall( m_pMessageBoxCANCEL->script() );
	}

	return true;
}

//----------------------------------------------------------------------------
// EOF
