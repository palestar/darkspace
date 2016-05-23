/*
	ViewGame.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef VIEWGAME_H
#define VIEWGAME_H

#include "System/Messages.h"
#include "Gui3d/WindowView.h"
#include "Gui3d/ButtonPopup.h"
#include "Gui3d/WindowLayout.h"
#include "DarkSpace/NounUnit.h"
#include "Interface/BarHUD.h"
#include "Standard/Settings.h"
#include "Interface/GameDocument.h"
#include "Interface/WindowEnhancement.h"
#include "Interface/InterfaceDll.h"

//---------------------------------------------------------------------------------------------------

enum MessageBoxType {
	MBT_OKCANCEL,			// ok & cancel buttons are displayed.
	MBT_OK,					// only the ok button is displayed.
	MBT_YESNO,				// yes and no buttons are displayed instead of ok & cancel.
};

//! Interface for displaying a message box in the game, inherit from this class and push your object
//! into the message box queue, all message boxes will be displayed in the order they are pushed.
class DLL MessageBoxDialog : public Referenced
{
public:
	//! Types
	typedef Reference< MessageBoxDialog >		Ref;
	
	//! Construction
	MessageBoxDialog( MessageBoxType a_eType, const char * a_pMessage );

	//! Interface
	virtual void	onDispayed();			// invoked when the message box is displayed.
	virtual void	onOK();					// invoked if the user clicks the ok/yes button
	virtual void	onCancel();				// invoked if the user clicks the cancel/no button

	MessageBoxType	m_eType;
	CharString		m_sMessage;
	GameDocument *	m_pDoc;					// our document object
};

//! Interface for displaying a message line in the game.. a message line is a single line of text
//! displayed as near to the center of the screen as possible without overlapping other message lines.
class DLL MessageLine : public Referenced
{
public:
	//! Types
	typedef Reference< MessageLine >			Ref;
	typedef WeakReference< MessageLine >		WeakRef;

	//! Construction
	MessageLine( const char * a_pText = "", Color a_cColor = WHITE, bool a_bLarge = false );

	CharString		m_sText;
	Color			m_cColor;
	bool			m_bLarge;
};

//---------------------------------------------------------------------------------------------------

class DLL ViewGame : public WindowView::View
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef Reference< ViewGame >	Ref;

	enum Mode
	{
		TACTICAL,			// tactical mode
		NAVIGATION,			// navigation mode
		PLANET,				// planet mode
	};
	enum ChatMode
	{
		FACTION,
		FLEET,
		YELL,
		TARGET,
	};
	// context actions that occur when a player right clicks on something in the world..
	enum ContextAction {		
		CA_NONE,
		CA_ATTACK,					// attack cursor target
		CA_BOMB,					// bomb cursor target
		CA_ORBIT,					// orbit target
		CA_FOLLOW,					// follow target
		CA_SHADOW,					// shadow target
		CA_LOAD,					// load target
		CA_BUILD,					// build target
		CA_RELOAD,					// reload target

		CA_ORDER_ATTACK,			// ship orders
		CA_ORDER_CAPTURE,
		CA_ORDER_DEFEND,
		CA_ORDER_RECON,
		CA_ORDER_BUILD,
		CA_ORDER_RELOAD,
		CA_ORDER_MOVE,

		CA_ORDER_UNIT_ATTACK,		// unit orders
		CA_ORDER_UNIT_RAZE,				
		CA_ORDER_UNIT_MOVE
	};

	//! Data
	static ViewGame	*	sm_pViewGame;						// static pointer to this object, NULL if not active
	static bool			sm_bDisableTacticalActions;

	// Construction
	ViewGame();

	// View interface
	void				onActivate();
	void				onDeactivate();
	void				onUpdate( float t );
	bool				onMessage( const Message & msg );
	void				onDocumentUpdate();

	void				onRender( RenderContext & context, 
							const RectInt & window );

	//! Accessors
	ContextAction		contextAction() const;
	const CharString &	contextActionText() const;
	Noun *				orderTarget() const;
	Noun *				contextTarget() const;
	bool				redAlert() const;

	//! Mutators
	void				pushMessageBox( MessageBoxDialog * pDialog );
	void				popMessageBox( MessageBoxDialog * pDialog );
	void				pushMessageLine( MessageLine * pLine );
	void				popMessageLine( MessageLine * pLine );

	//! Render this view, this isn't done in onRender() because that is called before sub-views onRender() is called.
	bool				renderViewGame( RenderContext & context, 
							const RectInt & w,
							bool a_bRedAlertBorder = true );
	// update the current context action, should be called only if cursor is over
	bool				updateAction( bool bOrderMode );				
	// execute the current action, usually called by ViewTactical/ViewNavigation when right button is released.
	bool				doAction( bool bOrderMode );					

	// {{BEGIN_DATA
	WindowButton *				m_pButtonGroupPromote;
	WindowButton *				m_pButtonGroupRemove;
	WindowButton *				m_pButtonGroupInvite;
	WindowText *				m_pHintText;
	WindowEnhancement *			m_pEnhancementWindow;
	WindowButton *				m_pMessageBoxCANCEL;
	WindowButton *				m_pMessageBoxOK;
	WindowText *				m_pMessageBox;
	WindowButton *				m_pButtonCancelOrder;
	ButtonPopup *				m_pPopupTargetButtons;
	WindowButton *				m_pButtonRightConsole;
	NodeWindow *				m_pTargetFrame;
	NodeWindow *				m_pLayoutTargetButtons;
	WindowButton *				m_pButtonShadow;
	WindowButton *				m_pButtonScoutMission;
	WindowButton *				m_pButtonDefendMission;
	WindowButton *				m_pButtonCaptureMission;
	WindowButton *				m_pButtonAttackMission;
	WindowButton *				m_pButtonTarget;
	WindowButton *				m_pButtonYell;
	WindowButton *				m_pButtonFaction;
	WindowButton *				m_pButtonFleet;
	WindowButton *				m_pButtonNextTarget;
	WindowButton *				m_pButtonUpTarget;
	WindowButton *				m_pButtonOrderRaze;
	WindowButton *				m_pButtonOrderDefend;
	WindowButton *				m_pButtonOrderAttack;
	WindowButton *				m_pButtonNext;
	WindowButton *				m_pButtonPrev;
	WindowButton *				m_pButtonRepairNow;
	WindowButton *				m_pButtonRepairLast;
	WindowButton *				m_pButtonAutoDefense;
	WindowButton *				m_pButtonUseStructure;
	WindowButton *				m_pButtonApplyEnhancement;
	WindowText *				m_pTextConfirm;
	NodeWindow *				m_pWindowConfirm;
	WindowButton *				m_pButtonSendChat;
	WindowEdit *				m_pEditTransferCredits;
	NodeWindow *				m_pWindowTransferCredits;
	WindowButton *				m_pButtonTransferCredits;
	WindowButton *				m_pButtonPlanet;
	WindowButton *				m_pButtonUnlock;
	WindowButton *				m_pButtonLock;
	NodeWindow *				m_pViewPlanet;
	NodeWindow *				m_pTargetButtons2;
	NodeWindow *				m_pViewTrade;
	WindowButton *				m_pButtonJettison;
	WindowButton *				m_pButtonCamera;
	WindowButton *				m_pButtonContinue;
	WindowButton *				m_pButtonPause;
	WindowText *				m_pSignatureText;
	BarHUD *					m_pSignatureBar;
	NodeWindow *				m_pRightConsole;
	NodeWindow *				m_pLeftConsole;
	WindowEdit *				m_pChatEdit;
	NodeWindow *				m_pHelpWindow;
	WindowButton *				m_pButtonDisband;
	WindowButton *				m_pButtonScrap;
	WindowButton *				m_pButtonLoad;
	WindowButton *				m_pButtonOrbit;
	WindowButton *				m_pButtonFollow;
	WindowText *				m_pTargetDescription;
	WindowButton *				m_pButtonNavigation;
	WindowButton *				m_pButtonHelp;
	NodeWindow *				m_pIncomingMessages;
	WindowText *				m_pShipStatusText;
	WindowText *				m_pEnergyText;
	BarHUD *					m_pEnergyBar;
	WindowText *				m_pHullText;
	BarHUD *					m_pHullBar;
	NodeWindow *				m_pViewNavigation;
	NodeWindow *				m_pViewTactical;
	WindowButton *				m_pButtonPauseImport;
	WindowButton *				m_pButtonPauseExport;
	WindowButton *				m_pButtonContinueImport;
	WindowButton *				m_pButtonContinueExport;
	// END_DATA}}

	// {{BEGIN_MSG
	bool				onButtonExport(const Message & msg);
	bool				onButtonImport(const Message & msg);
	bool				onButtonScrapEnhancement(const Message & msg);
	bool				onButtonGroupPromote(const Message & msg);
	bool				onButtonGroupRemove(const Message & msg);
	bool				onButtonGroupInvite(const Message & msg);
	bool				onButtonRemoveEnhancement(const Message & msg);
	bool				onButtonEnhUp(const Message & msg);
	bool				onButtonShowEnhancements(const Message & msg);
	bool				onButtonApplyEnhancement(const Message & msg);
	bool				onMessageBoxCANCEL(const Message & msg);
	bool				onMessageBoxOK(const Message & msg);
	bool				onButtonAbortOrder(const Message & msg);
	bool				onButtonCancelOrder(const Message & msg);
	bool				onButtonOrderRaze(const Message & msg);
	bool				onButtonOrderDefend(const Message & msg);
	bool				onButtonOrderAttack(const Message & msg);
	bool				onButtonTargetYard(const Message & msg);
	bool				onButtonTargetPort(const Message & msg);
	bool				onButtonTargetSupply(const Message & msg);
	bool				onButtonTargetJumpGate(const Message & msg);
	bool				onButtonTargetDistress(const Message & msg);
	bool				onButtonTargetFriendly(const Message & msg);
	bool				onButtonTargetEnemy(const Message & msg);
	bool				onButtonShadow(const Message & msg);
	bool				onButtonScoutMission(const Message & msg);
	bool				onButtonDefendMission(const Message & msg);
	bool				onButtonCaptureMission(const Message & msg);
	bool				onButtonAttackMission(const Message & msg);
	bool				onButtonTarget(const Message & msg);
	bool				onButtonYell(const Message & msg);
	bool				onButtonFaction(const Message & msg);
	bool				onButtonFleet(const Message & msg);
	bool				onButtonNextTarget(const Message & msg);
	bool				onButtonUpTarget(const Message & msg);
	bool				onButtonNext(const Message & msg);
	bool				onButtonPrev(const Message & msg);
	bool				onButtonRepairNow(const Message & msg);
	bool				onButtonRepairLast(const Message & msg);
	bool				onButtonAutoDefense(const Message & msg);
	bool				onButtonUseStructure(const Message & msg);
	bool				onButtonConfirmCancel(const Message & msg);
	bool				onButtonConfirmOkay(const Message & msg);
	bool				onEditTransferCredits(const Message & msg);
	bool				onButtonTransferCredits(const Message & msg);
	bool				onButtonPlanet(const Message & msg);
	bool				onButtonUnlock(const Message & msg);
	bool				onButtonLock(const Message & msg);
	bool				onButtonJettison(const Message & msg);
	bool				onButtonCamera(const Message & msg);
	bool				onButtonContinue(const Message & msg);
	bool				onButtonPause(const Message & msg);
	bool				onChatEdit(const Message & msg);
	bool				onButtonDisband(const Message & msg);
	bool				onButtonScrap(const Message & msg);
	bool				onButtonLoad(const Message & msg);
	bool				onButtonOrbit(const Message & msg);
	bool				onButtonFollow(const Message & msg);
	bool				onButtonNavigation(const Message & msg);
	bool				onButtonHelp(const Message & msg);
	// END_MSG}}

private:
	//! Types
	typedef std::list< MessageBoxDialog::Ref >		DialogList;
	typedef std::list< MessageLine::WeakRef >		MessageList;

	// Accessors
	Noun *				myTarget() const;			// resolves NounDropTarget/NounBeacon targets to the correct object
	NounGadget *		myGadgetTarget() const;		

	// Mutators
	void				changeMode( Mode mode );

	// Static
	static void			hideAllButtons( BaseNode * pNode );
	static bool			isValidGroup( NounShip * pShip, dword group );
	
	// Data
	GameDocument *		m_pDoc;				// our client object
	Mode				m_Mode;				// our current mode
	ChatMode			m_ChatMode;			// current chat mode
	float				m_CollisionCheck;

	bool				m_CameraFromNavigation;

	Noun::wRef			m_LastTarget;

	Settings *			m_pSettings;

	DialogList			m_DialogList;		// list of active message boxes
	MessageBoxDialog::Ref
						m_pActiveDialog;	// our currently active dialog
	MessageList			m_MessageList;		// active message line objects

	NounStructure::wRef	m_ConfirmScrap;
	NounUnit::wRef		m_ConfirmDisband;
	Noun::wRef			m_ConfirmJettison;
	NounStructure::wRef	m_ConfirmUseStructure;

	ButtonEnhancement::wRef
						m_ConfirmRemoveEnhancement;

	ContextAction		m_eContextAction;
	CharString			m_sContextAction;
	Noun::WeakRef		m_wpOrderTarget;
	Noun::WeakRef		m_wpContextTarget;

	bool				m_bRedAlert;		// true if an enemy ship is near
	MessageLine::Ref	m_pJumpingMessage;
	MessageLine::Ref	m_pJumpDisabledMessage;
	MessageLine::Ref	m_pCaptureMessage;
	MessageLine::Ref	m_pDespawnMessage;
	MessageLine::Ref	m_pOutOfCombatMessage;
	MessageLine::Ref	m_pRankLockMessage;
	dword				m_nRankTimeout;		// rank lock message timeout

	// toggling Enhancements
	int					m_CurrentEnhancement;
	float				m_nRotatingEnhancementTick;

};

//---------------------------------------------------------------------------------------------------

inline ViewGame::ContextAction ViewGame::contextAction() const
{
	return m_eContextAction;
}

inline const CharString & ViewGame::contextActionText() const
{
	return m_sContextAction;
}

inline Noun * ViewGame::orderTarget() const
{
	return m_wpOrderTarget;
}

inline Noun * ViewGame::contextTarget() const
{
	return m_wpContextTarget;
}

inline bool ViewGame::redAlert() const
{
	return m_bRedAlert;
}

//----------------------------------------------------------------------------


inline MessageLine::MessageLine( const char * a_pText /*= ""*/, Color a_cColor /*= WHITE*/, bool a_bLarge /*= false*/ ) 
		: m_sText( a_pText ), m_cColor( a_cColor ), m_bLarge( a_bLarge )
{
	ViewGame::sm_pViewGame->pushMessageLine( this );
}

//---------------------------------------------------------------------------------------------------

#endif

//----------------------------------------------------------------------------
// EOF
