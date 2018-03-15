/*
	ViewGame.cpp
	(c)2000 Palestar, Richard Lyle
*/

#include "Debug/Assert.h"
#include "File/FileDisk.h"
#include "Standard/StringBuffer.h"
#include "System/Keyboard.h"
#include "DarkSpace/NounUnit.h"
#include "World/NounTarget.h"
#include "DarkSpace/NounDropTarget.h"
#include "DarkSpace/NounBeacon.h"
#include "DarkSpace/NounCargo.h"
#include "DarkSpace/NounJumpGate.h"
#include "DarkSpace/CargoGadget.h"
#include "DarkSpace/GadgetReload.h"
#include "DarkSpace/GadgetBuild.h"
#include "DarkSpace/NounStar.h"
#include "DarkSpace/VerbScrap.h"
#include "DarkSpace/VerbDisband.h"
#include "DarkSpace/VerbSetStructureActive.h"
#include "DarkSpace/VerbEnterOrbit.h"
#include "DarkSpace/VerbUseStructure.h"
#include "DarkSpace/VerbDistress.h"
#include "DarkSpace/VerbUnload.h"
#include "DarkSpace/VerbLoad.h"
#include "DarkSpace/VerbLockPlanet.h"
#include "DarkSpace/VerbApplyEnhancement.h"
#include "DarkSpace/VerbImportExport.h"
#include "DarkSpace/VerbTransferCredits.h"
#include "DarkSpace/VerbOrderUnit.h"
#include "DarkSpace/VerbOrderShip.h"
#include "DarkSpace/VerbUseSingleGadget.h"
#include "DarkSpace/VerbRepairGadget.h"
#include "DarkSpace/VerbUseMode.h"
#include "DarkSpace/VerbRemoveEnhancement.h"
#include "DarkSpace/StructureDefense.h"
#include "DarkSpace/StructureDepot.h"
#include "DarkSpace/StructurePort.h"
#include "DarkSpace/StructureShipyard.h"
#include "DarkSpace/GadgetShield.h"
#include "DarkSpace/GadgetArmor.h"
#include "DarkSpace/GadgetBeamWeapon.h"
#include "DarkSpace/GadgetSlot.h"
#include "DarkSpace/Resource.h"
#include "Interface/WindowObserve.h"
#include "Interface/TipHelp.h"
#include "DarkSpace/CargoEnhancement.h"
#include "Interface/ViewGame.h"


#include "DarkSpace/StructurePort.h"

//----------------------------------------------------------------------------

const float CHECK_FOR_COLLISION_TIME = 1.0f;
const float COLLISION_WARNING_TIME = 5.0f;

static Constant MAX_TARGET_RANGE( "MAX_TARGET_RANGE", 500000.0f );

ViewGame *		ViewGame::sm_pViewGame=NULL;
bool			ViewGame::sm_bDisableTacticalActions = false;

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( ViewGame, WindowView::View );
REGISTER_FACTORY_KEY( ViewGame, 4241436226245074821 );

ViewGame::ViewGame() : 
	m_eContextAction( CA_NONE ),
	m_Mode( TACTICAL ),
	m_ChatMode( FACTION ),
	m_pDoc( NULL ),
	m_CameraFromNavigation( false ),
	m_bRedAlert( false ),
	m_nRankTimeout( 0 )
{}

//----------------------------------------------------------------------------

void ViewGame::onActivate()
{
	// set the view for tips
	ViewGame::sm_pViewGame = this;
	//{{BEGIN_DATA_INIT
	DATA_MAP( m_pButtonGroupPromote, WindowButton, "ButtonGroupPromote" );
	DATA_MAP( m_pButtonGroupRemove, WindowButton, "ButtonGroupRemove" );
	DATA_MAP( m_pButtonGroupInvite, WindowButton, "ButtonGroupInvite" );
	DATA_MAP( m_pHintText, WindowText, "HintText" );
	DATA_MAP( m_pEnhancementWindow, WindowEnhancement, "EnhancementWindow" );
	m_pMessageBoxCANCEL = WidgetCast<WindowButton>( window()->findNode( "MessageBoxCANCEL" ) );
	m_pMessageBoxOK = WidgetCast<WindowButton>( window()->findNode( "MessageBoxOK" ) );
	m_pMessageBox = WidgetCast<WindowText>( window()->findNode( "MessageBox" ) );
	m_pButtonCancelOrder = WidgetCast<WindowButton>( window()->findNode( "ButtonCancelOrder" ) );
	m_pPopupTargetButtons = WidgetCast<ButtonPopup>( window()->findNode( "PopupTargetButtons" ) );
	m_pButtonRightConsole = WidgetCast<WindowButton>( window()->findNode( "ButtonRightConsole" ) );
	m_pTargetFrame = WidgetCast<NodeWindow>( window()->findNode( "TargetFrame" ) );
	m_pLayoutTargetButtons = WidgetCast<NodeWindow>( window()->findNode( "LayoutTargetButtons" ) );
	m_pButtonShadow = WidgetCast<WindowButton>( window()->findNode( "ButtonShadow" ) );
	m_pButtonScoutMission = WidgetCast<WindowButton>( window()->findNode( "ButtonScoutMission" ) );
	m_pButtonDefendMission = WidgetCast<WindowButton>( window()->findNode( "ButtonDefendMission" ) );
	m_pButtonCaptureMission = WidgetCast<WindowButton>( window()->findNode( "ButtonCaptureMission" ) );
	m_pButtonAttackMission = WidgetCast<WindowButton>( window()->findNode( "ButtonAttackMission" ) );
	m_pButtonTarget = WidgetCast<WindowButton>( window()->findNode( "ButtonTarget" ) );
	m_pButtonYell = WidgetCast<WindowButton>( window()->findNode( "ButtonYell" ) );
	m_pButtonFaction = WidgetCast<WindowButton>( window()->findNode( "ButtonFaction" ) );
	m_pButtonFleet = WidgetCast<WindowButton>( window()->findNode( "ButtonFleet" ) );
	m_pButtonNextTarget = WidgetCast<WindowButton>( window()->findNode( "ButtonNextTarget" ) );
	m_pButtonUpTarget = WidgetCast<WindowButton>( window()->findNode( "ButtonUpTarget" ) );
	m_pButtonOrderRaze = WidgetCast<WindowButton>( window()->findNode( "ButtonOrderRaze" ) );
	m_pButtonOrderDefend = WidgetCast<WindowButton>( window()->findNode( "ButtonOrderDefend" ) );
	m_pButtonOrderAttack = WidgetCast<WindowButton>( window()->findNode( "ButtonOrderAttack" ) );
	m_pButtonNext = WidgetCast<WindowButton>( window()->findNode( "ButtonNext" ) );
	m_pButtonPrev = WidgetCast<WindowButton>( window()->findNode( "ButtonPrev" ) );
	m_pButtonRepairNow = WidgetCast<WindowButton>( window()->findNode( "ButtonRepairNow" ) );
	m_pButtonRepairLast = WidgetCast<WindowButton>( window()->findNode( "ButtonRepairLast" ) );
	m_pButtonAutoDefense = WidgetCast<WindowButton>( window()->findNode( "ButtonAutoDefense" ) );
	m_pButtonUseStructure = WidgetCast<WindowButton>( window()->findNode( "ButtonUseStructure" ) );
	m_pButtonApplyEnhancement = WidgetCast<WindowButton>( window()->findNode( "ButtonApplyEnhancement" ) );
	m_pButtonSendChat = WidgetCast<WindowButton>( window()->findNode( "ButtonSendChat" ) );
	m_pEditTransferCredits = WidgetCast<WindowEdit>( window()->findNode( "EditTransferCredits" ) );
	m_pWindowTransferCredits = WidgetCast<NodeWindow>( window()->findNode( "WindowTransferCredits" ) );
	m_pButtonTransferCredits = WidgetCast<WindowButton>( window()->findNode( "ButtonTransferCredits" ) );
	m_pButtonPlanet = WidgetCast<WindowButton>( window()->findNode( "ButtonPlanet" ) );
	m_pButtonUnlock = WidgetCast<WindowButton>( window()->findNode( "ButtonUnlock" ) );
	m_pButtonLock = WidgetCast<WindowButton>( window()->findNode( "ButtonLock" ) );
	m_pViewPlanet = WidgetCast<NodeWindow>( window()->findNode( "ViewPlanet" ) );
	m_pTargetButtons2 = WidgetCast<NodeWindow>( window()->findNode( "TargetButtons2" ) );
	m_pButtonJettison = WidgetCast<WindowButton>( window()->findNode( "ButtonJettison" ) );
	m_pButtonCamera = WidgetCast<WindowButton>( window()->findNode( "ButtonCamera" ) );
	m_pButtonContinue = WidgetCast<WindowButton>( window()->findNode( "ButtonContinue" ) );
	m_pButtonPause = WidgetCast<WindowButton>( window()->findNode( "ButtonPause" ) );
	m_pSignatureText = WidgetCast<WindowText>( window()->findNode( "SignatureText" ) );
	m_pSignatureBar = WidgetCast<BarHUD>( window()->findNode( "SignatureBar" ) );
	m_pRightConsole = WidgetCast<NodeWindow>( window()->findNode( "RightConsole" ) );
	m_pLeftConsole = WidgetCast<NodeWindow>( window()->findNode( "LeftConsole" ) );
	m_pChatEdit = WidgetCast<WindowEdit>( window()->findNode( "ChatEdit" ) );
	m_pHelpWindow = WidgetCast<NodeWindow>( window()->findNode( "HelpWindow" ) );
	m_pButtonDisband = WidgetCast<WindowButton>( window()->findNode( "ButtonDisband" ) );
	m_pButtonScrap = WidgetCast<WindowButton>( window()->findNode( "ButtonScrap" ) );
	m_pButtonLoad = WidgetCast<WindowButton>( window()->findNode( "ButtonLoad" ) );
	m_pButtonOrbit = WidgetCast<WindowButton>( window()->findNode( "ButtonOrbit" ) );
	m_pButtonFollow = WidgetCast<WindowButton>( window()->findNode( "ButtonFollow" ) );
	m_pTargetDescription = WidgetCast<WindowText>( window()->findNode( "TargetDescription" ) );
	m_pButtonNavigation = WidgetCast<WindowButton>( window()->findNode( "ButtonNavigation" ) );
	m_pButtonHelp = WidgetCast<WindowButton>( window()->findNode( "ButtonHelp" ) );
	m_pIncomingMessages = WidgetCast<NodeWindow>( window()->findNode( "IncomingMessages" ) );
	m_pShipStatusText = WidgetCast<WindowText>( window()->findNode( "ShipStatusText" ) );
	m_pEnergyText = WidgetCast<WindowText>( window()->findNode( "EnergyText" ) );
	m_pEnergyBar = WidgetCast<BarHUD>( window()->findNode( "EnergyBar" ) );
	m_pHullText = WidgetCast<WindowText>( window()->findNode( "HullText" ) );
	m_pHullBar = WidgetCast<BarHUD>( window()->findNode( "HullBar" ) );
	m_pViewNavigation = WidgetCast<NodeWindow>( window()->findNode( "ViewNavigation" ) );
	m_pViewTactical = WidgetCast<NodeWindow>( window()->findNode( "ViewTactical" ) );
	m_pButtonPauseImport = WidgetCast<WindowButton>( window()->findNode( "ButtonPauseImport" ) );	
	m_pButtonPauseExport = WidgetCast<WindowButton>( window()->findNode( "ButtonPauseExport" ) );
	m_pButtonContinueImport = WidgetCast<WindowButton>( window()->findNode( "ButtonContinueImport" ) );	
	m_pButtonContinueExport = WidgetCast<WindowButton>( window()->findNode( "ButtonContinueExport" ) );	
	//END_DATA_INIT}}

#ifdef _DEBUG
	m_pSettings = new Settings( "ClientD" );
#else
	m_pSettings = new Settings( "Client" );
#endif

	m_pDoc = WidgetCast<GameDocument>( document() );
	ASSERT( m_pDoc );

	// 
	// show help 
	m_pHelpWindow->showWindow( m_pSettings->get( "showHelp", 1 ) != 0 );
	m_ChatMode = (ChatMode)m_pSettings->get( "chatMode", m_ChatMode );

	// hide all sub windows
	m_pIncomingMessages->showWindow();
	m_pTargetFrame->hideWindow();
	m_pWindowTransferCredits->hideWindow();

	// hide all target buttons
	m_pButtonFollow->hideWindow();
	m_pButtonLoad->hideWindow();
	m_pButtonOrbit->hideWindow();
	m_pButtonDisband->hideWindow();
	m_pButtonScrap->hideWindow();
	m_pButtonContinue->hideWindow();
	m_pButtonPause->hideWindow();
	m_pButtonCamera->showWindow();
	m_pButtonJettison->hideWindow();
	m_pButtonLock->hideWindow();
	m_pButtonUnlock->hideWindow();
	m_pButtonTransferCredits->hideWindow();
	m_pButtonGroupInvite->hideWindow();
	m_pButtonGroupRemove->hideWindow();
	m_pButtonGroupPromote->hideWindow();
	m_pButtonPauseImport->hideWindow();
	m_pButtonPauseExport->hideWindow();
	m_pButtonContinueImport->hideWindow();
	m_pButtonContinueExport->hideWindow();	

	// hide hint window
	m_pHintText->hideWindow();
	// hide message box
	m_pMessageBox->hideWindow();

	// start in tactical mode
	m_Mode = TACTICAL;
	m_pViewNavigation->hideWindow();
	m_pViewTactical->showWindow();
	m_pViewTactical->onActivate();
	m_pViewPlanet->hideWindow();

	// setup the hull bar
	m_pHullBar->setHeadColor( Color( 255, 0, 0), Color( 216, 228, 52) );
	m_pHullBar->setTailColor( Color( 255, 0, 0), Color( 47, 205, 24 ) );
	// setup the energy bar
	m_pEnergyBar->setHeadColor( Color( 255, 0, 0), Color( 123, 60, 230) );
	m_pEnergyBar->setTailColor( Color( 255, 0, 0), Color( 57, 215, 232 ) );

	m_pSignatureBar->setHeadColor( DARK_BLUE, SKYBLUE );
	m_pSignatureBar->setTailColor( DARK_BLUE, BLUE );

	m_CollisionCheck = 0.0f;
	m_CameraFromNavigation = false;

	// reset the camera time
	GameDocument::s_fCamera = 0.0f;
	// inform script we are entering the game
	m_pDoc->context()->scriptCall( "onTactical()" );

	// init the enhancement icons
	m_nRotatingEnhancementTick =0;
	m_CurrentEnhancement = 0;
}

void ViewGame::onDeactivate()
{
	m_pSettings->put( "showHelp", m_pHelpWindow->visible() ? 1 : 0 );
	m_pSettings->put( "chatMode", m_ChatMode );

	// called before this view is destroyed
	m_ConfirmScrap = NULL;
	m_ConfirmDisband = NULL;
	m_ConfirmJettison = NULL;
	m_ConfirmUseStructure = NULL;
	m_ConfirmRemoveEnhancement = NULL;
	m_LastTarget = NULL;

	sm_pViewGame = NULL;
}

// rotating enhancements change at this rate
const int ROTATING_ENHANCEMENT_RATE	= 2.0f;	


void ViewGame::onUpdate( float t )
{
	ASSERT( m_pDoc );

	GameDocument::s_fCamera += t;

	GameContext * pContext = m_pDoc->context();
	ASSERT( pContext );
	NounShip * pShip = m_pDoc->ship();
	if (! pShip )
		return;

	GameClient * pClient = m_pDoc->client();
	ASSERT( pClient );

	// update buttons
	m_pButtonHelp->setTargetAlpha( m_pHelpWindow->visible() ? 1.0f : 0.35f );
	m_pButtonNavigation->setTargetAlpha( m_pViewNavigation->visible() ? 1.0f : 0.35f  );
	m_pButtonPlanet->setTargetAlpha( m_pViewPlanet->visible() ? 1.0f : 0.35f  );

	m_pButtonTarget->setButtonDown( m_ChatMode == TARGET  );
	m_pButtonYell->setButtonDown( m_ChatMode == YELL );
	m_pButtonFaction->setButtonDown( m_ChatMode == FACTION  );
	m_pButtonFleet->setButtonDown( m_ChatMode == FLEET );

	if ( m_pButtonRightConsole->buttonDown() )
	{
		// show hide target window
		Noun * pTarget = myTarget();
		if ( pTarget != NULL )
		{
			bool bIsShiftDown = Keyboard::shiftDown();

			// show the target window
			m_pTargetFrame->showWindow();
			// has the target changed, if so hide all buttons
			if ( pTarget != m_LastTarget )
			{
				// hide all buttons
				hideAllButtons( m_pLayoutTargetButtons );
				// set the last target
				m_LastTarget = pTarget;
			}
			
			bool bIsFriend = pShip->isFriend( pTarget );

			// engineering screen is only allowed on friendly planets
			m_pButtonPlanet->setEnable(pShip->isFriend(pTarget) && pTarget->isRootNoun());
			// enable disable ship command buttons
			m_pButtonShadow->updateButton( pShip->canCommand( NounShip::SHADOW, pTarget ), 
				!pShip->isCommand( NounShip::SHADOW, pTarget ) );
			m_pButtonFollow->updateButton( pShip->canCommand( NounShip::FOLLOW, pTarget ), 
				!pShip->isCommand( NounShip::FOLLOW, pTarget ) );

			bool bCanOrbit = pShip->canCommand( NounShip::ORBIT, pTarget );
			m_pButtonOrbit->updateButton( bCanOrbit, 
				!pShip->isCommand( NounShip::ORBIT, pTarget ) );

			// show order buttons
			m_pButtonAttackMission->showWindow( pShip->canSendOrder( NounShip::ATTACK, pTarget ) );
			m_pButtonCaptureMission->showWindow( pShip->canSendOrder( NounShip::CAPTURE, pTarget ) );
			m_pButtonDefendMission->showWindow( pShip->canSendOrder( NounShip::DEFEND, pTarget ) );
			m_pButtonScoutMission->showWindow( pShip->canSendOrder( NounShip::RECON, pTarget ) );

			// camera is only allowed for friendly targets
			m_pButtonCamera->updateButton( pShip->isFriend( pTarget ) && pTarget->isRootNoun() );
			// show load button, enable window if item can actually be loaded
			m_pButtonLoad->updateButton( VerbLoad::canLoad( pTarget, pShip ) );
			// show jettison button for any cargo that can be jettisoned
			m_pButtonJettison->updateButton( VerbUnload::canUnload( pShip, pTarget, pShip ) );

			// get the zone parent of our target
			Noun * pZoneParent = pTarget->zoneParent();
			// show next/prev buttons for current target
			if ( !m_pPopupTargetButtons->up() )
			{
				m_pButtonUpTarget->showWindow( pTarget != pZoneParent );
				m_pButtonNextTarget->showWindow( (pTarget == pZoneParent && pTarget->childCount() > 0) || 
					(pTarget != pZoneParent && pTarget->zone() != NULL) );
			}
			
			// update the target description
			CharString sTargetinfo;
			float targetDistance = (pShip->worldPosition() - pTarget->worldPosition()).magnitude();
			sTargetinfo += CharString().format( "%s<X;130>%.1f gu\n", pTarget->displayName( true ).cstr(), targetDistance );

			if ( WidgetCast<NounPlanet>( pTarget ) )
			{
				NounPlanet * pPlanet = (NounPlanet *)pTarget;
				sTargetinfo += pPlanet->status();

				// show lock buttons
				m_pButtonLock->updateButton( VerbLockPlanet::canLock( pPlanet, pShip ) );
				m_pButtonUnlock->updateButton( VerbLockPlanet::canUnlock( pPlanet, pShip) );
			}
			else if ( WidgetCast<NounShip>( pTarget ) )
			{
				NounShip * pTargetShip = (NounShip *)pTarget;
				sTargetinfo += pTargetShip->status();

				if ( bIsFriend && pTargetShip->order() != NounShip::NOORDER )
					sTargetinfo += CharString().format("\n%s", pTargetShip->orderString() );

				if(pTargetShip->enhancementCount() > 0)
				{
					// update rotating enancement timer
					m_nRotatingEnhancementTick += t;
					if ( m_nRotatingEnhancementTick > ROTATING_ENHANCEMENT_RATE )
					{
						m_nRotatingEnhancementTick = 0;
						m_CurrentEnhancement++;
					}

					if(m_CurrentEnhancement >= pTargetShip->enhancementCount())
						m_CurrentEnhancement = 0;

					NounEnhancement * pEnhancement = pTargetShip->enhancement( m_CurrentEnhancement );
					sTargetinfo += CharString().format("\nEnhancements: (%d of %d)\n<color;%6.6x>%s</color>",
							(m_CurrentEnhancement + 1),pTargetShip->enhancementCount(),
							pEnhancement->color().RGB(), pEnhancement->getName());
				}
				
				m_pButtonCancelOrder->updateButton( pTargetShip->canOrder( NounShip::NOORDER, NULL, pShip ) );
				// show the transfer credits button and window, hide if the confirmation window is being displayed
				m_pButtonTransferCredits->updateButton( VerbTransferCredits::canTransferCredits( pShip, pTargetShip, 1) );
				m_pWindowTransferCredits->showWindow( m_pEditTransferCredits->editing() );
			}
			else if ( WidgetCast<NounUnit>( pTarget ) )
			{
				NounUnit * pUnit = (NounUnit *)pTarget;
				sTargetinfo += pUnit->status();
			
				m_pButtonDisband->updateButton( VerbDisband::canDisband( pUnit, pShip ) );
				bool bCanOrder = VerbOrderUnit::canOrder( pUnit, pShip );
				m_pButtonOrderAttack->updateButton( bCanOrder );
				m_pButtonOrderDefend->updateButton( bCanOrder );
				m_pButtonOrderRaze->updateButton( bCanOrder );
			}
			else if ( WidgetCast<NounJumpGate>( pTarget ) )
			{
				NounJumpGate * pGate = (NounJumpGate *)pTarget;
				sTargetinfo += pGate->status();
			}
			else if ( WidgetCast<NounGadget>( pTarget ) )
			{
				NounGadget * pGadget = (NounGadget *)pTarget;
				sTargetinfo += pGadget->status();
				sTargetinfo += CharString().format("\n%s", pGadget->description() );

				// is this gadget attached to this ship
				if ( pGadget->parentNoun() == pShip )
				{
					GadgetBeamWeapon * pBeamWeapon = WidgetCast<GadgetBeamWeapon>( pGadget );
					bool canPointDefense = pBeamWeapon != NULL ? pBeamWeapon->allowPointDefense() : false;

					bool isGadgetDamaged = pGadget->damage() > 0;
					bool isGadgetSlot = WidgetCast<GadgetSlot>( pGadget ) != NULL;
					bool isUseMode = pGadget->useModeCount() > 1;

					m_pButtonAutoDefense->updateButton( canPointDefense );
					m_pButtonRepairLast->updateButton( isGadgetDamaged );
					m_pButtonRepairNow->updateButton( isGadgetDamaged );
					m_pButtonNext->updateButton( isUseMode );
					m_pButtonPrev->updateButton( isUseMode );
				}

			}
			else if ( WidgetCast<NounStructure>( pTarget ) )
			{
				NounStructure * pStructure = (NounStructure *)pTarget;
				sTargetinfo += pStructure->status(); 

				bool bCanPause = VerbSetStructureActive::canPauseStructure( pStructure, pShip );
				bool bCanContinue = VerbSetStructureActive::canContinueStructure( pStructure, pShip );
				bool bCanUsestructure = VerbUseStructure::canUse( pStructure, pShip );
				bool bCanScrap = VerbScrap::canScrap( pStructure, pShip );

				m_pButtonScrap->showWindow(bCanScrap );
				m_pButtonContinue->showWindow( bCanContinue );
				m_pButtonPause->showWindow(bCanPause );
				m_pButtonUseStructure->showWindow( bCanUsestructure );
				
				if ( WidgetCast<StructurePort>( pStructure ) && bIsFriend )
				{
					bool bCanImport = VerbImportExport::canImport( pStructure->planet() );
					bool bCanExport = VerbImportExport::canExport( pStructure->planet() );
					bool bCantImport = !bCanImport;
					bool bCantExport = !bCanExport;

					m_pButtonPauseImport->showWindow( bCanImport );
					m_pButtonContinueImport->showWindow( bCantImport );

					m_pButtonPauseExport->showWindow( bCanExport );
					m_pButtonContinueExport->showWindow( bCantExport );					
				}
			}
			else if ( WidgetCast<CargoEnhancement>( pTarget ) )
			{
				sTargetinfo += CharString().format("\n%s", ((CargoEnhancement *)pTarget)->status() );
				bool bCanApplyEnhancement = VerbApplyEnhancement::canApply((CargoEnhancement *)pTarget,pShip);
				if(((CargoEnhancement *)pTarget)->enhancement()->applyType() == NounEnhancement::AT_ATTACH)
					m_pButtonApplyEnhancement->setTip("Attach Enhancement");
				else
					m_pButtonApplyEnhancement->setTip("Use Item");
				m_pButtonApplyEnhancement->updateButton(bCanApplyEnhancement);
			}
			else if ( WidgetCast<CargoGadget>( pTarget ) )
			{
				NounGadget * pGadget = ((CargoGadget *)pTarget)->gadget();
				sTargetinfo += CharString().format("\n%s", pGadget->description() );
			}

			m_pTargetDescription->setText( sTargetinfo );

			// update grouping buttons
			m_pButtonGroupInvite->showWindow( pClient->canGroupInvite( pTarget ) );
			m_pButtonGroupRemove->showWindow( pClient->canGroupRemove( pTarget ) );
			m_pButtonGroupPromote->showWindow( pClient->canGroupPromote( pTarget ) );
		}
		else
			m_pTargetFrame->hideWindow();
	}


	// update the energy bar
	m_pEnergyBar->setPosition( pShip->energyRatio() );
	m_pEnergyText->setText( CharString().format( "Energy: %.1f", float( pShip->energy() ) / 1000.0f ) );
	// update the hull bar
	m_pHullBar->setPosition( pShip->damageRatioInv() );
	m_pHullText->setText( CharString().format("Hull: %d%%", int( pShip->damageRatioInv() * 100 ) ) );
	// update the signature bar
	m_pSignatureBar->setPosition( Clamp( pShip->signature(), 0.0f, 50.0f ) / 50.0f  );
	m_pSignatureText->setText( CharString().format( "Signature: %.1f", pShip->signature() ) );

	// check for enemy ship near
	m_bRedAlert = false;
	for(int i=0;i<pShip->contactCount() && !m_bRedAlert;++i)
		if ( pShip->isEnemy( pShip->contact(i) ) )
			m_bRedAlert = true;

	// update the hint text
	const char * pLeftHint = m_pDoc->leftHint();
	const char * pRightHint = m_pDoc->rightHint();
	if ( pLeftHint || pRightHint )
	{
		CharString sHintText;
		if ( pLeftHint != NULL )
			sHintText += CharString().format("<image;LEFT> %s", pLeftHint );
		if ( pRightHint != NULL )
			sHintText += CharString().format("<X;0;Y;16;image;RIGHT> %s", pRightHint );
		
		m_pHintText->setText( sHintText );
		m_pHintText->showWindow();
	}
	else
		m_pHintText->hideWindow();

	// update the ship status line
	dword tick = pContext->tick();

	CharString sShipStatus;

	if ( m_Mode == TACTICAL )
	{
		sShipStatus += CharString().format("System: %s", pShip->starSystem() );
		if ( pContext->type() == GameContext::METAVERSE )
			sShipStatus += CharString().format( "\nSector: %s", pShip->serverName() );
		sShipStatus += CharString().format("\nTime: %2.2u:%2.2u:%2.2u", 
			tick / TICKS_PER_HOUR, 
			(tick / TICKS_PER_MINUTE) % 60, 
			(tick / TICKS_PER_SECOND) % 60 );

		if ( pContext->timeLimit() > 0 )
		{
			dword remain = pContext->timeLimit() - tick;
			sShipStatus += CharString().format(" - %2.2u:%2.2u:%2.2u", 
				remain / TICKS_PER_HOUR, 
				(remain / TICKS_PER_MINUTE) % 60, 
				(remain / TICKS_PER_SECOND) % 60 );
		}
		sShipStatus += CharString().format("\nCredits: %s", FormatNumber<char,int>( pShip->credits() ) );
		if ( pShip->command() != NounShip::NOCOMMAND )
			sShipStatus += CharString().format("\nOrder: %s", pShip->commandString() );		// display the current helm command

		// reset camera if targetting enemies as most of the logic is currently tied to onButtonDown
		if (m_pDoc->focus()->factionId() != pShip->factionId())
			m_pDoc->setFocus(pShip);
		
	}
	m_pShipStatusText->setText( sShipStatus );

	// check for a new order
	static NounShip::Order	lastOrder;
	static Noun *			lastOrderTarget;
	if ( pShip->order() != lastOrder || pShip->orderTarget() != lastOrderTarget )
	{
		// new order received, play sound
		lastOrder = pShip->order();
		lastOrderTarget = pShip->orderTarget();

		if ( pShip->order() != NounShip::NOORDER )
			window()->playSound( "ORDERRECEIVED" );
	}

	// check for collision
	m_CollisionCheck += t;
	if ( m_CollisionCheck > CHECK_FOR_COLLISION_TIME )
	{
		m_CollisionCheck = 0.0f;

		Vector3 origin( pShip->worldPosition() );
		Vector3 direction( pShip->worldVelocity() * COLLISION_WARNING_TIME );

		for(int i=0;i<pShip->contactCount();i++)
		{
			Noun * pContact = pShip->contact( i );
			if ( WidgetCast<NounPlanet>( pContact ) == NULL && WidgetCast<NounStar>( pContact ) == NULL )
				continue;

			Vector3 intersect;
			if ( pContact->intersect( origin, direction, intersect ) )
			{
				window()->playSound( "CollisionAlert" );
				break;
			}
		}
	}

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

	// we need to reset our focus if a planet has been captured whilst in engineering screen
	if (m_Mode == PLANET)
	{
		NounPlanet * pPlanet = WidgetCast<NounPlanet>(m_pDoc->planetTarget());
		if (pPlanet != NULL)
		{
			if (pPlanet->factionId() != pShip->factionId())
				changeMode(TACTICAL);
		}
	}

}

bool ViewGame::onMessage( const Message & msg )
{
	ASSERT( m_pDoc );

	GameContext * pContext = m_pDoc->context();
	ASSERT( pContext );

	//{{BEGIN_MSG_MAP
	MESSAGE_MAP( WB_BUTTONUP, 3838194152, onButtonExport);
	MESSAGE_MAP( WB_BUTTONUP, 876578339, onButtonExport);
	MESSAGE_MAP( WB_BUTTONUP, 3469095403, onButtonImport);
	MESSAGE_MAP( WB_BUTTONUP, 507479584, onButtonImport);
	MESSAGE_MAP( WB_BUTTONUP, 1489228428, onButtonScrapEnhancement);
	MESSAGE_MAP( WB_BUTTONUP, 1077320045, onButtonGroupPromote);
	MESSAGE_MAP( WB_BUTTONUP, 3290485422, onButtonGroupRemove);
	MESSAGE_MAP( WB_BUTTONUP, 328456872, onButtonGroupInvite);
	MESSAGE_MAP( WB_BUTTONUP, 2369808366, onButtonRemoveEnhancement);
	MESSAGE_MAP( WB_BUTTONUP, 3547696933, onButtonEnhUp);
	MESSAGE_MAP( WB_BUTTONUP, 3547696965, onButtonEnhUp);
	MESSAGE_MAP( WB_BUTTONUP, 3547696997, onButtonEnhUp);
	MESSAGE_MAP( WB_BUTTONUP, 3547695685, onButtonEnhUp);
	MESSAGE_MAP( WB_BUTTONUP, 3547695717, onButtonEnhUp);
	MESSAGE_MAP( WB_BUTTONUP, 3547696005, onButtonEnhUp);
	MESSAGE_MAP( WB_BUTTONUP, 3547696037, onButtonEnhUp);
	MESSAGE_MAP( WB_BUTTONUP, 3547696069, onButtonEnhUp);
	MESSAGE_MAP( WB_BUTTONUP, 3547696101, onButtonEnhUp);
	MESSAGE_MAP( WB_BUTTONUP, 3547695877, onButtonEnhUp);
	MESSAGE_MAP( WB_BUTTONUP, 3547695909, onButtonEnhUp);
	MESSAGE_MAP( WB_BUTTONUP, 3547695941, onButtonEnhUp);
	MESSAGE_MAP( WB_BUTTONUP, 2648656488, onButtonShowEnhancements);
	MESSAGE_MAP( WB_BUTTONUP, 3369159190, onButtonApplyEnhancement);
	MESSAGE_MAP( WB_BUTTONUP, 1947446322, onMessageBoxCANCEL);
	MESSAGE_MAP( WB_BUTTONUP, 857413197, onMessageBoxOK);
	MESSAGE_MAP( WB_BUTTONUP, 1297067625, onButtonAbortOrder);
	MESSAGE_MAP( WB_BUTTONUP, 1402546913, onButtonCancelOrder);
	MESSAGE_MAP( WB_BUTTONUP, 2276611452, onButtonOrderRaze);
	MESSAGE_MAP( WB_BUTTONUP, 1083490971, onButtonOrderDefend);
	MESSAGE_MAP( WB_BUTTONUP, 599193466, onButtonOrderAttack);
	MESSAGE_MAP( WB_BUTTONUP, 1000934661, onButtonTargetYard);
	MESSAGE_MAP( WB_BUTTONUP, 993922821, onButtonTargetPort);
	MESSAGE_MAP( WB_BUTTONUP, 203370444, onButtonTargetSupply);
	MESSAGE_MAP( WB_BUTTONUP, 1006793111, onButtonTargetJumpGate);
	MESSAGE_MAP( WB_BUTTONUP, 3041943984, onButtonTargetDistress);
	MESSAGE_MAP( WB_BUTTONUP, 378576430, onButtonTargetFriendly);
	MESSAGE_MAP( WB_BUTTONUP, 1306136455, onButtonTargetEnemy);
	MESSAGE_MAP( WB_BUTTONUP, 1609169792, onButtonShadow);
	MESSAGE_MAP( WB_BUTTONUP, 4048941282, onButtonScoutMission);
	MESSAGE_MAP( WB_BUTTONUP, 3487455382, onButtonDefendMission);
	MESSAGE_MAP( WB_BUTTONUP, 310826742, onButtonCaptureMission);
	MESSAGE_MAP( WB_BUTTONUP, 3603953565, onButtonAttackMission);
	MESSAGE_MAP( WB_BUTTONDOWN, 2362987489, onButtonTarget);
	MESSAGE_MAP( WB_BUTTONDOWN, 1488015155, onButtonYell);
	MESSAGE_MAP( WB_BUTTONDOWN, 3090946401, onButtonFaction);
	MESSAGE_MAP( WB_BUTTONDOWN, 683493611, onButtonFleet);
	MESSAGE_MAP( WB_BUTTONUP, 1366481305, onButtonNextTarget);
	MESSAGE_MAP( WB_BUTTONUP, 3963033914, onButtonUpTarget);
	MESSAGE_MAP( WB_BUTTONUP, 1505827891, onButtonNext);
	MESSAGE_MAP( WB_BUTTONUP, 1479207027, onButtonPrev);
	MESSAGE_MAP( WB_BUTTONUP, 2981508253, onButtonRepairNow);
	MESSAGE_MAP( WB_BUTTONUP, 920750390, onButtonRepairLast);
	MESSAGE_MAP( WB_BUTTONUP, 29309963, onButtonAutoDefense);
	MESSAGE_MAP( WB_BUTTONUP, 3103999357, onButtonUseStructure);
	MESSAGE_MAP( WE_EDITDONE, 1602461257, onEditTransferCredits);
	MESSAGE_MAP( WB_BUTTONUP, 2673418001, onButtonTransferCredits);
	MESSAGE_MAP( WB_BUTTONUP, 2548880352, onButtonPlanet);
	MESSAGE_MAP( WB_BUTTONUP, 3543943169, onButtonUnlock);
	MESSAGE_MAP( WB_BUTTONUP, 1508144083, onButtonLock);
	MESSAGE_MAP( WB_BUTTONUP, 3347918290, onButtonJettison);
	MESSAGE_MAP( WB_BUTTONUP, 1294536004, onButtonCamera);
	MESSAGE_MAP( WB_BUTTONUP, 1199820260, onButtonContinue);
	MESSAGE_MAP( WB_BUTTONUP, 73732811, onButtonPause);
	MESSAGE_MAP( WE_EDITDONE, 3966988240, onChatEdit);
	MESSAGE_MAP( WB_BUTTONUP, 2572688435, onButtonDisband);
	MESSAGE_MAP( WB_BUTTONUP, 38197355, onButtonScrap);
	MESSAGE_MAP( WB_BUTTONUP, 1508145715, onButtonLoad);
	MESSAGE_MAP( WB_BUTTONUP, 996079851, onButtonOrbit);
	MESSAGE_MAP( WB_BUTTONUP, 289274757, onButtonFollow);
	MESSAGE_MAP( WB_BUTTONUP, 2434104373, onButtonNavigation);
	MESSAGE_MAP( WB_BUTTONUP, 1503743155, onButtonHelp);
	//END_MSG_MAP}}

	switch( msg.message )
	{
	case HM_CHAR:
		switch( msg.wparam )
		{
		case HK_RETURN:
			if (! m_pButtonSendChat->buttonDown() )
				m_pButtonSendChat->onButtonDown();

			m_pChatEdit->onBeginEdit();
			return true;
		}
		break;
	case HM_KEYDOWN:
		switch( msg.wparam & 0xff )
		{
		case HK_ESCAPE:
			if ( m_Mode != TACTICAL )
			{
				changeMode( TACTICAL );
				return true;
			}
			if ( m_pDoc->focus() != m_pDoc->ship() )
			{
				// restore focus back to the ship
				m_pDoc->setFocus( m_pDoc->ship() );
				GameDocument::s_fCamera = 0.0f;
				return  true;
			}

			m_pDoc->setScene( "Main" );
			return true;
		}
		break;
	}

	return false;
}

void ViewGame::onDocumentUpdate()
{
	// document data has changed, update this view if needed
}

void ViewGame::onRender( RenderContext & context, const RectInt & window )
{
	// DO NO RENDERING HERE, it will be cleared by the other views on top..
}

//---------------------------------------------------------------------------------------------------

void ViewGame::pushMessageBox( MessageBoxDialog * pDialog )
{
	m_DialogList.push_back( pDialog );
	pDialog->m_pDoc = m_pDoc;
}

void ViewGame::popMessageBox( MessageBoxDialog * pDialog )
{
	if ( pDialog == m_pActiveDialog )
	{
		m_pMessageBox->hideWindow();
		m_pActiveDialog = NULL;
	}
	else
        m_DialogList.remove( pDialog );
}

void ViewGame::pushMessageLine( MessageLine * pLine )
{
	bool bFound = false;
	for( MessageList::iterator iMessage = m_MessageList.begin(); 
		iMessage != m_MessageList.end(); ++iMessage )
	{
		if ( (*iMessage).pointer() == pLine )
		{
			bFound = true;
			break;
		}
	}

	if (! bFound )
		m_MessageList.push_back( pLine );
}

void ViewGame::popMessageLine( MessageLine * pLine )
{
	m_MessageList.remove( pLine );
}

bool ViewGame::renderViewGame( RenderContext & context, const RectInt & w, bool a_bRedAlertBorder /*= true*/ )
{
	GameDocument * pDoc = (GameDocument *)document();
	if (! pDoc )
		return false;
	GameContext * pContext = pDoc->context();
	if (! pContext )
		return false;
	NounShip * pShip = pDoc->ship();

	if ( pShip != NULL )
	{
		// are we in jump or jumping?
		GadgetJumpDrive * pJump = pShip->jumpDrive();
		bool inJump = pJump != NULL ? pJump->engaged() || pJump->jumping() : false;

		if ( inJump )
		{
			if (! m_pJumpingMessage )
				m_pJumpingMessage = new MessageLine();

			// display the ETA
			if ( pJump->jumping() )
				m_pJumpingMessage->m_sText.format( "Jump Alignment... %.1f s", TICK_DURATION_S * pJump->jumpTime() );
			else
				m_pJumpingMessage->m_sText.format( "Jumping... %.1f s", TICK_DURATION_S * pJump->jumpTime() );
		}
		else if ( m_pJumpingMessage )
			m_pJumpingMessage = NULL;

		if ( (pShip->flags() & NounShip::FLAG_JUMP_DISABLED) != 0 )
			m_pJumpDisabledMessage = new MessageLine( "Jump Drive Disabled...", RED, true );
		else if ( m_pJumpDisabledMessage )
			m_pJumpDisabledMessage = NULL;
			
		int nRank =  pShip->rank();
		int nMaxRank = m_pDoc->client()->maxRank();	
		
		if ( nRank > nMaxRank && m_nRankTimeout == 0 ) // this will only ever be true if you're rank locked and the message hasn't been displayed, simple and effective
		{
			m_nRankTimeout = pContext->tick() + 200;
			m_pRankLockMessage = new MessageLine( "You are in a rank locked sector and will not gain prestige.", RED, true );
		}
		else if ( m_pRankLockMessage &&  pContext->tick() >= m_nRankTimeout )
			m_pRankLockMessage = NULL;
			
		if (! pShip->isOutOfCombat() )
		{
			if (! m_pOutOfCombatMessage )
				m_pOutOfCombatMessage = new MessageLine();

			m_pOutOfCombatMessage->m_sText.format(  "Out of combat in %.1f s", pShip->OutOfCombatTimer() );
		}
		else if ( m_pOutOfCombatMessage )
			m_pOutOfCombatMessage = NULL;

		// check if we are capturing any planets..
		NounPlanet * pCaptureTarget = WidgetCast<NounPlanet>( pShip->captureTarget() );
		if ( pCaptureTarget != NULL && !pShip->isFriend( pCaptureTarget ) )
		{
			float fPercent = pCaptureTarget->control();
			fPercent = Clamp<float>( 100.0f - ((fPercent / pCaptureTarget->maxControl()) * 100.0f), 0.0f, 100.0f );

			m_pCaptureMessage = new MessageLine( CharString().format( "Capturing %s [%.1f%%]...", 
				pCaptureTarget->name(), fPercent ), GREEN, true );
		}
		else if ( m_pCaptureMessage )
			m_pCaptureMessage = NULL;

		if ( pShip->despawning() )
		{
			if (! m_pDespawnMessage )
				m_pDespawnMessage = new MessageLine();

			m_pDespawnMessage->m_sText.format( "Docking... %.1f s", TICK_DURATION_S * pShip->despawnTicks() );
		}
		else if ( m_pDespawnMessage )
			m_pDespawnMessage = NULL;
	}

	if ( a_bRedAlertBorder && redAlert() )
	{
		int nAlpha = (pContext->tick() % TICKS_PER_SECOND);
		if ( nAlpha <= 10 )
			nAlpha = nAlpha * (255 / 10);
		else
			nAlpha = 255 - ((nAlpha - 10) * (255 / 10));

		RectInt glowRect( w );
		glowRect.inset( 20 );

		PrimitiveMaterial::push( context.display(), PrimitiveMaterial::ALPHA, false );
		NodeWindow::pushGlow( context, glowRect, 20, Color(0,0,0,0), Color(255,0,0,nAlpha), 0, 0 );
	}

	WindowStyle * pStyle = View::window()->windowStyle();
	ASSERT( pStyle );

	Font * pFont = pStyle->font();
	Font * pLargeFont = pStyle->largeFont();

	// start all messages in the lower center part of the screen..
	int y = ((w.bottom + w.top) >> 1) + (w.height() >> 2);
	int cx = (w.right + w.left) >> 1;

	for( MessageList::iterator iMessage = m_MessageList.begin();
		iMessage != m_MessageList.end(); )
	{
		MessageLine * pLine = *iMessage;
		if ( pLine != NULL )
		{
			Font * pDrawFont = pLine->m_bLarge ? pLargeFont : pFont;
			if ( pDrawFont != NULL )
			{
				SizeInt textSize( pDrawFont->size( pLine->m_sText ) );
				PointInt textPos( cx - (textSize.width >> 1), y );

				Font::push( context.display(), pDrawFont, textPos, pLine->m_sText, pLine->m_cColor );
				y += textSize.height + 2;
			}

			++iMessage;
		}
		else
		{
			// message line destroyed, just remove it from the list automatically..
			m_MessageList.erase( iMessage++ );
		}
	}

	return true;
}

bool ViewGame::updateAction( bool bOrderMode )
{
	if (! m_pDoc || !m_pDoc->ship() )
		return false;
	NounShip * pShip = m_pDoc->ship();

	// update right click action..
	m_eContextAction = CA_NONE;
	m_sContextAction = "";

	Noun * pCursorTarget = m_pDoc->cursorTarget();
	if ( pCursorTarget != NULL )
	{
		Noun * pTarget = m_pDoc->target();
		if ( bOrderMode )
		{
			if ( pTarget != pShip && VerbOrderShip::canOrder( pTarget, pShip ) )
			{
				m_wpOrderTarget = pTarget;

				// if drop target, resolve to the planet ..
				if ( WidgetCast<NounDropTarget>( pCursorTarget ) )
					pCursorTarget = ((NounDropTarget *)pCursorTarget)->planet();

				bool bValidShipType = true;
				NounShip * pTargetShip = (NounShip *)pTarget;
				switch( pTargetShip->type() )
				{
				case NounShip::ENGINEER:
					if ( VerbOrderShip::canOrderShip( pTargetShip, NounShip::BUILD, pCursorTarget, pShip ) )
					{
						m_eContextAction = CA_ORDER_BUILD;
						m_sContextAction.format( "ORDER %s BUILD %s", pTargetShip->name(), pCursorTarget->name() );
					}
					break;
				case NounShip::SUPPLY:
				case NounShip::PLATFORM_SUPPLY:
					if ( VerbOrderShip::canOrderShip( pTargetShip, NounShip::RELOAD, pCursorTarget, pShip ) )
					{
						m_eContextAction = CA_ORDER_RELOAD;
						m_sContextAction.format( "ORDER %s RELOAD %s", pTargetShip->name(), pCursorTarget->name() );
					}
					break;
				case NounShip::TRANSPORT:
					if ( VerbOrderShip::canOrderShip( pTargetShip, NounShip::CAPTURE, pCursorTarget, pShip ) )
					{
						m_eContextAction = CA_ORDER_CAPTURE;
						m_sContextAction.format( "ORDER %s CAPTURE %s", pTargetShip->name(), pCursorTarget->name() );
					}
					break;
				case NounShip::SCOUT:
					if ( VerbOrderShip::canOrderShip( pTargetShip, NounShip::RECON, pCursorTarget, pShip ) )
					{
						m_eContextAction = CA_ORDER_RECON;
						m_sContextAction.format( "ORDER %s RECON %s", pTargetShip->name(), pCursorTarget->name() );
					}
					break;
				case NounShip::FRIGATE:
				case NounShip::DESTROYER:
				case NounShip::CRUISER:
				case NounShip::DREAD:
				case NounShip::STATION:
				case NounShip::PLATFORM:
					if ( pShip->isFriend( pCursorTarget ) 
						&& VerbOrderShip::canOrderShip( pTargetShip, NounShip::DEFEND, pCursorTarget, pShip ) )
					{
						m_eContextAction = CA_ORDER_DEFEND;
						m_sContextAction.format( "ORDER %s DEFEND %s", pTargetShip->name(), pCursorTarget->name() );
					}
					else if ( !pShip->isFriend( pCursorTarget )
						&& VerbOrderShip::canOrderShip( pTargetShip, NounShip::ATTACK, pCursorTarget, pShip ) )
					{
						m_eContextAction = CA_ORDER_ATTACK;
						m_sContextAction.format( "ORDER %s ATTACK %s", pTargetShip->name(), pCursorTarget->name() );
					}
					break;
				default:
					// unknown or unsupported ship type..
					bValidShipType = false;
					break;
				}

				// any valid ship can hold position at X ... fallback to this order if nothing better is available.
				if ( bValidShipType && m_eContextAction == CA_NONE &&
					VerbOrderShip::canOrderShip( pTargetShip, NounShip::HOLD, pCursorTarget, pShip ) )
				{
					m_eContextAction = CA_ORDER_MOVE;
					m_sContextAction.format( "ORDER %s MOVE TO %s", pTargetShip->name(), pCursorTarget->name() );
				}
			}
			else if ( VerbOrderUnit::canOrder( pTarget, pShip ) )
			{
				m_wpOrderTarget = pTarget;

				if ( WidgetCast<NounStructure>( pCursorTarget ) && !pShip->isFriend( pCursorTarget ) )
				{
					m_eContextAction = CA_ORDER_UNIT_RAZE;
					m_sContextAction.format( "ORDER %s RAZE %s", pTarget->name(), pCursorTarget->name() );
				}
				else if ( WidgetCast<NounUnit>( pCursorTarget ) && !pShip->isFriend( pCursorTarget ) )
				{
					m_eContextAction = CA_ORDER_UNIT_ATTACK;
					m_sContextAction.format( "ORDER %s ATTACK %s", pTarget->name(), pCursorTarget->name() );
				}
				else if ( WidgetCast<NounDropTarget>( pCursorTarget ) 
					|| WidgetCast<NounUnit>( pCursorTarget ) 
					|| WidgetCast<NounStructure>( pCursorTarget ) )
				{
					m_eContextAction = CA_ORDER_UNIT_MOVE;
					m_sContextAction.format( "ORDER %s MOVE TO %s", pTarget->name(), pCursorTarget->name() );
				}
			}
		}
		else if (! sm_bDisableTacticalActions )
		{
			// if drop target, resolve to the planet ..
			if ( WidgetCast<NounDropTarget>( pCursorTarget ) )
				pCursorTarget = ((NounDropTarget *)pCursorTarget)->planet();

			Noun * pTarget = m_pDoc->target();

			bool bIsEnemy = pShip->isEnemy( pCursorTarget );
			
			if ( WidgetCast<NounShip>( pCursorTarget ) 
				&& pCursorTarget != pShip  )
			{
				if ( bIsEnemy )
				{
					if ( pShip->canAlpha( pCursorTarget ) )
					{
						// cursor is over a enemy ship
						m_eContextAction = CA_ATTACK;
						m_sContextAction.format("ATTACK %s", pCursorTarget->name() );
					}
				}
				
				if ( m_eContextAction == CA_NONE )
				{
					GadgetReload * pReload = GadgetReload::getGadgetReload( pShip );
					GadgetBuild * pBuild = GadgetBuild::getGadgetBuild( pShip );

					if ( pReload != NULL && pReload->usable( pCursorTarget, false ) )
					{
						m_eContextAction = CA_RELOAD;
						m_sContextAction.format( "RELOAD %s", pCursorTarget->name() );
					}
					else if ( pBuild != NULL && pBuild->usable( pCursorTarget, false ) )
					{
						m_eContextAction = CA_BUILD;
						m_sContextAction.format( "BUILD %s", pCursorTarget->name() );
					}
					else if ( !pShip->isCommand( NounShip::FOLLOW, pCursorTarget )
						&& !Keyboard::shiftDown() )
					{
						// cursor is over a ship
						m_eContextAction = CA_FOLLOW;
						m_sContextAction.format( "FOLLOW %s", pCursorTarget->name() );

						if ( !pShip->isCommand( NounShip::SHADOW, pCursorTarget ) )
							m_sContextAction += "\nHOLD SHIFT TO SHADOW";
					}
					else if ( !pShip->isCommand( NounShip::SHADOW, pCursorTarget )
						&& Keyboard::shiftDown() )
					{
						m_eContextAction = CA_SHADOW;
						m_sContextAction.format( "SHADOW %s", pCursorTarget->name() );
					}
				}
			}
			else if ( WidgetCast<NounPlanet>( pCursorTarget ) )
			{
				if ( bIsEnemy )
				{
					if ( pShip->canBomb( pCursorTarget ) )
					{
						m_eContextAction = CA_BOMB;
						m_sContextAction.format("BOMB %s", pCursorTarget->name());
					}
				}
				
				if ( m_eContextAction == CA_NONE )
				{
					if ( !pShip->isCommand( NounShip::ORBIT, pCursorTarget ) 
						&& pShip->orbiting() != pCursorTarget )
					{
						m_eContextAction = CA_ORBIT;
						m_sContextAction.format( "ORBIT %s", pCursorTarget->name());
					}
				}
			}
			else if ( WidgetCast<NounJumpGate>( pCursorTarget )
				&& !pShip->isCommand( NounShip::FOLLOW, pCursorTarget ) )
			{
				m_eContextAction = CA_FOLLOW;
				m_sContextAction.format( "ENTER %s", pCursorTarget->name());
			}
			else if ( WidgetCast<NounCargo>( pCursorTarget )
				&& VerbLoad::canLoad( pCursorTarget, pShip ) )
			{
				m_eContextAction = CA_LOAD;
				m_sContextAction.format( "LOAD %s", pCursorTarget->name());
			}
			else if ( WidgetCast<NounStructure>( pCursorTarget ) )
			{
				if ( bIsEnemy )
				{
					if ( pShip->canBomb( pCursorTarget ) )
					{
						m_eContextAction = CA_BOMB;
						m_sContextAction.format("BOMB %s", pCursorTarget->name());
					}
				}
				else
				{
					GadgetBuild * pBuild = GadgetBuild::getGadgetBuild( pShip );

					if ( pBuild != NULL && pBuild->usable( pCursorTarget, false ) )
					{
						m_eContextAction = CA_BUILD;
						m_sContextAction.format( "BUILD %s", pCursorTarget->name() );
					}
				}
			}
		}
	}

	if ( m_eContextAction != CA_NONE )
	{
		setCursorState( USER1 );
		m_wpContextTarget = pCursorTarget;

		// set the right mouse button hint
		m_pDoc->setRightHint( m_sContextAction );

		// set the tool tip on the mouse cursor as well
		CharString sCursorTip( m_sContextAction );
		int nFirstNewline = sCursorTip.find( '\n' );
		if ( nFirstNewline >= 0 )
			sCursorTip[nFirstNewline] = 0;		// keep the cursor tip one line

		setCursorTip( sCursorTip );
	}
	else
	{
		setCursorState( SELECT );
		setCursorTip( "" );

		m_pDoc->setRightHint( "" );
	}

	if ( pCursorTarget != NULL && pCursorTarget != m_pDoc->target() )
		m_pDoc->setLeftHint( CharString().format("TARGET %s", pCursorTarget->name() ) );
	else
		m_pDoc->setLeftHint( "" );

	return m_eContextAction != CA_NONE;
}

bool ViewGame::doAction( bool bOrderMode )
{
	if (! m_pDoc  )
		return false;
	NounShip * pShip = m_pDoc->ship();
	if (! pShip )
		return false;

	switch( m_eContextAction )
	{
	case CA_ATTACK:
		pShip->verbAlpha( m_wpContextTarget );
		break;
	case CA_BOMB:
		pShip->verbBomb( m_wpContextTarget );
		break;
	case CA_ORBIT:
		pShip->verbCommand( NounShip::ORBIT, m_wpContextTarget );
		break;
	case CA_FOLLOW:
		pShip->verbCommand( NounShip::FOLLOW, m_wpContextTarget );
		break;
	case CA_SHADOW:
		pShip->verbCommand( NounShip::SHADOW, m_wpContextTarget );
		break;
	case CA_LOAD:
		if ( VerbLoad::canLoad( m_wpContextTarget, pShip ) )
			Verb::Ref( new VerbLoad( pShip, m_wpContextTarget ) );
		break;
	case CA_BUILD:
		for(int i=0;i<pShip->childCount();++i)
		{
			GadgetBuild * pBuild = WidgetCast<GadgetBuild>( pShip->child(i) );
			if ( pBuild != NULL )
				pShip->useGadget( pBuild, m_wpContextTarget, false );
		}
		break;
	case CA_RELOAD:
		for(int i=0;i<pShip->childCount();++i)
		{
			GadgetReload * pReload = WidgetCast<GadgetReload>( pShip->child(i) );
			if ( pReload != NULL )
				pShip->useGadget( pReload, m_wpContextTarget, false );
		}
		// ship orders
	case CA_ORDER_ATTACK:
		Verb::Ref( new VerbOrderShip( WidgetCast<NounShip>( m_wpOrderTarget), NounShip::ATTACK, m_wpContextTarget, pShip ) );
		break;
	case CA_ORDER_CAPTURE:
		Verb::Ref( new VerbOrderShip( WidgetCast<NounShip>( m_wpOrderTarget), NounShip::CAPTURE, m_wpContextTarget, pShip ) );
		break;
	case CA_ORDER_DEFEND:
		Verb::Ref( new VerbOrderShip( WidgetCast<NounShip>( m_wpOrderTarget), NounShip::DEFEND, m_wpContextTarget, pShip ) );
		break;
	case CA_ORDER_RECON:
		Verb::Ref( new VerbOrderShip( WidgetCast<NounShip>( m_wpOrderTarget), NounShip::RECON, m_wpContextTarget, pShip ) );
		break;
	case CA_ORDER_BUILD:
		Verb::Ref( new VerbOrderShip( WidgetCast<NounShip>( m_wpOrderTarget), NounShip::BUILD, m_wpContextTarget, pShip ) );
		break;
	case CA_ORDER_RELOAD:
		Verb::Ref( new VerbOrderShip( WidgetCast<NounShip>( m_wpOrderTarget), NounShip::RELOAD, m_wpContextTarget, pShip ) );
		break;
	case CA_ORDER_MOVE:
		Verb::Ref( new VerbOrderShip( WidgetCast<NounShip>( m_wpOrderTarget), NounShip::MOVE, m_wpContextTarget, pShip ) );
		break;
		// unit orders
	case CA_ORDER_UNIT_ATTACK:
		Verb::Ref( new VerbOrderUnit( WidgetCast<NounUnit>( m_wpOrderTarget ), NounUnit::ATTACK, m_wpContextTarget, pShip ) );
		break;
	case CA_ORDER_UNIT_RAZE:
		Verb::Ref( new VerbOrderUnit( WidgetCast<NounUnit>( m_wpOrderTarget ), NounUnit::RAZE, m_wpContextTarget, pShip ) );
		break;
	case CA_ORDER_UNIT_MOVE:
		Verb::Ref( new VerbOrderUnit( WidgetCast<NounUnit>( m_wpOrderTarget ), NounUnit::MOVE, m_wpContextTarget, pShip ) );
		break;
	default:
		return false;
	}

	// play confirmation sound..
	window()->playSound( "CONTEXT_ACTION" );

	updateAction( bOrderMode );
	return true;
}

//----------------------------------------------------------------------------


bool ViewGame::onButtonHelp(const Message & msg)
{
	// show/hide the help window
	if ( m_pHelpWindow->visible() )
		m_pHelpWindow->hideWindow();
	else
		m_pHelpWindow->showWindow();
	return true;
}

bool ViewGame::onButtonNavigation(const Message & msg)
{
	if ( m_Mode != NAVIGATION )
		changeMode( NAVIGATION );
	else
		changeMode( TACTICAL );
	return true;
}

bool ViewGame::onButtonPlanet(const Message & msg)
{
	if ( m_Mode != PLANET )
		changeMode( PLANET );
	else
		changeMode( TACTICAL );
	return true;
}

bool ViewGame::onButtonFollow(const Message & msg)
{
	m_pDoc->ship()->verbCommand( NounShip::FOLLOW, myTarget() );
	return true;
}

bool ViewGame::onButtonOrbit(const Message & msg)
{
	m_pDoc->ship()->verbCommand( NounShip::ORBIT, myTarget() );
	return true;
}

bool ViewGame::onButtonShadow(const Message & msg)
{
	m_pDoc->ship()->verbCommand( NounShip::SHADOW, myTarget() );
	return true;
}

bool ViewGame::onButtonLoad(const Message & msg)
{
	NounShip * pShip = m_pDoc->ship();
	Noun * pLoad = myTarget();

	// if loading from a port, just call use structur, so resources get updated properly
	// still using the CanLoad call for the button availabilty since it checks cargo room and everything
	if(WidgetCast<StructurePort>(pLoad))
	{
		StructurePort *pStructure = WidgetCast<StructurePort>(pLoad);
		if(VerbUseStructure::canUse( pStructure, pShip ))
			Verb::Ref( new VerbUseStructure( pStructure, m_pDoc->ship()) );
	}
	else if ( VerbLoad::canLoad( pLoad, pShip ) )
	{
		// send the verb to load the object
		Verb::Ref( new VerbLoad( pShip, pLoad ) );

		// if loading a unit from the planet, automatically select any other friendly unit in the same hex...
		Noun * pParent = pLoad->parentNoun();
		if ( WidgetCast<NounPlanet>( pParent ) && WidgetCast<NounUnit>( pLoad ) )
		{
			int nHex = ((NounUnit *)pLoad)->hex();
			for(int i=0;i<pParent->childCount();++i)
			{
				NounUnit * pUnit = WidgetCast<NounUnit>( pParent->child(i) );
				if ( pUnit && pUnit != pLoad && pUnit->hex() == nHex && VerbLoad::canLoad( pUnit, pShip ) )
				{
					m_pDoc->setTarget( pUnit );
					return true;
				}
			}
		}
		
		m_pDoc->setTarget( pParent );
	}

	return true;
}

bool ViewGame::onButtonScrap(const Message & msg)
{
	NounStructure * pScrap = WidgetCast<NounStructure>( myTarget() );
	if ( pScrap != NULL )
	{
		if ( pScrap != m_ConfirmScrap )
		{
			m_pDoc->pushChat( CharString().format( "<color;ffffff>Construction: Confirm scrap of %s...</color>", pScrap->name()) );
			m_ConfirmScrap = pScrap;
		}
		else
		{
			NounPlanet * pPlanet = WidgetCast<NounPlanet>( pScrap->planet() );
			if ( pPlanet != NULL )
			{
				Noun * pTarget = new NounDropTarget( pPlanet, pScrap->hex() );
				m_pDoc->setTarget( pTarget );
			}
			else
				m_pDoc->setTarget( pScrap->parentNoun() );
			Verb::Ref( new VerbScrap( pScrap, m_pDoc->ship()) );
		}
	}

	return true;
}

bool ViewGame::onButtonDisband(const Message & msg)
{
	NounUnit * pDisband = WidgetCast<NounUnit>( myTarget() );
	if ( pDisband != NULL )
	{
		if ( pDisband != m_ConfirmDisband )
		{
			m_pDoc->pushChat( CharString().format( "<color;ffffff>SERVER: Confirm disband of %s...</color>", pDisband->name()) );
			m_ConfirmDisband = pDisband;
		}
		else
		{
			m_pDoc->setTarget( pDisband->parentNoun() );
			Verb::Ref( new VerbDisband( pDisband, m_pDoc->ship()) );
		}

	}
	return true;
}

struct ChatModeLookup {
	const char *		m_pPrefix;
	ViewGame::ChatMode	m_eMode;
};

static ChatModeLookup CHAT_MODE_LOOKUP[] = 
{
	{ "/faction",	ViewGame::FACTION },
	{ "/team",		ViewGame::FLEET },
	{ "/fleet",		ViewGame::FLEET },
	{ "/yell",		ViewGame::YELL },
	{ "/target",	ViewGame::TARGET }
};
const int CM_LOOKUP_COUNT = sizeof(CHAT_MODE_LOOKUP)/sizeof(CHAT_MODE_LOOKUP[0]);

bool ViewGame::onChatEdit(const Message & msg)
{
	CharString send( m_pChatEdit->text() );
	if ( send.length() > 0 )
	{
		if ( send[ 0 ] == '/' )
		{
			for( int i=0;i<CM_LOOKUP_COUNT;++i)
				if ( strnicmp( send, CHAT_MODE_LOOKUP[i].m_pPrefix, 
					strlen( CHAT_MODE_LOOKUP[i].m_pPrefix ) ) == 0 )
				{
					m_ChatMode = CHAT_MODE_LOOKUP[i].m_eMode;
					break;
				}
		}
		else if ( send[0] != '/' )
		{
			for(int i=0;i<CM_LOOKUP_COUNT;++i)
				if ( CHAT_MODE_LOOKUP[i].m_eMode == m_ChatMode )
				{
					send.format( "%s %s", CHAT_MODE_LOOKUP[i].m_pPrefix, send.cstr() );
					break;
				}
		}

		m_pDoc->sendChat( send );
		m_pChatEdit->setText( "" );
	}

	// hide the communications window
	if ( m_pButtonSendChat->buttonDown() )
		m_pButtonSendChat->onButtonUp();

	return true;
}

bool ViewGame::onButtonPause(const Message & msg)
{
	NounStructure * pStructure = WidgetCast<NounStructure>( myTarget() );
	if ( pStructure != NULL) 
		Verb::Ref( new VerbSetStructureActive( pStructure, m_pDoc->ship(), false ) );

	return true;
}

bool ViewGame::onButtonContinue(const Message & msg)
{
	NounStructure * pStructure = WidgetCast<NounStructure>( myTarget() );
	if ( pStructure != NULL )
		Verb::Ref( new VerbSetStructureActive( pStructure, m_pDoc->ship(), true ) );

	return true;
}

bool ViewGame::onButtonImport(const Message & msg)
{
	NounStructure * pStructure = WidgetCast<NounStructure>( myTarget() );
	if ( pStructure != NULL )
	{
		bool bExport = VerbImportExport::canExport( pStructure->planet() );
		if ( VerbImportExport::canImport( pStructure->planet() ) )
			Verb::Ref( new VerbImportExport( pStructure->planet(), false, bExport ) );
		else
			Verb::Ref( new VerbImportExport( pStructure->planet(), true, bExport ) );
	}

	return true;
}

bool ViewGame::onButtonExport(const Message & msg)
{
	NounStructure * pStructure = WidgetCast<NounStructure>( myTarget() );
	if ( pStructure != NULL )
	{
		bool bImport = VerbImportExport::canImport( pStructure->planet() );
		if ( VerbImportExport::canExport( pStructure->planet() ) )
			Verb::Ref( new VerbImportExport( pStructure->planet(), false, bImport ) );
		else
			Verb::Ref( new VerbImportExport( pStructure->planet(), true, bImport ) );
	}

	return true;
}

bool ViewGame::onButtonCamera(const Message & msg)
{
	if ( m_pDoc->focus() != myTarget()  )
	{
		// change to tactical mode
		if ( m_Mode != TACTICAL )
			changeMode( TACTICAL );

		m_pDoc->setFocus( myTarget() );
		GameDocument::s_fCamera = 0.0f;

		return true;
	}

	m_pDoc->setFocus( m_pDoc->ship() );
	GameDocument::s_fCamera = 0.0f;
	return true;
}

bool ViewGame::onButtonJettison(const Message & msg)
{
	Noun * pJettison = myTarget();
	if ( pJettison != NULL )
	{
		if ( pJettison != m_ConfirmJettison )
		{
			m_pDoc->pushChat( CharString().format( "<color;ffffff>Cargo: Confirm jettison of %s...", pJettison->name()) );
			m_ConfirmJettison = pJettison;
		}
		else
		{
			m_pDoc->setTarget( pJettison );
			m_pDoc->ship()->verbJettison( pJettison );
		}
	}
	return true;
}

bool ViewGame::onButtonLock(const Message & msg)
{
	NounPlanet * pPlanet = WidgetCast<NounPlanet>( myTarget() );
	if ( VerbLockPlanet::canLock( pPlanet, m_pDoc->ship() ) )
		Verb::Ref( new VerbLockPlanet( pPlanet, m_pDoc->ship(), true) );

	return true;
}

bool ViewGame::onButtonUnlock(const Message & msg)
{
	NounPlanet * pPlanet = WidgetCast<NounPlanet>( myTarget() );
	if ( VerbLockPlanet::canUnlock( pPlanet, m_pDoc->ship() ) )
		Verb::Ref( new VerbLockPlanet( pPlanet, m_pDoc->ship(), false) );

	return true;
}

bool ViewGame::onButtonTransferCredits(const Message & msg)
{
	m_pWindowTransferCredits->showWindow();
	m_pEditTransferCredits->onBeginEdit();
	return true;
}

bool ViewGame::onEditTransferCredits(const Message & msg)
{
	int credits = strtol( m_pEditTransferCredits->text(), NULL, 10 );

	NounShip * pSrc = m_pDoc->ship();
	NounShip * pDst = WidgetCast<NounShip>( myTarget() );
	if ( VerbTransferCredits::canTransferCredits( pSrc, pDst, credits ) )
		Verb::Ref( new VerbTransferCredits( pSrc, pDst, credits) );
	
	m_pWindowTransferCredits->hideWindow();
	return true;
}

bool ViewGame::onButtonUseStructure(const Message & msg)
{
	NounStructure * pStructure = WidgetCast<NounStructure>( myTarget() );
	if ( pStructure != NULL )
	{
		// does using this structure require confirmation?
		if ( pStructure->useConfirm( m_pDoc->ship() ) )
		{
			if ( m_ConfirmUseStructure != pStructure )
			{
				// yes, confirm the use of this structure by the players ship
				m_pDoc->pushChat( CharString().format( "%s, execute command again...", 
					pStructure->useConfirmMessage( m_pDoc->ship() )) );
				m_ConfirmUseStructure = pStructure;

				return true;
			}
		}

		// use structure without confirmation
		Verb::Ref( new VerbUseStructure( pStructure, m_pDoc->ship()) );
		return true;
	}

	return true;
}


//----------------------------------------------------------------------------

Noun * ViewGame::myTarget() const
{
	Noun * pTarget = m_pDoc->target();
	if ( WidgetCast<NounDropTarget>( pTarget ) )
		pTarget = ((NounDropTarget *)pTarget)->planet();

	return pTarget;
}

NounGadget * ViewGame::myGadgetTarget() const
{
	NounGadget * pGadget = WidgetCast<NounGadget>( m_pDoc->target() );
	if ( pGadget != NULL && pGadget->parentNoun() == m_pDoc->ship() )
		return pGadget;

	return NULL;
}

void ViewGame::changeMode( Mode mode )
{
	if ( m_Mode != mode )
	{
		// switch the mode
		m_Mode = mode;
		// reset the camera animation time
		GameDocument::s_fCamera = 0.0f;

		// always restore focus to the players ship when they change modes
		m_pDoc->setFocus( m_pDoc->ship() );

		// show/hide windows based on the mode
		switch( m_Mode )
		{
		case TACTICAL:
			m_pViewNavigation->hideWindow();
			m_pViewTactical->showWindow();
			m_pViewPlanet->hideWindow();
			//window()->playSound( "TACTICAL" );
			break;
		case NAVIGATION:
			m_pViewTactical->hideWindow();
			m_pViewNavigation->showWindow();
			m_pViewPlanet->hideWindow();
			//window()->playSound( "NAVIGATION" );
			break;
		case PLANET:
			m_pViewTactical->hideWindow();
			m_pViewNavigation->hideWindow();
			m_pViewPlanet->showWindow();
			break;
		}
	}
}

//----------------------------------------------------------------------------

void ViewGame::hideAllButtons( BaseNode * pNode )
{
	// check if this node is a button, if so then hide it
	WindowButton * pButton = WidgetCast<WindowButton>( pNode );
	if ( pButton != NULL )
		pButton->hideWindow();

	// recurse into all children
	for(int i=0;i<pNode->childCount();i++)
		hideAllButtons( pNode->child(i) );
}

//----------------------------------------------------------------------------

bool ViewGame::onButtonAutoDefense(const Message & msg)
{
	Verb::Ref( new VerbUseSingleGadget( myGadgetTarget(), NULL, true ) );
	return true;
}

bool ViewGame::onButtonRepairLast(const Message & msg)
{
	Verb::Ref( new VerbRepairGadget( myGadgetTarget(), NounShip::LAST ) );
	return true;
}

bool ViewGame::onButtonRepairNow(const Message & msg)
{
	Verb::Ref( new VerbRepairGadget( myGadgetTarget(), NounShip::NOW ) );
	return true;
}

bool ViewGame::onButtonPrev(const Message & msg)
{
	NounGadget * pGadget = myGadgetTarget();
	ASSERT( pGadget );

	int prevMode = pGadget->useMode() - 1;
	if ( prevMode < 0 )
		prevMode = pGadget->useModeCount() - 1;

	Verb::Ref( new VerbUseMode( pGadget, prevMode ) );
	return true;
}

bool ViewGame::onButtonNext(const Message & msg)
{
	NounGadget * pGadget = m_pDoc->gadgetTarget();
	ASSERT( pGadget );

	int nextMode = (pGadget->useMode() + 1) % pGadget->useModeCount();
	Verb::Ref( new VerbUseMode( pGadget, nextMode ) );
	return true;
}

bool ViewGame::onButtonUpTarget(const Message & msg)
{
	Noun * pTarget = myTarget();
	if ( pTarget != NULL )
		m_pDoc->setTarget( pTarget->parentNoun() );
	return true;
}

bool ViewGame::onButtonNextTarget(const Message & msg)
{
	Noun * pTarget = myTarget();
	if ( pTarget != NULL )
	{
		Noun * pParent = pTarget->zoneParent();
		if ( pParent != NULL )
		{
			GameContext * pContext = m_pDoc->context();
			ASSERT( pContext );
			NounShip * pShip = m_pDoc->ship();
			if (! pShip )
				return true;

			bool bLastFound = false;
			if ( pParent == pTarget )
				bLastFound = true;

			for(int i=0;i<pParent->childCount();i++)
			{
				Noun * pChild = WidgetCast<Noun>( pParent->child(i) );
				if ( pChild != NULL )
				{
					if ( !bLastFound && pChild == pTarget )
					{
						// found our previous target in the child list of the zone parent
						bLastFound = true;
					}
					else if ( bLastFound && pShip->validateTarget( pChild ) )
					{
						m_pDoc->setTarget( pChild );
						return true;
					}
				}
			}

			// no additional children objects found, set target back to the parent object
			m_pDoc->setTarget( pParent );
		}
	}
	return true;
}

bool ViewGame::onButtonFleet(const Message & msg)
{
	m_ChatMode = FLEET;
	return true;
}

bool ViewGame::onButtonFaction(const Message & msg)
{
	m_ChatMode = FACTION;
	return true;
}

bool ViewGame::onButtonYell(const Message & msg)
{
	m_ChatMode = YELL;
	return true;
}

bool ViewGame::onButtonTarget(const Message & msg)
{
	m_ChatMode = TARGET;
	return true;
}

bool ViewGame::onButtonAttackMission(const Message & msg)
{
	m_pDoc->ship()->sendOrder( NounShip::ATTACK, m_pDoc->target() );
	return true;
}

bool ViewGame::onButtonCaptureMission(const Message & msg)
{
	m_pDoc->ship()->sendOrder( NounShip::CAPTURE, m_pDoc->target() );
	return true;
}

bool ViewGame::onButtonDefendMission(const Message & msg)
{
	m_pDoc->ship()->sendOrder( NounShip::DEFEND, m_pDoc->target() );
	return true;
}

bool ViewGame::onButtonScoutMission(const Message & msg)
{
	m_pDoc->ship()->sendOrder( NounShip::RECON, m_pDoc->target() );
	return true;
}

bool ViewGame::onButtonCancelOrder(const Message & msg)
{
	NounShip * pShip = WidgetCast<NounShip>( m_pDoc->target() );
	if ( pShip != NULL && pShip->order() != NounShip::NOORDER )
		pShip->verbOrder( NounShip::NOORDER, NULL, m_pDoc->ship() );

	return true;
}

bool ViewGame::onButtonAbortOrder(const Message & msg)
{
	if ( m_pDoc->ship()->order() != NounShip::NOORDER )
		m_pDoc->ship()->verbOrder( NounShip::NOORDER, NULL );
	return true;
}

bool ViewGame::onButtonTargetEnemy(const Message & msg)
{
	NounShip * pShip = m_pDoc->ship();
	if (! pShip )
		return true;
	GameContext * pContext = m_pDoc->context();
	ASSERT( pContext );

	Noun *	bestTarget = NULL;
	float	bestRange = 0.0f;

	for(int j=0;j<pContext->zoneCount();++j)
	{
		NodeZone * pZone = pContext->zone(j);
		for(int i=0;i<pZone->childCount();++i)
		{
			NounShip * pShipContact = WidgetCast<NounShip>( pZone->child(i) );
			if ( !pShip->validateTarget( pShipContact ) )
				continue;
			if ( !pShip->isEnemy( pShipContact ) )
				continue;
			
			float range = (pShip->worldPosition() - pShipContact->worldPosition()).magnitude();
			if ( bestTarget == NULL || range < bestRange )
			{
				bestTarget = pShipContact;
				bestRange = range;
			}
		}
	}

	if ( bestTarget != NULL )
		m_pDoc->setTarget( bestTarget );
	else
		pShip->clientChat( CharString("<color;ffffff>Tactical: No Enemy ships detected.") );
	return true;
}

bool ViewGame::onButtonTargetFriendly(const Message & msg)
{
	NounShip * pShip = m_pDoc->ship();
	if (! pShip )
		return true;
	GameContext * pContext = m_pDoc->context();
	ASSERT( pContext );

	Noun *	bestTarget = NULL;
	float	bestRange = MAX_TARGET_RANGE;

	for(int j=0;j<pContext->zoneCount();++j)
	{
		NodeZone * pZone = pContext->zone(j);
		for(int i=0;i<pZone->childCount();++i)
		{
			NounShip * pShipContact = WidgetCast<NounShip>( pZone->child(i) );
			if (! pShip->validateTarget( pShipContact ) )
				continue;
			if ( pShipContact == pShip )
				continue;		// don't target self
			if ( !pShip->isFriend( pShipContact ) )
				continue;		// don't target enemy
			
			float range = (pShip->worldPosition() - pShipContact->worldPosition()).magnitude();
			if ( range < bestRange )
			{
				bestTarget = pShipContact;
				bestRange = range;
			}
		}
	}

	if ( bestTarget != NULL )
		m_pDoc->setTarget( bestTarget );
	else
		pShip->clientChat( CharString("<color;ffffff>Tactical: No Friendly ships detected.") );
	return true;
}

bool ViewGame::onButtonTargetDistress(const Message & msg)
{
	Noun * pDistress = m_pDoc->validateNoun( VerbDistress::sm_LastDistress );

	if ( pDistress != NULL )
		m_pDoc->setTarget( pDistress );
	else
		m_pDoc->ship()->clientChat( CharString( "<color;ffffff>Comms: No Distress signal received...</color>" ) );

	return true;
}

bool ViewGame::onButtonTargetJumpGate(const Message & msg)
{
	NounShip * pShip = m_pDoc->ship();
	if (! pShip )
		return true;
	GameContext * pContext = m_pDoc->context();
	ASSERT( pContext );

	Noun *	bestTarget = NULL;
	float	bestRange = 0.0f;

	for(int j=0;j<pContext->zoneCount();++j)
	{
		NodeZone * pZone = pContext->zone(j);

		for(int i=0;i<pZone->childCount();i++)
		{
			NounJumpGate * pGate = WidgetCast<NounJumpGate>( pZone->child(i) );
			if ( !pShip->validateTarget( pGate ) )
				continue;
			if ( !pShip->isFriend( pGate ) )
				continue;
		
			float range = (pShip->worldPosition() - pGate->worldPosition()).magnitude();
			if ( bestTarget == NULL || range < bestRange )
			{
				bestTarget = pGate;
				bestRange = range;
			}
		}
	}

	if ( bestTarget != NULL )
		m_pDoc->setTarget( bestTarget );
	else
		pShip->clientChat( CharString( "<color;ffffff>Tactical: Jump Gate not found...</color>" ) );
	return true;
}

bool ViewGame::onButtonTargetSupply(const Message & msg)
{
	NounShip * pShip = m_pDoc->ship();
	if (! pShip )
		return true;
	GameContext * pContext = m_pDoc->context();
	ASSERT( pContext );

	Noun *	bestTarget = NULL;
	float	bestRange = 0.0f;

	for(int j=0;j<pContext->zoneCount();++j)
	{
		NodeZone * pZone = pContext->zone(j);
		for(int i=0;i<pZone->childCount();++i)
		{
			Noun * pContact = WidgetCast<Noun>( pZone->child(i) );
			if ( !pShip->validateTarget( pContact ) )
				continue;
			if ( !pShip->isFriend( pContact ) )
				continue;

			bool valid = false;
			if ( WidgetCast<NounShip>( pContact ) != NULL )
			{
				if ( ((NounShip *)pContact)->type() == NounShip::SUPPLY || 
					((NounShip *)pContact)->type() == NounShip::STATION ||
					((NounShip *)pContact)->type() == NounShip::PLATFORM_SUPPLY )
					valid = true;
			}
			else if ( WidgetCast<NounPlanet>( pContact ) != NULL )
			{
				NounPlanet * pPlanet = (NounPlanet *)pContact;
				if ( (pPlanet->flags() & NounPlanet::FLAG_HAS_DEPOT) == 0 )
					continue;

				StructureDepot * pDepot = NULL;
				for(int j=0;pDepot == NULL && j<pPlanet->childCount();j++)
					pDepot = WidgetCast<StructureDepot>( pPlanet->child(j) );

				if ( pDepot != NULL )
					pContact = pDepot;
				else
					pContact = pPlanet;
				valid = true;
			}

			if ( valid )
			{
				float range = (pShip->worldPosition() - pContact->worldPosition()).magnitude();
				if ( bestTarget == NULL || range < bestRange )
				{
					bestTarget = pContact;
					bestRange = range;
				}
			}
		}
	}

	if ( bestTarget != NULL )
		m_pDoc->setTarget( bestTarget );
	else
		pShip->clientChat( CharString("<color;ffffff>Tactical: Depot/Supply not found...</color>") );
	return true;
}

bool ViewGame::onButtonTargetPort(const Message & msg)
{
	NounShip * pShip = m_pDoc->ship();
	if (! pShip )
		return true;
	GameContext * pContext = m_pDoc->context();
	ASSERT( pContext );

	Noun *	bestTarget = NULL;
	float	bestRange = 0.0f;

	for(int j=0;j<pContext->zoneCount();++j)
	{
		NodeZone * pZone = pContext->zone(j);
		for(int i=0;i<pZone->childCount();++i)
		{
			NounPlanet * pPlanet = WidgetCast<NounPlanet>( pZone->child(i) );
			if ( !pShip->validateTarget( pPlanet ) )
				continue;
			if ( !pShip->isFriend( pPlanet ) )
				continue;
			if ( (pPlanet->flags() & NounPlanet::FLAG_HAS_PORT) == 0 )
				continue;
			
			float range = (pShip->worldPosition() - pPlanet->worldPosition()).magnitude();
			if ( bestTarget == NULL || range < bestRange )
			{
				StructurePort * pPort = NULL;
				for(int j=0;pPort == NULL && j<pPlanet->childCount();j++)
					pPort = WidgetCast<StructurePort>( pPlanet->child(j) );

				if ( pPort != NULL )
					bestTarget = pPort;
				else
					bestTarget = pPlanet;
				bestRange = range;
			}
		}
	}

	if ( bestTarget != NULL )
		m_pDoc->setTarget( bestTarget );
	else
		pShip->clientChat( CharString("<color;ffffff>Tactical: Starport not found...</color>") );
	return true;
}

bool ViewGame::onButtonTargetYard(const Message & msg)
{
	NounShip * pShip = m_pDoc->ship();
	if (! pShip )
		return true;
	GameContext * pContext = m_pDoc->context();
	ASSERT( pContext );

	Noun *	bestTarget = NULL;
	float	bestRange = MAX_TARGET_RANGE;

	for(int j=0;j<pContext->zoneCount();++j)
	{
		NodeZone * pZone = pContext->zone(j);
		for(int i=0;i<pZone->childCount();++i)
		{
			NounPlanet * pPlanet = WidgetCast<NounPlanet>( pZone->child(i) );
			if ( !pShip->validateTarget( pPlanet ) )
				continue;
			if ( !pShip->isFriend( pPlanet ) )
				continue;
			if ( (pPlanet->flags() & NounPlanet::FLAG_HAS_SHIPYARD) == 0 )
				continue;
			
			float range = (pShip->worldPosition() - pPlanet->worldPosition()).magnitude();
			if ( range < bestRange )
			{
				StructureShipyard * pYard = NULL;
				for(int J=0;pYard == NULL && J<pPlanet->childCount();J++)
					pYard = WidgetCast<StructureShipyard>( pPlanet->child(J) );

				if ( pYard != NULL )
					bestTarget = pYard;
				else
					bestTarget = pPlanet;
				bestRange = range;
			}
		}
	}

	if ( bestTarget != NULL )
		m_pDoc->setTarget( bestTarget );
	else
		pShip->clientChat( CharString("<color;ffffff>Tactical: Shipyard not found...</color>") );

	return true;
}

bool ViewGame::onButtonOrderAttack(const Message & msg)
{
	Verb::Ref( new VerbOrderUnit( (NounUnit *)myTarget(), NounUnit::ATTACK, NULL, m_pDoc->ship() ) );
	return true;
}

bool ViewGame::onButtonOrderDefend(const Message & msg)
{
	Verb::Ref( new VerbOrderUnit( (NounUnit *)myTarget(), NounUnit::HOLD, NULL, m_pDoc->ship() ) );
	return true;
}

bool ViewGame::onButtonOrderRaze(const Message & msg)
{
	Verb::Ref( new VerbOrderUnit( (NounUnit *)myTarget(), NounUnit::RAZE, NULL, m_pDoc->ship() ) );
	return true;
}

bool ViewGame::onMessageBoxOK(const Message & msg)
{
	m_pMessageBox->hideWindow();
	
	if ( m_pActiveDialog.valid() )
	{
		m_pActiveDialog->onOK();
		m_pActiveDialog = NULL;
	}
	else
	{
		m_pDoc->context()->scriptCall( m_pMessageBoxOK->script() );
	}

	return true;
}

bool ViewGame::onMessageBoxCANCEL(const Message & msg)
{
	m_pMessageBox->hideWindow();

	if ( m_pActiveDialog.valid() )
	{
		m_pActiveDialog->onCancel();
		m_pActiveDialog = NULL;
	}
	else
	{
		m_pDoc->context()->scriptCall( m_pMessageBoxCANCEL->script() );
	}

	return true;
}

bool ViewGame::onButtonShowEnhancements(const Message & msg)
{
	if ( m_pEnhancementWindow->visible() )
		m_pEnhancementWindow->hideWindow();
	else
		m_pEnhancementWindow->showWindow();
	return true;	

}

bool ViewGame::onButtonEnhUp(const Message & msg)
{
	m_ConfirmRemoveEnhancement = NULL;

	ButtonEnhancement * pConfirm = WidgetCast<ButtonEnhancement>(m_pEnhancementWindow->getSelected() );

	return true; //m_pEnhancementWindow->onButtonUp(msg);
}


bool ViewGame::onButtonRemoveEnhancement(const Message & msg)
{
	ButtonEnhancement * pConfirm = WidgetCast<ButtonEnhancement>(m_pEnhancementWindow->getSelected() );
	if ( pConfirm != NULL )
	{
		if ( pConfirm != m_ConfirmRemoveEnhancement )
		{
			CharString sEnhName = pConfirm->enhancement()->getName();

			// get initial removal cost
			int nRemovalCost =  pConfirm->enhancement()->calculateRemoveCost();
			if ( nRemovalCost > 0 )
			{
				m_pDoc->pushChat( CharString().format( "<color;ffffff>Enhancement: Confirm removal of '%s' for %s credits?</color>", 
					sEnhName, FormatNumber<char,int>( nRemovalCost) ) );
			}
			else
				m_pDoc->pushChat( CharString().format( "<color;ffffff>Enhancement: Confirm removal of '%s'?</color>", sEnhName) );

			m_ConfirmRemoveEnhancement = pConfirm;
		}
		else
		{
			NounEnhancement *pEnh = m_pEnhancementWindow->getSelected()->enhancement();
			if(pEnh)
				Verb::Ref( new VerbRemoveEnhancement( m_pDoc->ship(),pEnh, false ));
			m_ConfirmRemoveEnhancement = NULL;
			m_pEnhancementWindow->setSelected(NULL);
		}
	}
	return true;
}

bool ViewGame::onButtonScrapEnhancement(const Message & msg)
{
	ButtonEnhancement * pConfirm = WidgetCast<ButtonEnhancement>(m_pEnhancementWindow->getSelected() );
	if ( pConfirm != NULL )
	{
		if ( pConfirm != m_ConfirmRemoveEnhancement )
		{
			CharString sEnhName = pConfirm->enhancement()->getName();

			// get initial removal cost
			m_pDoc->pushChat( CharString().format( "<color;ffffff>Enhancement: Confirm scrao of '%s', it will be destroyed?</color>", sEnhName) );
			m_ConfirmRemoveEnhancement = pConfirm;
		}
		else
		{
			NounEnhancement *pEnh = m_pEnhancementWindow->getSelected()->enhancement();
			if(pEnh)
				Verb::Ref( new VerbRemoveEnhancement( m_pDoc->ship(),pEnh, true ));
			m_ConfirmRemoveEnhancement = NULL;
			m_pEnhancementWindow->setSelected(NULL);
		}
	}
	return true;
}

bool ViewGame::onButtonApplyEnhancement(const Message & msg)
{
	CargoEnhancement * pCargo= WidgetCast<CargoEnhancement>( myTarget() );
	if( VerbApplyEnhancement::canApply( pCargo, m_pDoc->ship() ) )
		Verb::Ref( new VerbApplyEnhancement( m_pDoc->ship(), pCargo ) );
	return true;
}

bool ViewGame::onButtonGroupInvite(const Message & msg)
{
	m_pDoc->client()->groupInvite( m_pDoc->target() );
	m_pButtonGroupInvite->hideWindow();
	return true;
}

bool ViewGame::onButtonGroupRemove(const Message & msg)
{
	m_pDoc->client()->groupRemove( m_pDoc->target() );
	m_pButtonGroupRemove->hideWindow();
	return true;
}

bool ViewGame::onButtonGroupPromote(const Message & msg)
{
	m_pDoc->client()->groupPromote( m_pDoc->target() );
	m_pButtonGroupPromote->hideWindow();
	return true;
}

//---------------------------------------------------------------------------------------------------

MessageBoxDialog::MessageBoxDialog( MessageBoxType a_eType, const char * a_pMessage ) : 
	m_eType( a_eType ), 
	m_sMessage( a_pMessage ), 
	m_pDoc( NULL )
{}

void MessageBoxDialog::onDispayed()
{}

void MessageBoxDialog::onOK()
{}

void MessageBoxDialog::onCancel()
{}

//----------------------------------------------------------------------------
// EOF


