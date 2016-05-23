/*
	GadgetBeamWeapon.cpp
	(c)1999 Palestar Development, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Display/PrimitiveSetTransform.h"
#include "Display/PrimitiveTriangleFan.h"
#include "System/Messages.h"
#include "World/NounTarget.h"
#include "Constants.h"
#include "SceneryEffect.h"
#include "NounNebula.h"
#include "NounField.h"
#include "NounProjectile.h"
#include "NounJumpGate.h"
#include "GadgetBeamWeapon.h"
#include "GameContext.h"
#include "StructureDefense.h"

//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_FACTORY( GadgetBeamWeapon, NounGadget ); 
REGISTER_FACTORY_KEY( GadgetBeamWeapon, 4099427496223188790LL );

BEGIN_ABSTRACT_PROPERTY_LIST( GadgetBeamWeapon, NounGadget );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Energy );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Duration );
	ADD_REPLICATED_PROPERTY( m_PointDefense, TICKS_PER_SECOND );
	ADD_PROPERTY( m_Target );
END_PROPERTY_LIST();

GadgetBeamWeapon::GadgetBeamWeapon() :
	m_Energy( 0 ), 
	m_Duration( -1 ), 
	m_PointDefense( false ), 
	m_Signature( 0 ), 
	m_nEnergyTick( 0 ), 
	m_fChargeRate( 0.0f ),
	m_bDetachHitEffect( true )
{}

//----------------------------------------------------------------------------

bool GadgetBeamWeapon::read( const InStream & input )
{
	if (! NounGadget::read( input ) )
		return false;
	if (! m_Target.valid() && m_Duration > 0 )
		m_Duration = 0;
	if( m_PointDefense && !allowPointDefense() )
		m_PointDefense = false;
	return true;
}

//----------------------------------------------------------------------------

void GadgetBeamWeapon::render( RenderContext &context, 
					const Matrix33 & frame, 
					const Vector3 & position )
{
	NounGadget::render( context, frame, position );

	// render the beam
	if ( useActive() && m_Target.valid() && m_Duration > 0 )
	{
		Vector3 positionVS( context.worldToView( position ) );
		float fLength = length() * calculateModifier( MT_BEAM_RANGE );
		if (! context.sphereVisible( positionVS, fLength ) )
			return;

		Noun * pTarget = m_Target;
		Vector3 position2( pTarget->worldPosition() );			
		
		if ( ( ( position2 - worldPosition() ).magnitude() - pTarget->radius() ) > fLength )
			return;		
		
		if ( !checkFacing( position2 ) )
			return;

		int tailAlpha = 32;

		if ( m_Hit.valid() )
			tailAlpha = 128;

		Vector3 direction( pTarget->worldPosition() - position );
		Vector3 head( position );
		Vector3 tail( head + direction );

		// calculate the material wrap
		float h = 0.05f;
		float w = 10.0f;

		Material * pTracerMaterial = tracerMaterial();
		if ( pTracerMaterial != NULL )
		{
			h = pTracerMaterial->height();
			w = pTracerMaterial->width();
			Material::push( context, pTracerMaterial );
		}
		else
			Material::push( context, Color(255,0,0), true, PrimitiveMaterial::ADDITIVE );

		float u = (head - tail).magnitude() / w;

		const Vector3	N( 0,0, 0);
		const Vector3	Y( 0, h, 0 );
		const Vector3	X( h, 0, 0 );
		const Color		HC( 255,255,255,255 );
		const Color		TC( 255,255,255,tailAlpha );

		VertexL beamY[4] = 
		{
			VertexL( head + Y, N, HC, u, 0.0f ),
			VertexL( tail + Y, N, TC, 0.0f, 0.0f ),
			VertexL( tail - Y, N, TC, 0.0f, 1.0f ),
			VertexL( head - Y, N, HC, u, 1.0f ),
		};
		VertexL beamX[4] = 
		{
			VertexL( head + X, N, HC, u, 0.0f ),
			VertexL( tail + X, N, TC, 0.0f, 0.0f ),
			VertexL( tail - X, N, TC, 0.0f, 1.0f ),
			VertexL( head - X, N, HC, u, 1.0f ),
		};
		
		DisplayDevice * pDisplay = context.device();
		ASSERT( pDisplay );

		context.pushIdentity();
		PrimitiveTriangleFanDL::push( pDisplay, 4, beamY );
		PrimitiveTriangleFanDL::push( pDisplay, 4, beamX );
	}
}

//----------------------------------------------------------------------------

void GadgetBeamWeapon::setup()
{
	NounGadget::setup();

	// start out with full energy
	m_Energy = energyCost();
	m_Duration = -1;
	m_Signature = 0;
}

void GadgetBeamWeapon::release()
{
	NounGadget::release();

	m_Hit = NULL;
	m_Target = NULL;
	m_Duration = -1;
}

void GadgetBeamWeapon::simulate( dword nTick )
{
	if ( m_Duration > 0 )
	{
		// reduce the beam duration
		m_Duration--;

		// validate the target
		bool targetValid = false;
		if ( WidgetCast<NounTarget>( m_Target.pointer() ) != NULL )
			targetValid = true;
		else if ( m_Target.valid() && m_Target->zone() != NULL )
			targetValid = true;

		if ( targetValid )
		{
			float fLength = length() * calculateModifier( MT_BEAM_RANGE );
			Vector3 beamOrigin( worldPosition() );
			Vector3 beamEnd( m_Target->worldPosition() );
			Vector3 direction( beamEnd - beamOrigin );
			direction.normalize();
			direction *= fLength;

			// check for collision
			Array< GameContext::NounCollision > check;
			if ( context()->proximityCheck( beamOrigin, fLength, check ) )
			{
				NounGame *		pCollide = NULL;
				float			maxDistance = fLength;
				float			collideDistance = maxDistance;
				Vector3			collidePosition = beamEnd;

				for(int i=0;i<check.size();i++)
				{
					NounGame * pCheck = WidgetCast<NounGame>( check[ i ].pNoun );
					if ( pCheck == NULL )
						continue;		
					if ( pCheck == (Noun *)parent() )
						continue;		// don't collide with our own ship!

					if ( WidgetCast<NounProjectile>( pCheck ) )
						if ( pCheck != m_Target )
							continue;		// don't destroy projectiles that are not our target!
					if ( WidgetCast<NounNebula>( pCheck ) || WidgetCast<NounField>( pCheck ) )
						continue;			// don't collide with nebula or fields

					// object is close, final check
					Vector3 intersect;
					if ( pCheck->intersect( beamOrigin, direction, intersect ) )
					{
						float distance = (beamOrigin - intersect).magnitude();
						if ( distance < collideDistance )
						{
							pCollide = pCheck;
							collideDistance = distance;
							collidePosition = intersect;
						}
					}
				}

				// does the projectile have a valid collision
				if ( pCollide != NULL )
				{
					// create the hit effect
					if ( context()->isClient() )
					{
						if (! m_Hit.valid() )
						{
							// create the hit effect
							SceneryEffect * pHit = new SceneryEffect;
							pHit->setNounContext( new NounContext( hitEffect() ) );
							pHit->setContext( context() );
							pHit->setPosition( collidePosition );
							pHit->setFrame( Matrix33( direction ) );
							pHit->setLife( m_Duration * TICK_DURATION_S );
							context()->attachNoun( pHit );

							// track the current hit effect
							m_Hit = pHit;
						}
						else
						{
							// update the current hit effect position
							m_Hit->setWorldPosition( collidePosition );
						}
					}

					if ( pCollide->canDamage( damageType() ) )
					{
						float fDamageMod = calculateModifier( MT_BEAM_DAMAGE );

						int inflict = damage() * fDamageMod;
						if ( damageFalloff() <= 1 )
						{
							if ( !reverseFalloff() )
							{
								// damage is less with range							
								int falloff = inflict * ( 1.0f - damageFalloff() );
								float scale = 1.0f - ( collideDistance / maxDistance );
								inflict -= scale * falloff;
							}
							else
							{
								// damage is more with range
								int falloff = inflict * damageFalloff();								
								float scale = collideDistance / maxDistance;
								inflict += scale * falloff;
							}
						}

						if ( damageRandom() > 0 )
						{
							int nRandomDamage = (int)(damageRandom() * fDamageMod);
							if ( nRandomDamage > 0 )
								inflict += rand() % nRandomDamage;
						}

						if ( inflict > 0 )		// make sure the falloff didn't negate the damage
						{
							pCollide->inflictDamage( tick(), parentBody(), inflict, damageType(), 
								pCollide->worldFrame() * (collidePosition - pCollide->worldPosition()) );
						}
					}
					else if ( WidgetCast<NounProjectile>( pCollide ) )
					{
						// explode the projectile
						((NounProjectile *)pCollide)->explode();
					}
				}
				else if ( m_Hit.valid() )
				{
					// stop the hit effect
					if ( m_bDetachHitEffect )
						((SceneryEffect *)m_Hit.pointer())->setLife( 0.0f );
					m_Hit = NULL;
				}
			}
		}

		// has the beam finished
		if ( m_Duration <= 0 )
		{
			// stop any hit effect
			if ( m_Hit.valid() )
			{
				if ( m_bDetachHitEffect )
					((SceneryEffect *)m_Hit.pointer())->setLife( 0.0f );
				m_Hit = NULL;
			}
			m_Target = NULL;
		}
	}

	NounGadget::simulate( nTick );
}

//----------------------------------------------------------------------------

void GadgetBeamWeapon::inflictDamage( dword nWhen, Noun * pFrom, int damage, dword type, const Vector3 & direction )
{
	NounGadget::inflictDamage( nWhen, pFrom, damage, type, direction );

	if ( type & DAMAGE_ELF )
	{
		int drained = Min( m_Energy, damage );
		//m_Energy = Clamp( m_Energy - drained, 0, energyCost());

		// give the energy to the draining ship
		if ( WidgetCast<NounShip>( pFrom ) )
			((NounShip *)pFrom)->setEnergy( ((NounShip *)pFrom)->energy() + drained );
	}
}

//----------------------------------------------------------------------------

NounGadget::Type GadgetBeamWeapon::type() const
{
	return WEAPON;
}

NounGadget::EnergyClass GadgetBeamWeapon::energyClass() const
{
	return ENERGY_CLASS_WEAPONS;
}

dword GadgetBeamWeapon::hotkey() const
{
	return HK_SPACE;
}

float GadgetBeamWeapon::addSignature() const
{
	return m_Signature;
}

CharString GadgetBeamWeapon::useTip( Noun * pTarget, bool shift ) const
{
	if ( shift )
		return "Toggle Point Defense.";
	
	// usage information
	CharString tip;
	float fEnergyMod = calculateModifier( MT_BEAM_ENERGY );
	float fChargeScale = damageRatioInv() * fEnergyMod;
	tip += CharString().format("\nRecharge Time:   <X;100>%.1fs", energyCost() / ( ( energyCharge() * fChargeScale ) * TICKS_PER_SECOND ) );
	tip += CharString().format("\nEnergy Cost:<X;100>%.1f", energyCost() / 1000.0f );
	float fLength = length() * calculateModifier( MT_BEAM_RANGE );
	tip += CharString().format("\nMax Range:<X;100>%.1fgu", fLength );
	
	// direct damage
	float fDamageMod = calculateModifier( MT_BEAM_DAMAGE );
	float fDamage = damage() * fDamageMod;
	if ( damageRandom() > 0 )
	{
		float fMaxDamage = fDamage + (damageRandom() * fDamageMod);
		tip += CharString().format( "\nDamage:<X;100>%.0f-%.0f", fDamage * duration(), fMaxDamage * duration() );
	}
	else
		tip += CharString().format( "\nDamage:<X;100>%.1f", fDamage * duration() );

	return tip;
}

bool GadgetBeamWeapon::usable( Noun * pTarget, bool shift ) const
{
	if ( shift && allowPointDefense() )
		return true;		// allow point defense to be toggled
	if (! NounGadget::usable( pTarget, shift ) )
		return false;
	if ( pointDefense() && WidgetCast<NounShip>( pTarget ) )
		return false;		// don't waste beams on ships
	if ( WidgetCast<NounShip>( parent() ) )
		if ( ((NounShip *)parent())->testFlags( NounShip::FLAG_CLOAKED|NounShip::FLAG_IN_SAFE_ZONE ) )
			return false;			// weapons are disabled
	if ( pTarget == NULL )
		return false;
	if ( destroyed() )
		return false;
	if ( m_Energy < energyCost() )
		return false;

	Vector3 position( pTarget->worldPosition() );
	float fLength = length() * calculateModifier( MT_BEAM_RANGE );
	if ( ((position - worldPosition()).magnitude() - pTarget->radius()) > fLength )
		return false;

	// finally check the facing of the weapon
	return( checkFacing( position ) );
}

int GadgetBeamWeapon::usableWhen() const
{
	return Min<int>( energyCost() > 0 ? (100 - ((m_Energy * 100) / energyCost())) : 0, NounGadget::usableWhen() );
}

bool GadgetBeamWeapon::useActive() const
{
	return m_Duration > 0;
}

//----------------------------------------------------------------------------

void GadgetBeamWeapon::use( dword when, Noun * pTarget, bool shift )
{
	if ( shift )
	{
		// toggle point defense
		if ( allowPointDefense() )
			m_PointDefense = !m_PointDefense;
	}
	else if ( pTarget != NULL )
	{
		// if this is a ship, pick a random subsystem on the enemy ship to be our target
		NounShip * pShip = WidgetCast<NounShip>( pTarget );
		m_Target = pShip == NULL ? pTarget : pShip->randomGadget(when, (int)worldPosition().magnitude2() );
		m_Energy = 0;
		m_Duration = duration();
		m_nEnergyTick = when + duration();
		
		// create the use effect
		if ( context()->isClient() )
		{
			SceneryEffect * pEffect = createUseEffect();
			if ( pEffect != NULL )
				pEffect->setLife( m_Duration * TICK_DURATION_S );
		}
	}
}

int GadgetBeamWeapon::useEnergy( dword nTick, int energy )
{
	if ( m_Energy < energyCost() )
	{
		int nElapsed = nTick - m_nEnergyTick;
		m_nEnergyTick = nTick;
		if ( nElapsed < 0 )
			return energy;		// currently firing

		// energy modifier makes weapons charge faster by using more energy.. cooldown gives the same effect 
		float fEnergyMod = calculateModifier( MT_BEAM_ENERGY );
		float fChargeScale = damageRatioInv() * fEnergyMod;

		m_fChargeRate += (fChargeScale * energyCharge()) * nElapsed;
		int chargeRate = floor( m_fChargeRate );
		m_fChargeRate -= chargeRate;		// leave fractional amount in the float for next update..
		int charge = Min( Min( energyCost() - m_Energy, chargeRate ), energy );

		// reduce ship energy
		energy -= charge;
		m_Energy += charge;

		// calculate the signature
		m_Signature = 0.1f * charge;
	}
	else
	{
		m_Signature = 0.0f;
	}
    
	return energy;
}

bool GadgetBeamWeapon::updateLogic()
{
	if ( useActive() || usableWhen() > 0 )
		return true;		// early out if not ready to fire or currently in use...

	if ( WidgetCast<NounShip>( parent() ) )
	{
		NounShip * pShip = (NounShip *)parent();
		
		// turn on point defense if not on already...
		if ( !pointDefense() && type() != WEAPON_BOMB && allowPointDefense() )
			setPointDefense( true );

		NounGame * pTarget = WidgetCast<NounGame>( pShip->commandTarget() );
		if (! pTarget )
			return true;	// no current target
		if ( pTarget != pShip->lastFireTarget() || !pShip->isWeaponsFree() )
			return true;	// can't hit target
		if ( isFriend( pTarget ) )
			return true;	// don't attack friendly targets
		if( WidgetCast<NounJumpGate>( pTarget ) 
			|| WidgetCast<CargoResource>( pTarget ) )
			return true;	// don't attack jump gates or resources
		if (! pTarget->canDamage( damageType() ) )
			return true;	// can't damage this target, so don't use this weapon
		if ( type() == WEAPON_BOMB && WidgetCast<NounPlanet>( pTarget ) == NULL )
			return true;	// only use bomb weapons on planets..

		if ( usable( pTarget, false ) )
			pShip->useGadget( this, pTarget, false );
	}
	else if ( WidgetCast<StructureDefense>( parent() ) )
	{
		StructureDefense * pStructure = (StructureDefense *)parent();
		if (! pStructure->active() )
			return true;
		NounPlanet * pPlanet = pStructure->planet();
		if (! pPlanet )
			return false;

		Noun *	pBestTarget = NULL;
		float	fBestRange = 0.0f;;

		// look for incoming enemy projectiles, pick the best target
		for(int j=0;j<pPlanet->contactCount();j++)
		{
			NounGame * pContact = WidgetCast<NounGame>( pPlanet->contact( j ) );
			if ( pContact != NULL && pContact->enablePD() && !pPlanet->isFriend( pContact ) )
			{
				float fRange = (worldPosition() - pContact->worldPosition()).magnitude();
				if ( (!pBestTarget || fRange < fBestRange) && usable( pContact, false ) 
					&& !StructureDefense::isTargeted( pPlanet, pContact) )
				{
					pBestTarget = pContact;
					fBestRange = fRange;
				}
			}
		}

		if ( pBestTarget )
		{
			// set the target now, so no other beam weapon tries to destroy the projectile
			setTarget( pBestTarget );
			// use the beam weapon on the projectile
			pStructure->useGadget( pBestTarget, this );
			return true;
		}

		for(int j=0;j<pPlanet->contactCount();j++)
		{
			NounShip * pShip = WidgetCast<NounShip>( pPlanet->contact( j ) );
			if ( pShip != NULL && pPlanet->isEnemy( pShip ) )
			{
				float fRange = (worldPosition() - pShip->worldPosition()).magnitude();
				if ( (!pBestTarget || fRange < fBestRange) && usable( pShip, false ) )
				{
					pBestTarget = pShip;
					fBestRange = fRange;
				}
			}
		}

		if ( pBestTarget != NULL )
			pStructure->useGadget( pBestTarget, this );

		return true;
	}

	return false;
}

//----------------------------------------------------------------------------
// EOF
