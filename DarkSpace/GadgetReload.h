/*
	GadgetReload.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef GADGET_RELOAD_H
#define GADGET_RELOAD_H

#include "NounGadget.h"
#include "NounShip.h"
#include "NounDrone.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL GadgetReload : public NounGadget, public NounDrone::Action
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	GadgetReload();

	// BaseNode interface
	void					render( RenderContext &context, 
								const Matrix33 & frame, 
								const Vector3 & position );
	// Noun interface
	void					setup();
	void					release();
	void					simulate( dword nTick );	

	// NounGadget interface
	Type					type() const;					// get gadget type
	dword					hotkey() const;					// gadget hotkey
	CharString				status() const;

	bool					usable( Noun * pTarget, 
								bool shift ) const;			// can gadget be used on target
	bool					useActive() const;				// is the gadget current on or off
	bool					needReload( Noun * pWhom ) const;

	bool					reload( Noun * pWhom );
	void					use( dword when, Noun * pTarget,
								bool shift);				// use gadget
	bool					updateLogic();

	// NounDrone::Action interface
	void					droneUpdate( NounDrone * pDrone );
	void					droneAction( NounDrone * pDrone );	
	void					droneHome( NounDrone * pDrone );

	// Accessors
	NounShip *				reloading() const;				// who are we reloading
	int						reloads() const;				// reloads available

	// Interface
	virtual int				maxReloads() const = 0;			// maximum number of reloads
	virtual int				reloadRate() const = 0;			// rate this device is reloaded
	virtual int				repairRate() const = 0;			// repair rate
	virtual float			reloadRange() const = 0;		// range in gu device can repair from

	// Helpers
	static GadgetReload *	getGadgetReload( NounShip * pShip, bool bUsable = false );

protected:
	// Data
	NounShip::wRef			m_Reloading;					// who are we reloading
	int						m_Reloads;						// how many reloads do we have

	// non-serilized
	dword					m_nReloadTick;
	int						m_DroneCount;
};

//----------------------------------------------------------------------------

inline NounShip * GadgetReload::reloading() const
{
	return m_Reloading;
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
