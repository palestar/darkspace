/*
	GadgetCloak.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef GADGET_CLOAK_H
#define GADGET_CLOAK_H

#include "Constants.h"
#include "NounGadget.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL GadgetCloak : public NounGadget
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	GadgetCloak();

	// Noun interface
	void					setup();
	void					initialize();

	// NounGadget interface
	Type					type() const;				// get gadget type
	dword					hotkey() const;				// gadget hotkey

	float					addSignature() const;			// this device affects the signature of the ship
	virtual CharString		useTip( Noun * pTarget,
								bool shift ) const;	
	float					addVisibility() const;		

	bool					usable( Noun * pTarget, 
								bool shift ) const;	// can gadget be used on target
	int						usableWhen() const;
	bool					useActive() const;			// is the gadget current on or off
	void					use( dword when, Noun * pTarget,
								bool shift);		// use gadget
	int						useEnergy( dword nTick, 
								int energy );		// use energy
	bool					updateLogic();

	// Accessors
	virtual float			energyCost() const = 0;			// energy cost per point of signature to cloak as a % of parents maxEnergy()
	virtual int				cooldown() const = 0;			// how many ticks for the device to cooldown
	bool					active() const;

protected:
	// Data
	float					m_fCloak;				// how much signature is being reduced
	float					m_fSubVisibility;		// how much is the visiblity being reduced
	// non-serialized
	dword					m_nEnergyTick;
	bool					m_bCloakTimerState;		// true if set for cloaking, false for uncloaking..
	float					m_fCloakTime;			// cloak time in seconds
	float					m_fTimeElapsed;			// how much of the cloaking time has elapsed
	float					m_fCloakedTime;			// how long this ship has been cloaked for
};

//----------------------------------------------------------------------------

inline float GadgetCloak::addSignature() const
{
	return -m_fCloak;
}

inline float GadgetCloak::addVisibility() const
{
	return -m_fSubVisibility;
}

//----------------------------------------------------------------------------

inline bool GadgetCloak::active() const
{
	return (flags() & FLAG_ACTIVE) != 0;
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
