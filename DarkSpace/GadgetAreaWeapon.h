/*
	GadgetAreaWeapon.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef GADGETAREAWEAPON_H
#define GADGETAREAWEAPON_H

#include "Constants.h"
#include "NounGadget.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL GadgetAreaWeapon : public NounGadget
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	GadgetAreaWeapon();
	// Noun interface
	void			setup();
	// NounGadget interface
	Type			type() const;					// get gadget type
	EnergyClass		energyClass() const;
	dword			hotkey() const;					// gadget hotkey

	int				usableWhen() const;
	virtual CharString		useTip( Noun * pTarget,
								bool shift ) const;			
	bool			usable( Noun * pTarget, 
						bool shift ) const;			// can gadget be used on target
	void			use( dword when, Noun * pTarget,
						bool shift);				// use gadget
	int				useEnergy( dword nTick, 
							int energy );			// use energy
	bool			updateLogic();

	// Accessors
	virtual int		energyNeeded() const = 0;		// how much energy is needed
	virtual int		chargeRate() const = 0;			// how fast does this weapon charge
	virtual float	range() const = 0;				// range of this weapon
	virtual int		damageCaused() const = 0;		// how much damage is caused to objects within range()
	virtual int		damageRandom() const = 0;		// additional random damage
	virtual dword	damageType() const = 0;			// type of damage inflicted by this weapon
	virtual int		damageRolls() const = 0;		// how many times to cause damage

private:
	// Data
	int				m_Energy;
	// non-serialized
	dword			m_nEnergyTick;
	float			m_fChargeRate;
};

#endif

//----------------------------------------------------------------------------
//EOF

