/*
	NounFighter.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include <algorithm>

#include "Debug/Assert.h"
#include "Math/Helpers.h"
#include "World/TraitMovement.h"

#include "NounFighter.h"
#include "NounShip.h"
#include "NounNebula.h"
#include "GadgetBeamWeapon.h"
#include "GadgetFighterBay.h"
#include "GameContext.h"
#include "GadgetScanner.h"
#include "GadgetECM.h"
#include "GadgetECCM.h"
#include "VerbUseGadget.h"
#include "VerbUseSingleGadget.h"

//---------------------------------------------------------------------------------------------------

const int		FIGHTER_UPDATE_RATE = TICKS_PER_SECOND * 1.5f;
const int		FIGHTER_ENERGY = 100;
const float		FIGHTER_EVADE_DISTANCE = 400.0f;			// distance when to begin evading target
const float		FIGHTER_ATTACK_DISTANCE = 800.0f;			// distance when evade is switched back to attack mode
const float		FIGHTER_GROUPING_DISTANCE = 170.0f;			// maximum distance between two figthers to enter formation
const float		FIGHTER_LEADER_DISTANCE = 70.0f;			// distance fighter maintains to it's group leader
const float		FIGHTER_BREAK_FORMATION_DISTANCE = 400.0f;	// distance to target at which fighters break their formation
const float		FIGHTER_ROLL_RATE = (PI * 2) / TICKS_PER_SECOND;

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounFighter, NounSmartProjectile );
REGISTER_FACTORY_KEY( NounFighter, 4296314509346749013LL );

BEGIN_PROPERTY_LIST( NounFighter, NounSmartProjectile )
	ADD_TRANSMIT_PROPERTY( m_Name );
	ADD_REPLICATED_PROPERTY( m_nState, TICKS_PER_SECOND * 2 );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Children );
END_PROPERTY_LIST();

NounFighter::NounFighter() : m_nState( ATTACK ), m_bDestroyed( false ), m_nUpdateTick( 0 )
{
	attachTrait( new TraitMovement( true ) );
}

//---------------------------------------------------------------------------------------------------

bool NounFighter::read( const InStream & input )
{
	if (! NounSmartProjectile::read( input ) )
		return false;

	if (! findTrait( TraitMovement::classKey() ) )
		attachTrait( new TraitMovement( true ) );

	return true;
}

//----------------------------------------------------------------------------

void NounFighter::onDetaching()
{
	if ( !m_bDestroyed )
	{
		GadgetWeapon * pWeapon = weapon();
		if ( pWeapon != NULL )
			pWeapon->setAmmo( pWeapon->ammo() + 1 );
	}
	NounSmartProjectile::onDetaching();
}

//----------------------------------------------------------------------------

NounType NounFighter::nounType() const
{
	return TYPE_LOCAL_OBJECT;
}

void NounFighter::collision( Noun * pCollide, const Vector3 & intersect )
{
	if ( WidgetCast<NounBody>( pCollide ) )
	{
		if ( pCollide == owner() )
		{
			if ( m_nState == RETURN && isServer() )
				setDetach();

			return;
		}

		float fDistance = (m_Position - pCollide->position()).magnitude();
		float R2 = radius() + pCollide->radius();

		if ( fDistance < R2 )
		{
			float fInside = R2 - fDistance;
			m_Position.y += fInside;
		}
	}
	else if ( WidgetCast<NounFighter>( pCollide ) )
	{
		// push this fighter outside the radius of the other fighter
		Vector3 delta( m_Position - pCollide->position() );
		float fDistance = delta.magnitude();
		float R2 = radius() + pCollide->radius();

		if ( fDistance < R2 )
		{
			delta.normalize();
			m_Position += delta * (R2 - fDistance);
		}
	}
}

void NounFighter::simulate( dword nTick )
{
	if ( isServer() )
	{
		if ( nTick >= m_nUpdateTick )
		{
			bool bFireWeapons = false;

			m_nUpdateTick = nTick + (TICKS_PER_SECOND * 1);
			switch( m_nState )
			{
			case ATTACK:
				if ( validateTarget( m_pTarget ) )
				{
					if ( isFriend( m_pTarget ) )
					{
						setState( PROTECT );
						break;
					}
					// calculate an intercept position
					Vector3 target;
					intercept( m_pTarget->worldPosition(), m_pTarget->worldVelocity(), target );

					// get the distance before we normalize
					Vector3 delta( target - worldPosition() );
					float distance = delta.magnitude();

					// normalize and turn towards our target
					delta.normalize();
					control( delta );

					// fire weapons!
					bFireWeapons = true;

					if ( (distance - m_pTarget->radius()) < FIGHTER_EVADE_DISTANCE )
						setState( EVADE );
				}
				else
				{
					// target gone, roll and return home..
					setState( ROLL );
				}
				break;
			case EVADE:
				if ( validateTarget( m_pTarget ) )
				{
					if ( isFriend( m_pTarget ) )
					{
						setState( PROTECT );
						break;
					}
					
					Vector3 target( m_pTarget->worldPosition() );
					Vector3 delta( worldPosition() - target );
					float distance = delta.magnitude();

					delta.normalize();
					control( delta );

					if ( (distance - m_pTarget->radius()) > FIGHTER_ATTACK_DISTANCE )
						setState( ATTACK );
				}
				else
				{
					// target gone..
					setState( ROLL );
				}
				break;
			case ROLL:
				setState( RETURN );
			case RETURN:
				if ( validateTarget( m_pTarget ) )
				{
					if ( isFriend( m_pTarget ) )
						setState( PROTECT );
					else
						setState( ATTACK ); // we've reacquired a target
				}
				else if ( owner() != NULL )
				{
					// calculate the intercept position
					Vector3 target;
					intercept( owner()->worldPosition(), owner()->worldVelocity(), target );
					// get the delta from our current position
					Vector3 delta( target - worldPosition() );
					// control this fighter
					delta.normalize();
					control( delta );
				}
				else
				{
					// base lost... destroy this fighter..
					// TODO: let fighter look for a new home
					LOG_DEBUG_HIGH( "NounFighter", "owner is NULL, self-destructing." );
					m_nLife = 0;
				}
				break;
			case PROTECT:
				if ( validateTarget( m_pTarget ) )
				{
					if (! isFriend( m_pTarget ) )
					{
						setState( ATTACK );
						break;
					}
					// calculate our target position			
					Vector3 offset( RandomVector( -1.0f, 1.0f ) );
					offset.normalize();
					Vector3 targetPosition( m_pTarget->worldPosition() + ( offset * ( m_pTarget->radius() * 1.5 ) ) );				

					Vector3 target;
					intercept( targetPosition, m_pTarget->worldVelocity(), target );

					// get the distance before we normalize
					Vector3 delta( target - worldPosition() );
					// normalize and turn towards our target
					delta.normalize();
					control( delta );
				}
				else
					setState( RETURN );
				break;
			}

			// use gadgets
			Array< NounGadget *> use;
			for(int i=0;i<childCount();i++)
			{
				NounGadget * pGadget = WidgetCast<NounGadget>( child(i) );
				if (! pGadget )
					continue;
					
				if ( WidgetCast<GadgetWeapon>( pGadget ) )
				{
					if (bFireWeapons && pGadget->usable( m_pTarget, false ) )
						use.push( pGadget );
				}
				else if ( WidgetCast<GadgetBeamWeapon>( pGadget ) )
				{
					GadgetBeamWeapon * pBeamWeapon = (GadgetBeamWeapon *)pGadget;
					if ( pBeamWeapon->allowPointDefense() )
						updatePointDefense( pBeamWeapon );
					else if ( bFireWeapons && pBeamWeapon->usable( m_pTarget, false) )
						use.push( pGadget );
				}
				else if( WidgetCast<GadgetECCM>( pGadget ) 
					|| WidgetCast<GadgetECM>( pGadget ) 
					|| WidgetCast<GadgetScanner>( pGadget ) )
				{
					if ( !pGadget->useActive() && pGadget->usable( m_pTarget, false ) )
						use.push( pGadget );
				}
			}

			if ( use.size() > 0 )
				Verb::Ref( new VerbUseGadget( this, use, m_pTarget, false ) );

			// check the distance from home, and determine when we need to turn around and head back to make it in time
			if ( owner() != NULL )
			{
				float fDistance = (owner()->worldPosition() - worldPosition()).magnitude();
				float fTime = fDistance / m_fVelocity;

				// once our life is less than the amount of time it will take to get back home,
				// we turn around and head back to our parent..
				if ( m_nLife < ((int)timeTick(fTime * 0.85f)) )
					setState( RETURN );
			}
			else
				explode();
		}

		Vector3 vOrientation( orientation() );

		// update fighter roll
		if ( m_nState != ROLL )
			vOrientation.z -= (vOrientation.z * 0.05f);	
		else
			vOrientation.z += FIGHTER_ROLL_RATE * TICK_DURATION_S;
		
		float fPitch = Clamp( m_fPitch, -m_fTurnRate, m_fTurnRate ) * TICK_DURATION_S;
		m_fPitch -= fPitch;
		vOrientation.x += fPitch;

		float fYaw = Clamp( m_fYaw, -m_fTurnRate, m_fTurnRate ) * TICK_DURATION_S;
		m_fYaw -= fYaw;
		vOrientation.y += fYaw;

		setOrientation( vOrientation );
		float fDistance = 0;
		if ( m_pTarget != NULL )
			fDistance = (m_pTarget->worldPosition() - worldPosition()).magnitude();

		if ( state() == PROTECT && fDistance < 200 )
			setVelocity( m_Frame.k * ( m_fVelocity * 0.5f ) );
		else
			setVelocity( m_Frame.k * m_fVelocity );

		// update gadgets
		for(int i=0;i<childCount();i++)
		{
			NounGadget * pGadget = WidgetCast<NounGadget>( child(i) );
			if ( pGadget != NULL )
				pGadget->useEnergy( nTick, FIGHTER_ENERGY );
		}

		// Not calling NounSmartProjectile::simulate( dword nTick ) on purpose...
		NounProjectile::simulate( nTick );
	}
	else
	{
		// update the trail if we have one
		if ( isClient() && (flags() & FLAG_DO_TRAIL) != 0 )
			m_Trail.updateTrail( worldPosition() );

		// update gadgets
		for(int i=0;i<childCount();i++)
		{
			NounGadget * pGadget = WidgetCast<NounGadget>( child(i) );
			if ( pGadget != NULL )
				pGadget->useEnergy( nTick, FIGHTER_ENERGY );
		}

		Noun::simulate( nTick );
	}
}

void NounFighter::explode()
{
	m_bDestroyed = true;

	GadgetWeapon * pWeapon = weapon();
	if ( pWeapon != NULL )
		pWeapon->onProjectileDead( this );

	NounProjectile::explode();
}

//----------------------------------------------------------------------------

NounFighter::State NounFighter::state() const
{
	return (NounFighter::State)m_nState;
}

void NounFighter::setState( State state )
{
	m_nState = state;
}

void NounFighter::updatePointDefense( GadgetBeamWeapon * pCheck )
{
	if ( isServer() )
	{
		NounShip * pShip = (NounShip *)owner();
		if ( pShip == NULL )
			pShip = WidgetCast<NounShip>( m_pTarget );
			
		if ( pShip != NULL )
		{	
			for(int h=0;h<pShip->contactCount();++h)
			{		
				NounGame * pContact = (NounGame *)pShip->contact( h );
				if ( pContact != NULL && pContact->enablePD() )
				{
					if ( isFriend( pContact ) )
						continue;

					// any other weapons on this ship have this projectile as their target
					GadgetBeamWeapon * pBeam = NULL;
					bool bTargeted = false;
					
					if ( pCheck != NULL )
					{
						if ( pCheck->usable( pContact, false ) )
						{
							// we've found a usable beam that meets our criteria, set it for later
							pBeam = pCheck;
							
							// we need to check and see if our projectile is targetted by another fighter now that we've found a usable beam
							// fill our vector list with fighterbay's from the players ship
							std::vector< GadgetFighterBay * > fighterbay;
							FindNodeClasses( pShip, fighterbay );
							for(size_t j = 0; j < fighterbay.size(); ++j )
							{
								// get that fighter bays fighters
								Array< Noun::wRef > projectiles = fighterbay[ j ]->projectiles();
								for( int k = 0; k < projectiles.size(); ++k )
								{
									NounFighter * pFighter = (NounFighter *)projectiles[ k ].pointer();
									if ( pFighter != NULL )
									{
										// itterate through the fighters weapons and find its PD beam and check if it's targetting our target
										for( int l=0;l<pFighter->childCount();++l )
										{
											GadgetBeamWeapon * pFighterBeam = NULL;
											if ( WidgetCast<GadgetBeamWeapon>( pFighter->child( l ) ) )
												pFighterBeam = WidgetCast<GadgetBeamWeapon>( pFighter->child( l ) );
											if ( pFighterBeam != NULL )
												if ( pFighterBeam->target() == pContact )
													bTargeted = true;
										}
									}
								}
							}
						}
					}

					// if projectile not targeted, check for usability
					if (! bTargeted && pBeam != NULL )
					{
						// fire weapon at target, set the target first so no other beam weapons attempt to fire
						// at the same projectile this loop
						pBeam->setTarget( pContact );
						// push gadget into the array so it will be used by the calling function
						new VerbUseSingleGadget( pBeam, pContact );
					}
				}
			}
		}
	}
}

//----------------------------------------------------------------------------
//EOF
