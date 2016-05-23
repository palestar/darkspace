/*
	GadgetWeapon.cpp

	Base class for any type of weapon that shoots a projectile
	(c)2000 Palestar, Richard Lyle
*/


#include "Debug/Assert.h"
#include "System/Messages.h"
#include "Constants.h"
#include "NounProjectile.h"
#include "NounSmartProjectile.h"
#include "NounStructure.h"
#include "NounJumpGate.h"
#include "GadgetFighterBay.h"
#include "GadgetWeapon.h"
#include "GadgetBeamWeapon.h"
#include "GameContext.h"
#include "StructureDefense.h"

//----------------------------------------------------------------------------

static Constant		MAX_DETONATION_RANGE( "MAX_DETONATION_RANGE", 10000.0f );
static Constant		MAX_ZONE_PROJECTILES( "MAX_ZONE_PROJECTILES", 512 );
static Constant		WEAPON_CHARGE_SIGNATURE( "WEAPON_CHARGE_SIGNATURE", 0.1f );
static Constant		MAX_SHIP_RADIUS( "MAX_SHIP_RADIUS", 200.0f );
static Constant		AI_MINE_RANGE( "AI_MINE_RANGE", 150.0f );				// how close to another ship to drop a mine
static Constant		AI_CLOSE_RANGE( "AI_CLOSE_RANGE", 300.0f );			// range ship needs to be within to use a close-range weapon
static Constant		AI_MEDIUM_RANGE( "AI_MEDIUM_RANGE", 100.0f );			// minimum range ship needs to be in order to use torpedos
static Constant		AI_LONG_RANGE( "AI_LONG_RANGE", 400.0f );			// range AI needs to be to use missiles and fighters.

//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_FACTORY( GadgetWeapon, NounGadget ); 
REGISTER_FACTORY_KEY( GadgetWeapon, 4099312795259541527LL );

BEGIN_ABSTRACT_PROPERTY_LIST( GadgetWeapon, NounGadget );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Energy );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Ammo );
		//ADD_PROPERTY_FLAGS( m_Ammo, PF_REPLICATE );
END_PROPERTY_LIST();

GadgetWeapon::GadgetWeapon() : 
	m_Energy( 0 ), 
	m_Ammo( 0 ), 
	m_nMaxAmmo( 0 ),
	m_RepeatCount( 0 ), 
	m_nEnergyTick( 0 ), 
	m_fRepeatDelay( 0.0f ), 
	m_fChargeRate( 0.0f ),
	m_Signature( 0 )
{}

//----------------------------------------------------------------------------

void GadgetWeapon::onDetaching()
{
	// detach all projectiles from this weapon, preventing exploits by bypassing projectile limits
	for(int i=m_Projectiles.size()-1;i>=0;i--)
		m_Projectiles[i]->setDetach();

	// projectiles must be detached before we call the base onDetaching(), that will call release()
	// which will remove all the projectiles
	NounGadget::onDetaching();
}

//----------------------------------------------------------------------------

void GadgetWeapon::setup()
{
	NounGadget::setup();
	m_nMaxAmmo = ammoMax() * calculateModifier( MT_WEAPON_AMMO, false );
	m_Ammo = m_nMaxAmmo;
	m_Energy = energyCost();
	m_nEnergyTick = tick();
}

void GadgetWeapon::initialize()
{
	NounGadget::initialize();
	m_nEnergyTick = tick();
	m_nMaxAmmo = ammoMax() * calculateModifier( MT_WEAPON_AMMO, false );
}

void GadgetWeapon::release()
{
	NounGadget::release();
	m_Target = NULL;
	m_Projectiles.release();
}

void GadgetWeapon::inflictDamage( dword nWhen, Noun * pFrom, int damage, dword type, const Vector3 & direction )
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

NounGadget::Type GadgetWeapon::type() const
{
	return WEAPON;
}

NounGadget::EnergyClass GadgetWeapon::energyClass() const
{
	return ENERGY_CLASS_WEAPONS;
}

dword GadgetWeapon::hotkey() const
{
	return HK_SPACE;
}

CharString GadgetWeapon::status() const
{
	return ammoCost() > 0 ? CharString().format("%d", m_Ammo) : "";
}

int GadgetWeapon::addValue() const
{
	return ammoMax() * ammoResources();
}

float GadgetWeapon::addSignature() const
{
	return m_Signature;
}

CharString GadgetWeapon::useTip( Noun * pTarget, bool shift ) const
{
	CharString tip;
	if ( shift )
		return "Detonate Projectiles.";
	
	float fEnergyMod = calculateModifier( MT_WEAPON_ENERGY );
	float fChargeScale = damageRatioInv() * fEnergyMod;
	tip += CharString().format("\nRecharge Time:<X;100>%.1fs", energyCost() / ( ( energyCharge() * fChargeScale ) * TICKS_PER_SECOND ) );
	tip += CharString().format("\nEnergy Cost:<X;100>%.1f", energyCost() / 1000.0f );
	tip += CharString().format("\nMax Range:<X;100>%.1fgu", ( projectileLife() * calculateModifier( MT_WEAPON_RANGE ) ) * projectileVelocity() );
	
	if ( armTime() > 0 )
		tip += CharString().format("\nArming Range:<X;100>%.1fgu", armTime() * projectileVelocity() );
	
	if ( WidgetCast<GadgetFighterBay>( this ) )
	{
		Scene * pScene = WidgetCast<Scene>( (GadgetFighterBay *)this->nounContext()->resource( "FIGHTER" ) );
		NounFighter * pFighter = WidgetCast<NounFighter>( pScene->node()->child( 0 ) );
		if ( pFighter != NULL )
		{
			GadgetBeamWeapon * pBeam = NULL;
			GadgetWeapon * pWeapon = NULL;
			for( int i=0;i<pFighter->childCount();++i )
			{
				if ( WidgetCast<GadgetBeamWeapon>( pFighter->child( i ) ) )
					pBeam = WidgetCast<GadgetBeamWeapon>( pFighter->child( i ) );
				else if ( WidgetCast<GadgetWeapon>( pFighter->child( i ) ) )
					pWeapon = WidgetCast<GadgetWeapon>( pFighter->child( i ) );

			}
				
			if ( pBeam != NULL )
			{
				float fDamageMod = calculateModifier( MT_BEAM_DAMAGE );
				float fDamage = pBeam->damage() * fDamageMod;
				if ( pBeam->damageRandom() > 0 )
				{
					float fMaxDamage = fDamage + ( pBeam->damageRandom() * fDamageMod );
					tip += CharString().format( "\nDamage:<X;100>%.0f-%.0f", fDamage * pBeam->duration(), fMaxDamage * pBeam->duration() );
				}
				else
					tip += CharString().format( "\nDamage:<X;100>%.1f", fDamage * pBeam->duration() );
			}
			
			if ( pWeapon != NULL )
			{
				// direct damage
				float fDamage = pWeapon->damage() * calculateModifier( MT_WEAPON_DAMAGE );
				if ( fDamage > 0 )
				{
					if ( pWeapon->damageRandom() > 0 )
					{
						float fDamageMax = fDamage + pWeapon->damageRandom() * calculateModifier( MT_WEAPON_DAMAGE );
						tip += CharString().format( "\nDamage:<X;100>%.0f-%.0f", fDamage * pWeapon->projectileCount(), fDamageMax * pWeapon->projectileCount() );
					}
					else
						tip += CharString().format( "\nDamage:<X;100>%.1f", fDamage * pWeapon->projectileCount());
				}
					
				// area damage
				fDamage = pWeapon->areaDamage() * calculateModifier( MT_WEAPON_DAMAGE );
				if ( fDamage > 0 )
				{
					if ( pWeapon->areaDamageRandom() > 0 )
					{
						float fDamageMax = fDamage + pWeapon->areaDamageRandom();
						tip += CharString().format( "\nArea Damage:<X;100>%.0f-%.0f", fDamage, fDamageMax );
					}
					else 		
						tip += CharString().format("\nArea Damage:<X;100>%.0f", fDamage );
				}				
			}
			
		}
	}
	else
	{
		// direct damage
		float fDamage = damage() * calculateModifier( MT_WEAPON_DAMAGE );
		if ( fDamage > 0 )
		{
			if ( damageRandom() > 0 )
			{
				float fDamageMax = fDamage + damageRandom() * calculateModifier( MT_WEAPON_DAMAGE );
				tip += CharString().format( "\nDamage:<X;100>%.0f-%.0f", fDamage * projectileCount(), fDamageMax * projectileCount() );
			}
			else
				tip += CharString().format( "\nDamage:<X;100>%.1f", fDamage * projectileCount());
		}
			
		// area damage
		fDamage = areaDamage() * calculateModifier( MT_WEAPON_DAMAGE );
		if ( fDamage > 0 )
		{
			if ( areaDamageRandom() > 0 )
			{
				float fDamageMax = fDamage + areaDamageRandom();
				tip += CharString().format( "\nArea Damage:<X;100>%.0f-%.0f", fDamage, fDamageMax );
			}
			else 		
				tip += CharString().format("\nArea Damage:<X;100>%.0f", fDamage );
		}
	}

	return tip;
}

bool GadgetWeapon::needReload( Noun * pWhom ) const
{
	return m_Ammo < m_nMaxAmmo;
}

bool GadgetWeapon::usable( Noun * pTarget, bool bShift ) const
{
	if ( bShift )
		return true;				// can always explode our own projectiles
	if (! NounGadget::usable( pTarget, bShift ) )
		return false;
	if ( WidgetCast<NounShip>( parent() ) )
		if ( ((NounShip *)parent())->testFlags( NounShip::FLAG_CLOAKED|NounShip::FLAG_IN_SAFE_ZONE ) )
			return false;			// weapons are disabled
	if ( destroyed() )
		return false;
	if ( m_Energy < energyCost() )
		return false;				// weapon still recharging
	if ( m_Ammo < ammoCost() )
		return false;				// out of ammo
	if ( m_Projectiles.size() >= maxProjectiles() )
		return false;				// maximum number of active projectiles
	if ( !zone() )
		return false;				// maximum number of projectiles already in our zone

	if (! needsTarget() )
		return true;				// gadget doesn't need a target, so don't check facing or range
	if ( pTarget == NULL )
		return false;

	// get the intercept point of the target, so we can check the facing based on the intercept point instead of
	// the targets current position
	Vector3 position;
	intercept( pTarget, position );		// TODO: get the accuracy from the gunner skill of the ship itself
	
	// optionally check the range to the intercept point
	if ( checkRange()  )
	{
		float fDistance = (position - worldPosition()).magnitude();
		if ( fDistance > maxRange() || fDistance < minRange() )
			return false;				// out of range or too close
	}

	// finally check the facing of the weapon
	return( checkFacing( position ) );
}

int GadgetWeapon::usableWhen() const
{
	return Min<int>( energyCost() > 0 ? (100 - ((m_Energy * 100) / energyCost())) : 0, NounGadget::usableWhen() );
}

bool GadgetWeapon::reload( Noun * pWhom )
{
	m_Ammo = Clamp( m_Ammo + ammoReload(), 0, m_nMaxAmmo );
	return m_Ammo < m_nMaxAmmo;
}

void GadgetWeapon::use( dword when, Noun * pTarget, bool bShift)
{
	if ( bShift )
	{
		// explode all projectiles belonging to this weapon, enum backwards as they will get removed from the list as they explode/detach
		for(int i=m_Projectiles.size()-1;i>=0;i--)
		{
			Noun * pNoun = m_Projectiles[i];

			if ( WidgetCast<NounProjectile>( pNoun ) )
				((NounProjectile *)pNoun)->explode();
			else
				pNoun->setDetach();
		}
	}
	else
	{
		NounGadget::use( when, pTarget, bShift );

		// set the energy to zero
		m_Energy = 0;
		m_nEnergyTick = when;
		// subtract ammo usage
		m_Ammo -= ammoCost();

		if ( projectileCount() > 1 )
		{
			// multiple projectiles -- setup projectile creation by the useEnergy() function
			m_Target = pTarget;
			m_RepeatCount = projectileCount();
			m_fRepeatDelay = projectileDelay() * calculateModifier( MT_WEAPON_COOLDOWN, true );
		}
		else
		{
			// create the single and only projectile
			createProjectile( when, pTarget );
		}
	}
}

int GadgetWeapon::useEnergy( dword nTick, int energy )
{
	int nElapsed = nTick - m_nEnergyTick;
	m_nEnergyTick = nTick;

	if ( m_RepeatCount > 0 )
	{
		m_fRepeatDelay -= nElapsed;
		if ( m_fRepeatDelay <= 0.0f )
		{
			createProjectile( nTick, m_Target );
			m_fRepeatDelay += projectileDelay() * calculateModifier( MT_WEAPON_COOLDOWN, true );

			m_RepeatCount--;
			if ( m_RepeatCount <= 0 )
				m_Target = NULL;
		}
	}

	if ( m_Energy < energyCost() )
	{
		float fEnergyMod = calculateModifier( MT_WEAPON_ENERGY );
		float fChargeScale = damageRatioInv() * fEnergyMod;

		m_fChargeRate += energyCharge() * fChargeScale * nElapsed;
		int chargeRate = floor( m_fChargeRate );
		m_fChargeRate -= chargeRate;		// leave fractional amount in the float for next update..
		int charge = Min( Min( energyCost() - m_Energy, chargeRate ), energy );

		// reduce ship energy
		energy -= charge;
		// charge this weapon
		m_Energy += charge;
		// calculate the signature
		m_Signature = WEAPON_CHARGE_SIGNATURE * charge;
	}
	else
	{
		m_Signature = 0.0f;
	}

	return energy;
}

bool GadgetWeapon::updateLogic()
{
	if ( useActive() || usableWhen() > 0 )
		return true;		// early out if not ready to fire or currently in use...

	if ( WidgetCast<NounShip>( parent() ) )
	{
		NounShip * pShip = (NounShip *)parent();
		Noun * pTarget = pShip->commandTarget();
		if (! pTarget )
			return true;	// no current target
		if ( pTarget != pShip->lastFireTarget() || !pShip->isWeaponsFree() )
			return true;	// can't hit target
		if (! isEnemy( pTarget ) )
			return true;	// don't attack friendly/neutral targets
		if( WidgetCast<NounJumpGate>( pTarget ) )
			return true;	// don't attack jump gates

		bool bIsPlanet = pShip->isTargetPlanet( pTarget ) != NULL;
		bool bFireWeapon = false;

		if (! bIsPlanet )
		{
			float fDistance = pShip->lastFireDistance();
			switch( type() )
			{
			case WEAPON_MINE:
				bFireWeapon = fDistance < AI_MINE_RANGE	&& pShip->getFacing( pTarget->worldPosition() ) == FACING_BACK;
				break;
			case WEAPON_CLOSE:
				bFireWeapon = fDistance < AI_CLOSE_RANGE;
				break;
			case WEAPON_MEDIUM:
				bFireWeapon = fDistance > AI_MEDIUM_RANGE;
				break;
			case WEAPON_RANGED:
				bFireWeapon = fDistance > AI_LONG_RANGE;
				break;
			case WEAPON_FIGHTER:
				bFireWeapon = fDistance > AI_LONG_RANGE;
				break;
			case WEAPON_BOMB:
				break;
			default:
				bFireWeapon = true;
				break;
			}
		}
		else if ( type() == WEAPON_BOMB )
		{
			bFireWeapon = true;
		}

		if ( bFireWeapon && usable( pTarget, false ) )
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

		NounShip *	pBestTarget = NULL;
		float		fBestRange = 10000.0f;

		for(int j=0;j<pPlanet->contactCount();j++)
		{
			NounShip * pShip = WidgetCast<NounShip>( pPlanet->contact( j ) );
			if ( pShip != NULL && !pPlanet->isFriend( pShip ) )
			{
				float fRange = (worldPosition() - pShip->worldPosition()).magnitude();
				if ( fRange < fBestRange && usable( pShip, false ) )
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

void GadgetWeapon::onModifiersChanged()
{
	m_nMaxAmmo = ammoMax() * calculateModifier( MT_WEAPON_AMMO, false );
	m_Ammo = Clamp( m_Ammo, 0, m_nMaxAmmo );
}

//----------------------------------------------------------------------------

void GadgetWeapon::onProjectileActive( Noun * pProj )
{
	// add projectile to projectile list
	m_Projectiles.push( pProj );
}

void GadgetWeapon::onProjectileDead( Noun * pProj )
{
	// remove this projectile from the projectile list
	m_Projectiles.removeSearchSwap( pProj );
}

bool GadgetWeapon::hit( Noun * pTarget, NounProjectile * pProj, const Vector3 & intersect )
{
	NounBody * pParent = parentBody();
	if ( isMine() )
	{
		if ( pParent != NULL && pParent->isFriend( pTarget ) )
			return false;	// don't explode when it's a friendly target
	}

	if (! WidgetCast<NounGame>( pTarget ) )
		return true;
	if (! ((NounGame *)pTarget)->canDamage( damageType() ) )
		return true;

	// get the amount of life remaining for the projectile
	float fLife = TICK_DURATION_S * pProj->life();
	// get the amount of time the projectile was active
	float fMaxLife = projectileLife() * calculateModifier( MT_WEAPON_RANGE );
	float fTime = fMaxLife - fLife;
	if ( fTime < armTime() )
		return true;		// projectile was not armed yet, it inflicts no damage...

	int d = damage() * calculateModifier( MT_WEAPON_DAMAGE );
	if ( d > 0 )
	{
		if ( damageFalloff() <= 1 )
		{
			if ( !reverseFalloff() )
			{
				// damage is less with range				
				int falloff = d * ( 1.0f - damageFalloff() );
				if ( fLife > 0.0f  && fMaxLife > 0.0f && fLife < fMaxLife )
				{
					float r = 1.0f - (fLife / fMaxLife);
					d -= falloff * r;
				}
			}
			else
			{
				// damage is more with range			
				int falloff = d * damageFalloff();
				if ( fLife > 0.0f  && fMaxLife > 0.0f && fLife < fMaxLife )
				{
					float r = fLife / fMaxLife;
					d += falloff * r;
				}				
			}
		}

		if ( damageRandom() > 0 )
			d += rand() % (int)(damageRandom() * calculateModifier( MT_WEAPON_DAMAGE ));

		if ( d > 0 )
		{
			Vector3 vDirection = intersect - pTarget->worldPosition();
			vDirection.normalize();

			// inflict the damage
			((NounGame *)pTarget)->inflictDamage( tick(), parentBody(), d, damageType(), 
				pTarget->worldFrame() * vDirection );
		}
	}

	return true;

}

void GadgetWeapon::explode( NounProjectile * pProj )
{
	Noun::Context * pContext = pProj->context();
	if (! pContext )
		return;

	if ( areaDamage() > 0 && areaDamageRadius() > 0 )
	{
		Vector3 vPosition = pProj->worldPosition();

		// get the amount of life remaining for the projectile
		float fLife = TICK_DURATION_S * pProj->life();
		// get the amount of time the project was active
		float fMaxLife = projectileLife() * calculateModifier( MT_WEAPON_RANGE );
		float fTime = fMaxLife - fLife;

		if ( fTime < armTime() )
			return;			// projectile is a dud, no explosion...

		Array< GameContext::NounCollision > collide;
		if ( pContext->proximityCheck( vPosition, areaDamageRadius() + MAX_SHIP_RADIUS, collide ) )
		{
			for(int i=0;i<collide.size();i++)
			{
				NounGame * pCollide = WidgetCast<NounGame>( collide[i].pNoun );
				if ( pCollide != NULL && pCollide->canDamage( damageType() ) )
				{
					float fDistance = collide[i].fDistance - pCollide->radius();
					if ( fDistance < areaDamageRadius() )
					{
						float damageRatio = 1.0f - (fDistance / areaDamageRadius());
						int damage = damageRatio * areaDamage();
						if ( areaDamageRandom() > 0 )
							damage += rand() % (int)areaDamageRandom();
						
						if ( damage > 0 )
						{
							damage *= calculateModifier( MT_WEAPON_DAMAGE );

							Vector3 vDirection = vPosition - pCollide->worldPosition();
							vDirection.normalize();

							pCollide->inflictDamage( tick(), parentBody(), damage, damageType(), 
								pCollide->worldFrame() * vDirection );
						}
					}
				}
			}
		}
	}
}

//----------------------------------------------------------------------------

void GadgetWeapon::intercept( Noun * pTarget, Vector3 & i ) const
{
	if ( pTarget != NULL )
		intercept( pTarget->worldPosition(), pTarget->worldVelocity(), i );
}

const int MAX_RECURSE_COUNT = 3;

void GadgetWeapon::intercept( const Vector3 & target,		// target world position
							const Vector3 & velocity,		// target velocity in world space
							Vector3 & i ) const				// the intercept point
{
	Vector3 myVelocity( 0.0f );
	//if ( inheritVelocity() )
	//	myVelocity = worldVelocity();

	// get the position of this gadget
	Vector3 myPosition( worldPosition() );
	// calculate the distance to the target
	float distance = (target - myPosition).magnitude();
	// caluclate the projectile time to the target
	float t = distance / projectileVelocity();	
	// calculate the beginning intercept point
	i = target + (velocity * t);// - (myVelocity * t);
	
	// calculate the number of times to recurse
	int recurseCount = MAX_RECURSE_COUNT;
	while( recurseCount > 0 )
	{
		recurseCount--;

		// recalculate the intercept point again
		distance = (i - myPosition).magnitude();
		t = distance / projectileVelocity();
		i = target + (velocity * t);// - (myVelocity * t);
	}
}

NounProjectile * GadgetWeapon::projectileFactory()
{
	NounProjectile * pProj = projectileSmart() ? new NounSmartProjectile() : new NounProjectile();
	if ( pProj != NULL )
		pProj->setName( nounContext()->name() );

	return pProj;
}

NounProjectile * GadgetWeapon::createProjectile( dword when, Noun * pTarget )
{
	// this function determines the velocity of the projectiles based opon if this weapon
	// is on a turret or not... 
	
	// create the use effect
	if ( context()->isClient() )
		createUseEffect();

	if ( turret() && pTarget != NULL )
	{
		// -- turret weapons fire in the direction of the target

		// get the intercept point for this weapon
		Vector3 position;
		intercept( pTarget, position );		// TODO: get the accuracy from the gunner skill of the ship itself

		// calculate the direction of the projectile
		Vector3 velocity( position - worldPosition() );
		velocity.normalize();
		velocity *= projectileVelocity();
				

		return createProjectile( when, velocity, pTarget );
	}

	// -- non-turret weapons are usually for fixed and guided weapons such as missiles
	// the initial velocity of the projectile is based on the world frame of this gadget

	Vector3 velocity( worldFrame().k * projectileVelocity() );

	return createProjectile( when, velocity, pTarget );
}

NounProjectile * GadgetWeapon::createProjectile( dword when, const Vector3 & velocity, Noun * pTarget )
{
	// create the projectile only if this is either not a collidable projectile or
	// its a collidable projectile and we are the server, collidable projectiles are replicated
	// from the server to the relavent clients automatically.
	if ( (isServer() && projectileCollidable()) || !projectileCollidable() )
	{
		// create the SceneryProjectile object
		NounProjectile * pProj = projectileFactory();
		if ( pProj != NULL )
		{
			// initialize the projectile
			pProj->initializeProjectile( this, pTarget, velocity, when );
			// attach projectile to the world
			context()->attachNoun( pProj );
			// return the created projectile
			return pProj;
		}
	}

	return NULL;
}


//----------------------------------------------------------------------------
// EOF
