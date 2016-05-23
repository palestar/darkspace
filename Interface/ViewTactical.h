/*
	ViewTactical.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef VIEWTACTICAL_H
#define VIEWTACTICAL_H

#include "World/Noun.h"
#include "Display/PrimitiveLineList.h"
#include "Gui3d/WindowView.h"
#include "System/Messages.h"
#include "DarkSpace/NounPlanet.h"
#include "DarkSpace/NounDropTarget.h"
#include "World/NounZoneTarget.h"
#include "Interface/GameDocument.h"
#include "Interface/SliderHUD.h"
#include "Interface/ViewGame.h"
#include "Gui3d/WindowLayout.h"

class ViewGame;

//----------------------------------------------------------------------------

class ViewTactical : public WindowView::View
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef Reference< ViewTactical >	Ref;

	// Construction
	ViewTactical();

	// View interface
	void				onActivate();
	void				onDeactivate();
	void				onUpdate( float t );
	bool				onMessage( const Message & msg );
	void				onDocumentUpdate();

	void				onHide();
	void				onRender( RenderContext & context, 
							const RectInt & window );
	bool				onCursorMove( const RectInt & window, 
							const PointInt & position,		// window and position are local coordinates
							const PointInt & delta );
	bool				onLeftDown( const RectInt & window, 
							const PointInt & position );
	bool				onLeftUp( const RectInt & window, 
							const PointInt & position );
	bool				onRightDown( const RectInt & window,
							const PointInt & position );
	bool				onRightUp( const RectInt & window,
							const PointInt & position );

	// {{BEGIN_DATA
	WindowButton *		m_pDetectedStatus;
	WindowButton *		m_pButtonPointDefense;
	WindowButton *		m_pButtonShields;
	WindowText *		m_pHeadingText;
	SliderHUD *			m_pHeadingBar;
	NodeWindow *		m_pCargoButtons;
	NodeWindow *		m_pGadgetLayout;
	WindowText *		m_pVelocityText;
	SliderHUD *			m_pVelocityBar;
	// END_DATA}}


	// {{BEGIN_MSG
	bool				onButtonSelfDestruct(const Message & msg);
	bool				onButtonSendSupply(const Message & msg);
	bool				onButtonSendDistress(const Message & msg);
	bool				onButtonCameraNext(const Message & msg);
	bool				onButtonShields(const Message & msg);
	bool				onButtonPointDefense(const Message & msg);
	bool				onHeadingBar(const Message & msg);
	bool				onVelocityBar(const Message & msg);
	// END_MSG}}

	// static
	static Matrix33		sm_CameraFrame;		// destination camera frame
	static Vector3		sm_CameraPosition;
	static Vector3		sm_AdjustPosition;

	// Helpers
	static void			renderRecticle( DisplayDevice * pDevice, const PointInt & center, int hsize, int vsize, Color diffuse );
	static void			renderLeftArrow( DisplayDevice * pDevice, const PointInt & center, int hsize, int vsize, Color diffuse );
	static void			renderRightArrow( DisplayDevice * pDevice, const PointInt & center, int hsize, int vsize, Color diffuse );
	static void			renderUpArrow( DisplayDevice * pDevice, const PointInt & center, int hsize, int vsize, Color diffuse );
	static void			renderDownArrow( DisplayDevice * pDevice, const PointInt & center, int hsize, int vsize, Color diffuse );

private:
	// Types
	enum CameraMode
	{
		FREE,
		TARGET,
		TACTICAL,	
		FOLLOW,

		FIRST_CAMERA_MODE = FREE,
		LAST_CAMERA_MODE = FOLLOW
	};
	typedef std::list< NounDropTarget::Ref >		DropList;

	// Data
	GameDocument *		m_pDoc;				// our client object
	dword				m_UseGadgetTick;	// last tick on which a verb was issued

	bool				m_bLeftDown;		// left mouse is down
	float				m_fLeftDownTime;	// how long has the left button been down
	bool				m_bRightDown;		// right mouse button is down
	float				m_fRightDownTime;	// how long has the right button been down
	PointInt			m_CursorPosition2;	// current cursor position
	Vector3				m_CursorPosition;	// 3d position of the cursor

	float				m_CameraYaw;	
	float				m_CameraPitch;
	
	CameraMode			m_CameraMode;		// current camera mode
	CameraMode			m_LastCameraMode;
	Noun::wRef			m_CameraLock;		// camera is locked onto which object
	float				m_CameraLockTime;
	float				m_CameraZoom;
	int					m_CameraZoomLevel;	// current zoom level

	float				m_TargetTime;		// how long has this been the current target
	float				m_UpdateChatter;

	float				m_ScannerRange;		// current contact range
	float				m_fUpdateContacts;	// update contacts time

	bool				m_bShowContacts;		// show contacts
	Array< Noun::wRef >	
						m_Contacts;			// current contacts
	SortedArray< WidgetKey >
						m_OldContacts;		// sorted list of previous contacts

	NounPlanet::wRef	m_pPlanetTarget;
	DropList			m_DropTargets;

	NounZoneTarget::Ref	m_pManualTarget;

	Noun::wRef			m_Orbiting;			// which object are we currently oribiting

	bool				m_BeginControl;		
	bool				m_UpdateControl;	// ship manual controls
	float				m_fYaw;
	float				m_fYawV;
	float				m_SetHeading;
	float				m_SetHeadingV;
	float				m_SetVelocity;		// desired ship velocity
	float				m_SetVelocityV;		// change in set velocity

	bool				m_Adjust;
	
	bool				m_bYawControl;		// control ship using yaw if true, otherwise control by set heading
	bool				m_bShowShipStatus;	// show shield/armor status
	bool				m_bShowFriendlyStatus;	// show allied shield/armor status
	bool				m_bShowTurnStatus;	// show turning pie indicator

	bool				m_SelfDestruct;
	bool				m_SelfDestructConfirm;
	MessageLine::Ref	m_pSelfDestructMessage;
	dword				m_SelfDestructTime;

	int					m_ArmorRingBlink;	//what armor ring we are on to blink
	bool				m_bRingBlinked;		//did we blink already

	float				m_fUpdateButtons;

	dword				m_nDistressTick;
	
	// Mutators
	void				setCameraMode( CameraMode mode, bool pushMessage = true );
	void				nextCameraMode();

	void				renderShipStatus( RenderContext & context, NounShip * pShip );
	void				renderTurnStatus( RenderContext & context, NounShip * pShip );
	void				renderGadgetTarget( RenderContext & context, NounShip * pShip, NounGadget * pGadget );
	void				renderWeaponArc( RenderContext & context, NounShip * pShip, Facing nFace, float fRange );
	void				updateContacts();
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF