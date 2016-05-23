/*
	GadgetPulseShield.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef GADGETPULSESHIELD_H
#define GADGETPULSESHIELD_H

#include "Constants.h"
#include "NounGadget.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL GadgetPulseShield : public NounGadget
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	GadgetPulseShield();

	// Noun interface
	void			setup();
	// NounGadget interface
	Type			type() const;					// get gadget type
	dword			hotkey() const;					// gadget hotkey

	int				usableWhen() const;
	virtual CharString		useTip( Noun * pTarget,
								bool shift ) const;				
	bool			usable( Noun * pTarget, 
						bool shift ) const;			// can gadget be used on target
	void			use( dword when, Noun * pTarget,
						bool shift);				// use gadget
	int				useEnergy( dword nTick, 
						int energy );				// use energy

	// Accessors
	virtual int		energyNeeded() const = 0;
	virtual int		chargeRate() const = 0;
	virtual float	range() const = 0;
	virtual int		chance() const = 0;

private:
	int				m_Energy;
};

//----------------------------------------------------------------------------

#endif

//----------------------------------------------------------------------------
//EOF
