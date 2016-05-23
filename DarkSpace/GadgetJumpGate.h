/*
	GadgetJumpGate.h

	This ship devices creates a pair of linked NounJumpGate objects
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef GADGETJUMPGATE_H
#define GADGETJUMPGATE_H

#include "Constants.h"
#include "NounGadget.h"
#include "GameDll.h"



//----------------------------------------------------------------------------

class DLL GadgetJumpGate : public NounGadget
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	GadgetJumpGate();

	// Noun interface
	void			setup();
	void			initialize();

	// NounGadget interface
	Type			type() const;					// get gadget type
	dword			hotkey() const;					// gadget hotkey
	virtual CharString		useTip( Noun * pTarget,
								bool shift ) const;

	int				usableWhen() const;
	bool			usable( Noun * pTarget, 
						bool shift ) const;			// can gadget be used on target
	void			use( dword when, Noun * pTarget,
						bool shift);				// use gadget
	int				useEnergy( dword nTick, 
						int energy );				// use energy

	// Accessors
	virtual int		energyNeeded() const = 0;
	virtual int		chargeRate() const = 0;
	virtual float	range() const = 0;				// maximum distance between gates
	virtual float	minRange() const = 0;			// minimum distance to open gates
	virtual dword	duration() const = 0;			// how long are the gates open after created
	virtual float	variation() const = 0;			// how much variation can occur in the destination position

	// Helpers
	NounContext *	gateContext() const;			// NounContext for the NounJumpGate objects

private:
	int				m_nEnergy;
	dword			m_nEnergyTick;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
