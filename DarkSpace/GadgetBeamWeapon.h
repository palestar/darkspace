/*
	GadgetBeam.h
	(c)1999 Palestar Development, Richard Lyle
*/

#ifndef GADGET_BEAM_WEAPON_H
#define GADGET_BEAM_WEAPON_H

#include "NounGadget.h"
#include "NounShip.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL GadgetBeamWeapon : public NounGadget
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	GadgetBeamWeapon();
	// Widget interface
	bool					read( const InStream & );
	// BaseNode interface
	void					render( RenderContext &context, 
								const Matrix33 & frame, 
								const Vector3 & position );
	// Noun interface
	void					setup();
	void					release();
	void					simulate( dword nTick );

	void					inflictDamage( dword nWhen, Noun * pFrom, 
								int damage, dword type, 
								const Vector3 & direction );
	// NounGadget interface
	Type					type() const;					// get gadget type
	EnergyClass				energyClass() const;

	dword					hotkey() const;					// gadget hotkey
	float					addSignature() const;	
	virtual CharString		useTip( Noun * pTarget,
								bool shift ) const;

	bool					usable( Noun * pTarget, 
								bool shift ) const;			// can gadget be used on target
	int						usableWhen() const;				// when can the gadget be used again
	bool					useActive() const;				// is the gadget current on or off

	void					use( dword when, Noun * pTarget,
								bool shift);				// use gadget
	int						useEnergy( dword nTick, 
								int energy );
	bool					updateLogic();

	virtual int				energyCost() const = 0;			// energy cost to use beam weapon
	virtual int				energyCharge() const = 0;		// how fast does the weapon charge
	virtual int				damage() const = 0;				// amount of damage inflicted, total possible damage = damage * duration
	virtual int				damageRandom() const = 0;		// amount of random damage added
	virtual float			damageFalloff() const = 0;		// damage falloff
	virtual bool			reverseFalloff() const = 0;		// is the falloff reversed (does it increase with distance)	
	virtual dword			damageType() const = 0;			// type of damage caused
	virtual int				duration() const = 0;			// how many ticks is the beam active
	virtual float			length() const = 0;				// length (range) of the beam
	virtual bool			allowPointDefense() const = 0;	// true if this beam weapon can be used for point defense

	// Accessors
	bool					pointDefense() const;			// is weapon in point defense mode
	Noun *					target() const;
	// Mutators
	void					setPointDefense( bool on );		// set weapon to point defense mode
	void					setTarget( Noun * pTarget );

	//! Helpers
	float					maxRange() const;

protected:
	// Data
	int						m_Energy;						// how much energy does this weapon have
	int						m_Duration;						// how many ticks to keep firing
	bool					m_PointDefense;
	Noun::tRef				m_Target;						// beam weapon target

	// non-serialize
	dword					m_nEnergyTick;
	float					m_fChargeRate;
	Noun::Ref				m_Hit;
	float					m_Signature;
	bool					m_bDetachHitEffect;				// if true, then the hit effect is detached when the beam stops
};

//----------------------------------------------------------------------------

inline bool GadgetBeamWeapon::pointDefense() const
{
	return m_PointDefense;
}

inline Noun * GadgetBeamWeapon::target() const
{
	return m_Target;
}

//----------------------------------------------------------------------------

inline void GadgetBeamWeapon::setPointDefense( bool on )
{
	m_PointDefense = on;
}

inline void	GadgetBeamWeapon::setTarget( Noun * pTarget )
{
	m_Target = pTarget;
}

//---------------------------------------------------------------------------------------------------

inline float GadgetBeamWeapon::maxRange() const
{
	return length() * calculateModifier( MT_BEAM_RANGE );
}

//----------------------------------------------------------------------------

#endif

//----------------------------------------------------------------------------
// EOF
