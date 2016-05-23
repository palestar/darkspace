/*
	GadgetShield.cpp
	Base class for shields
	(c)2000 Palestar, Richard Lyle
*/


#include "Debug/Trace.h"
#include "Debug/Assert.h"
#include "System/Messages.h"
#include "SceneryEffect.h"
#include "GadgetShield.h"
#include "GameContext.h"
#include "resource.h"

//----------------------------------------------------------------------------

static Constant DEFENSE_MODE_CHARGE_MOD( "DEFENSE_MODE_CHARGE_MOD", 4.0f );

IMPLEMENT_ABSTRACT_FACTORY( GadgetShield, NounGadget ); 
REGISTER_FACTORY_KEY( GadgetShield, 4099634614070272882LL );

BEGIN_ABSTRACT_PROPERTY_LIST( GadgetShield, NounGadget );
	ADD_REPLICATED_PROPERTY( m_Flags, TICKS_PER_SECOND );
	ADD_REPLICATED_PROPERTY( m_Charge, TICKS_PER_SECOND * 5 );
END_PROPERTY_LIST();

//----------------------------------------------------------------------------

GadgetShield::GadgetShield() :
	m_Charge( 0 ),
	m_fModifier( 1.0f ),
	m_fChargeRate( 0.0f ),
	m_Mode( 0 )		// default to on
{
	// turn on shields by default..
	m_Flags |= FLAG_ACTIVE;
}

//----------------------------------------------------------------------------

void GadgetShield::setup()
{
	NounGadget::setup();
	m_nDamage = 0;
	// we dont want shields on planets starting at 100% shielding
	if ( !WidgetCast<NounShip>( parent() ) )
		m_Charge = 0;
	else
		m_Charge = maxCharge();
	m_Flags |= FLAG_ACTIVE;
	m_Mode = 0;
}

void GadgetShield::initialize()
{
	NounGadget::initialize();
	m_Flags |= FLAG_ACTIVE;
	m_Mode = 0;
}

//----------------------------------------------------------------------------

NounGadget::Type GadgetShield::type() const
{
	return SHIELD;
}

NounGadget::EnergyClass GadgetShield::energyClass() const
{
	return ENERGY_CLASS_DEFENSE;
}

dword GadgetShield::hotkey() const
{
	if ( (flags() & FLAG_MOUNT_FORE) == (flags() & FLAG_MOUNT_FULL) )
		return HK_NUMPAD8;
	else if ( (flags() & FLAG_MOUNT_RIGHT) == (flags() & FLAG_MOUNT_FULL) )
		return HK_NUMPAD6;
	else if ( (flags() & FLAG_MOUNT_LEFT) == (flags() & FLAG_MOUNT_FULL) )
		return HK_NUMPAD4;
	else if ( (flags() & FLAG_MOUNT_AFT) == (flags() & FLAG_MOUNT_FULL) )
		return HK_NUMPAD2;

	return HK_NUMPAD5;
}

CharString GadgetShield::status() const
{
	return CharString().format("%d%%", (m_Charge * 100) / maxCharge());
}

CharString GadgetShield::description() const
{
	CharString sDesc( NounGadget::description() );
	return sDesc;
}

bool GadgetShield::canDamage( dword type ) const
{
	return false;
}

CharString GadgetShield::useTip( Noun * pTarget, bool shift ) const
{
	CharString tip;
	if ( shift )
	{
		switch( m_Mode )
		{
		case 0:
			tip = "Deactivate Shields\n";
			break;
		case 1:
			tip = "Activate Shields\n";
			break;
		}
	}
	else
		tip = "Reinforce Shield\n";
	
	// usage information
	tip += CharString().format("\nCurrent HP:<X;100>%d/%d", m_Charge, maxCharge() );
	float fMod = ( calculateModifier( MT_DEFENSE ) * 100 ) - 100;
	if ( fMod > 0 )
		tip += CharString().format("\nDamage Reduction:<X;100>%.1f%%", fMod );
	fMod = calculateModifier( MT_DEFENSE_RECHARGE );
	float fChargeRate = chargeRate() * fMod;
	NounShip * pShip = WidgetCast<NounShip>( parent() );
	if ( pShip != NULL && pShip->isOutOfCombat() )
		fChargeRate *= DEFENSE_MODE_CHARGE_MOD;
	tip += CharString().format("\nCharge Rate p/s:<X;100>%.1f", fChargeRate * TICKS_PER_SECOND );
	tip += CharString().format("\nEnergy Cost p/s:<X;100>%.1f", ( energyCost() * TICKS_PER_SECOND ) / 1000.0f );

	return tip;
}

bool GadgetShield::usable( Noun * pTarget, bool shift ) const
{
	if (! NounGadget::usable( pTarget, shift ) )
		return false;
	if ( destroyed() )
		return false;
	return true;
}


int	GadgetShield::useModeCount() const
{
	return 2;
}

int GadgetShield::useMode() const
{
	return m_Mode;
}


bool GadgetShield::useActive() const
{
	return active();
}

int	lessChargeFirst( GadgetShield * pS1, GadgetShield * pS2 )
{
	return pS1->charge() - pS2->charge();	
}

void GadgetShield::useMode( int mode)
{
	m_Mode = mode;	

	switch( m_Mode )
	{
	case 0:	// normal mode
		setFlags( FLAG_ACTIVE, true );
		break;
	case 1:	// off
		setFlags( FLAG_ACTIVE, false );
		break;
	}
}

void GadgetShield::use( dword when, Noun * pTarget, bool shift)
{
	NounGadget::use( when, pTarget, shift );

	// create use effect
	createUseEffect();
	if ( ! shift )
	{
		int needed = maxCharge() - m_Charge;
		if ( needed > 0 )
		{
			// suck energy from other shield facings into this one
			BaseNode * pParent = parent();
			if ( pParent != NULL )
			{
				Array<GadgetShield *> arShields;
				// get all available shield facings and sum up their energy
				int energypool = 0;
				for(int i=0;i<pParent->childCount();i++)
				{
					GadgetShield * pShield = WidgetCast<GadgetShield>( pParent->child(i) );
					if ( pShield == NULL || (pShield->mountFlags() & mountFlags()) != 0 )
						continue;		// ignore shields on the same facing

					arShields.push( pShield );
					energypool += pShield->m_Charge;
				}
					
				// sort them, those with lowest charge first
				arShields.qsort( lessChargeFirst );
				
				// each use should refill this shield facing by 20%, or up to maximum if less
				needed = Min<int>( needed, maxCharge() * 0.2f );
				int shieldCount = arShields.size();
				for( int i = 0;i<shieldCount;i++)
				{
					GadgetShield * pShield = arShields[i];
					energypool -= pShield->m_Charge;				// this shield will be used and no longer add to the pool
					float equalpart = needed / ( shieldCount - i );	// every shield needs to give that much energy so the needed charge is reached
					float loadbalancer = 1.0f;						// have less than the others -> give less than the others
					if( i + 1 < shieldCount && needed < energypool )// isn't the last shield and have more than enough energy ?
					{	
						float averageenergy = energypool / ( shieldCount - ( i + 1 ) );	// average energy each shield remaining in the pool has
						ASSERT( pShield->m_Charge <= averageenergy );					// this array is sorted
						loadbalancer = pShield->m_Charge / averageenergy;	// only give a fair share, thus after giving energy to
																			// the same shield multiple times, all remaining shields
																			// will hold the same amount of energy
					}
					
					ASSERT( loadbalancer <= 1.0f );
					int suck = Min<int>( pShield->m_Charge, equalpart * loadbalancer );
					pShield->m_Charge = pShield->m_Charge - suck;
					// add to our charge
					m_Charge += suck;
					// subtract from the needed charge
					needed -= suck;
				}
			}
		}
	}
	else
	{
		// move to the next use mode..
		useMode( (useMode() + 1) % useModeCount() );
	}
}

int GadgetShield::useEnergy( dword nTick, int energy )
{
	if ( active() )
	{
		int nCost = energyCost();
		if ( energy >= nCost )
		{
			// subtract energy cost from shield charge
			energy -= nCost;

			// charge the shield
			if ( m_Charge < maxCharge() && energy >= chargeEnergyCost() )
			{
				energy -= chargeEnergyCost();

				float fMod = calculateModifier( MT_DEFENSE_RECHARGE );
				m_fChargeRate += chargeRate() * fMod;
				
				// check to see if the ship is out of combat, if so, boost the recharge rate
				NounShip * pShip = WidgetCast<NounShip>( parent() );
				if ( pShip != NULL && pShip->isOutOfCombat() )
					m_fChargeRate *= DEFENSE_MODE_CHARGE_MOD;
					
				int chargeRate = floor( m_fChargeRate );
				m_fChargeRate -= chargeRate;		// leave fractional amount in the float for next update..
				int charge = Min( maxCharge() - m_Charge, chargeRate );
					
				m_Charge += charge;
			}
		}
	}

	return energy;
}

//----------------------------------------------------------------------------

int GadgetShield::deflect( dword nType, int damage, Facing facing, const Vector3 & vPosition )
{
	if ( !active() || !canDeflect( nType ) )
		return damage;	// shield not active or doesn't deflect this type of damage
	if ( (facingMountFlags( facing ) & mountFlags()) == 0 )
		return damage;	// wrong facing

	float fMod = damageModifier( nType ) * calculateModifier( MT_DEFENSE, true );
	if ( fMod <= 0.0f )
		return 0;		// full damage deflected!	

	int nModifiedDamage = fMod * damage;
	int nDeflected = Min( nModifiedDamage, m_Charge );
	m_Charge -= nDeflected;

	// show hit effect
	// createHitEffect( vPosition );

	// divide the deflected damage amount by the modifier, to convert back into normal units
	return damage - (nDeflected / fMod);
}

static Vector3 FACING_VECTOR[] = 
{
	Vector3( 0, 0, 1 ),		// FRONT
	Vector3( 1, 0, 0 ),		// RIGHT
	Vector3( -1, 0, 0 ),	// LEFT
	Vector3( 0, 0, -1 ),	// BACK
	Vector3( 0, 1, 0),		// FULL
};

void GadgetShield::createHitEffect( const Vector3 & vDirection )
{
	// create effect client side only
	if ( context()->isClient() && parent() != NULL )
	{
		// show shield hit effect
		SceneryEffect * pHit = new SceneryEffect;
		pHit->setNounContext( new NounContext(hitEffect()) );
		pHit->setContext( context() );

		// set the frame
		pHit->setFrame( Matrix33( vDirection ) );
		// set the position
		pHit->setPosition( vDirection * parent()->radius() );
		// set the animation time
		pHit->setSegment( "Effect" );

		// attach this effect to our parent object
		parent()->attachNode( pHit );
		//context()->attachNoun( pHit, this );	// attach the hit effect to the ship
	}
}

void GadgetShield::setCharge( int charge )
{
	m_Charge = charge;
}

void GadgetShield::setModifier( float fModifier )
{
	m_fModifier = fModifier;
}

//----------------------------------------------------------------------------
// EOF
