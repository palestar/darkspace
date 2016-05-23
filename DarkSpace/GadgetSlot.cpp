/*
	GadgetSlot.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "GadgetSlot.h"
#include "CargoGadget.h"
#include "NounShip.h"
#include "VerbRefitGadget.h"
#include "GameContext.h"
#include "resource.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( GadgetSlot, NounGadget );
REGISTER_FACTORY_KEY( GadgetSlot, 4337591588959340383LL );

BEGIN_PROPERTY_LIST( GadgetSlot, NounGadget )
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Type );
END_PROPERTY_LIST();

GadgetSlot::GadgetSlot()
{
	m_Type = UNKNOWN;
}

//----------------------------------------------------------------------------

NounGadget::Type GadgetSlot::type() const
{
	return m_Type;
}

bool GadgetSlot::useTarget() const
{
	return false;
}

bool GadgetSlot::canDamage( dword type ) const
{
	return false;
}

int GadgetSlot::durability() const
{
	return 0;
}

float GadgetSlot::addMass() const
{
	return 0.0f;
}

int	GadgetSlot::addValue() const
{
	return 0;
}

CharString GadgetSlot::useTip( Noun * pTarget, bool shift ) const
{
	if ( usable( pTarget, shift ) )
		return CharString().format( "Install %s", pTarget->name());

	return "Empty Slot";
}

bool GadgetSlot::usable( Noun * pTarget, bool shift ) const
{
	//if ( WidgetCast<CargoGadget>( pTarget ) )
	//	return VerbRefitGadget::canRefit( this, (CargoGadget *)pTarget );

	return false;
}

void GadgetSlot::use( dword when, Noun * pTarget, bool shift )
{
	if ( isServer() )
	{
		CargoGadget * pCargo = WidgetCast<CargoGadget>( pTarget );
		if ( pCargo != NULL )
		{
			if ( grabReference() )		// don't die if we are deleted yet	
			{
				// get our parent before we call replaceNoun(), because that will detach this object
				NounShip * pShip = WidgetCast<NounShip>( parentBody() );

				GameContext * pContext = WidgetCast<GameContext>( context() );
				ASSERT( pContext );

				// create the gadget
				NounGadget * pGadget = pCargo->gadget();
				if ( pGadget != NULL )
				{
					pGadget->setTick( when );
					pGadget->setPosition( position() );
					pGadget->setFrame( frame() );

					pGadget->clearFlags( FLAG_MOUNT_FULL );
					pGadget->addFlags( flags() & FLAG_MOUNT_FULL );
					
					// do not setup() the gadget, as that would reload it automatically

					// attach to the ship
					pShip->attachNode( pGadget );
					// detach the cargo
					pShip->detachNodeSwap( pCargo );

					// reinitialize the gadgets for the ship
					pShip->initializeGadgets();
					pShip->updateCharacter();
				}

				releaseReference();
			}

			setDetach();
		}
	}
}

void GadgetSlot::setType( Type type )
{
	m_Type = type;
}

//----------------------------------------------------------------------------
//EOF
