/*
	GadgetEMP.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "GadgetEMP.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_FACTORY( GadgetEMP, NounGadget ); 
REGISTER_FACTORY_KEY( GadgetEMP, 4346009593719263115LL );

BEGIN_ABSTRACT_PROPERTY_LIST( GadgetEMP, NounGadget );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Energy );
END_PROPERTY_LIST();

GadgetEMP::GadgetEMP()
{
	m_Energy = 0;
}

//----------------------------------------------------------------------------

void GadgetEMP::setup()
{
	NounGadget::setup();
	m_Energy = energyNeeded();
}

//----------------------------------------------------------------------------

NounGadget::Type GadgetEMP::type() const
{
	return WEAPON;
}

dword GadgetEMP::hotkey() const
{
	return 'V';
}

//----------------------------------------------------------------------------

int GadgetEMP::usableWhen() const
{
	return Min<int>( (100 - ((m_Energy * 100) / energyNeeded())), NounGadget::usableWhen() );
}

bool GadgetEMP::usable( Noun * pTarget, bool shift ) const
{
	if (! NounGadget::usable( pTarget, shift ) )
		return false;
	if ( destroyed() )
		return false;
	if ( m_Energy < energyNeeded() )
		return false;
	if ( isCloaked() || isSafeZone() )
		return false;		
	return true;
}

void GadgetEMP::use( dword when, Noun * pTarget, bool shift)
{
	NounGadget::use( when, pTarget, shift );

	createUseEffect( false );
	m_Energy = 0;

	Vector3 origin( worldPosition() );

	Array< GameContext::NounCollision > collide;
	if ( context()->proximityCheck( origin, range(), collide ) )
	{
		for(int i=0;i<collide.size();i++)
		{
			NounGame * pCollide = WidgetCast<NounGame>( collide[ i ].pNoun );
			if ( pCollide != NULL && pCollide != parent() 
				&& pCollide->canDamage( DAMAGE_EMP ) 
				&& collide[ i ].fDistance < range() )
			{
				float damageRatio = 1.0f - (collide[ i ].fDistance / range());
				int damage = damageRatio * damageCaused();

				if ( damage > 0 )
				{
					// send the damage verb
					pCollide->inflictDamage( tick(), parentBody(), damage, DAMAGE_EMP, 
						pCollide->worldFrame() * (origin - pCollide->worldPosition()) );
				}
			}
		}
	}
}

int GadgetEMP::useEnergy( dword nTick, int energy )
{
	if ( m_Energy < energyNeeded() )
	{
		int use =  damageRatioInv() * chargeRate();
		int charge = Min( Min( energyNeeded() - m_Energy, use ), energy );

		energy -= charge;
		m_Energy += charge;
	}

	return energy;
}

//----------------------------------------------------------------------------
//EOF
