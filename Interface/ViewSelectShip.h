/*
	ViewSelectShip.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef VIEWSELECTSHIP_H
#define VIEWSELECTSHIP_H

#include "System/Messages.h"
#include "Render3D/NodeSpin.h"
#include "Gui3d/WindowView.h"
#include "World/NounTemplate.h"
#include "DarkSpace/NounShip.h"
#include "DarkSpace/NounCargo.h"
#include "Interface/ViewGame.h"
#include "WindowNavigation.h"
#include "WindowShipSelection.h"

//----------------------------------------------------------------------------

class ViewSelectShip : public WindowView::View
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef Reference< ViewSelectShip >	Ref;

	// Construction
						ViewSelectShip();
	// View interface
	void				onActivate();
	void				onDeactivate();
	void				onUpdate( float t );
	bool				onMessage( const Message & msg );
	void				onDocumentUpdate();

	void				onRender( RenderContext & context, 
							const RectInt & window );

	//! Mutators
	void				pushMessageBox(MessageBoxDialog * pDialog);
	void				popMessageBox(MessageBoxDialog * pDialog);

	// {{BEGIN_DATA
	WindowButton *				m_pButtonBuyCargo;
	WindowText *				m_pItemDescription;
	WindowText *				m_pCreditsStatus;
	WindowList *				m_pItemList;
	NodeWindow *				m_pBuyWindow;
	WindowText *				m_pTextLabel;
	WindowNavigation *			m_pNavigationWindow;
	WindowText *				m_pTextShips;
	WindowText *				m_pTextCargo;
	WindowButton *				m_pButtonRemoveCargo;
	NodeWindow *				m_pCargoButtons;
	WindowButton *				m_pButtonRemoveShip;
	NodeWindow *				m_pSlotButtons;
	NodeWindow *				m_pLeftConsole;
	WindowButton *				m_pButtonUnknown;
	WindowButton *				m_pButtonStation;
	WindowButton *				m_pButtonSupply;
	WindowButton *				m_pButtonEngineer;
	WindowButton *				m_pButtonTransport;
	WindowButton *				m_pButtonDread;
	WindowButton *				m_pButtonCruiser;
	WindowButton *				m_pButtonDestroyer;
	WindowButton *				m_pButtonFrigate;
	WindowButton *				m_pButtonScout;
	NodeWindow *				m_pShipButtons;
	NodeWindow *				m_pIncomingMessages;
	WindowEdit *				m_pChatEdit;
	NodeWindow *				m_pChatEditWindow;
	NodeWindow *				m_pShipDescriptionFrame;
	WindowShipSelection *		m_pShipRender;
	WindowButton *				m_pButtonOkay;
	WindowText *				m_pShipDescription;
	WindowButton *				m_pMessageBoxCANCEL;
	WindowButton *				m_pMessageBoxOK;
	WindowText *				m_pMessageBox;
	WindowEdit *				m_pFilterEdit;
	WindowButton *				m_pButtonTier3;
	WindowButton *				m_pButtonTier2;
	WindowButton *				m_pButtonTier1;	
	// END_DATA}}

	// {{BEGIN_MSG
	bool				onButtonCancelBuy(const Message & msg);
	bool				onButtonBuyItem(const Message & msg);
	bool				onButtonBuyCargo(const Message & msg);
	bool				onItemList(const Message & msg);
	bool				onButtonRemoveCargo(const Message & msg);
	bool				onButtonRemoveShip(const Message & msg);
	bool				onButtonUnknown(const Message & msg);
	bool				onButtonStation(const Message & msg);
	bool				onButtonSupply(const Message & msg);
	bool				onButtonEngineer(const Message & msg);
	bool				onButtonTransport(const Message & msg);
	bool				onButtonDread(const Message & msg);
	bool				onButtonCruiser(const Message & msg);
	bool				onButtonDestroyer(const Message & msg);
	bool				onButtonFrigate(const Message & msg);
	bool				onButtonScout(const Message & msg);
	bool				onChatEdit(const Message & msg);
	bool				onCancel(const Message & msg);
	bool				onOkay(const Message & msg);
	bool				onMessageBoxCANCEL(const Message & msg);
	bool				onMessageBoxOK(const Message & msg);
	bool				onFilterEdit(const Message & msg);
	bool				onButtonTier3(const Message & msg);
	bool				onButtonTier2(const Message & msg);
	bool				onButtonTier1(const Message & msg);	
	// END_MSG}}

	void				onRemoveShipDialogYes(NounShip * pSelectedShip);

private:
	// Types
	typedef std::list< MessageBoxDialog::Ref >		DialogList;
	class ShipButton : public WindowButton
	{
	public:
		// Construction
		ShipButton( NodeWindow * pParent, ViewSelectShip * pView, NounShip * pShip, Noun * pSpawn, 
			bool bAvailable, bool bCanBuild );
		// NodeWindow interface
		void				onRender( RenderContext & context,
								const RectInt & window );		// client render, window is in screen coordinates
		// WindowButton interface
		void				onButtonUp();
		Color				labelColor() const;
		// Mutators
		void				setButton( ViewSelectShip * pView, NounShip * pShip, Noun * pSpawn, 
								bool bAvailable, bool bCanBuild );

	private:
		ViewSelectShip *	m_pView;
		NounShip::Ref		m_pShip;
		Noun::Ref			m_pSpawn;
		bool				m_bAvailable;
		bool				m_bCanBuild;
	};
	class CargoButton : public WindowButton
	{
	public:
		// Construction
							CargoButton( NodeWindow * pParent, ViewSelectShip * pView, NounCargo * pCargo, bool available );
		// NodeWindow interface
		void				onUpdate( float t );
		void				onRender( RenderContext & context,
								const RectInt & window );		// client render, window is in screen coordinates
		// WindowButton interface
		void				onButtonUp();
		// Mutators
		void				setButton( ViewSelectShip * pView, NounCargo * pCargo, bool available  );

	private:
		ViewSelectShip *	m_pView;
		NounCargo::Ref		m_pCargo;
		Material::Link		m_Icon;
	};

	
	friend class ShipButton;
	friend class CargoButton;

	// Mutators
	void				onSelectShip( NounShip * pShip, Noun * pSpawn );			// called by ShipButton()
	void				onSelectCargo( NounCargo * pCargo );

	WindowButton *		getFilterButton( NounShip::Type type );
	bool				updateShipList();

	// Data
	bool				m_bShipSelected;
	NounShip::Type		m_FilterType;

	Noun::wRef			m_pLastSpawn;
	Noun::wRef			m_pSelectedSpawn;
	int					m_nRank;
	NounShip::Ref		m_pSelectedShip;
	Array< Noun::Ref >
						m_SelectedCargo;
	float				m_UpdateShipList;

	Hash< WidgetKey, NounTemplate::Ref >
						m_ShipContextHash;
	bool				m_bRemoveShip;
	bool				m_bRemoveCargo;
	DialogList			m_DialogList;		// list of active message boxes
	MessageBoxDialog::Ref
						m_pActiveDialog;	// our currently active dialog
	CharString			m_sFilter;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
