/*
	GadgetJumpDrive.h

	Base class for jump drives - can only be attached to a NounShip object!
	(c)1999 Palestar Development, Richard Lyle
*/

#ifndef GADGET_JUMP_DRIVE_H
#define GADGET_JUMP_DRIVE_H

#include "NounGadget.h"
#include "Constants.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL GadgetJumpDrive : public NounGadget
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef WidgetReference< GadgetJumpDrive >	wRef;

	enum Flags {
		FLAG_JUMPING		= 0x1,
		FLAG_ENGAGED		= 0x2,
	};

	// Construction
	GadgetJumpDrive();
	// Widget interface
	bool					read( const InStream & );

	// BaseNode interface
	void					render( RenderContext &context, 
								const Matrix33 & frame, 
								const Vector3 & position );
	// Noun interface
	void					setup();
	void					initialize();
	void					release();
	void					simulate( dword nTick );	

	// NounGadget interface
	Type					type() const;					// get gadget type
	EnergyClass				energyClass() const;

	dword					hotkey() const;				
	CharString				status() const;

	float					addSignature() const;			// this device affects the signature of the ship
	virtual CharString		useTip( Noun * pTarget,
								bool shift ) const;		
	bool					needReload( Noun * pWhom ) const;

	bool					usable( Noun * pTarget,		
								bool shift ) const;			// can gadget be used on target, called before we issue VerbUseGadget
	int						usableWhen() const;
	bool					useActive() const;				// is the gadget current on or off
	void					use( dword when, Noun * pTarget, 
								bool shift );				// use gadget, returns created object if any
	int						useEnergy( dword nTick, 
								int energy );

	bool					reload( Noun * pWhom );			// reload gadget

	// Accessors
	virtual int				energyCost() const = 0;			// how much energy is required to engage the engines
	virtual int				energyCharge() const = 0;		// how fast do the jump drives charge
	virtual int				maxFuel() const = 0;			// how much fuel can this drive have
	virtual int				refuelRate() const = 0;			// what rate is this gadget refueled
	virtual int				fuelCost() const = 0;			// how much fuel is required to sustain the engines
	virtual float			velocity() const = 0;			// what is the drive velocity

	int						energy() const;
	int						fuel() const;					// how much fuel do we have available
	dword					jumpTime() const;
	const Vector3 &			jumpVelocity() const;
	Noun *					jumpTarget() const;

	// Mutators
	void					beginJump( Noun * pTarget );
	void					doJump( dword when, dword jumpTime,
								const Vector3 & jump );		// called by VerbJump()
	void					abortJump( dword when );		// called by VerbJumpAbort()
	void					clearJump();					// called when a ship enters a jumpgate, if ship is transfered to another server this prevents some problems!

	// Helpers
	bool					jumping() const;
	bool					engaged() const;

	float					fuelCostMass() const;			
	int						energyCostMass() const;
	float					maxRange() const;				// calculate the maximum range with the available fuel
	void					refuel(int a_percent);			// used by enhancements to instantly refuel by percent

protected:
	// Data
	int						m_Energy;						// how much energy does this jump have
	int						m_Fuel;							// how much fuel do we have
	int						m_JumpTime;						// when will the jump complete
	Vector3					m_JumpVelocity;

	Noun::tRef				m_JumpTarget;

	// non-serialized
	float					m_FuelUse;
	float					m_FuelFactor;
	float					m_EnergyFactor;

	// Accessors
	bool					canJump( Noun * pTarget, bool shift ) const;	// returns true if we can engage jump to target
	// Mutators
	void					calcluateFactors();
	void					createHitEffect();
	void					endJump();
};

//----------------------------------------------------------------------------

inline int GadgetJumpDrive::energy() const
{
	return m_Energy;
}

inline int GadgetJumpDrive::fuel() const
{
	return m_Fuel;
}

inline dword GadgetJumpDrive::jumpTime() const
{
	return m_JumpTime;
}

inline const Vector3 & GadgetJumpDrive::jumpVelocity() const
{
	return m_JumpVelocity;
}

inline Noun * GadgetJumpDrive::jumpTarget() const
{
	return m_JumpTarget;
}

//----------------------------------------------------------------------------

inline bool GadgetJumpDrive::jumping() const
{
	return (flags() & FLAG_JUMPING) != 0;
}

inline bool GadgetJumpDrive::engaged() const
{
	return (flags() & FLAG_ENGAGED) != 0;
}

inline float GadgetJumpDrive::fuelCostMass() const
{
	return m_FuelFactor * fuelCost() * calculateModifier( MT_JUMPDRIVE_RANGE, true );
}

inline int GadgetJumpDrive::energyCostMass() const
{
	return (int)(m_EnergyFactor * energyCost());
}

inline float GadgetJumpDrive::maxRange() const
{
	const float MAX_RANGE = 1000000000.0f;		// 1 billion GU

	// calculate how many seconds of fuel we have
	float fFuelPerSecond = fuelCostMass() * TICKS_PER_SECOND;
	if ( fFuelPerSecond > 0.0f )
	{
		// calulate the number of seconds we can jump with out given fuel.
		float fTime = float( m_Fuel ) / fFuelPerSecond;
		// range is the time * velocity
		float fRange = velocity() * fTime;
		if ( fRange < MAX_RANGE )
			return fRange;
	}

	// fuel cost per second is 0, so just return a very large number..
	return MAX_RANGE;
}

inline void GadgetJumpDrive::refuel(int a_percent)
{
	float fAddAmount = maxFuel() * (float(a_percent * 0.01));
	m_Fuel = Min(m_Fuel+(int)fAddAmount,maxFuel());
}


//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
