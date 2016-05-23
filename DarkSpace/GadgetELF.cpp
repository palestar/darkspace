/*
	GadgetELF.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "GadgetELF.h"
#include "GameContext.h"
#include "resource.h"

//----------------------------------------------------------------------------

const int	ELF_UPDATE_RATE = TICKS_PER_SECOND * 5;
//! what is the max energy that can be leeched per tick from a single target...
const int	MAX_ENERGY_LEECH = 50;

//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_FACTORY( GadgetELF, NounGadget ); 
REGISTER_FACTORY_KEY( GadgetELF, 4368350708589970753LL );

BEGIN_ABSTRACT_PROPERTY_LIST( GadgetELF, NounGadget );
END_PROPERTY_LIST();

GadgetELF::GadgetELF()
{
	m_nUpdateAffected = key() % ELF_UPDATE_RATE;
}

//----------------------------------------------------------------------------

bool GadgetELF::read( const InStream & input )
{
	if (! NounGadget::read( input ) )
		return false;
	m_nUpdateAffected = key() % ELF_UPDATE_RATE;
	return true;
}

//----------------------------------------------------------------------------

void GadgetELF::render( RenderContext &context, 
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

void GadgetELF::release()
{
	NounGadget::release();
	m_Affect.release();
}

//----------------------------------------------------------------------------

NounGadget::Type GadgetELF::type() const
{
	return WEAPON;
}

dword GadgetELF::hotkey() const
{
	return 'V';
}

bool GadgetELF::usable( Noun * pTarget, bool shift ) const
{
	if ( active() )
		return true;
	if (! NounGadget::usable( pTarget, shift ) )
		return false;
	if ( WidgetCast<NounShip>( parent() ) && ((NounShip *)parent())->testFlags( NounShip::FLAG_CLOAKED|NounShip::FLAG_IN_SAFE_ZONE ) )
		return false;				// weapons are disabled
	if ( destroyed() )
		return false;
	return true;
}

bool GadgetELF::useActive() const
{
	return active();
}

void GadgetELF::use( dword when, Noun * pTarget, bool shift)
{
	if ( active() )
		NounGadget::use( when, pTarget, shift );

	// set the device active
	setFlags( FLAG_ACTIVE, !active() );
	message( CharString().format( "<color;ffffff>Tactical: %s %s.", name(), active() ? "Active" : "Inactive" ) ); 
}

int GadgetELF::useEnergy( dword nTick, int energy )
{
	if ( active() )
	{
		Vector3 myPosition( worldPosition() );

		m_nUpdateAffected++;
		if ( m_nUpdateAffected >= ELF_UPDATE_RATE )
		{
			m_nUpdateAffected -= ELF_UPDATE_RATE;
			m_Affect.release();

			// update list of ship list
			Array< GameContext::NounCollision > nouns;
			if ( context()->proximityCheck( worldPosition(), range(), nouns, CLASS_KEY(NounShip) ) )
			{
				for(int i=0;i<nouns.size();i++)
				{
					NounShip * pAffect = (NounShip *)nouns[i].pNoun;
					if ( pAffect != NULL && pAffect != parentBody() )
						m_Affect.push( pAffect );
				}
			}
		}

		// drain energy from the affected ships... based on range
		for(int i=0;i<m_Affect.size();i++)
		{
			NounShip * pAffect = m_Affect[ i ];

			float fDistance = (pAffect->worldPosition() - myPosition).magnitude();
			if ( fDistance < range() )
			{
				int leech = damageRatioInv() * ((fDistance * pAffect->energy()) / range());
				if ( leech > MAX_ENERGY_LEECH )
					leech = MAX_ENERGY_LEECH;
				pAffect->setEnergy( pAffect->energy() - leech );

				energy += leech;
			}
		}
	}

	return energy;
}

bool GadgetELF::updateLogic()
{
	NounShip * pShip = WidgetCast<NounShip>( parent() );
	if (! pShip )
		return false;
	if (! useActive() )
		pShip->useGadget( this, NULL, false );
	return true;
}

//----------------------------------------------------------------------------
//EOF
