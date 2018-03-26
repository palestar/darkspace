#ifndef GADGET_ARMOR_R
#define GADGET_ARMOR_R

#include "Constants.h"
#include "NounGadget.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL GadgetArmorReactive : public NounGadget
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	GadgetArmorReactive();

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

	int						armor() const;					// current armor level
	float					modifier() const;				// armor modifier, set by the parent of this armor (i.e. ship)
	float					armorPercent() const;			// armor() / maxArmor(), 1.0 = full armor, 0.0 = no armor

	// Mutators
	void					setArmor( int armor );
	void					setModifier( float fModifier );
	float					damageModifier( dword nType );	// modifer for specific damage types

	// called by NounShip::inflictDamage()
	int						deflect( dword nType, int damage, Facing facing );			// deflect damage

private:
	// Data
	int						m_Armor;
	// non-serialized
	dword					m_nArmorTick;
	dword					m_nReflectTick;
	float					m_fModifier;
	int						m_nDamageRepaired = 0;
	int						m_nIncDamage = 0;
	int						m_nKineticDamage = 0;
	float					m_fKineticResist = 0;
	int						m_nEnergyDamage = 0;
	float					m_fEnergyResist = 0;
	int						m_nPSIDamage = 0;
	float					m_fPSIResist = 0;
	int						m_nEMPDamage = 0;
	float					m_fEMPResist = 0;
	int						m_nELFDamage = 0;
	float					m_fELFResist = 0;
};

//----------------------------------------------------------------------------

inline int GadgetArmorReactive::armor() const
{
	return m_Armor;
}

inline float GadgetArmorReactive::modifier() const
{
	return m_fModifier;
}

inline float GadgetArmorReactive::armorPercent() const
{
	return (float)armor() / maxArmor();
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
