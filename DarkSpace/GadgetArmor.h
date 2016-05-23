/*
	GadgetArmor.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef GADGET_ARMOR_H
#define GADGET_ARMOR_H

#include "Constants.h"
#include "NounGadget.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL GadgetArmor : public NounGadget
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	GadgetArmor();

	// Noun interface
	void					setup();						// reset gadget to starting state
	void					simulate( dword nTick );

	// NounGadget interface
	Type					type() const;					// get gadget type
	EnergyClass				energyClass() const;

	CharString				status() const;
	float					addMass() const;				// mass of this gadget

	bool					canDamage( dword type ) const;	// can this device be damaged
	virtual CharString		useTip( Noun * pTarget,
								bool shift ) const;	
	bool					needRepair( Noun * pWhom ) const;

	bool					repair( Noun * pWhom, int nDamage );	// repair the armor

	// Accessors
	virtual bool			canDeflect( dword nType ) const = 0;
	virtual int				maxArmor() const = 0;			// strength of the armor
	virtual int				repairRate() const = 0;			// repair rate of the armor
	virtual float			damageModifier( dword nType ) const = 0;		// modifer for specific damage types

	int						armor() const;					// current armor level
	float					modifier() const;				// armor modifier, set by the parent of this armor (i.e. ship)
	float					armorPercent() const;			// armor() / maxArmor(), 1.0 = full armor, 0.0 = no armor

	// Mutators
	void					setArmor( int armor );
	void					setModifier( float fModifier );

	// called by NounShip::inflictDamage()
	int						deflect( dword nType, int damage, Facing facing );			// deflect damage

private:
	// Data
	int						m_Armor;
	// non-serialized
	dword					m_nArmorTick;
	dword					m_nTipArmorTick;
	float					m_fModifier;
	int						m_nDamageRepaired;
};

//----------------------------------------------------------------------------

inline int GadgetArmor::armor() const
{
	return m_Armor;
}

inline float GadgetArmor::modifier() const
{
	return m_fModifier;
}

inline float GadgetArmor::armorPercent() const
{
	return (float)armor() / maxArmor();
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
