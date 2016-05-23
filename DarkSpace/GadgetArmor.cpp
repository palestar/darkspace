/*
	GadgetArmor.cpp
	(c)2000 Palestar, Richard Lyle
*/


#include "Debug/Assert.h"
#include "GadgetArmor.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_FACTORY( GadgetArmor, NounGadget ); 
REGISTER_FACTORY_KEY( GadgetArmor, 4137179553683648623LL );

BEGIN_ABSTRACT_PROPERTY_LIST( GadgetArmor, NounGadget );
	ADD_REPLICATED_PROPERTY( m_Armor, TICKS_PER_SECOND * 5 );
END_PROPERTY_LIST();

GadgetArmor::GadgetArmor() : m_Armor( 0 ), m_fModifier( 1.0f ), m_nArmorTick( 0 ), m_nTipArmorTick( 0 ), m_nDamageRepaired( 0 )
{}

//----------------------------------------------------------------------------

void GadgetArmor::setup()
{
	NounGadget::setup();
	// start out at full strength
	m_Armor = maxArmor();
	m_nDamageRepaired = 0;
}

void GadgetArmor::simulate( dword nTick )
{
	if ( nTick >= m_nArmorTick )
	{
		m_nArmorTick = nTick + TICKS_PER_SECOND;
		// repair the armor
		int nMax = maxArmor();
		if ( m_Armor < nMax )
		{
			float fMod = calculateModifier( MT_DEFENSE_RECHARGE );
			m_Armor = Clamp<int>( m_Armor + (repairRate() * fMod), 0, nMax );
		}
	}
	
	if ( nTick >= m_nTipArmorTick )
	{
		m_nTipArmorTick = nTick + ( TICKS_PER_SECOND * 5 );
		m_nDamageRepaired = 0;
	}

	NounGadget::simulate( nTick );
}

//----------------------------------------------------------------------------

NounGadget::Type GadgetArmor::type() const
{
	return ARMOR;
}

NounGadget::EnergyClass GadgetArmor::energyClass() const
{
	return ENERGY_CLASS_DEFENSE;
}

CharString GadgetArmor::status() const
{
	return CharString().format("%d%%", ((armor() * 100) / maxArmor()) );
}

float GadgetArmor::addMass() const
{
	return armor() / 250000.0f;
}

bool GadgetArmor::canDamage( dword type ) const
{
	return false;
}

CharString GadgetArmor::useTip( Noun * pTarget, bool shift ) const
{
	CharString tip;
	
	// usage information
	tip += CharString().format("\nCurrent HP:<X;100>%d/%d", m_Armor, maxArmor() );
	float fMod = ( calculateModifier( MT_DEFENSE ) * 100 ) - 100;
	if ( fMod > 0 )
		tip += CharString().format("\nDamage Reduction:<X;100>%.0f%%", fMod );
	fMod = calculateModifier( MT_DEFENSE_RECHARGE );

	return tip;
}

bool GadgetArmor::needRepair( Noun * pWhom ) const
{
	if ( NounGadget::needRepair(pWhom) )
		return true;
	return armor() < maxArmor();
}

//----------------------------------------------------------------------------

bool GadgetArmor::repair( Noun * pWhom, int nDamage )
{
	// collect external repairs over the last second for fancy tooltips
	m_nDamageRepaired += nDamage;
	// repair the armor
	m_Armor = Clamp( m_Armor + nDamage, 0, maxArmor() );
	return m_Armor < maxArmor();
}

void GadgetArmor::setArmor( int armor )
{
	m_Armor = Clamp( armor, 0, maxArmor() );
}

void GadgetArmor::setModifier( float fModifier )
{
	m_fModifier = fModifier;
}

int GadgetArmor::deflect( dword nType, int damage, Facing nFacing )
{
	if ( !canDeflect( nType ) )
		return damage;	// doesn't deflect this type of damage
	if ( (facingMountFlags( nFacing ) & mountFlags()) == 0 )
		return damage;	// wrong facing

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
