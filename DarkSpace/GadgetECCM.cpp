/*
	GadgetECCM.cpp
	(c)2000 Palestar, Richard Lyle
*/


#include "Debug/Assert.h"
#include "GadgetECCM.h"
#include "GameContext.h"
#include "resource.h"
#include "StructureDefense.h"
#include "ShipPlatform.h"

//---------------------------------------------------------------------------------------------------

static Constant ECCM_PRESTIGE_SCALE( "ECCM_PRESTIGE_SCALE", 1.0f );

//----------------------------------------------------------------------------

const int ECCM_UPDATE_RATE = TICKS_PER_SECOND * 5;

//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_FACTORY( GadgetECCM, NounGadget ); 
REGISTER_FACTORY_KEY( GadgetECCM, 4137158420330050133LL );

BEGIN_ABSTRACT_PROPERTY_LIST( GadgetECCM, NounGadget );
	ADD_REPLICATED_PROPERTY( m_Flags, TICKS_PER_SECOND );
END_PROPERTY_LIST();

GadgetECCM::GadgetECCM() : m_nUpdateTick( 0 )
{}

//----------------------------------------------------------------------------

void GadgetECCM::render( RenderContext &context, 
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

void GadgetECCM::release()
{
	NounGadget::release();

	// release affected object pointers
	m_Affect.release();
}

static bool isPlatform( NounGame * pNoun )
{
	return WidgetCast<ShipPlatform>( pNoun ) != NULL;
}

void GadgetECCM::simulate( dword nTick )
{
	if ( active() && nTick >= m_nUpdateTick )
	{
		m_nUpdateTick = nTick + ECCM_UPDATE_RATE;
		m_Affect.release();

		float fRange = range() * calculateModifier( MT_EWAR_RANGE );
		// affect the ECM of all ships within range
		Array< GameContext::NounCollision > nouns;
		if ( context()->proximityCheck( worldPosition(), fRange, nouns ) )
		{
			for(int i=0;i<nouns.size();i++)
			{
				NounGame * pAffect = WidgetCast<NounGame>( nouns[ i ].pNoun );
				if ( pAffect != NULL && pAffect->enableECM() && isEnemy( pAffect )  )
				{
					// give 1 point of scout per minute of ECCM coverage on an enemy
					if ( !isPlatform( pAffect ) )
					{
						gameContext()->gameUser()->onScout( parentNoun(), 
							ECCM_PRESTIGE_SCALE * ((float)ECCM_UPDATE_RATE / (float)TICKS_PER_MINUTE) );	
					}

					m_Affect.push( pAffect );
				}
			}
		}
	}

	NounGadget::simulate( nTick );
}

//----------------------------------------------------------------------------

NounGadget::Type GadgetECCM::type() const
{
	return SPECIAL_OFFENSIVE;
}

dword GadgetECCM::hotkey() const
{
	return 'P';
}

CharString GadgetECCM::useTip( Noun * pTarget, bool shift ) const
{
	CharString tip;
	float fMod = calculateModifier( MT_EWAR_STRENGTH );
	tip += CharString().format("\nSignature:<X;100>+%.1f", strength() * fMod );
	fMod = calculateModifier( MT_EWAR_RANGE );
	tip += CharString().format("\nRange:<X;100>%.0f", range() * fMod );	
	fMod = calculateModifier( MT_EWAR_ENERGY );
	tip += CharString().format("\nEnergy Usage p/s:<X;100>%.1f", ( ( energyCost() * fMod ) * TICKS_PER_SECOND ) / 1000.0f );
	
	return tip;
}

bool GadgetECCM::usable( Noun * pTarget, bool shift ) const
{
	if ( active() )
		return true;
	if (! NounGadget::usable( pTarget, shift ) )
		return false;
	if ( destroyed() )
		return false;

	return true;
}

bool GadgetECCM::useActive() const
{
	return active();
}

void GadgetECCM::use( dword when, Noun * pTarget, bool shift)
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

int GadgetECCM::useEnergy( dword nTick, int energy )
{
	if ( active() )
	{
		energy -= energyCost() * calculateModifier( MT_EWAR_ENERGY, true);
		if ( energy >= 0 )
		{
			float fRange = range() * calculateModifier( MT_EWAR_RANGE );
			if ( fRange > 0.0f )
			{
				// modify the signature on all affected objects
				for(int i=0;i<m_Affect.size();i++)
				{
					NounGame * pAffect = WidgetCast<NounGame>( m_Affect[i] );
					if ( pAffect != NULL )
					{
						float fDistance = (worldPosition() - pAffect->worldPosition()).magnitude();
						if ( fDistance < fRange )
						{
							pAffect->addSignature( nTick, damageRatioInv() * ( strength() * calculateModifier(MT_EWAR_STRENGTH) ) );
						}
					}
				}
			}
		}
	}

	return energy;
}

bool GadgetECCM::updateLogic()
{
	if ( WidgetCast<NounShip>( parent() ) )
	{
		NounShip * pShip = (NounShip *)parent();
		if ( useActive() )
			pShip->useGadget( this, NULL, false );		

		return true;
	}
	else if ( WidgetCast<StructureDefense>( parent() ) )
	{
		StructureDefense * pStructure = (StructureDefense *)parent();
		if ( pStructure->active() && !useActive() )
			pStructure->useGadget( NULL, this );		// turn on ECCM 
		else if ( !pStructure->active() && useActive() )
			pStructure->useGadget( NULL, this );		// turn off ECCM

		return true;
	}

	return false;
}

//----------------------------------------------------------------------------
// EOF
