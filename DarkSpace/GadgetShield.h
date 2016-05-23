/*
	GadgetShield.h
	Base class for shields
	(c)2000 Palestar, Richard Lyle
*/

#ifndef GADGET_SHIELD_H
#define GADGET_SHIELD_H

#include "Constants.h"
#include "NounGadget.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL GadgetShield : public NounGadget
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	GadgetShield();

	// Noun interface
	void					setup();
	void					initialize();

	// NounGadget interface
	virtual Type			type() const;					// get gadget type
	virtual EnergyClass		energyClass() const;

	virtual dword			hotkey() const;				
	virtual CharString		status() const;	
	virtual CharString		description() const;

	bool					canDamage( dword type ) const;	// can this device be damaged

	virtual CharString		useTip( Noun * pTarget,
								bool shift ) const;
	virtual bool			usable( Noun * pTarget, 
								bool shift ) const;			// can gadget be used on target
	virtual int				useModeCount() const;
	virtual int				useMode() const;
	virtual bool			useActive() const;				// is the gadget current on or off

	virtual void			useMode(int nMode);
	virtual void 			use( dword when, Noun * pTarget,
								bool shift);				// use gadget
	virtual int				useEnergy( dword nTick, 
								int energy );

	// Accessors
	virtual int				energyCost() const = 0;			// energy cost per tick for shields to be active
	virtual int				chargeEnergyCost() const = 0;	// energy cost when shields are below maxCharge()
	virtual int				maxCharge() const = 0;			// maximum charge of the shield
	virtual int				chargeRate() const = 0;			// rate at which charge is regained

	virtual bool			canDeflect( dword nType ) const = 0;			// do these shields deflect the specified type of damage
	virtual float			damageModifier( dword nType ) const = 0;		// modifer for specific damage types

	bool					active() const;
	int						charge() const;
	float					modifier() const;
	float					shieldPercent() const;			// charge() / maxCharge(), 1.0 = full shields, 0.0f = no shields

	// Mutators called by GadgetWeapon and GadgetBeamWeapon
	int						deflect( dword nType, int damage, Facing facing, const Vector3 & vOffset );			// deflect damage

	void					createHitEffect( const Vector3 & vOffset );
	void					setCharge( int charge );
	void					setModifier( float fModifier );

private:
	// Data
	int						m_Charge;						// total energy in shields capacitors
	int						m_Mode;

	// non-serialized
	float					m_fModifier;					// shield strength modifier
	float					m_fChargeRate;
};

//----------------------------------------------------------------------------

inline bool GadgetShield::active() const
{
	return (flags() & FLAG_ACTIVE) != 0;
}

inline int GadgetShield::charge() const
{
	return m_Charge;
}

inline float GadgetShield::modifier() const
{
	return m_fModifier;
}

inline float GadgetShield::shieldPercent() const
{
	return (float)charge() / maxCharge();
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
