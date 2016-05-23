/*
	GadgetTECCM.cpp
	(c)2000 Palestar, Richard Lyle
*/


#include "Debug/Assert.h"
#include "GadgetTECCM.h"
#include "GameContext.h"
#include "resource.h"
#include "StructureDefense.h"
#include "VerbGadgetOff.h"
#include "ShipPlatform.h"

//---------------------------------------------------------------------------------------------------

static Constant ECCM_PRESTIGE_SCALE( "ECCM_PRESTIGE_SCALE", 1.0f );

//----------------------------------------------------------------------------

const int ECCM_UPDATE_RATE = TICKS_PER_SECOND * 5;

//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_FACTORY( GadgetTECCM, NounGadget ); 

BEGIN_ABSTRACT_PROPERTY_LIST( GadgetTECCM, NounGadget );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Target);
	ADD_REPLICATED_PROPERTY( m_Flags, TICKS_PER_SECOND );
END_PROPERTY_LIST();

GadgetTECCM::GadgetTECCM() : m_nUpdateTick( 0 )
{}

//----------------------------------------------------------------------------

void GadgetTECCM::render( RenderContext &context, 
				const Matrix33 & frame, 
				const Vector3 & position )
{
	if ( active() )
	{
		Scene * pUseEffect = useEffect();
		if ( pUseEffect != NULL )
			pUseEffect->render( context, frame, position );
			
		Scene * pHitEffect = hitEffect();
		if ( pHitEffect != NULL && m_Target.valid() && !m_Target->testFlags( NounShip::FLAG_CLOAKED ) )
			pHitEffect->render( context, m_Target->worldFrame(), m_Target->worldPosition() );
	}
}

//----------------------------------------------------------------------------

void GadgetTECCM::release()
{
	NounGadget::release();
}

static bool isPlatform( NounGame * pNoun )
{
	return WidgetCast<ShipPlatform>( pNoun ) != NULL;
}

void GadgetTECCM::simulate( dword nTick )
{
	if ( active() && nTick >= m_nUpdateTick )
	{
		m_nUpdateTick = nTick + ECCM_UPDATE_RATE;

		// give 1 point of scout per minute of ECCM coverage on an enemy
		if ( m_Target.valid() && !isPlatform( m_Target ) )
		{
			gameContext()->gameUser()->onScout( parentNoun(), 
				ECCM_PRESTIGE_SCALE * ((float)ECCM_UPDATE_RATE / (float)TICKS_PER_MINUTE) );	
		}
	}

	NounGadget::simulate( nTick );
}

//----------------------------------------------------------------------------

NounGadget::Type GadgetTECCM::type() const
{
	return SPECIAL_OFFENSIVE;
}

dword GadgetTECCM::hotkey() const
{
	return 'P';
}

CharString GadgetTECCM::useTip( Noun * pTarget, bool shift ) const
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

bool GadgetTECCM::usable( Noun * pTarget, bool shift ) const
{
	if ( active() )
		return true;
	if (! NounGadget::usable( pTarget, shift ) )
		return false;
	if ( destroyed() )
		return false;
	if ( WidgetCast<NounShip>( pTarget ) == NULL )
		return false;		// can only target ships
	if ( ((NounShip *)pTarget)->isDestroyed() )
		return false;		// can't target a destroyed ship
	if ( pTarget == parent() )
		return false;		// can't target ourself
	if ( shift )
		return false;
	if ( isFriend( pTarget) )
		return false;

	float distance = (worldPosition() - pTarget->worldPosition()).magnitude();
	if ( distance > (range() * calculateModifier( MT_EWAR_RANGE )) )
		return false;		

	return true;
}

bool GadgetTECCM::useActive() const
{
	return active();
}

void GadgetTECCM::use( dword when, Noun * pTarget, bool shift)
{
	if ( active() )
	{
        NounGadget::use( when, pTarget, shift );
		if( isServer() )
		{
			setFlags( FLAG_ACTIVE, false );
			message( CharString().format( "<color;ffffff>Tactical: %s Inactive", name() ), true );  
		}
		return;
	}

	if(!shift && pTarget != NULL)
	{
		if(WidgetCast<NounShip>( pTarget ) != NULL)
		{
            m_Target = (NounShip *)pTarget;
			if ( isServer() )
			{
				setFlags( FLAG_ACTIVE, true );
				message( CharString().format( "<color;ffffff>Tactical: %s activated at %s.", name(), m_Target->name() ), true );  
			}
		}
	}
}

int GadgetTECCM::useEnergy( dword nTick, int energy )
{
	if ( active() )
	{
		bool bTurnOff = true;

		if ( m_Target.valid() && !m_Target->isDestroyed() )
		{
			int nCost = energyCost() * calculateModifier( MT_EWAR_ENERGY, true);
			float fDistance = (worldPosition() - m_Target->worldPosition()).magnitude();
			float fRange = range() * calculateModifier( MT_EWAR_RANGE );

			if ( fDistance <= fRange && energy >= nCost )
			{
				bTurnOff = false;

				energy -= nCost;
				m_Target->addSignature( nTick, damageRatioInv() * ( strength() * calculateModifier(MT_EWAR_STRENGTH) ) );
			}
		}

		if ( bTurnOff && isLocal() )
	        Verb::Ref( new VerbGadgetOff( this ) );
	}

	return energy;
}

bool GadgetTECCM::updateLogic()
{
	if ( WidgetCast<NounShip>( parent() ) )
	{
		NounShip * pShip = (NounShip *)parent();
		NounGame * pTarget = WidgetCast<NounGame>( pShip->commandTarget() );
		
		if (! pTarget )
			return true;	// no current target
		if ( usable( pTarget, false ) )
			return true;	// not usable on current target

		switch( pShip->command() )
		{
		case NounShip::ATTACK:
			// turn on TECCM when attacking target
			if ( !useActive() || m_Target != pTarget )
				pShip->useGadget( this, pTarget, false );		
			break;
		default:
			// turn off TECCM
			if ( useActive() )
				pShip->useGadget( this, NULL, false );	
			break;
		}		
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------
// EOF
