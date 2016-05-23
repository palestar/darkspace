/*
	GadgetTECM.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef GADGET_TECM_H
#define GADGET_TECM_H

#include "Constants.h"
#include "NounGadget.h"
#include "NounShip.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL GadgetTECM : public NounGadget
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	GadgetTECM();

	// BaseNode interface
	void					render( RenderContext &context, 
								const Matrix33 & frame, 
								const Vector3 & position );
	// Noun interface
	void					release();
	void					simulate( dword nTick );

	// NounGadget interface
	Type					type() const;					// get gadget type
	dword					hotkey() const;					// gadget hotkey

	virtual CharString		useTip( Noun * pTarget,
								bool shift ) const;
	bool					usable( Noun * pTarget, 
								bool shift ) const;			// can gadget be used on target
	bool					useActive() const;				// is the gadget current on or off
	void					use( dword when, Noun * pTarget,
								bool shift);				// use gadget
	int						useEnergy( dword nTick,
								int energy );
	bool					updateLogic();

	// Accessors
	virtual int				energyCost() const = 0;			// energy cost to use 
	virtual float			strength() const = 0;			// how much is my sensor increases
	virtual float			range() const = 0;				// area affected
	virtual int				cooldown() const = 0;			// how many ticks for the device to cooldown

	bool					active() const;					// how many ticks remaining

protected:
	// Data
	NounShip::wRef			m_Target;						// who are we assisting
private:
	// non-serialized
	dword					m_nUpdateTick;
};

//----------------------------------------------------------------------------

inline bool GadgetTECM::active() const
{
	return (flags() & FLAG_ACTIVE) != 0;
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
