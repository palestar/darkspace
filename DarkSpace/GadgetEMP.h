/*
	GadgetEMP.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef GADGETEMP_H
#define GADGETEMP_H

#include "Constants.h"
#include "NounGadget.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL GadgetEMP : public NounGadget
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	GadgetEMP();

	// Noun interface
	void			setup();
	// NounGadget interface
	Type			type() const;					// get gadget type
	dword			hotkey() const;					// gadget hotkey

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
	virtual float	range() const = 0;
	virtual int		damageCaused() const = 0;

private:
	int				m_Energy;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
