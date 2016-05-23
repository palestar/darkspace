/**
	@file GadgetAura.cpp

	(c)2012 Palestar Inc
	@author Richard Lyle @date 7/2/2012 8:54:50 PM
*/

#include "Debug/Assert.h"
#include "GadgetAura.h"
#include "NounShip.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( GadgetAura, NounGadget ); 

BEGIN_PROPERTY_LIST( GadgetAura, NounGadget );
	ADD_TRANSMIT_PROPERTY( m_pField );
END_PROPERTY_LIST();

GadgetAura::GadgetAura()
{}

//----------------------------------------------------------------------------

void GadgetAura::render( RenderContext &context, 
				const Matrix33 & frame, 
				const Vector3 & position )
{
	if ( m_pUseEffect.valid() )
		m_pUseEffect->render( context, frame, position );
}

//----------------------------------------------------------------------------

void GadgetAura::initialize()
{
	NounGadget::initialize();

	if ( m_pField.valid() )
	{
		m_pUseEffect = useEffect();

		// attach our field as child, it will apply it's enhancements to anyone colliding with it..
		NounField::removeAllFields( this );
		attachNode( m_pField );
	}
}

void GadgetAura::release()
{
	NounGadget::release();
}

//----------------------------------------------------------------------------

CharString GadgetAura::useTip( Noun * pTarget, bool shift ) const
{
	CharString tip;
	// usage information
	tip += CharString().format("\nRange: <X;125>%.0fgu", m_pField->radius() );
	tip += CharString().format("\nAura Mode:<X;125>%s", m_pField->isFriendsOnly() == true ? "Allies" : "Enemies" );
	for ( int i = 0; i < m_pField->modifierCount(); ++i )
	{
		ShipModifier * pMod = WidgetCast<ShipModifier>( m_pField->modifier( i ) );
		tip += CharString().format("\n%s:<X;125>%d%%", NounShip::modifierTypeText( pMod->type() ), pMod->value() );
	}
	
	return tip;
}


NounGadget::Type GadgetAura::type() const
{
	return AURA;
}

//---------------------------------------------------------------------------------------------------
//EOF
