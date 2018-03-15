/*
	NounShip.h

	This noun is a star-ship
	(c)1999 Palestar, Richard Lyle
*/

//#define PROFILE_OFF

#include "Debug/Assert.h"
#include "Debug/Trace.h"
#include "Debug/Profile.h"
#include "Math/Helpers.h"
#include "Render3D/NodeSprite.h"
#include "World/TraitMovement.h"
#include "World/TraitReplication.h"
#include "TraitOrbit.h"
#include "TraitDisplayName.h"
#include "Trail.h"
#include "Constants.h"
#include "NounShip.h"
#include "NounGadget.h"
#include "NounPlanet.h"
#include "NounStar.h"
#include "NounNebula.h"
#include "World/NounTarget.h"
#include "NounProjectile.h"
#include "NounUnit.h"
#include "NounStructure.h"
#include "NounJumpGate.h"
#include "NounBeacon.h"
#include "CargoGadget.h"
#include "CargoEnhancement.h"
#include "SceneryEffect.h"
#include "SceneryDebris.h"
#include "GadgetShield.h"
#include "GadgetArmor.h"
#include "GadgetDrive.h"
#include "VerbUseGadget.h"
#include "VerbEnterOrbit.h"
#include "VerbDestroyShip.h"
#include "GameContext.h"
#include "NounEnhancement.h"
#include "VerbApplyEnhancement.h"
#include "DefaultLootTables.h"
#include "Network/LogClient.h"		// LogLevels
#include "NounTrail.h"
#include "resource.h"

#include <float.h>
#include <algorithm>
#include <math.h>

//----------------------------------------------------------------------------

static Constant DESTROYED_SHIP_RESOURCE( "DESTROYED_SHIP_RESOURCE", 0.125f );	// when a ship is destroyed, what amount of it's build costs are turned into CargoResource objects
static Constant PLANET_REPAIR_RATE( "PLANET_REPAIR_RATE", 300 );
static Constant SHIP_GADGET_REPAIR_RATE( "PLANET_GADGET_REPAIR_RATE", 300 );
static Constant DAMAGE_POINTS_DIV( "DAMAGE_POINTS_DIV", 20000.0f );
static Constant NOHULL_DAMAGE_POINTS_SCALE( "NOHULL_DAMAGE_POINTS_SCALE", 0.1f );
static Constant CHECK_FIRE_INTERVAL( "CHECK_FIRE_INTERVAL", 2.5f );
static Constant CLIENT_SIDE_DAMAGE( "CLIENT_SIDE_DAMAGE", 0.5f );
static Constant OUT_OF_COMBAT_TIME( "OUT_OF_COMBAT_TIME", TICKS_PER_SECOND * 30 ); // 30s base + 30s per ship level in the setOutOfCombat() function

const int		SHIP_UPDATE_RATE	= TICKS_PER_SECOND;					// ship is updated every SHIP_UPDATE_RATE ticks
const int		DEFAULT_REPAIR_RATE = TICKS_PER_SECOND * 5;				// when orbiting a friendly planet, how often to repair/resupply ship
const int		HYDROGEN_REFUEL_RATE = TICKS_PER_SECOND;				// when orbiting a planet with hydrogen, how often to refuel JD
const float		MAX_SCANNER_RANGE = 5000.0f;							// maximum range updateContacts() uses for detecting all objects

const int		USE_GADGET_LATENCY = 10;								// how many ticks in latency is added to all gadget use verbs, useGadgets()
const int		INTERCEPT_RECURSE_COUNT = 2;							// intercept() uses this to determine how much to refine the intercept position

const int		INGORE_COLLISION_TIME = TICKS_PER_SECOND * 2;			// amount of time we will ignore collision before starting

//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_FACTORY( NounShip, NounBody ); 
REGISTER_FACTORY_KEY( NounShip, 4033937477208710886LL );

BEGIN_ABSTRACT_PROPERTY_LIST( NounShip, NounBody );
	ADD_TRANSMIT_PROPERTY( m_Name );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Flags );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Traits );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_nTeamId );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_nUserId );
	ADD_REPLICATED_PROPERTY( m_nCredits, TICKS_PER_SECOND );
	ADD_REPLICATED_PROPERTY( m_Rank, TICKS_PER_SECOND );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Children );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_DetectionFlags );
	ADD_REPLICATED_PROPERTY( m_Damage, TICKS_PER_SECOND * 5 );
	ADD_REPLICATED_PROPERTY( m_Energy, TICKS_PER_SECOND * 5 );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_fVelocity );
	ADD_REPLICATED_UP_PROPERTY( m_fSetVelocity, TICKS_PER_SECOND );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_RepairQueue );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Order );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_OrderTarget );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_pOrderFrom );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_nOrderTime );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Command );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_CommandTarget );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_pHome );
	ADD_REPLICATED_PROPERTY( m_bOutOfCombat, TICKS_PER_SECOND );
	ADD_PROPERTY( m_nAllyTime );
	//ADD_TRANSMIT_UPDATE_PROPERTY( m_Group );
	//ADD_TRANSMIT_PROPERTY( m_Verbs );
	//ADD_TRANSMIT_PROPERTY( m_fHeading );
	//ADD_TRANSMIT_PROPERTY( m_fSetHeading );
	//ADD_TRANSMIT_PROPERTY( m_fYaw );
END_PROPERTY_LIST();

bool NounShip::sm_bShipSelection = true;

//---------------------------------------------------------------------------------------------------

NounShip::NounShip()
{
	attachTrait( new TraitMovement() );

	m_Damage = 0;
	m_Energy = 100;
	m_fVelocity = 0;
	m_fSetVelocity = 0;
	m_fHeading = 0.0f;
	m_fSetHeading = 0.0f;
	m_fYaw = 0.0f;
	m_nCredits = 0;
	m_Rank = 0;

	m_fRoll = 0.0f;
	m_fPitch = 0.0f;
	m_fPlane = 0.0f;
	m_nAllyTime = 0;
	m_Command = NOCOMMAND;
	m_Order = NOORDER;
	m_nOrderTime = 0;
	m_UseGadgetTick = 0;
	m_UseShift = false;
	m_DamageBits = 0;
	m_bDespawning = false;
	m_nDespawnTicks = 0;
	m_nCombatTick = 0;

	m_Signature = 0;
	m_fFade = 0.0f;
	m_Visibility = 1.0f;
	m_Alpha = 255;
	m_MaxDamage = 1;
	m_MaxEnergy = 1;
	m_MaxVelocity = 1;
	m_MaxCargo = 0;
	m_MaxThrust = 1;
	m_MaxAcceleration = 1;
	m_MaxYaw = 1;
	m_nRepairedDamage = 0;
	m_Sensor = 0;
	m_View = 0;

	m_fLastSetHeading = 0;
	m_fLastSetVelocity = 0;
	m_fLastYaw = 0;

	m_nOrderState = 0;
	m_fCunning = 0.0f;
	m_nLastDistress = 0;
	m_nLastJumpCheck = 0;
	m_bLoadUnits = false;
	m_bAttacked = false;
	m_nLastEvadeVector = 0;

	m_nLastFireCheck = 0;
	m_fLastFireDistance = 0.0f;
	m_bWeaponsFree = false;

	m_bShipEnabled = true;
	m_bDynamicsEnabled = true;
	m_nLogicTick = 0;
	m_nDynamicsTick = 0;
	m_nRepairTick = 0;
	m_nUpdateTick = 0;
	m_bOutOfCombat = false;
	m_bIncoming = false;

	// until we add slots for enhancements, just set max to 8
	// TODO: Add 
	m_MaxEnhancements = 8;
	// zero out all modifiers
	resetModifiers();
}

//----------------------------------------------------------------------------

bool NounShip::read( const InStream & input )
{
	if (! NounBody::read( input ) )
		return false;

	// make sure we have all our traits after serialization
	if (! findTrait( TraitReplication::classKey() ) )
		attachTrait( new TraitReplication() );
	if (! findTrait( TraitMovement::classKey() ) )
		attachTrait( new TraitMovement() );
	if (! findTrait( TraitOrbit::classKey() ) )
		attachTrait( new TraitOrbit() );

	m_fHeading = m_fSetHeading = m_vOrientation.m_fYaw;
	m_fLastSetHeading = m_fSetHeading;
	m_fLastSetVelocity = m_fSetVelocity;
	m_fLastYaw = m_fYaw;

	// a bug caused these values to go NAN when all the drives were removed from the ship
#if defined(_WIN32)
	if ( _isnan( m_fHeading ) )
		m_fHeading = 0.0f;
	if ( _isnan( m_fVelocity ) )
		m_fVelocity = 0.0f;
#else
	if ( isnan( m_fHeading ) )
		m_fHeading = 0.0f;
	if ( isnan( m_fVelocity ) )
		m_fVelocity = 0.0f;
#endif

	// update the ship frame
	m_Frame = Matrix33( m_fPitch, m_fHeading, m_fRoll );

	initializeGadgets();
	initializeEnhancements();
	updateCharacter();
	updateDamageBits();

	// reset and reapply all modifiers
	resetModifiers();
	for(int i=0;i<m_Enhancements.size();++i)
		m_Enhancements[i]->applyModifiers( true );

	// until we add slots for enhancements, just set max to 8
	// TODO: Add 
	m_MaxEnhancements = 8;
	m_nIgnoreCollisionTick = 0;

	return true;
}

//----------------------------------------------------------------------------

void NounShip::onAttach( BaseNode * pChild )
{
	NounBody::onAttach( pChild );

	if ( WidgetCast<NounGadget>( pChild ) )
		initializeGadgets();
	else if (WidgetCast<NounEnhancement>( pChild ) )
		initializeEnhancements();
}

void NounShip::onAttached()
{
	NounBody::onAttached();
}

void NounShip::onChildDetached()
{
	NounBody::onChildDetached();

	initializeGadgets();
	initializeEnhancements();
}

void NounShip::onDetaching()
{
	if (! transfer() )
	{
		// clear any flags that might be set by gadgets or by fields.. (i.e. InterdictorField)
		clearFlags( FLAG_IN_SAFE_ZONE|FLAG_JUMP_DISABLED|FLAG_CONTROL_DISABLED|FLAG_DYNAMICS_DISABLED|FLAG_COMMAND_DISABLED|FLAG_JUMPING|FLAG_CLOAKED );
		// turn off jumpdrive if active
		if ( jumpDrive() )
			jumpDrive()->clearJump();
	}

	NounBody::onDetaching();
}

//----------------------------------------------------------------------------

void NounShip::preRender( RenderContext &context, 
					const Matrix33 & frame, 
					const Vector3 & position )
{
	if (! sm_bShipSelection )
	{
		if ( syncronized() )
		{
			m_fFade += context.elapsed();
			if ( m_fFade > 1.0f )
				m_fFade = 1.0f;
		}
		else
		{
			m_fFade -= context.elapsed();
			if ( m_fFade < 0.0f )
				m_fFade = 0.0f;
		}

		float fContextAlpha = context.alpha();
		float fAlpha = m_fFade * visibility(); 
		if ( isLocal() && fAlpha < 0.25f )
			fAlpha = 0.25f;

		if ( fAlpha > 0.0f )
		{
			context.setAlpha( fAlpha * fContextAlpha );

			// determine the render bits based on the state of the ship
			dword bits = ((!inOrbit() && worldVelocity().magnitude2() > 0.1f) ? THRUST : 0) | m_DamageBits;
			// show thrusters if jump drive is engaged
			if ( flags() & FLAG_JUMPING )
				bits |= THRUST;

			dword nContextBits = context.bits();
			context.setBits( bits );

			NounBody::preRender( context, frame, position );

			context.setAlpha( fContextAlpha );
			context.setBits( nContextBits );
		}
	}
	else
	{
		NounBody::preRender( context, frame, position );
	}
}

void NounShip::render( RenderContext &context, 
		const Matrix33 & frame, 
		const Vector3 & position )
{
	NounBody::render( context, frame, position );
}

//----------------------------------------------------------------------------

CharString NounShip::displayName( bool a_bAllowTags ) const
{
	String sName;
	if ( a_bAllowTags && (m_Flags & NounShip::FLAG_SUBSCRIBER) != 0 )
		sName += "<image;subscriber;16;16>";

	sName += BaseNode::name();
	TraitDisplayName * pDisplayName = findTrait<TraitDisplayName>();
	if ( pDisplayName != NULL )
		sName += CharString("\n") + pDisplayName->displayName();

	return sName;
}

NounType NounShip::nounType() const
{
	return TYPE_OBJECT;
}

bool NounShip::canCollide() const
{
	if ( tick() < m_nIgnoreCollisionTick )
		return false;
	return true;
}

bool NounShip::collidable() const
{
	return true;
}

bool NounShip::enableECM() const
{
	return true;
}

bool NounShip::canAttachCargo( Noun * pCargo ) const
{
	// make sure the cargo has a context, or isEnemy / isFriend won't work correctly!
	if (! pCargo->context() )
		pCargo->setContext( context() );

	if ( isEnemy( pCargo ) )
	{
		// if someone is trying to unload units onto this ship, allow +4 enemy units over max cargo capacity..
		return (availableCargo() + 4 - enemyUnitCount()) > 0;
	}	

	// friendly or neutral cargo item
	return availableCargo() > 0;
}

//----------------------------------------------------------------------------

void NounShip::setup()
{
	NounBody::setup();

	// clear all flags
	clearFlags( FLAG_ALL );
	// remove all damage
	m_Damage = 0;

	initializeGadgets();
	initializeEnhancements();
	initializeUnits();

	// update the characteristics of this ship
	updateCharacter();
	// fill energy
	m_Energy = maxEnergy();
}

void NounShip::initialize()
{
	NounBody::initialize();

	initializeGadgets();
	initializeEnhancements();
	initializeUnits();
	updateCharacter();

	m_nDynamicsTick = m_Tick;
	m_nIgnoreCollisionTick = m_Tick + INGORE_COLLISION_TIME;
	m_Signature = 0;
}

void NounShip::release()
{
	NounBody::release();

	// make sure everything gets released
	m_OrderTarget = NULL;
	m_pHome = NULL;
	m_CommandTarget = NULL;
	m_Contacts.release();
	m_Gadgets.release();
	m_Shields.release();
	m_Armor.release();
	m_Enhancements.release();
	m_UseGadgets.release();
	m_UseTarget = NULL;
	m_JumpDrive = NULL;
}

void NounShip::simulate( dword nTick )
{
	PROFILE_FUNCTION();

	// update the ship
	if ( !isSleeping() && m_bShipEnabled )
	{
		updateShip( nTick );			// update ship systems
		updateDynamics( nTick );		// update movement, roll, etc..
		updateCombat( nTick );			// update out of combat timer
	}

	NounBody::simulate( nTick );
}

void NounShip::update()
{
	NounBody::update();

	// reduce signature if any modifiers are attached to do so..
	m_LastSignature *= calculateModifier( MT_SIGNATURE, true );
	// reset repaired damage
	m_nRepairedDamage = 0;
}

void NounShip::collision( Noun * pCollide, const Vector3 & intersect )
{
	if ( WidgetCast<NounShip>( pCollide ) )
	{
		NounShip::WeakRef pCollideShip = (NounShip *)pCollide;

		// check if ship is already in list, if found then don't add it again..
		if ( std::find( m_ShipCollisions.begin(), m_ShipCollisions.end(), pCollideShip ) == m_ShipCollisions.end() )
			m_ShipCollisions.push_back( pCollideShip );
	}
}

bool NounShip::canDamage( dword type ) const
{
	return true;
}

void NounShip::inflictDamage( dword nWhen, Noun * pFrom, int damage, dword type, const Vector3 & direction )
{
	if ( isDestroyed() )		// don't bother if ship is already destroyed!
		return;

	// only increase timer if an enemy is attacking us - we don't want to promote griefing
	if ( isEnemy( pFrom ) )
		setOutOfCombat(); // update combat timer

	// if we are not the server, then halve the damage applied to avoid over estimatation the damage we are
	// inflicting on another ship.. 
	if (! isServer() )
		damage = damage * CLIENT_SIDE_DAMAGE;

	Facing eFacing = getFacing( atan2( direction.x, direction.z ), false );

	// apply any damage modifiers
	damage = damage * calculateModifier( MT_DAMAGE_REDUCTION, true );

	LOG_DEBUG_LOW( "NounShip", CharString().format("Damaged inflicted, When: %u, From: %s, Damage: %d, Type: 0x%X, Facing: %u", 
		nWhen, pFrom != NULL ? pFrom->name() : "NULL", damage, type, eFacing) );

	// callback so AI controlled ships can receive notification that they have been damaged by someone
	onAttacked( pFrom, damage, type );

	// calculate the points we're going to award to the attacker now before shields/armor reduce 
	// the damage applied to the hull.
	float fDamagePoints = float( damage ) / DAMAGE_POINTS_DIV; 

	// check shields
	if( damage > 0 )
	{	
		for(int i=0;i<m_Shields.size();i++)
			damage = m_Shields[i]->deflect( type, damage, eFacing, direction );
	}

	// check armor
	if ( damage > 0 )
	{
		for(int i=0;i<m_Armor.size();i++)
			damage = m_Armor[i]->deflect( type, damage, eFacing );
	}

	int internalDamage = damage;

	// inflict internal damage if hull is 50% or less
	int halfHull = maxDamage() >> 1;
	if ( damage > 0 && m_Damage > halfHull && (type & DAMAGE_EMP) == 0 )
	{
		// once hull is under 50%, start causing internal damage
		type |= DAMAGE_EMP;
		// scale internal damage up based on how much under 50% the hull 
		internalDamage = (damage * (m_Damage - halfHull)) / halfHull;
	}

	if ( damage > 0 && (type & (DAMAGE_EMP|DAMAGE_ELF)) )
	{
		// get the radius of this ship
		float myRadius = radius();
		ASSERT( myRadius > 0.0f );

		// calculate the hit position in object space of this ship
		Vector3 hitPosition( direction );
		hitPosition.normalize();				// direction is not normalized, it's the delta from the projectile/explosion, to the center of the ship
		hitPosition *= -(myRadius * 0.5f);		// flip the direction so it goes away from the center of the ship

		if ( isServer() && m_Gadgets.size() > 0 )
		{
			// seed with the time-stamp of the damage so we get the same gadget damaged
			// on all clients.
			srand( nWhen );
			int nPicked = rand() % m_Gadgets.size();

			NounGadget * pDamageGadget = m_Gadgets[ nPicked ];
			if ( pDamageGadget != NULL )
			{
				float fDistance = (pDamageGadget->position() - hitPosition).magnitude();
				float fScale = (1.0f - (fDistance / myRadius));

				if ( fScale > 0.0f )
					pDamageGadget->inflictDamage( nWhen, pFrom, fScale * internalDamage, type, direction );
			}
		}
	}

	if ( damage > 0 && (type & DAMAGE_ELF) )
	{
		// drain energy from main energy bank
		int drained = Min( energy(), damage );
		setEnergy( energy() - drained );

		// give energy to attacking ship
		if ( WidgetCast<NounShip>( pFrom ) )
			((NounShip *)pFrom)->setEnergy( ((NounShip *)pFrom)->energy() + drained );
	}

	// update the stats if it is another ship - NOTE this needs to happen before the DestroyShip
	// code below otherwise a player who is damaging this ship may not get credit/loot.
	if ( WidgetCast<NounShip>( pFrom ) )
	{
		NounShip * pFromShip = (NounShip *)pFrom;
		ASSERT( pFromShip );

		// set combat timer on attacking ship
		pFromShip->setOutOfCombat();

		// non-hull damages gives a lessor percentage of points..
		if ( damage <= 0 )
			fDamagePoints *= NOHULL_DAMAGE_POINTS_SCALE;

		if (! isFriend( pFrom ) )
		{
			// if the attacking ship is destroyed, then treat this as kamikaze points
			if ( pFromShip->isDestroyed() )
				gameContext()->gameUser()->onKamikaze( pFrom, 1.0f );
			else
				gameContext()->gameUser()->onDamageShip( pFrom, fDamagePoints, this);
		}
		else
		{
			gameContext()->gameUser()->onFriendlyFire( pFrom, fDamagePoints );
		}
	}

	if ( damage > 0 && (type & (DAMAGE_KINETIC|DAMAGE_ENERGY)) )
	{
		ASSERT( context() );

		// cap the damage, 
		damage = Min( maxDamage() - m_Damage, damage );

		// add the damage to the hull
		m_Damage = Min( m_Damage + damage, maxDamage() );
		// update the damage bits
		updateDamageBits();

		// check if the ship was destroyed
		if ( isServer() && isDestroyed() )
		{
			// ship is destroyed, send out verb to destroy this ship to all local clients
			Verb::Ref( new VerbDestroyShip( this, pFrom, false ) );
		}
	}
}

//---------------------------------------------------------------------------------------------------

CharString NounShip::status() const
{
	CharString sTargetInfo;

	// team & ship type..
	sTargetInfo += CharString().format("%s - %s\n", teamName(), nounContext()->name() );
	// hull %
	sTargetInfo += CharString().format("Hull: %d%%\n", int( damageRatioInv() * 100) );

	int armor = 0, maxArmor = 0;
	int shield = 0, maxShield = 0;
	for(int i=0;i<childCount();i++)
	{
		BaseNode * pChild = child(i);
		if ( WidgetCast<GadgetShield>( pChild ) )
		{
			shield += ((GadgetShield *)pChild)->charge();
			maxShield += ((GadgetShield *)pChild)->maxCharge();
		}
		else if ( WidgetCast<GadgetArmor>( pChild ) )
		{
			armor += ((GadgetArmor *)pChild)->armor();
			maxArmor += ((GadgetArmor *)pChild)->maxArmor();
		}
	}

	if ( maxShield > 0 )
		sTargetInfo += CharString().format("Shield: %d%%\n", (shield * 100) / maxShield);
	if ( maxArmor > 0 )
		sTargetInfo += CharString().format("Armor: %d%%\n", (armor * 100) / maxArmor );

	return sTargetInfo;
}

float NounShip::viewDetectScale() const
{
	return Max( m_fVelocity, 5.0f );
}

//---------------------------------------------------------------------------------------------------

int	NounShip::energyClassPriority( EnergyClass nClass ) const
{
	// TODO: Implement energy priority system, for now just return the energy class as the priority
	return (int)nClass;
}

bool NounShip::isMonster() const
{
	return false;
}

bool NounShip::enableGateSpawn() const
{
	return false;
}

bool NounShip::enableRollDynamics() const
{
	return true;
}

dword NounShip::despawnTime() const
{
	return 1 * TICKS_PER_SECOND;
}

float NounShip::controlWeight() const
{
	return 0.0f;
}

//----------------------------------------------------------------------------

int NounShip::value() const
{
	int value = baseValue();
	for(int i=0;i<childCount();i++)
	{
		NounGadget * pGadget = WidgetCast<NounGadget>( child(i) );
		if ( pGadget != NULL )
			value += pGadget->addValue() * (pGadget->level() + 1);
	}

	return( value );
}

//----------------------------------------------------------------------------
// TODO: add in enhancement data here
void NounShip::updateCharacter()
{
	PROFILE_FUNCTION();

	float m = mass();
	m *=  calculateModifier( MT_MASS );
	if ( m < 1.0f )
		m = 1.0f;

	m_Sensor = baseSensor();
	m_View = baseView();
	m_MaxDamage = baseDamage();
	m_MaxEnergy = baseEnergy();
	m_MaxVelocity = baseVelocity();
	m_MaxCargo = baseCargo();
	m_MaxThrust = baseThrust();

	for(int i=0;i<m_Gadgets.size();i++)
	{
		NounGadget * pGadget = m_Gadgets[i];

		m				+= pGadget->addMass();
		m_Sensor		+= pGadget->addSensor();
		m_MaxVelocity	+= pGadget->addMaxVelocity() * velocityModifier();
		m_MaxDamage		+= pGadget->addMaxDamage();
		m_MaxEnergy		+= pGadget->addMaxEnergy();
		m_MaxCargo		+= pGadget->addCargo();
		m_MaxThrust		+= pGadget->addThrust();
	}

	//calculate the acceleration by taking the thrust and dividing by the mass of the ship
	m_MaxAcceleration = m_MaxThrust / m;
	
	//calculate the turn rate (yaw) by taking the baseYaw(), dividing by the mass of the ship and adding a fraction of m_MaxAcceleration
	m_MaxYaw = (baseYaw() / m) + (m_MaxAcceleration / 10);

	//apply enhancements to ships, including gadget effects
	m_MaxVelocity *=  calculateModifier( MT_DRIVE_VELOCITY );
	m_MaxThrust	*= calculateModifier( MT_DRIVE_THRUST );
	m_MaxYaw *= calculateModifier( MT_TURNRATE );
	m_MaxEnergy *= calculateModifier( MT_SHIPENERGY );
}

void NounShip::initializeGadgets()
{
	m_JumpDrive = NULL;
	m_Gadgets.release();
	m_Shields.release();
	m_Armor.release();

	// make a list of all child objects first, sort them into the gadgets array sorted by type
	Array< NounGadget * > gadgets;
	for(int i=0;i<childCount();i++)
	{
		BaseNode * pChild = child(i);
		if ( WidgetCast<NounGadget>( pChild ) != NULL )
		{
			NounGadget * pGadget = (NounGadget *)pChild;

			// set the gadget level
			pGadget->setLevel( gadgetLevel() );

			if ( WidgetCast<GadgetJumpDrive>( pGadget ) )
			{
				m_JumpDrive = (GadgetJumpDrive *)pGadget;
			}
			else if ( WidgetCast<GadgetShield>( pGadget ) )
			{
				m_Shields.push( (GadgetShield *)pGadget );
				// set the shield modifier
				((GadgetShield *)pGadget)->setModifier( shieldModifier() );
			}
			else if ( WidgetCast<GadgetArmor>( pGadget ) )
			{
				m_Armor.push( (GadgetArmor *)pGadget );
				// set the armor modifier
				((GadgetArmor *)pGadget)->setModifier( armorModifier() );
			}

			bool bInserted = false;
			for(int k=0;k<gadgets.size() && !bInserted;k++)
				if ( energyClassPriority( pGadget->energyClass() ) < energyClassPriority( gadgets[k]->energyClass() ) )
				{
					bInserted = true;
					gadgets.insert( k, pGadget );
				}

			if (! bInserted )
				gadgets.push( pGadget );
		}
	}

	// now push all gadgets in order of energy priority level..
	for(int i=0;i<gadgets.size();i++)
		m_Gadgets.push( gadgets[ i ] );
}

void NounShip::initializeEnhancements()
{
	m_Enhancements.release();
	for(int i=0;i<childCount();i++)
	{
		BaseNode * pChild = child(i);
		if ( WidgetCast<NounEnhancement>( pChild ) )
			m_Enhancements.push( (NounEnhancement *)pChild );
	}
}

void NounShip::initializeUnits()
{
	// make sure all units onboard are friendly
	for(int i=0;i<childCount();i++)
		if ( WidgetCast<NounUnit>( child(i) ) )
		{
			((NounUnit *)child(i))->setTeamId( teamId() );
			((NounUnit *)child(i))->setUserId( userId() );
		}
}

bool NounShip::needRepair( Noun * pWhom )
{
	if ( m_Damage > 0 )
		return true;

	for(int i=0;i<childCount();i++)
	{
		NounGadget * pGadget = WidgetCast<NounGadget>( child(i) );
		if (! pGadget )
			continue;
		if ( pGadget->needRepair( pWhom ) )
			return true;
	}

	return false;
}

bool NounShip::repair( Noun * pFrom, int nRepairRate, bool bGadgetRepair )
{
	if (! needRepair( pFrom ) )
		return false;
		
	nRepairRate = Min( maxRepair() - m_nRepairedDamage, nRepairRate );
	if ( nRepairRate > 0 )
	{
		m_nRepairedDamage += nRepairRate;
		m_Damage = Max( m_Damage - nRepairRate, 0 );
		updateDamageBits();

		if ( bGadgetRepair ) 
		{
			for(int i=0;i<childCount();i++)
			{
				NounGadget * pGadget = WidgetCast<NounGadget>( child(i) );
				if (! pGadget )
					continue;
				// repair gadget damage
				pGadget->repair( pFrom, nRepairRate );
			}
		}
	}

	return true;
}

bool NounShip::needReload( Noun * pWhom )
{
	for(int i=0;i<childCount();i++)
	{
		NounGadget * pGadget = WidgetCast<NounGadget>( child(i) );
		if (! pGadget )
			continue;
		if ( pGadget->needReload(pWhom) )
			return true;
	}

	return false;
}

bool NounShip::reload( Noun * pFrom )
{
	if (! needReload( pFrom ) )
		return false;

	bool bReloading = false;
	for(int i=0;i<childCount();i++)
	{
		NounGadget * pGadget = WidgetCast<NounGadget>( child(i) );
		if (! pGadget )
			continue;

		// reload gadget
		bReloading |= pGadget->reload( pFrom );
	}

	return bReloading;
}

void NounShip::repairGadget( NounGadget * pRepair, When when )
{
	int position = m_RepairQueue.search( pRepair );
	if ( position < 0 )
		return;		// not currently in the queue

	switch( when )
	{
	case NOW:
		if ( position > 0 )
			m_RepairQueue.move( position, 0 );
		break;
	case SOONER:
		if ( position > 0 )
			m_RepairQueue.swap( position, position - 1 );
		break;
	case LATER:
		if ( position < (m_RepairQueue.size() - 1) )
			m_RepairQueue.swap( position, position + 1 );
		break;
	case LAST:
		if ( position < (m_RepairQueue.size() - 1) )
			m_RepairQueue.move( position, m_RepairQueue.size() - 1 );
		break;
	}
	updateVersion();
}

void NounShip::destroyShip( Noun * pKiller, bool bZeroVelocity )
{
	// make sure the damage is enough
	m_Damage = maxDamage();
	// clear the current order & command, so if they respawn the ship doesn't try to keep doing it's last command
	setCommand( NOCOMMAND, NULL );
	setOrder( NOORDER, NULL, NULL );

	// inform the context user first
	if ( pKiller != this )
	{
		gameContext()->gameUser()->onDestroyed( this, pKiller );
		if ( pKiller != NULL )
			gameContext()->gameUser()->onKill( pKiller, this );
	}
	else 
		gameContext()->gameUser()->onSelfDestruct( this );

	if ( WidgetCast<NounPlanet>( pKiller ) )
		gameContext()->gameUser()->onCollidePlanet( this );

	// create the primary explosion
	if ( context()->isClient() )
	{
		Scene * pScene = destroyEffect();
		if ( pScene != NULL )
		{
			// TODO: no need to add this explosion of this player is nowhere near the explosion
			// ship destroyed, create the explosion effect
			SceneryDebris * pEffect = new SceneryDebris;
			pEffect->setNounContext( new NounContext( pScene ) );
			pEffect->setContext( context() );
			pEffect->setPosition( worldPosition() );
			pEffect->setDelay( 0.0f );
			pEffect->setFrame( frame() );

			if (! bZeroVelocity )
				pEffect->setVelocity( Vector3( sin( m_fHeading ) * m_fVelocity, 0.0f, cos( m_fHeading) * m_fVelocity ) );
			if (! pEffect->setSegment( "Effect" ) )
				pEffect->setLife( 15.0f );

			context()->attachNoun( pEffect );
		}

		NounTrail::CopyTrailsIntoZone( this );
	}

	// list of destroyed items to remove from ship..
	std::list< Noun * > destroyed;

	// damage enhancements, once they reach their maxDamage() they are destroyed..
	for(int i=0;i<childCount();++i)
	{
		NounEnhancement * pEnh = WidgetCast<NounEnhancement>( child(i) );
		if ( pEnh != NULL )
		{
			if ( pEnh->incrementDamage() )
			{
				LOG_STATUS( "NounShip", "[ENHANCEMENT]:Destroyed:%s:%s:%u", pEnh->getName(), name(), userId() );
				message( CharString().format( "<color;ffffff>Comms: Enhancement '%s' destroyed!", pEnh->getName() ) );
				destroyed.push_back( pEnh );
			}
			continue;
		}

		// remove any beacons on death..
		NounBeacon * pBeacon = WidgetCast<NounBeacon>( child(i) );
		if ( pBeacon != NULL )
		{
			destroyed.push_back( pBeacon );
			continue;
		}
	}

	// server-side only logic
	if ( isServer() )
	{
		// check for area damage
		Array< GameContext::NounCollision > collide;
		if ( context()->proximityCheck( worldPosition(), explodeArea(), collide ) )
		{
			for(int i=0;i<collide.size();i++)
			{
				NounGame * pCollide = WidgetCast<NounGame>( collide[i].pNoun );
				if ( !pCollide || pCollide == this )
					continue;

				if ( pCollide->canDamage( DAMAGE_ENERGY | DAMAGE_KINETIC ) &&
					collide[i].fDistance < explodeArea() )
				{
					float damageRatio = 1.0f - (collide[i].fDistance / explodeArea());
					int damage = damageRatio * explodeDamage();
					
					if ( damage > 0 )
					{
						pCollide->inflictDamage( tick(), this, damage, DAMAGE_ENERGY | DAMAGE_KINETIC, 
							pCollide->worldFrame() * (worldPosition() - pCollide->worldPosition()) );
					}
				}
			}
		}

		// drop enhancements after we do area damage, otherwise we'll destroy the dropped loot..
		dropLoot();

		// space-monsters do not leave behind devices
		if ( ! isMonster() )
		{
			// eject some of the resources used to build the ship into space
			int nAmount = DESTROYED_SHIP_RESOURCE * buildCost( this );
			while( nAmount > 0 )
			{
				CargoResource::Ref pCargo = new CargoResource( nAmount );
				nAmount -= pCargo->quantity();

				Vector3 vPosition( worldPosition() + RandomVector( -radius(), radius() ) );
				vPosition.y = 0.0f;

				pCargo->setPosition( vPosition );
				context()->attachNoun( pCargo );
			}

			//// check for any cargo items...  if found put them into space
			//for(int i=0;i<childCount();i++)
			//{
			//	BaseNode * pChild = child(i);
		
			//	NounCargo::Ref pCargo;
			//	if ( WidgetCast<NounCargo>( pChild ) )
			//	{
			//		int roll = rand() % 100;
			//		if ( roll < ((NounCargo *)pChild)->durability() )
			//			pCargo = (NounCargo *)pChild->copy();
			//	}

			//	if ( pCargo.valid() )
			//	{
			//		Vector3 vPosition( worldPosition() + RandomVector( -5.0f, 5.0f ) );
			//		vPosition.y = 0.0f;

			//		pCargo->setPosition( vPosition );
			//		context()->attachNoun( pCargo );
			//	}
			//}
		}

		// last before detach so that players dont dupe thier enhancements
		// that were dropped
		// save off storage with this "active" ship
		if(userId() != 0)
			context()->user()->saveStorage(this);

	}

	// remove desrtroyed items from ship..
	while( destroyed.size() > 0 )
	{
		Noun * pDestroyed = destroyed.front();
		destroyed.pop_front();

		if ( pDestroyed )
			pDestroyed->detachSelf();
	}

	// lastly, detach the ship from the universe
	setDetach();
}

int NounShip::validateAllyShips()
{
	for( int i=0;i<(int)m_Allies.size();)
	{
		NounShip * pAlly = m_Allies[ i ];
		if (! validateNoun( pAlly ) )
			m_Allies.erase( m_Allies.begin() + i );
		else
			++i;
	}

	return m_Allies.size();
}

void NounShip::addAlly( NounShip * pAlly )
{
	if ( std::find( m_Allies.begin(), m_Allies.end(), pAlly ) == m_Allies.end() )
		m_Allies.push_back( pAlly );
}

void NounShip::setHome( Noun * pHome )
{
	m_pHome = pHome;
}

void NounShip::setCaptureTarget( Noun * pTarget )
{
	m_pCaptureTarget = pTarget;
	//m_nCombatTick = OUT_OF_COMBAT_TIME; // update ooc timer
}

void NounShip::despawn( dword nTicks /*= 0*/ )
{
	m_bDespawning = true;
	m_nDespawnTicks = nTicks;
}

void NounShip::cancelDespawn()
{
	m_bDespawning = false;
}

void NounShip::addIncoming( NounProjectile * a_pProj )
{
	if ( userId() == 0 && isLocal() )
	{
		m_Incoming.push( a_pProj );
		m_bIncoming = true;
	}
}

void NounShip::removeIncoming( NounProjectile * a_pProj )
{
	if ( userId() == 0 && isLocal() )
		m_Incoming.removeSearchSwap( a_pProj );
}

//---------------------------------------------------------------------------------------------------

void NounShip::message( const char * pMessage, bool a_bAllowRemote /*= false*/ )
{
	if ( isLocal() )
		clientChat( pMessage );
	else if ( a_bAllowRemote )
		privateChat( pMessage );
}

void NounShip::protection( Facing face, int & shieldmax, int & shielddamage, int & armormax, int & armordamage)
{
	shieldmax = 0;
	shielddamage = 0;
	armormax = 0;
	armordamage = 0;

	for(int i=0;i<m_Shields.size();i++)
	{
		GadgetShield * pShield = m_Shields[i];
		if ( (pShield->flags() & NounGadget::facingMountFlags(face)) != 0 && pShield->active() )
		{
			shieldmax += pShield->maxCharge();
			shielddamage += pShield->charge();
		}
	}

	for(int j=0;j<m_Armor.size();j++)
	{
		GadgetArmor * pArmor = m_Armor[j];
		if ( (pArmor->flags() & NounGadget::facingMountFlags(face)) != 0 )
		{
			armormax += pArmor->maxArmor();
			armordamage += pArmor->armor();
		}
	}

}

void NounShip::protection( Facing face, int & max, int & damage )
{
	max = 0;
	damage = 0;
	
	for(int i=0;i<m_Shields.size();i++)
	{
		GadgetShield * pShield = m_Shields[i];
		if ( (pShield->flags() & NounGadget::facingMountFlags(face)) != 0 && pShield->active() )
		{
			max += pShield->maxCharge();
			damage += pShield->charge();
		}
	}

	for(int j=0;j<m_Armor.size();j++)
	{
		GadgetArmor * pArmor = m_Armor[j];
		if ( (pArmor->flags() & NounGadget::facingMountFlags(face)) != 0 )
		{
			max += pArmor->maxArmor();
			damage += pArmor->armor();
		}
	}
}

GadgetJumpDrive * NounShip::jumpDrive() 
{
	if (! m_JumpDrive.valid() || m_JumpDrive->parent() != this )
	{
		for(int i=0;i<childCount();i++)
			if ( WidgetCast<GadgetJumpDrive>( child(i) ) )
				m_JumpDrive = (GadgetJumpDrive *)child(i);
	}

	return m_JumpDrive;
}

GadgetJumpGate * NounShip::jumpGate() 
{
	GadgetJumpGate * pJumpGate = NULL;
	for(int i=0;i<childCount();i++)
		if ( WidgetCast<GadgetJumpGate>( child(i) ) )
			pJumpGate = (GadgetJumpGate *)child(i);

	return pJumpGate;
}

GadgetCloak * NounShip::gadgetCloak() 
{
	GadgetCloak * pGadgetCloak = NULL;
	for(int i=0;i<childCount();i++)
		if ( WidgetCast<GadgetCloak>( child(i) ) )
			pGadgetCloak = (GadgetCloak *)child(i);

	return pGadgetCloak;
}

bool NounShip::canDetect( Noun * pNoun ) const
{
	return NounGame::canDetect( pNoun, sensor(), view() );
}

bool NounShip::canOrbit( Noun * pTarget )
{
	return VerbEnterOrbit::canOrbit( this, pTarget );
}

Facing NounShip::getFacing( float direction, bool bTowards /*= true*/ )
{
	// determine the facing from the direction
	if ( bTowards )
		direction += PI;

	float angle = NormalizeAngle( direction );
	//// build a vector from the direction of the impact
	//Vector3 local( sinf(direction), 0, cosf(direction) );
	//// check the facing
	//float angle = atan2( local.x, local.z );		// value is always from -PI to PI
	if ( angle > -PI4 )
	{
		if ( angle > PI4 )
		{
			if ( angle < (PI - PI4) )
				return FACING_RIGHT;
			return FACING_BACK;
		}
		return FACING_FRONT;
	}
	
	if ( angle > -(PI - PI4) )
		return FACING_LEFT;
	return FACING_BACK;
}

Facing NounShip::getFacing( const Vector3 & vWorld )
{
	Vector3 vDirection( worldFrame() * (vWorld - worldPosition()) );
	vDirection.normalize();
	return getFacing( atan2( vDirection.x, vDirection.z ), false );
}

//----------------------------------------------------------------------------

void NounShip::updateShip( dword nTick )
{
	PROFILE_FUNCTION();

	// update despawn/docking logic..
	updateDespawn();
	// update the ships gadgets
	updateGadgets( nTick );
	// update high-level AI
	updateLogic( nTick );		

	// repair / reload ship when in orbit
	if ( nTick >= m_nRepairTick )
	{
		m_nRepairTick = nTick + DEFAULT_REPAIR_RATE;
		if ( inOrbit() )
		{
			// handle ship repairs when in orbit around friendly planet with a ship yard
			NounPlanet * pPlanet = WidgetCast<NounPlanet>( orbiting() );
			if ( pPlanet != NULL )
			{
				if ( pPlanet->isFriend( this ) )
				{
					repair( pPlanet, PLANET_REPAIR_RATE, true );
					reload( pPlanet );
				}
				else if ( (pPlanet->flags() & NounPlanet::FLAG_HYDROGEN) != 0 )
				{
					// if planet has hydrogen, then refuel the jump drive
					if ( jumpDrive() != NULL )
						jumpDrive()->reload( pPlanet );
				}
			}
		}
	}

	if ( nTick >= m_nUpdateTick )
	{
		m_nUpdateTick = nTick + SHIP_UPDATE_RATE;
		// update characteristics
		updateCharacter();
		// update low-level AI
		updateCommand();
		// update the repair queue
		updateRepairQueue();
		// update the contact list
		updateContacts();
		// drain off some damage from the damage list
		updateThreats();
	}

	if ( m_bIncoming )
	{
		ASSERT( userId() == 0 );
		avoidCollision();
		m_bIncoming = false;
	}

	updatePointDefense();
	updateFireCheck();

}

void NounShip::updateCombat( dword nTick )
{
	dword nElasped = nTick - m_Tick;
	if ( m_nCombatTick > nElasped )
	{
		m_nCombatTick -= nElasped;
		m_bOutOfCombat = false;

		cancelDespawn();
	}
	else
	{
		m_nCombatTick = 0;
		m_bOutOfCombat = true;
	}
}

void NounShip::updateDespawn()
{
	if ( m_bDespawning )
	{		
		if ( m_nDespawnTicks > 0 )
			--m_nDespawnTicks;

		if ( m_nDespawnTicks == 0 )
		{
			if ( isServer() )
				setDetach();
			m_bDespawning = false;
		}
	}
}

void NounShip::updateFireCheck()
{
	if ( userId() == 0 && isLocal() && m_Command == ENGAGE && m_CommandTarget.valid() )
	{
		dword nNextFireCheck = m_nLastFireCheck + (CHECK_FIRE_INTERVAL * TICKS_PER_SECOND);
		if ( tick() > nNextFireCheck  )
		{
			m_nLastFireCheck = tick();
			m_pLastFireTarget = m_CommandTarget;
			m_fLastFireDistance = (m_CommandTarget->worldPosition() - worldPosition()).magnitude();

			CollisionInfo info;
			if ( checkForCollision( m_CommandTarget, &info, false ) )
			{
				if ( WidgetCast<NounShip>( info.m_pCollide ) == NULL || isFriend( info.m_pCollide ) )
					m_bWeaponsFree = false;		// something is in the way, no firing unless it's a enemy ship ..
				else
					m_bWeaponsFree = true;
			}
			else
				m_bWeaponsFree = true;		// no collision, allow firing...
		}
	}
}


void NounShip::updateDamageBits()
{
	// update the damage bits
	m_DamageBits = 0;
	float damage = damageRatio();
	if ( damage > 0.20f )
	{
		m_DamageBits |= DAMAGE_1;
		if ( damage > 0.40f )
		{
			m_DamageBits |= DAMAGE_2;
			if ( damage > 0.60f )
			{
				m_DamageBits |= DAMAGE_3;
				if ( damage > 0.80f )
				{
					m_DamageBits |= DAMAGE_4;
					if ( damage >= 1.0f )
						m_DamageBits |= DESTROYED;
				}
			}
		}
	}
}

void NounShip::updateRepairQueue()
{
	PROFILE_FUNCTION();

	// look for damaged items not currently in the repair queue
	for(int i=0;i<m_Gadgets.size();i++)
	{
		NounGadget * pGadget = m_Gadgets[i];
		if ( pGadget->damage() > 0 && m_RepairQueue.search( pGadget ) < 0 )
		{
			m_RepairQueue.push( pGadget );		// add item to repair queue
			updateVersion();
		}
	}

	// remove repaired items from queue
	if ( m_RepairQueue.size() > 0 )
	{
		// repair the first item in the queue
		NounGadget * pRepair = m_RepairQueue[ 0 ];
		if ( pRepair != NULL )
			pRepair->repair( this, Max<int>( 1, pRepair->maxDamage() / SHIP_GADGET_REPAIR_RATE ) );

		// remove repaired items from queue
		for(int i=0;i<m_RepairQueue.size();)
			if ( m_RepairQueue[i] == NULL || m_RepairQueue[i]->parent() != this || m_RepairQueue[i]->damage() <= 0 )
			{
				m_RepairQueue.remove( i );
				updateVersion();
			}
			else
				i++;		// next item
	}
}

void NounShip::updateContacts()
{
	PROFILE_FUNCTION();

	m_Contacts.release();

	Array< GameContext::NounCollision > contacts;
	context()->proximityCheck( worldPosition(), MAX_SCANNER_RANGE, contacts );

	for(int j=0;j<contacts.size();j++)
	{
		Noun * pContact = contacts[j].pNoun;
		if ( canDetect( pContact ) )
		{
			m_Contacts.push( pContact );

			// we only bother with checking planet's for sub-contacts for now...
			if ( WidgetCast<NounPlanet>( pContact ) )
			{
				// check if we can detect the children of this object
				for(int i=0;i<pContact->childCount();i++)
				{
					Noun * pChild = (Noun *)pContact->child(i);
					if ( canDetect( pChild ) )
						m_Contacts.push( pChild );
				}
			}
		}
	}

	// clear any invalid projectiles from the incoming list..
	for(int i=0;i<m_Incoming.size();)
	{
		if (! m_Incoming[i].valid() )
			m_Incoming.removeSwap( i );
		else
			++i;
	}
}

void NounShip::updateGadgets( dword nTick )
{
	PROFILE_FUNCTION();

	// check gadgets list, initialize internal gadget queues if needed
	if ( m_Gadgets.size() == 0 )
		initializeGadgets();

	// update energy, signature, and visibility of this ship
	int energy = m_Energy;
	float visibility = 1.0f;

	for(int i=0;i<m_Gadgets.size();i++)
	{
		NounGadget * pGadget = m_Gadgets[i];

		// use energy
		energy	= pGadget->useEnergy( nTick, energy );
		// update signature of this ship
		addSignature( nTick, pGadget->addSignature() );
		// update visibility
		visibility += pGadget->addVisibility();
	}

	m_Energy = Clamp( energy, 0, maxEnergy() );
	m_Visibility = Clamp( visibility, 0.0f, 1.0f );

	// use gadgets that are in the queue
	if ( m_UseGadgets.size() > 0 )
		if ( --m_UseGadgetTick <= 0 )
			useGadgets();
}

void NounShip::useGadgets()
{
	bool bSendVerb = false;

	for(int i=0;i<m_UseGadgets.size();i++)
	{
		NounGadget * pGadget = m_UseGadgets[i];
		if ( pGadget != NULL && pGadget->usable( m_UseTarget, m_UseShift ) )
		{
			bSendVerb = true;
			break;
		}
	}

	if ( bSendVerb )
	{
		// send the verb to use the gadgets
		Verb::Ref( new VerbUseGadget( this, m_UseGadgets, m_UseTarget, m_UseShift ) );
	}

	m_UseGadgetTick = USE_GADGET_LATENCY;
	m_UseGadgets.release();
	m_UseTarget = NULL;
	m_UseShift = false;
}


// calculate intercept point from ship's current position and velocity
Vector3 NounShip::intercept( Noun * pIntercept, float fVelocity )
{
	Vector3 target( pIntercept->worldPosition() );
	if ( fVelocity < 1.0f )
		return target;

	Vector3 velocity( pIntercept->worldVelocity() );
	Vector3 myPosition( worldPosition() );

	// calculate the distance to the target
	float distance = (target - myPosition).magnitude();
	// caluclate the projectile time to the target
	float t = distance / fVelocity;	
	// calculate the intercept point
	Vector3 intercept( target + (velocity * t) );

	// refine the intercept position
	int recurseCount = INTERCEPT_RECURSE_COUNT;
	while( recurseCount > 0 )
	{
		recurseCount--;

		// recalculate the intercept point again
		distance = (intercept - myPosition).magnitude();
		t = distance / fVelocity;
		intercept = target + (velocity * t);
	}

	return intercept;
}

Noun* NounShip::randomGadget(dword when, int deviant)
{
	srand(when+deviant);
	return m_Gadgets.size() != 0 ? m_Gadgets[rand() % m_Gadgets.size()] : NULL;
}

CharString NounShip::commandString()
{
	return CharString().format("%s %s", commandText( m_Command ), m_CommandTarget.valid() ? m_CommandTarget->name() : "" );
}

CharString NounShip::orderString()
{
	return CharString().format("%s %s", orderText( m_Order ), m_OrderTarget.valid() ? m_OrderTarget->name() : "" );
}

//----------------------------------------------------------------------------

const char * NounShip::commandText( Command command )
{
	static const char * COMMAND_TEXT[] = 
	{
		"NONE",		
		"ORBIT",
		"FOLLOW",
		"SHADOW",
		"EVADE",
		"ENGAGE",			
		"STOP",	
		"REFUEL",
	};

	if ( command >= 0 && command < sizeof(COMMAND_TEXT)/sizeof(COMMAND_TEXT[0]) )
		return COMMAND_TEXT[ command ];
	return "?";
}

const char * NounShip::orderText( Order order )
{
	static const char * ORDER_TEXT[] = 
	{
		"CANCEL",
		"ATTACK",
		"DEFEND",
		"CAPTURE",
		"GOTO",
		"REPAIR",
		"BEACON",
		"HOLD",
		"TRADE",
		"SCOUT",
		"BUILD",
		"FALLBACK"
	};
	if ( order >= 0 && order < sizeof(ORDER_TEXT)/sizeof(ORDER_TEXT[0]) )
		return ORDER_TEXT[ order ];
	return "?";
}

Color NounShip::orderColor( Order order )
{
	static const Color ORDER_COLORS[] =
	{
		WHITE,	// NOORDER
		RED,	// ATTACK
		BLUE,	// DEFEND
		ORANGE,	// CAPTURE
		GOLD,	// MOVE
		CYAN,	// RELOAD
		GREEN,	// BEACON
		BLACK,	// HOLD
		SKYBLUE,// TRADE
		YELLOW,	// RECON
		LAVENDER,	// BUILD
	};
	if ( order >= 0 && order < sizeof(ORDER_COLORS)/sizeof(ORDER_COLORS[0]) )
		return ORDER_COLORS[ order ];
	return GREY;
}

const char * NounShip::typeText( Type type )
{
	static const char * TYPE_TEXT[] = 
	{
		"UNKNOWN",
		"FIGHTER",
		"ENGINEER",
		"SUPPLY",
		"TRANSPORT",
		"SCOUT",
		"FRIGATE",
		"DESTROYER",
		"CRUISER",
		"DREAD",
		"STATION",
		"PLATFORM",
		"SUPPLY",
	};

	if ( type >= 0 && type < sizeof(TYPE_TEXT)/sizeof(TYPE_TEXT[0]) )
		return TYPE_TEXT[ type ];
	return "?";
}

int NounShip::buildTechnology( NounShip * pShip )
{
	int nTechnology = pShip->buildTechnology();
	for(int i=0;i<pShip->childCount();i++)
	{
		NounGadget * pGadget = WidgetCast<NounGadget>( pShip->child(i) );
		if ( pGadget != NULL )
			nTechnology = Max( nTechnology, pGadget->buildTechnology() );
	}

	return nTechnology;
}

int	NounShip::buildCost( NounShip * pShip )
{
	if ( pShip->enableGateSpawn() )
		return 0;		// no costs to spawn this ship..

	int nCost = pShip->buildCost();
	for(int i=0;i<pShip->childCount();i++)
	{
		NounGadget * pGadget = WidgetCast<NounGadget>( pShip->child(i) );
		if ( pGadget != NULL )
			nCost += pGadget->buildCost();
	}

	return nCost;
}

dword NounShip::buildFlags( NounShip * pShip )
{
	dword nFlags = pShip->buildFlags();
	for(int i=0;i<pShip->childCount();i++)
	{
		NounGadget * pGadget = WidgetCast<NounGadget>( pShip->child(i) );
		if ( pGadget != NULL )
			nFlags |= pGadget->buildFlags();
	}

	return nFlags;
}

//----------------------------------------------------------------------------

void NounShip::setOutOfCombat( dword timer )
{
	int nCombatTime = OUT_OF_COMBAT_TIME + (TICKS_PER_SECOND * (30 * gadgetLevel()));
	m_nCombatTick = timer > 0 ? timer : nCombatTime;	
}

//----------------------------------------------------------------------------
// EOF



