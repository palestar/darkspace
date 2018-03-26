
#include "Debug/Assert.h"
#include "GadgetArmorReactive.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_FACTORY( GadgetArmorReactive, NounGadget ); 

BEGIN_ABSTRACT_PROPERTY_LIST( GadgetArmorReactive, NounGadget );
	ADD_REPLICATED_PROPERTY( m_Armor, TICKS_PER_SECOND * 5 );
END_PROPERTY_LIST();

GadgetArmorReactive::GadgetArmorReactive() : m_Armor( 0 ), m_nReflectTick( 0 ), m_fModifier( 1.0f ), m_nArmorTick( 0 ), m_nDamageRepaired( 0 )
{}

//----------------------------------------------------------------------------

void GadgetArmorReactive::setup()
{
	NounGadget::setup();
	// start out at full strength
	m_Armor = maxArmor();
	m_nIncDamage = 0;
	m_nKineticDamage = 0;
	m_fKineticResist = 0;
	m_nEnergyDamage = 0;
	m_fEnergyResist = 0;
	m_nPSIDamage = 0;
	m_fPSIResist = 0;
	m_nEMPDamage = 0;
	m_fEMPResist = 0;
	m_nELFDamage = 0;
	m_fELFResist = 0;

}

void GadgetArmorReactive::simulate( dword nTick )
{
	if ( nTick >= m_nArmorTick )
	{
		m_nArmorTick = nTick + ( TICKS_PER_SECOND * 5 );
		m_nDamageRepaired = 0;

		// reset our armour resists damage counters otherwise we'll end up overflowing our values
		// this shouldn't effect things too badly, if we're not taking damage then our resists
		// drop off anyway, and resist calculations happen every second
		m_nIncDamage = 0;
		m_nKineticDamage = 0;
		m_nEnergyDamage = 0;
		m_nPSIDamage = 0;
		m_nEMPDamage = 0;
		m_nELFDamage = 0;
	}

	if (nTick >= m_nReflectTick)
	{
		m_nReflectTick = nTick + TICKS_PER_SECOND;
		// calculate our resists
		if (m_nIncDamage > 0)
		{
			if (m_nKineticDamage > 0)
				if ((m_nIncDamage / m_nKineticDamage) >= 0.5 && m_fKineticResist < 0.4f)
					m_fKineticResist += .01f;
				else
					m_fKineticResist -= .01f;
			if (m_nEnergyDamage > 0)
				if ((m_nIncDamage / m_nEnergyDamage) >= 0.5 && m_fKineticResist < 0.4f)
					m_fEnergyResist += .01f;
				else
					m_fEnergyResist -= .01f;
			if (m_nPSIDamage > 0)
				if ((m_nIncDamage / m_nPSIDamage) >= 0.5 && m_fKineticResist < 0.4f)
					m_fPSIResist += .01f;
				else
					m_fPSIResist -= .01f;
			if (m_nEMPDamage > 0)
				if ((m_nIncDamage / m_nEMPDamage) >= 0.5 && m_fKineticResist < 0.4f)
					m_fEMPResist += .01f;
				else
					m_fEMPResist -= .01f;
			if (m_nELFDamage > 0)
				if ((m_nIncDamage / m_nELFDamage) >= 0.5 && m_fKineticResist < 0.4f)
					m_fELFResist += .01f;
				else
					m_fELFResist -= .01f;
		}
		else
		{
			m_fKineticResist -= .01f;
			m_fEnergyResist -= .01f;
			m_fPSIResist -= .01f;
			m_fEMPResist -= .01f;
			m_fELFResist -= .01f;
		}
	}


	NounGadget::simulate( nTick );
}

//----------------------------------------------------------------------------

NounGadget::Type GadgetArmorReactive::type() const
{
	return ARMOR;
}

NounGadget::EnergyClass GadgetArmorReactive::energyClass() const
{
	return ENERGY_CLASS_DEFENSE;
}

CharString GadgetArmorReactive::status() const
{
	return CharString().format("%d%%", ((armor() * 100) / maxArmor()) );
}

float GadgetArmorReactive::addMass() const
{
	return armor() / 250000.0f;
}

bool GadgetArmorReactive::canDamage( dword type ) const
{
	return false;
}

CharString GadgetArmorReactive::useTip( Noun * pTarget, bool shift ) const
{
	CharString tip;
	
	// usage information
	tip += CharString().format("\nCurrent HP:<X;100>%d/%d", m_Armor, maxArmor() );
	float fMod = ( calculateModifier( MT_DEFENSE ) * 100 ) - 100;
	if ( fMod > 0 )
		tip += CharString().format("\nDamage Reduction:<X;100>%.0f%%", fMod );
	fMod = calculateModifier( MT_DEFENSE_RECHARGE );

#ifdef _DEBUG
	tip += CharString().format("\nKinetic Resist:<X;100>%.0f%%"
								"\nEnergy Resist: <X; 100>%.0f%%"
								"\nPSI Resist: <X; 100>%.0f%%"
								"\nEMP Resist: <X; 100>%.0f%%"
								"\nELF Resist: <X; 100>%.0f%%",
								1 - m_fKineticResist,
								1 - m_fEnergyResist,
								1 - m_fPSIResist,
								1 - m_fEMPResist,
								1 - m_fELFResist);
#endif

	return tip;
}

bool GadgetArmorReactive::needRepair( Noun * pWhom ) const
{
	if ( NounGadget::needRepair(pWhom) )
		return true;
	return armor() < maxArmor();
}

//----------------------------------------------------------------------------

bool GadgetArmorReactive::repair( Noun * pWhom, int nDamage )
{
	// collect external repairs over the last second for fancy tooltips
	m_nDamageRepaired += nDamage;
	// repair the armor
	m_Armor = Clamp( m_Armor + nDamage, 0, maxArmor() );
	return m_Armor < maxArmor();
}

void GadgetArmorReactive::setArmor( int armor )
{
	m_Armor = Clamp( armor, 0, maxArmor() );
}

void GadgetArmorReactive::setModifier( float fModifier )
{
	m_fModifier = fModifier;
}

float GadgetArmorReactive::damageModifier(dword nType)
{
	if ((nType & (DAMAGE_KINETIC)) != 0)
		return 1 - m_fKineticResist;
	if ((nType & (DAMAGE_ENERGY)) != 0)
		return 1 - m_fEnergyResist;
	if ((nType & (DAMAGE_PSI)) != 0)
		return 1 - m_fPSIResist;
	if ((nType & (DAMAGE_EMP)) != 0)
		return 1 - m_fEMPResist;
	if ((nType & (DAMAGE_ELF)) != 0)
		return 1 - m_fELFResist;

	return 1.0f;
}

int GadgetArmorReactive::deflect( dword nType, int damage, Facing nFacing )
{
	if ( !canDeflect( nType ) )
		return damage;	// doesn't deflect this type of damage
	if ( (facingMountFlags( nFacing ) & mountFlags()) == 0 )
		return damage;	// wrong facing

	// calculate incoming damage vs damage type
	// we need to reset this every 5 seconds in simulate()
	m_nIncDamage += damage;
	switch (nType)
	{
	case DAMAGE_KINETIC:
		m_nKineticDamage += damage;
	case DAMAGE_ENERGY:
		m_nEnergyDamage += damage;
	case DAMAGE_PSI:
		m_nPSIDamage += damage;
	case DAMAGE_EMP:
		m_nEMPDamage += damage;
	case DAMAGE_ELF:
		m_nELFDamage += damage;
	}

	float fMod = damageModifier( nType ) * calculateModifier( MT_DEFENSE, true );
	if ( fMod <= 0.0f )
		return 0;		// full damage reflected!	

	int nModifiedDamage = fMod * damage;
	int nDeflected = Min( nModifiedDamage, m_Armor );
	m_Armor -= nDeflected;
	if ( m_Armor > 0 )
		return 0;		// full damage is deflected...

	// return the un-modified damage value..
	return damage - (nDeflected / fMod);
}

//----------------------------------------------------------------------------
// EOF
