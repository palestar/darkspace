/*
	GadgetAutoRepair.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef GADGETAUTOREPAIR_H
#define GADGETAUTOREPAIR_H

#include "Constants.h"
#include "NounGadget.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL GadgetAutoRepair : public NounGadget
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	enum {
		FLAG_SHIP_DAMAGE	= 0x2,	
		FLAG_GADGET_DAMAGE	= 0x4,
	};
	// Construction
	GadgetAutoRepair();

	// Noun interface
	void					setup();
	void					simulate( dword nTick );

	// NounGadget interface
	Type					type() const;					// get gadget type
	CharString				status() const;					// get gadget status

	bool					canDamage( dword type ) const;	// can this device be damaged
	bool					needReload( Noun * pWhom ) const;

	bool					reload( Noun * pWhom );			// reload gadget
	int						useEnergy( dword nTick, 
								int energy );
	// Accessors
	int						supply() const;					// amount of materials for the auto-repair remaining, if supply <= 0 device does not repair

	virtual int				maxSupply() const = 0;			// maximum amount of supply the device has
	virtual int				reloadRate() const = 0;			// rate at which supply is reloaded
	virtual float			hullRepairTime() const = 0;		// repair time for hull
	virtual float			gadgetRepairTime() const = 0;	// repair time for gadgets
	virtual int				energyCost() const = 0;			// cost in energy per tick when device is repairing hull or devices

	// Mutators
	void					setSupply( int nSupply );		// set the repair supply

protected:
	// Data
	int						m_nSupply;
	dword					m_nRepairTick;
};

//----------------------------------------------------------------------------

inline int GadgetAutoRepair::supply() const
{
	return m_nSupply;
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
