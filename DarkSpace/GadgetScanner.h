/*
	GadgetScanner.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef GADGETSCANNER_H
#define GADGETSCANNER_H

#include "Constants.h"
#include "NounGadget.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL GadgetScanner : public NounGadget
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	GadgetScanner();

	// BaseNode interface
	void					render( RenderContext &context, 
								const Matrix33 & frame, 
								const Vector3 & position );
	// NounGadget interface
	Type					type() const;					// get gadget type
	dword					hotkey() const;					// gadget hotkey
	
	float					addSignature() const;
	float					addSensor() const;				
	
	bool					usable( Noun * pTarget, 
								bool shift ) const;			// can gadget be used on target
	bool					useActive() const;				// is the gadget current on or off
	void					use( dword when, Noun * pTarget,
								bool shift);				// use gadget
	int						useEnergy( dword nTick, 
								int energy );				// use energy
	bool					updateLogic();

	// Accessors
	virtual int				energyCost() const = 0;			// energy cost to use 
	virtual float			sensor() const = 0;				// how much is the sensor range increased
	virtual float			signature() const = 0;			// how much is the signature increased

	bool					active() const;					// how many ticks remaining

private:
};

//----------------------------------------------------------------------------

inline bool GadgetScanner::active() const
{
	return (flags() & FLAG_ACTIVE) != 0;
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
