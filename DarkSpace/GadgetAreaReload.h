/**
	@file GadgetAreaReload.h
	@brief Area based reload/repair device... repairs/reloads all ships within the area of this device
	while active.

	(c)2009 Palestar Inc
	@author Richard Lyle @date 6/13/2011 9:32:01 PM
*/

#ifndef GADGETAREARELOAD_H
#define GADGETAREARELOAD_H

#include "World/CollisionHashAbstract.h"

#include "NounGadget.h"
#include "NounShip.h"
#include "NounDrone.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL GadgetAreaReload : public NounGadget, public NounDrone::Action
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef CollisionHashAbstract::NounCollision	NounCollision;

	// Construction
	GadgetAreaReload();
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
	int						reloads() const;				// reloads available

	// Interface
	virtual int				maxReloads() const = 0;			// maximum number of reloads
	virtual int				reloadRate() const = 0;			// rate this device is reloaded
	virtual int				gadgetRepairRate() const = 0;	// gadget repair rate
	virtual int				hullRepairRate() const = 0;		// hull repair rate
	virtual float			reloadRange() const = 0;		// range in gu device can repair from

	// Helpers
	static GadgetAreaReload *	
							getGadgetReload( NounShip * pShip, bool bUsable = false );

protected:
	// Data
	int						m_Reloads;						// how many reloads do we have
	// non-serialized
	Array< NounCollision >	m_Reloading;					// who are we reloading
	int						m_nNextReloadIndex;				// who are we reloading next update
	dword					m_nReloadTick;
	int						m_DroneCount;
};

//----------------------------------------------------------------------------

inline int GadgetAreaReload::reloads() const
{
	return m_Reloads;
}

#endif

//---------------------------------------------------------------------------------------------------
//EOF
