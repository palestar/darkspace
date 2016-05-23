/*
	GadgetJumpGate.cpp
	(c)2004 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Math/Helpers.h"
#include "World/NounZoneTarget.h"
#include "GadgetJumpGate.h"
#include "NounJumpGate.h"
#include "NounStar.h"
#include "NounAsteroid.h"
#include "GameContext.h"
#include "resource.h"

//----------------------------------------------------------------------------

Constant	GADGET_JG_ORIGIN_OFFSET( "GADGET_JG_ORIGIN_OFFSET", 50.0f );
const int	MAX_JUMPGATE_ATTEMPTS = 10;

//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_FACTORY( GadgetJumpGate, NounGadget ); 
REGISTER_FACTORY_KEY( GadgetJumpGate, 4626281911507602914LL );

BEGIN_ABSTRACT_PROPERTY_LIST( GadgetJumpGate, NounGadget );
	ADD_REPLICATED_PROPERTY( m_nEnergy, TICKS_PER_SECOND * 10 );
END_PROPERTY_LIST();

GadgetJumpGate::GadgetJumpGate() : m_nEnergy( 0 ), m_nEnergyTick( 0 )
{}

//----------------------------------------------------------------------------

void GadgetJumpGate::setup()
{
	NounGadget::setup();
	m_nEnergy = energyNeeded();
	m_nEnergyTick = tick();
}

void GadgetJumpGate::initialize()
{
	NounGadget::initialize();
	m_nEnergyTick = tick();
}

//----------------------------------------------------------------------------

NounGadget::Type GadgetJumpGate::type() const
{
	return JUMP_GATE;
}

dword GadgetJumpGate::hotkey() const
{
	return 'Y';
}

CharString GadgetJumpGate::useTip( Noun * pTarget, bool shift ) const
{
	CharString tip;
	if ( shift )
		return "Emergency Wormhole Generation.";	
	// usage information
	float fMod = calculateModifier( MT_JUMPGATE_RANGE );
	tip += CharString().format("\nMax Distance:<X;100>%.0fgu", range() * fMod );
	tip += CharString().format("\nRecharge Time:<X;100>%.1fs", energyNeeded() / ( ( chargeRate() * TICKS_PER_SECOND ) * damageRatioInv() ) );
	tip += CharString().format("\nEnergy Usage:<X;100>%.1f", energyNeeded() / 1000.0f );		
	
	return tip;
}

//----------------------------------------------------------------------------

int GadgetJumpGate::usableWhen() const
{
	return Min<int>( (100 - ((m_nEnergy * 100) / energyNeeded())), NounGadget::usableWhen() );
}

bool GadgetJumpGate::usable( Noun * pTarget, bool shift ) const
{
	NounShip * pShip = WidgetCast<NounShip>( parent() );
	if ( pShip == NULL )
		return false;		// must be attached to a ship to work
	if (! NounGadget::usable( pTarget, shift ) )
		return false;
	if ( destroyed() )
		return false;		// destroyed
	if ( m_nEnergy < energyNeeded() )
		return false;		// still charging
	if ( !shift && !validateTarget( pTarget ) )
		return false;		// not a valid target
	if ( pTarget != NULL && pShip->context() != pTarget->context() )
		return false;		// target is in a different context (server)
	if ( gateContext() == NULL )
		return false;		// no gate context
	if ( parentBody() == NULL )
		return false;		// not attached to a NounBody object
	if ( pShip->testFlags( NounShip::FLAG_CLOAKED ) )
		return false;		// can't use WH if cloaked

	if( !shift )
	{
		float fRange = range() * calculateModifier( MT_JUMPGATE_RANGE );
		// check the range to the destination
		float fDistance = (worldPosition() - pTarget->worldPosition()).magnitude();
		if ( fDistance < minRange() || fDistance > fRange )
			return false;		// out of range

	}

	return true;
}

void GadgetJumpGate::use( dword when, Noun * pTarget, bool shift)
{
	NounGadget::use( when, pTarget, shift );

	if ( isServer() )
	{
		Noun * pParent = parentBody();
		ASSERT( pParent );

		// create the jumpgates
		NounJumpGate * pGate1 = new NounJumpGate;
		pGate1->setNounContext( gateContext() );
		pGate1->postInitialize();
		pGate1->setName( pGate1->nounContext()->name() );
		pGate1->setDuration( true, duration() );
		pGate1->setActive( false );						// set the gate to inactive, so it will create a use effect when it gets attached and activated
		pGate1->setTick( when );

		NounJumpGate * pGate2 = new NounJumpGate;
		pGate2->setNounContext( gateContext() );
		pGate2->postInitialize();
		pGate2->setName( pGate2->nounContext()->name() );
		pGate2->setDuration( true, duration() );
		pGate2->setActive( false );	
		pGate2->setTick( when );

		// link the gates
		pGate1->setDestination( pGate2 );
		pGate2->setDestination( pGate1 );
		
		// The variance factor increases the variance of the wormhole position up to factor 4, based on the creating ships speed.
		float fVarianceFactor = 1.0f;
		NounShip * pParentShip = WidgetCast< NounShip >( pParent );
		if( pParentShip )
		{
			if( pParentShip->velocity() > 20.0 ) 
				fVarianceFactor = 4.0f;
			else if( pParentShip->velocity() > 1.0 ) 
				fVarianceFactor = 1.0f + (0.15f * pParentShip->velocity());
		}

		if( shift )
		{
			// Find a target for the emergency gate. Target shouldn't be too close
			// Preference: Friendly planet with shipyard, without shipyard, random place in space
			Array< Noun * > arPlanets;
			Array< Noun * > arPlanetsSY;

			Array< GameContext::NounCollision > contacts;
			context()->proximityCheck( pParent->worldPosition(), 100000.0f, contacts, CLASS_KEY(NounPlanet) );

			for(int i=0;i<contacts.size();i++)
			{
				Noun * pContact = contacts[ i ].pNoun;
				if ( WidgetCast<NounPlanet>( pContact ) && pContact->isFriend( pParent ) && contacts[ i ].fDistance > minRange() )
				{
					arPlanets.push( pContact );
					
					if( ( ((NounPlanet*)pContact)->flags() & NounPlanet::FLAG_HAS_SHIPYARD ) != 0 )
						arPlanetsSY.push( pContact );
				}
			}

			// find a suitable target
			if ( arPlanetsSY.size() > 0 )
				pTarget = arPlanetsSY[ rand() % arPlanetsSY.size() ];
			else if ( arPlanets.size() > 0 )
				pTarget = arPlanets[ rand() % arPlanets.size() ];
		}

		NounZoneTarget::Ref pZoneTarget;
		if (! pTarget && shift )
		{
			Vector3 vTargetOffset( RandomVector( -1.0f, 1.0f ) );
			vTargetOffset.y = 0.0f;
			vTargetOffset.normalize();
			vTargetOffset *= RandomFloat( minRange(), range() * calculateModifier( MT_JUMPGATE_RANGE ) );

			pZoneTarget = new NounZoneTarget( pParent->worldPosition() + vTargetOffset, context() );
			pTarget = pZoneTarget;
		}

		// try to find a suitable place for the destination gate in 5 random places near the target before giving up
		int nAttempts = 0;
		bool bSucceeded = false;

		if ( pTarget != NULL )
		{
			while( !bSucceeded && ++nAttempts < MAX_JUMPGATE_ATTEMPTS )
			{	
				Vector3 vTargetPosition( pTarget->worldPosition() );
				vTargetPosition.y = 0.0f;

				Vector3 vDirection = RandomVector( -1, 1 );
				vDirection.y = 0.0f;
				vDirection.normalize();

				float fTargetOffset = (pTarget->radius() * 1.5f) + RandomFloat( 0.0f, variation() * fVarianceFactor );
				float fTargetRadius( pTarget->radius() );

				pGate1->setPosition( vTargetPosition + (vDirection * fTargetOffset ) );
				pGate1->setFrame( Matrix33( vDirection ) );
				
				Array< GameContext::NounCollision > collisions;
				if ( context()->proximityCheck( pGate1->position(), pGate1->radius() * 1.5f, collisions ) )
				{
					// we have collisions, check the objects, allow the gate if no planets, stars, or asteroids are colliding..
					bool bAbort = false;
					for( int i = 0 ; !bAbort && i < collisions.size() ; i++ )
					{
						if ( WidgetCast<NounStar>( collisions[i].pNoun ) ||
							WidgetCast<NounPlanet>( collisions[i].pNoun ) ||
							WidgetCast<NounAsteroid>( collisions[i].pNoun ) )
						{
							bAbort = true;
						}
					}

					bSucceeded = !bAbort;
				}
				else
				{
					// not a single noun overlapping the wormhole at this position, so success!
					bSucceeded = true;						
				}

			};	// end while
		}

		if ( bSucceeded )
		{
			// A suitable position for the destination WH was found.
			// However the fixed position for the WH in front of the creating ship might be blocked
			// So this needs to be checked too
			Vector3 vOriginPosition( pParent->worldPosition() );
			vOriginPosition.y = 0.0f;

			// now set the position of the origin gate
			float fOriginRadius = pParent->radius() + pGate2->radius() + GADGET_JG_ORIGIN_OFFSET;
			pGate2->setPosition( vOriginPosition + (pParent->frame().k * fOriginRadius ) );
			pGate2->setFrame( pParent->worldFrame() );
			
			// check if its too close to a jumpgate, planet or star
			Array< GameContext::NounCollision > collisions;
			if ( context()->proximityCheck( pGate2->position(), Min( pGate2->radius() * 1.5f, 110.0f ), collisions ) )
			{	
				for( int i = 0 ; i < collisions.size() ; i++ )
				{
					Noun * pNoun = collisions[i].pNoun;
					if( WidgetCast< NounPlanet>( pNoun ) != NULL
						|| WidgetCast< NounJumpGate>( pNoun ) != NULL 
						|| WidgetCast< NounStar>( pNoun ) != NULL )
					{
						bSucceeded = false;
						break;
					}
				}
			}

			if( bSucceeded )
			{
				// everythings still fine. Attach both WHs.
				ASSERT( pParent->zone() );

				// attach gate 1 before 2, because 1 is the destination
				context()->attachNoun( pGate1 );
				context()->attachNoun( pGate2 );

				m_nEnergy = 0;
			}
			else
			{
				pParent->privateChat( "Helm: Failed to create source wormhole!" );
			}
		}
		else
		{
			pParent->privateChat( "Helm: Failed to create destination wormhole!" );		
		}
	}

	createUseEffect( false );
	m_nEnergyTick = when;
}

int GadgetJumpGate::useEnergy( dword nTick, int energy )
{
	if ( m_nEnergy < energyNeeded() )
	{
		int nElapsed = nTick - m_nEnergyTick;
		m_nEnergyTick = nTick;

		int use =  damageRatioInv() * chargeRate();
		int charge = Min( Min( energyNeeded() - m_nEnergy, use ), energy ) * nElapsed;

		energy -= charge;
		m_nEnergy += charge;
	}

	return energy;
}

//----------------------------------------------------------------------------

NounContext * GadgetJumpGate::gateContext() const
{
	return WidgetCast<NounContext>( nounContext()->resource( "GATE" ) );
}

//----------------------------------------------------------------------------
//EOF
