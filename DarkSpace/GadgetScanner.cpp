/*
	GadgetScanner.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "GadgetScanner.h"
#include "GameContext.h"
#include "resource.h"
#include "VerbGadgetOff.h"

//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_FACTORY( GadgetScanner, NounGadget ); 
REGISTER_FACTORY_KEY( GadgetScanner, 4311140014912906405LL );

BEGIN_ABSTRACT_PROPERTY_LIST( GadgetScanner, NounGadget );
	ADD_REPLICATED_PROPERTY( m_Flags, TICKS_PER_SECOND );
END_PROPERTY_LIST();

GadgetScanner::GadgetScanner()
{}

//----------------------------------------------------------------------------

void GadgetScanner::render( RenderContext &context, 
				const Matrix33 & frame, 
				const Vector3 & position )
{
	if ( active() )
	{
		Scene * pUseEffect = useEffect();
		if ( pUseEffect != NULL )
			pUseEffect->render( context, frame, position );
	}
}

//----------------------------------------------------------------------------

NounGadget::Type GadgetScanner::type() const
{
	return SPECIAL_OFFENSIVE;
}

dword GadgetScanner::hotkey() const
{
	return 'K';
}

float GadgetScanner::addSignature() const
{
	return active() ? signature() : 0.0f;
}

float GadgetScanner::addSensor() const
{
	return active() ? (damageRatioInv() * sensor() * calculateModifier( MT_SCANNER ) ) : 0.0f;
}

bool GadgetScanner::usable( Noun * pTarget, bool shift ) const
{
	if ( active() )
		return true;
	if (! NounGadget::usable( pTarget, shift ) )
		return false;
	if ( destroyed() )
		return false;
	return true;
}

bool GadgetScanner::useActive() const
{
	return active();
}

void GadgetScanner::use( dword when, Noun * pTarget, bool shift)
{
	if ( active() )
		NounGadget::use( when, pTarget, shift );

	if ( isServer() )
	{
		// set the device active
		setFlags( FLAG_ACTIVE, !active() );
		message( CharString().format( "<color;ffffff>Tactical: %s %s.", name(), active() ? "Active" : "Inactive" ), true ); 
	}
}

int GadgetScanner::useEnergy( dword nTick, int energy )
{
	if ( active() )
	{
		energy -= energyCost();
		if ( (energy <= 0 || destroyed()) && isLocal() )
			Verb::Ref( new VerbGadgetOff( this ) );
	}

	return energy;
}

bool GadgetScanner::updateLogic()
{
	if ( WidgetCast<NounShip>( parent() ) )
	{
		NounShip * pShip = (NounShip *)parent();
		if (! useActive() )
			pShip->useGadget( this, NULL, false );
			
		return true;
	}
	
	return true;
}

//----------------------------------------------------------------------------
//EOF
