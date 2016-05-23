/*
	GadgetReactor.cpp
	(c)1999 PaleStar Development, Richard Lyle
*/


#include "Debug/Assert.h"
#include "NounShip.h"
#include "GadgetReactor.h"
#include "resource.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_FACTORY( GadgetReactor,  NounGadget  ); 
REGISTER_FACTORY_KEY( GadgetReactor, 4251763342571375032LL );

BEGIN_ABSTRACT_PROPERTY_LIST( GadgetReactor, NounGadget );
	ADD_REPLICATED_PROPERTY( m_Flags, TICKS_PER_SECOND );
END_PROPERTY_LIST();

GadgetReactor::GadgetReactor()
{
	m_Flags |= FLAG_ACTIVE;
}

//---------------------------------------------------------------------------------------------------

void GadgetReactor::setup()
{
	NounGadget::setup();
	m_Flags |= FLAG_ACTIVE;
}

void GadgetReactor::initialize()
{
	NounGadget::initialize();
	m_Flags |= FLAG_ACTIVE;
}

//----------------------------------------------------------------------------

NounGadget::Type GadgetReactor::type() const
{
	return REACTOR;
}

NounGadget::EnergyClass GadgetReactor::energyClass() const
{
	return ENERGY_CLASS_DRIVES;
}

int GadgetReactor::addMaxEnergy() const
{
	return( maxEnergy() );
}

float GadgetReactor::addSignature() const
{
	return( active() ? 0.25f : 0.0f );
}

CharString GadgetReactor::useTip( Noun * pTarget, bool shift ) const
{
	CharString tip;
	
	// usage information
	int generate = 0;
	if ( active() )
		generate = ( calculateModifier( MT_REACTOR ) * damageRatioInv() ) * energyGenerated();
		
	tip += CharString().format("\nEnergy p/s:<X;100>%.1f", ( generate * TICKS_PER_SECOND ) / 1000.0f );
	tip += CharString().format("\nEnergy Stored:<X;100>%.0f", maxEnergy() / 1000.0f );		
	
	return tip;
}

bool GadgetReactor::usable( Noun * pTarget, bool shift ) const
{
	if (! NounGadget::usable( pTarget, shift ) )
		return false;
	if ( destroyed() )
		return false;
	return true;
}

bool GadgetReactor::useActive() const
{
	return active();
}

void GadgetReactor::use( dword when, Noun * pTarget, bool shift)
{
	if ( active() )
		NounGadget::use( when, pTarget, shift );
	if ( isServer() )
		setFlags( FLAG_ACTIVE, !active() );
}

int GadgetReactor::useEnergy( dword nTick, int energy )
{
	if ( active() )
	{
		int generate = (calculateModifier( MT_REACTOR ) * damageRatioInv()) * energyGenerated();
		energy += generate;

		if ( destroyed() )
			setFlags( FLAG_ACTIVE, false );
	}

	return energy;
}

//----------------------------------------------------------------------------
// EOF


