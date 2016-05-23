/*
	NounPlanet.cpp
	(c)1999 PaleStar, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Debug/Profile.h"
#include "Render3D/NodeMesh.h"
#include "Render3D/NodeComplexMesh.h"
#include "Render3D/NodeComplexMesh2.h"
#include "Render3D/NodeLight.h"
#include "World/VerbChat.h"

#include "GadgetShield.h"
#include "NounPlanet.h"
#include "NounShip.h"
#include "NounUnit.h"
#include "NounStructure.h"
#include "NounProjectile.h"
#include "NounStar.h"
#include "NounEnhancement.h"
#include "CargoEnhancement.h"
#include "StructureDefense.h"
#include "StructureDepot.h"
#include "StructurePort.h"
#include "StructureHub.h"
#include "StructureFactory.h"
#include "Constants.h"
#include "VerbSetFlags.h"
#include "VerbScrap.h"
#include "VerbDestroyShip.h"
#include "VerbPlanetEvent.h"
#include "VerbRevolt.h"
#include "VerbDistress.h"
#include "VerbDisband.h"
#include "VerbCapturePlanet.h"
#include "VerbTradeRoute.h"
#include "VerbTransferResource.h"
#include "VerbEnterOrbit.h"
#include "resource.h"
#include "GameContext.h"
#include "ShipPlatform.h"
#include "TraitOrbit.h"
#include "GameProfile.h"

//----------------------------------------------------------------------------

const qword		DEFENSE_PROJECTILE	= StringHash64( "misc/planet_defenses/projectile.prt" ).hash();

const int		PRODUCTION_UPDATE	= TICKS_PER_SECOND * 10;
const int		ENCONOMY_UPDATE		= TICKS_PER_SECOND * 60;
const int		ALLEGIANCE_UPDATE	= TICKS_PER_SECOND * 300;
const int		EVENT_UPDATE		= TICKS_PER_SECOND * 300;
const float		CONTROL_UPDATE		= 5.0f;
const float		DEFENSE_UPDATE		= 0.1f;
const float		HEX_SIZE			= 6.0f;		// physical size of a hex
const float		HEX_HALF			= HEX_SIZE * 0.5f;
const int		DISTRESS_MESSAGE_DELAY = TICKS_PER_SECOND * 60;		// how often to send distress messages
const dword		MAX_NOUNS_PER_HEX	= 4;
const int		MIN_DAMAGE_SPLIT	= 5;		// damage is split between cappers no higher than this amount

static Constant PLANET_STATUS_LINE_DURATION( "PLANET_STATUS_LINE_DURATION", 2.0f );
static Constant	PLANET_SCANNER_RANGE( "PLANET_SCANNER_RANGE", 2000.0f );
static Constant	PLANET_SENSOR_RANGE( "PLANET_SENSOR_RANGE", 750.0f );
static Constant	PLANET_VIEW_RANGE( "PLANET_VIEW_RANGE", 4.0f );

static Constant	HEX_STACK_SPACE( "HEX_STACK_SPACE", 0.75f );		// spacing between stacked nouns
static Constant	PLANET_DEFAULT_RV( "PLANET_DEFAULT_RV", (PI * 2) / (60 * 30) );
static Constant	PLANET_DAMAGE_RADIUS( "PLANET_DAMAGE_RADIUS", 20.0f / 5000.0f );
static Constant PLANET_MAX_DAMAGE_RADIUS( "PLANET_MAX_DAMAGE_RADIUS", 500.0f );

// for each population killed, how much does it lower moral
static Constant	POPULATION_KILLED_MORAL( "POPULATION_KILLED_MORAL", 0.025f );
// exponential growth value for population ( 10% currently )
static Constant POPULATION_EXPONENTIAL_GROWTH( "POPULATION_EXPONENTIAL_GROWTH", 1.1f );
// how fast does moral increase per turn
static Constant	PLANET_MORAL_INCREASE( "PLANET_MORAL_INCREASE", 0.01f );
// how much of the population is killed by plague per turn
static Constant	PLAGUE_POPULATION_KILLED( "PLAGUE_POPULATION_KILLED", 0.15f );
// technology += (population * TECHNOLOGY_POPULATION)
static Constant	TECHNOLOGY_POPULATION( "TECHNOLOGY_POPULATION", 0.1f );		
// technology decay rate..
static Constant TECHNOLOGY_DECAY_RATE( "TECHNOLOGY_DECAY_RATE", 0.1f );

// what is the base demand for all planets..
static Constant PLANET_BASE_DEMAND( "PLANET_BASE_DEMAND", 0.1f );
// how do structures affect to the total resource demand for a planet
static Constant PLANET_STRUCTURE_DEMAND( "PLANET_STRUCTURE_DEMAND", 0.05f );
// how do friendly units affect a planets total resource demand
static Constant PLANET_UNIT_DEMAND( "PLANET_UNIT_DEMAND", 0.01f );
// how does population affect the resource demand
static Constant PLANET_POPULATION_DEMAND( "PLANET_POPULATION_DEMAND", 0.05f );
// for each factory, demand is increased by this amount
static Constant PLANET_FACTORY_DEMAND( "PLANET_FACTORY_DEMAND", 0.5f );
// modify the demand levels if the planet has a shipyard
static Constant PLANET_SHIPYARD_DEMAND( "PLANET_SHIPYARD_DEMAND", 20.0f );

//! at what percent of control will a planet become blockaded
static Constant PLANET_BLOCKADE_PERCENT( "PLANET_BLOCKADE_PERCENT", 0.5f );
//! What is the maximum change in control per second
static Constant MAX_CAPTURE_RATE( "MAX_CAPTURE_RATE", 33.33f );		// 5 min for 10,000 capture points
//! how much damage reduction per ship from defenses when capturing
static Constant CAPTURE_DAMAGE_SPLIT("CAPTURE_DAMAGE_SPLIT", 0.05f );
//! maximim amount of damage reduction for a group of ships capping a planet
static Constant MAX_DAMAGE_SPLIT("MAX_DAMAGE_SPLIT", 0.5f );
//! how close to a planet does a ship need to be to capture..
static Constant CAPTURE_RANGE( "CAPTURE_RANGE", 500.0f );
//! how close to a planet does a ship need to be to be damaged by it..
static Constant ATTACK_RANGE( "ATTACK_RANGE", 750.0f );
//! how much control per second does a ship add/remove..
static Constant SHIP_CAPTURE_RATE( "SHIP_CAPTURE_RATE", 6.0f );
//! how much control per second does a unit add/remove..
static Constant UNIT_CAPTURE_RATE( "UNIT_CAPTURE_RATE", 0.5f );
//! how much does damage from bombs change the control points, damage / DAMAGE_CAPTURE_RATE = control adjustment
static Constant DAMAGE_CAPTURE_RATE( "DAMAGE_CAPTURE_RATE", 2000.0f );
//! How many control points when are given when a planet is initially captured..
static Constant CAPTURE_CONTROL_POINTS( "CAPTURE_CONTROL_POINTS", 500.0f );
//! Velocity of defensive fire from planets
static Constant DEFENSE_PROJ_VELOCITY( "DEFENSE_PROJ_VELOCITY", 235.0f );

//! How many ticks a planet should be owned before it levels, multiplied by the current level
static Constant ALLEGIANCE_XP_MOD( "ALLEGIANCE_XP_MOD", TICKS_PER_HOUR );
//! The affect allegiance has on various planet systems
static Constant ALLEGIANCE_EFFECT( "ALLEGIANCE_EFFECT", 0.1f );

//! How much we scale prestige by 0.0001 = 10,000
static Constant AWARD_SCALAR( "AWARD_SCALAR", 0.0001f );

typedef std::map< int, float >		IntFloatMap;


//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounPlanet, NounBody );
REGISTER_FACTORY_KEY( NounPlanet, 4083315223543632941LL );

BEGIN_PROPERTY_LIST( NounPlanet, NounBody );
	CLEAR_PROPERTY_FLAGS( m_DetectionFlags, PF_UPDATE );
	ADD_TRANSMIT_PROPERTY( m_Name );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_nTeamId );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Children );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_fControl );
	ADD_REPLICATED_PROPERTY( m_fPopulation, TICKS_PER_SECOND * 30 );
	ADD_REPLICATED_PROPERTY( m_fTechnology, TICKS_PER_SECOND * 30 );
	ADD_REPLICATED_PROPERTY( m_fMoral, TICKS_PER_SECOND * 30 );
	ADD_REPLICATED_PROPERTY( m_fControl, TICKS_PER_SECOND * 5 );
	ADD_REPLICATED_PROPERTY( m_nAllegiance, TICKS_PER_SECOND * 30 );
	ADD_REPLICATED_PROPERTY( m_nAllegianceXP, TICKS_PER_SECOND * 30 );
	ADD_REPLICATED_PROPERTY( m_nAllegianceXPRequired, TICKS_PER_SECOND * 30 );
	ADD_TRANSMIT_PROPERTY( m_fRotVelocity );
	ADD_REPLICATED_PROPERTY( m_nLockRank, TICKS_PER_SECOND * 5 );
		ADD_PROPERTY_FLAGS( m_nLockRank, PF_GLOBAL );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_TradeRoutes );
	ADD_REPLICATED_PROPERTY( m_fResources, TICKS_PER_SECOND * 30 );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_bImportStatus );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_bExportStatus );
END_PROPERTY_LIST();

//----------------------------------------------------------------------------

NounPlanet::NounPlanet() :
	m_fRotVelocity( PLANET_DEFAULT_RV ),
	m_fPopulation( 0.0f ),
	m_fTechnology( 0.0f ),
	m_nLastDistress( 0 ),
	m_nLastEvent( 0 ),
	m_fMoral( 1.0f ),
	m_nLockRank( 0 ),
	m_fResources( 1000.0f ),
	m_fResourceDemand( 1.0f ),
	m_bImportStatus ( true ),
	m_bExportStatus ( true ),
	m_fControl( 0.0f ),
	m_nAllegiance( 0 ),
	m_nAllegianceXP( 0 ),
	m_nAllegianceXPRequired( ALLEGIANCE_XP_MOD ),
	m_nControlTick( 0 ),
	m_nDefenseTick( 0 ),
	m_bDefenseEnabled( false ),
	m_nLastDefenseTarget( 0 ),
	m_pDefenseProjectile( DEFENSE_PROJECTILE )
{}

//---------------------------------------------------------------------------------------------------

bool NounPlanet::read( const InStream & input )
{
	if (! NounBody::read( input ) )
		return false;

	return true;
}

//---------------------------------------------------------------------------------------------------

void NounPlanet::render( RenderContext &context, 
							const Matrix33 & frame, 
							const Vector3 & position )
{
	NounBody::render( context, frame, position );
}

//----------------------------------------------------------------------------

NounType NounPlanet::nounType() const
{
	return TYPE_LANDMARK;
}

bool NounPlanet::collidable() const
{
	return true;
}

bool NounPlanet::canOrbit() const
{
	return true;		
}

bool NounPlanet::canAttachCargo( Noun * pCargo ) const
{
	if ( maxUnits() <= 0 )
		return false;		// don't allow anything to be unloaded onto infernos or gas planets..
		
	if ( WidgetCast<CargoEnhancement>( pCargo ) || WidgetCast<NounEnhancement>( pCargo ) )
		return false;		// dont allow enhancements to be unloaded to planets.

	// make sure the cargo has a context, or isEnemy / isFriend won't work correctly!
	if (! pCargo->context() )
		pCargo->setContext( context() );

	if ( WidgetCast<NounUnit>( pCargo ) )
	{
		if ( isFriend( pCargo ) )
			return friendlyUnitCount() < maxUnits();
		else
			return enemyUnitCount() < maxUnits();
	}

	return true;
}

float NounPlanet::baseSignature() const
{
	return radius() * 100.0f;
}

//----------------------------------------------------------------------------

void NounPlanet::setup()
{
	NounBody::setup();
	setControl( 0 );
	setResources( 1000.0f );
	setAllegiance ( 0 );

	if ( maxUnits() > 0 )
		setControl( CAPTURE_CONTROL_POINTS );
}

void NounPlanet::initialize()
{
	NounBody::initialize();


	attachTrait<TraitOrbit>()->setRotationalVelocity( m_fRotVelocity );
	
	m_nControlTick = tick() + seed( CONTROL_UPDATE * TICKS_PER_SECOND );
	m_nProductionTick = tick() + seed( PRODUCTION_UPDATE );
	m_nEconomyTick = tick() + seed( ENCONOMY_UPDATE );
	m_nAllegianceTick = tick() + seed( ALLEGIANCE_UPDATE );
	m_nDistressTick = tick() + seed( DISTRESS_MESSAGE_DELAY );
	m_pDefenseProjectile.load( false );

	updateStatusLines();
}

bool NounPlanet::postInitialize()
{
	if (! Noun::postInitialize() )
		return false;

	if ( m_NounContext.valid() )
	{
		Scene * pScene = m_NounContext->scene( false );
		if ( pScene == NULL )
			return false;

		// set radius to maximum extent
		m_fRadius = 32768.0f;
		// find the terrain object in our scene
		if ( findTerrain( pScene->node(), Matrix33( true ), Vector3( true ) ) )
		{
			// look for an existing path map object inside the scene of this planet, if not found then create one dynamically
			m_pMap = PathMap::findMap( m_pTerrain );
			if ( m_pMap )
			{
				// now that we have a valid hex map, snap all units and structures to their hexes... 
				for(int i=0;i<childCount();++i)
				{
					BaseNode * pChild = child( i );
					if ( WidgetCast<NounUnit>( pChild ) )
						((NounUnit *)pChild)->updatePosition();
					else if ( WidgetCast<NounStructure>( pChild ) )
						((NounStructure *)pChild)->updatePosition();
				}
			}
		}
		m_fRadius2 = m_fRadius * m_fRadius;

		// if planet is a light source, turn on the ambient flag..
		if ( FindNodeClass<NodeLight>( pScene->node() ) != NULL )
			m_nNodeFlags |= NF_AMBIENT;
	}

	return true;
}

void NounPlanet::release()
{
	// unregister this planet with the star
	if ( maxUnits() > 0 )
	{
		NounStar * pStar = WidgetCast<NounStar>( orbitRoot() );
		if ( pStar != NULL )
			pStar->removePlanet( this );
	}

	clearCappers();
	m_Attackers.release();
	m_Contacts.release();
	m_TradeRoutes.release();
	m_pTerrain = NULL;
	m_pMap = NULL;

	NounBody::release();
}

void NounPlanet::simulate( dword nTick )
{
	updateDefense( nTick );
	updateControl( nTick );
	updateProduction( nTick );
	updateEconomy( nTick );
	updateAllegiance( nTick );

	NounBody::simulate( nTick );
}

void NounPlanet::collision( Noun * pCollide, const Vector3 & intersect )
{
	// destroy our ship if we hit a planet..
	if ( WidgetCast<NounShip>( pCollide ) && pCollide->isLocal() 
		&& !((NounShip *)pCollide)->isDestroyed() )
	{
		Verb::Ref( new VerbDestroyShip( (NounShip *)pCollide, this, true ) );
	}
}

bool NounPlanet::canDamage( dword type ) const
{
	return (type & DAMAGE_PLANET) != 0;
}

void NounPlanet::inflictDamage( dword nWhen, Noun * pFrom, int damage, dword type, const Vector3 & direction )
{
	if ( isServer() )
	{
		if ( isEnemy( pFrom ) )
		{
			if ( (m_nLastDistress + DISTRESS_MESSAGE_DELAY) < context()->tick() )
			{
				m_nLastDistress = context()->tick();
				Verb::Ref( new VerbDistress( this, VerbDistress::ATTACKED ) );
			}
		}
	}
	
	if ( isEnemy( pFrom ) )
	{
		NounShip * pFromShip = (NounShip *)pFrom;

		// set combat timer on attacking ship
		pFromShip->setOutOfCombat();
	}

	// deflect damage, check for structures with shields
	float fProtection = 0.0f;
	for(int i=0;i<childCount();i++)
	{
		NounStructure * pStructure = WidgetCast<NounStructure>( child(i) );
		if ( pStructure != NULL && pStructure->active() )
		{
			fProtection += pStructure->protection();

			for(int j=0;j<pStructure->childCount();j++)
			{
				GadgetShield * pShield = WidgetCast<GadgetShield>( pStructure->child(j) );
				if ( pShield != NULL )
				{
					pShield->addFlags( NounGadget::FLAG_MOUNT_FULL );
					damage = pShield->deflect( type, damage, FACING_FRONT, direction );
					if ( damage <= 0 )
						return;
				}
			}
		}
	}

	// reduce damage by protection from structures..
	damage -= damage * fProtection;
	if ( damage <= 0 )
		return;
		
	if ( ( flags() & FLAG_NOT_CAPTURABLE ) == 0 )
	{
		// reduce control
		m_fControl -= damage / DAMAGE_CAPTURE_RATE;
		if ( m_fControl < 0.0f )
			m_fControl = 0.0f;
	}

	// kill population
	if ( (type & (DAMAGE_BIO|DAMAGE_PLANET)) != 0 )
	{
		if ( m_fPopulation > 0.0f )
		{
			// decimate the population, the higher the population the faster and easier it's killed
			float fDP = Max( 100000.0f / m_fPopulation, 1000.0f );
			float fKilled = (float)damage / fDP;

			killPopulation( fKilled );
		}
	}

	Vector3 P( direction );
	P.normalize();
	P *= radius();

	//TRACE( CharString().format("NounPlanet::inflictDamage, position = %.1f,%.1f,%.1f", P.x,P.y,P.z) );

	float damageRadius = Clamp<float>( 1.0f + (PLANET_DAMAGE_RADIUS * damage), 1.0f, PLANET_MAX_DAMAGE_RADIUS );
	
	// cause damage to units on the surface
	for(int i=0;i<childCount();i++)
	{
		NounGame * pNoun = WidgetCast<NounGame>( child(i) );
		if (!pNoun || !pNoun->canDamage( type ) )
			continue;		// can't damage this noun with this type of damage

		float distance = (pNoun->position() - P).magnitude();
		if ( distance < damageRadius )
			pNoun->inflictDamage( nWhen, pFrom, (1.0f - (distance / damageRadius)) * damage, type, direction );
	}
}

CharString NounPlanet::status() const
{
	CharString sTargetInfo( NounBody::status() );
	sTargetInfo += CharString().format("%s - %s\n", teamName(), description() );

	if ( m_StatusLines.size() > 0 )
	{
		for(int i=0;i<m_StatusLines.size();++i)
			sTargetInfo += CharString("\n") + m_StatusLines[i];
	}

	return sTargetInfo;
}

int NounPlanet::resourceCount() const
{
	return NounGame::resourceCount() + static_cast<int>( m_fResources );
}

bool NounPlanet::canShipCollide() const
{
	return true;
}

int NounPlanet::useResources( int nWanted )
{
	int nUsed = NounGame::useResources( nWanted );
	nWanted -= nUsed;

	int nTake = Min( (int)m_fResources, nWanted );
	m_fResources -= nTake;
	nUsed += nTake;

	return nUsed;
}

//----------------------------------------------------------------------------

const char * NounPlanet::description() const
{
	return "Planet";
}

int NounPlanet::baseValue() const
{
	return 0;
}

int NounPlanet::baseHabitat() const
{
	return 0;
}

int NounPlanet::baseFood() const
{
	return 0;
}

int NounPlanet::maxUnits() const
{
	return 32;
}

int NounPlanet::maxStructures() const
{
	return 32;
}

int NounPlanet::maxResources() const
{
	return 100000;
}

float NounPlanet::maxControl() const
{
	return 10000.0f + ( 1000.0f * allegiance() );
}

int NounPlanet::maxAllegiance() const
{
	return 10;
}

float NounPlanet::sensor() const
{
	return PLANET_SENSOR_RANGE;
}

float NounPlanet::view() const
{
	return PLANET_VIEW_RANGE;
}

//----------------------------------------------------------------------------

void NounPlanet::setRotVelocity( float rv )
{
	m_fRotVelocity = rv;
}

void NounPlanet::setPopulation( float fPopulation )
{
	m_fPopulation = fPopulation;
}

void NounPlanet::setTechnology( float fTechnology )
{
	m_fTechnology = fTechnology;
}

void NounPlanet::setResources( float fResources )
{
	m_fResources = Clamp<float>( fResources, 0, maxResources() );
}

void NounPlanet::setAllegiance( int a_nAllegiance )
{
	m_nAllegiance = a_nAllegiance;
}

void NounPlanet::setAllegianceXP( int a_nAllegianceXP )
{
	m_nAllegianceXP = a_nAllegianceXP;
}

void NounPlanet::setControl( float a_fControl )
{
	m_fControl = a_fControl;
}

float NounPlanet::addResources( float fProduced )
{
	m_fResources = Clamp<float>( m_fResources + fProduced, 0, maxResources() );
	return m_fResources;
}

void NounPlanet::setMoral( float moral )
{
	m_fMoral = moral;
}

void NounPlanet::setLockRank( int rank )
{
	m_nLockRank = rank;
}

void NounPlanet::addTradeRoute( NounPlanet * pPlanet )
{
	m_TradeRoutes.push( pPlanet );
	updateVersion();
}

void NounPlanet::removeTradeRoute( int n )
{
	m_TradeRoutes.remove( n );
	updateVersion();
}

void NounPlanet::removeTradeRoute( NounPlanet * pPlanet )
{
	m_TradeRoutes.removeSearch( pPlanet );
	updateVersion();
}

void NounPlanet::setImport( bool a_bImport )
{
	if ( isServer() )
	{
		if ( a_bImport )
			factionChat( CharString().format( "<color;ffffff>Comms: %s is now importing goods.", name() ) );
		else
			factionChat( CharString().format( "<color;ffffff>Comms: %s has stopped importing goods.", name() ) );
	}
	m_bImportStatus = a_bImport;
}

void NounPlanet::setExport( bool a_bExport )
{
	if ( isServer() )
	{
		if ( a_bExport )
			factionChat( CharString().format( "<color;ffffff>Comms: %s is now exporting goods.", name() ) );
		else
			factionChat( CharString().format( "<color;ffffff>Comms: %s has stopped exporting goods.", name() ) );
	}
		m_bExportStatus = a_bExport;
}

float NounPlanet::killPopulation( float fKill )
{
	if ( m_fPopulation >= 1.0f )
	{
		fKill = Clamp<float>( fKill, 0, m_fPopulation );
		if ( fKill > 0 )
		{
			// decrease moral based on the population killed
			m_fMoral = Max( 0.0f, m_fMoral - (POPULATION_KILLED_MORAL * fKill) );
			// decrease technology level
			m_fTechnology = m_fPopulation > 0 ? Max( m_fTechnology - ((m_fTechnology / m_fPopulation) * fKill), 0.0f ) : 0;
			// decrease the population
			m_fPopulation -= fKill;
		}

		if ( m_fPopulation <= 0.0f )
		{
			// all population has been killed
			if ( isServer() )
				globalChat( CharString().format( "<color;ffffff>Comms: %s: colony has been destroyed!", name() ) );
			setTeamId( 0 );
			setControl( CAPTURE_CONTROL_POINTS);
		}
	}

	return m_fPopulation;
}

//----------------------------------------------------------------------------

int NounPlanet::value() const
{
	return baseValue() + (population() * 1000) + (structureCount() * 5000);
}

int NounPlanet::workersAvailable() const
{
	return m_fPopulation;
}

int NounPlanet::workersNeeded() const
{
	int workers = 0;
	for(int i=0;i<childCount();i++)
	{
		NounStructure * pStructure = WidgetCast<NounStructure>( child(i) );
		if ( pStructure != NULL )
			workers += pStructure->workers();
	}

	return workers;
}

int NounPlanet::power() const
{
	int power = 0;
	for(int i=0;i<childCount();i++)
	{
		NounStructure * pStructure = WidgetCast<NounStructure>( child(i) );
		if ( pStructure != NULL && pStructure->active() )
			power += pStructure->power();
	}

	return power;
}

int NounPlanet::powerAvailable() const
{
	int power = 0;
	for(int i=0;i<childCount();i++)
	{
		NounStructure * pStructure = WidgetCast<NounStructure>( child(i) );
		if ( pStructure != NULL && pStructure->active() && pStructure->power() > 0 )
			power += pStructure->power();
	}

	return power;
}

int NounPlanet::powerUsed() const
{
	int power = 0;
	for(int i=0;i<childCount();i++)
	{
		NounStructure * pStructure = WidgetCast<NounStructure>( child(i) );
		if ( pStructure != NULL && pStructure->active() && pStructure->power() < 0 )
			power -= pStructure->power();
	}

	return power;
}

int NounPlanet::powerNeeded() const
{
	int power = 0;
	for(int i=0;i<childCount();i++)
	{
		NounStructure * pStructure = WidgetCast<NounStructure>( child(i) );
		if ( pStructure != NULL && pStructure->power() < 0 )
			power -= pStructure->power();
	}

	return power;
}

int NounPlanet::food() const
{
	int food = baseFood();
	for(int i=0;i<childCount();i++)
	{
		NounStructure * pStructure = WidgetCast<NounStructure>( child(i) );
		if ( pStructure != NULL && pStructure->active() )
			food += pStructure->food();
	}

	return food;
}

int NounPlanet::foodNeeded() const
{
	return m_fPopulation;
}

int NounPlanet::habitat() const
{
	int habitat = baseHabitat();
	for(int i=0;i<childCount();i++)
	{
		NounStructure * pStructure = WidgetCast<NounStructure>( child(i) );
		if ( pStructure != NULL && pStructure->active() )
			habitat += pStructure->habitat();
	}

	return habitat;
}

int NounPlanet::mining() const
{
	int mining = 0;
	for(int i=0;i<childCount();i++)
	{
		NounStructure * pStructure = WidgetCast<NounStructure>( child(i) );
		if ( pStructure != NULL && pStructure->active() )
			mining += pStructure->mining();
	}

	if ( flags() & FLAG_MINING_FIND)
		mining = mining << 2;	// mining * 4
	if ( flags() & FLAG_STRIKE )
		mining = mining >> 1;	// mining / 2

	return mining;
}

int NounPlanet::research() const
{
	int research = 0;
	for(int i=0;i<childCount();i++)
	{
		NounStructure * pStructure = WidgetCast<NounStructure>( child(i) );
		if ( pStructure != NULL && pStructure->active() )
			research += pStructure->research();
	}

	return research;
}

int NounPlanet::production() const
{
	int nProduction = 0;
	for(int i=0;i<childCount();i++)
	{
		NounStructure * pStructure = WidgetCast<NounStructure>( child(i) );
		if ( pStructure != NULL && pStructure->active() )
			nProduction += pStructure->production();
	}

	if ( flags() & FLAG_BOOM )
		nProduction *= 2;
	if ( flags() & FLAG_SMUGGLERS )
		nProduction /= 2;

	return nProduction;
}

bool NounPlanet::isPowerShortage() const
{
	return (flags() & FLAG_POWER_SHORT) != 0;
}

bool NounPlanet::isFoodShortage() const
{
	return (flags() & FLAG_FOOD_SHORT) != 0;
}

bool NounPlanet::isWorkerShortage() const
{
	return (flags() & FLAG_WORKER_SHORT) != 0;
}
bool NounPlanet::isTechShortage() const
{
	return (flags() & FLAG_TECH_SHORT) != 0;
}
bool NounPlanet::canGovern( Noun * pWho ) const
{
	if ( WidgetCast<NounShip>( pWho ) )
	{
		NounShip * pShip = (NounShip *)pWho;
		if ( pShip->factionId() == factionId() && pShip->rank() >= m_nLockRank )
			return true;		// yes, same faction and higher rank
	}

	return false;
}


bool NounPlanet::canBuild( Noun * pWho, bool bUpgrade ) const
{
	if (! canGovern( pWho ) )
		return false;
	if ( bUpgrade )
		return (structureCount() - 1) < maxStructures();

	return structureCount() < maxStructures();
}

bool NounPlanet::canDetect( Noun * pNoun ) const
{
	return NounGame::canDetect( pNoun, sensor(), view() );
}

CharString NounPlanet::resourceNames() const
{
	CharString sNames;
	if ( flags() & FLAG_HYDROGEN )
		sNames += CharString().format("%s ", flagName( FLAG_HYDROGEN ) );
	if ( flags() & FLAG_OXYGEN )
		sNames += CharString().format("%s ", flagName( FLAG_OXYGEN ) );
	if ( flags() & FLAG_METALS )
		sNames += CharString().format("%s ", flagName( FLAG_METALS ) );
	if ( flags() & FLAG_HEAVY_METALS )
		sNames += CharString().format("%s ", flagName( FLAG_HEAVY_METALS ) );
	if ( flags() & FLAG_CYRO_METALS )
		sNames += CharString().format("%s ", flagName( FLAG_CYRO_METALS ) );
	if ( flags() & FLAG_HYPER_MATTER )
		sNames += CharString().format("%s ", flagName( FLAG_HYPER_MATTER ) );
	if ( flags() & FLAG_DARK_MATTER )
		sNames += CharString().format("%s ", flagName( FLAG_DARK_MATTER ) );
	if ( flags() & FLAG_URDANIAM )
		sNames += CharString().format("%s ", flagName( FLAG_URDANIAM ) );
	
	return sNames;
}

int NounPlanet::unitCount() const
{
	int unitCount = 0;
	for(int i=0;i<childCount();i++)
	{
		BaseNode * pNode = child(i);
		if ( WidgetCast<NounUnit>( pNode ) )
			unitCount++;
	}

	return unitCount;
}

int NounPlanet::friendlyUnitCount() const
{
	int unitCount = 0;
	for(int i=0;i<childCount();i++)
	{
		BaseNode * pNode = child(i);
		if ( WidgetCast<NounUnit>( pNode ) && isFriend( (Noun *)pNode ) )
			unitCount++;
	}

	return unitCount;
}

int NounPlanet::enemyUnitCount() const
{
	int unitCount = 0;
	for(int i=0;i<childCount();i++)
	{
		BaseNode * pNode = child(i);
		if ( WidgetCast<NounUnit>( pNode ) && isEnemy( (Noun *)pNode ))
			unitCount++;
	}

	return unitCount;
}

int NounPlanet::structureCount() const
{
	int nStructureCount = 0;
	for(int i=0;i<childCount();i++)
		if ( WidgetCast<NounStructure>( child(i) ) )
			nStructureCount++;

	return nStructureCount;
}

bool NounPlanet::isBuildTarget() const
{
	if ( maxStructures() == 0 )
		return false;

	int nStructureCount = 0;
	for(int i=0;i<childCount();i++)
	{
		NounStructure * pStructure = WidgetCast<NounStructure>( child(i) );
		if ( pStructure != NULL )
		{
			if ( pStructure->damage() > 0 || pStructure->isBuilding() )
				return true;		// structure is damaged or incomplete!
			++nStructureCount;
		}
	}

	return nStructureCount < maxStructures();
}

int NounPlanet::factoryCount() const
{
	int nFactoryCount = 0;
	for(int i=0;i<childCount();i++)
		if ( WidgetCast<StructureFactory>( child(i) ) )
			nFactoryCount++;

	return nFactoryCount;
}

int NounPlanet::findHex( const Vector3 & vNear, bool bStructure )
{
	int nBestHex = -1;
	float fBestDistance = 0.0f;

	for(int i=0;i<hexCount();++i)
	{
		float fDistance = (vNear - hex(i).position).magnitude();
		if ( nBestHex >= 0 && fDistance > fBestDistance )
			continue;

		bool bOccupied = false;
		if ( bStructure )
		{
			// make sure no structures are already in this hex...
			for(int j=0;j<childCount() && !bOccupied;++j)
				if ( WidgetCast<NounStructure>( child(j) ) )
					bOccupied = ((NounStructure *)child(j))->hex() == i;
		}
		else
		{
			// count the number of units in the hex...
			int nCount = 0;
			for(int j=0;j<childCount() && nCount < MAX_NOUNS_PER_HEX;++j)
			{
				if ( WidgetCast<NounUnit>( child(j) ) )
				{
					NounUnit * pUnit = (NounUnit *)child(j);
					if ( pUnit->hex() == i )
						nCount++;
				}
			}

			bOccupied = nCount >= MAX_NOUNS_PER_HEX;
		}

		if (! bOccupied )
		{
			nBestHex = i;
			fBestDistance = fDistance;
		}
	}

	return nBestHex;
}

//----------------------------------------------------------------------------

const char * NounPlanet::flagName( dword nFlags )
{
	if ( nFlags & FLAG_HYDROGEN )
		return "HYDROGEN";
	else if ( nFlags & FLAG_OXYGEN )
		return "OXYGEN";
	else if ( nFlags & FLAG_METALS )
		return "METALS";
	else if ( nFlags & FLAG_HEAVY_METALS )
		return "HEAVY METALS";
	else if ( nFlags & FLAG_CYRO_METALS )
		return "CYRO METALS";
	else if ( nFlags & FLAG_HYPER_MATTER )
		return "HYPER MATTER";
	else if ( nFlags & FLAG_DARK_MATTER )
		return "DARK MATTER";
	else if ( nFlags & FLAG_URDANIAM )
		return "URDANIAM";
	else if ( nFlags & FLAG_PLAGUE )
		return "<color;0000ff>PLAGUE</color>";
	else if ( nFlags & FLAG_REVOLUTION )
		return "<color;0000ff>REVOLUTION</color>";
	else if ( nFlags & FLAG_RESESSION )
		return "<color;0000ff>RESESSION</color>";
	else if ( nFlags & FLAG_STRIKE )
		return "<color;0000ff>STRIKE</color>";
	else if ( nFlags & FLAG_BOOM )
		return "<color;2FFFAD>BOOM</color>";
	else if ( nFlags & FLAG_SMUGGLERS )
		return "<color;0000ff>SMUGGLERS</color>";
	else if ( nFlags & FLAG_MINING_FIND )
		return "<color;2FFFAD>MINING FIND</color>";
	else if ( nFlags & FLAG_FOOD_SHORT )
		return "<color;0000ff>FOOD SHORTAGE</color>";
	else if ( nFlags & FLAG_POWER_SHORT )
		return "<color;0000ff>POWER SHORTAGE</color>";
	else if ( nFlags & FLAG_WORKER_SHORT )
		return "<color;0000ff>WORKER SHORTAGE</color>";
	else if ( nFlags & FLAG_TECH_SHORT )
		return "<color;0000ff>TECHNOLOGY SHORTAGE</color>";
	else if ( nFlags & FLAG_HAS_DEPOT )
		return "DEPOT";
	else if ( nFlags & FLAG_HAS_PORT )
		return "PORT";
	else if ( nFlags & FLAG_HAS_SHIPYARD )
		return "SHIPYARD";
	else if ( nFlags & FLAG_HAS_HUB )
		return "HUB";
	else if ( nFlags & FLAG_BLOCKADE )
		return "<color;0000ff>BLOCKADE</color>";
	else if ( nFlags & FLAG_ENEMY )
		return "<color;0000ff>ENEMY DETECTED</color>";
	else if ( nFlags & FLAG_NOT_CAPTURABLE )
		return "<color;0000ff>NOT CAPTURABLE</color>";		

	return "";
}

//----------------------------------------------------------------------------

// this routine searches the nodes for a mesh with the smallest radius, which is always the planets terrain
bool NounPlanet::findTerrain( BaseNode * pBaseNode, const Matrix33 & mFrame, const Vector3 & vPosition )
{
	Matrix33	mNodeFrame = mFrame;
	Vector3		vNodePosition = vPosition;

	NodeTransform * pNode = WidgetCast<NodeTransform>( pBaseNode );
	if ( pNode != NULL )
	{
		vNodePosition = (mFrame % pNode->position()) + vPosition;
		mNodeFrame = mFrame % pNode->frame();

		if ( WidgetCast<NodeComplexMesh>( pBaseNode ) || 
			WidgetCast<NodeComplexMesh2>( pBaseNode ) || 
			WidgetCast<NodeMesh>( pBaseNode ) )
		{
			// create a matrix without rotation so that if the object is rotated, it doesn't artificially expand the radius
			// because of the corners of the box
			Matrix33 mScale( Matrix33( true ) * mNodeFrame.scale() );
			BoxHull hull( pBaseNode->hull(), mScale, vNodePosition );

			float r = Max( Max( hull.max.x - hull.min.x, 
				hull.max.y - hull.min.y), 
				hull.max.z - hull.min.z);
			r = r * 0.5f;									// convert from diameter to radius

			if ( r < m_fRadius )
			{
				m_pTerrain = pBaseNode;
				m_fRadius = r;
			}
		}
	}

	// check the child nodes
	for(int i=0;i<pBaseNode->childCount();i++)
		findTerrain( pBaseNode->child(i), mNodeFrame, vNodePosition );

	// return true if we have a valid terrain node
	return m_pTerrain.valid();
}

void NounPlanet::updateDefense( dword nTick )
{
	if (! isClient() )
		return;		// client-side only faux effect
	if (! m_bDefenseEnabled )
		return;		// no defense bases on this planet
	if ( nTick < m_nDefenseTick )
		return;		// not time to update yet..
	m_nDefenseTick = nTick + (DEFENSE_UPDATE * TICKS_PER_SECOND);

	// if not loaded, load the NounContext for our projectiles..
	if (! m_pDefenseProjectile.isLoaded() )
		return;

	if ( m_nLastDefenseTarget >= m_Attackers.size() )
		m_nLastDefenseTarget = m_Attackers.size() - 1;
	for(int i=0;i<m_Attackers.size();++i)
	{
		int nContact = (m_nLastDefenseTarget + i + 1) % m_Attackers.size();

		NounShip * pTarget = WidgetCast<NounShip>( m_Attackers[nContact] );
		if (! pTarget )
			continue;		// not a ship..
		if (! isEnemy( pTarget ) )
			continue;		// not an enemy, continue..
		if ( (rand() % 100) < 30 )
			continue;		// skip this ship, just to make the fire seem a bit more random

		m_nLastDefenseTarget = nContact;

		Vector3 vProjOrigin( worldPosition() );
		Vector3 vTarget( pTarget->worldPosition() );
		Vector3 vProjDirection( vTarget - vProjOrigin );
		float fProjDistance = vProjDirection.magnitude();
		vProjDirection *= 1.0f / fProjDistance;

		// try to predict the position of the ship into the future..
		vTarget += pTarget->worldVelocity() * (fProjDistance / DEFENSE_PROJ_VELOCITY);
		// recalculate values now
		vProjDirection = vTarget - vProjOrigin;
		fProjDistance = vProjDirection.magnitude();
		vProjDirection *= 1.0f / fProjDistance;

		Array< StructureDefense * > bases;
		for(int k=0;k<childCount();++k)
		{
			StructureDefense * pBase = WidgetCast<StructureDefense>( child(k) );
			if (! pBase || pBase->defense() == 0 )
				continue;
			
			bases.push( pBase );
		}

		// pick a random base to fire at this ship..
		while( bases.size() > 0 )
		{
			int nPicked = rand() % bases.size();
			StructureDefense * pPicked = bases[ nPicked ];
			bases.removeSwap( nPicked );		// remove the picked base from the list..

			Vector3 vDirection( vTarget - pPicked->worldPosition() );
			float fDistance = vDirection.magnitude();
			vDirection *= 1.0f / fDistance;

			if ( (vDirection | vProjDirection) < 0.0f )
				continue;						// base is on the wrong side of the plane to fire at our target
			
			vProjOrigin = pPicked->worldPosition();
			vProjDirection = vDirection;
			fProjDistance = fDistance;
			break;
		}

		NounProjectile * pProjectile = new NounProjectile();
		pProjectile->setTeamId( teamId() );
		pProjectile->setFrame( Matrix33( vProjDirection ) );
		pProjectile->setPosition( vProjOrigin );
		pProjectile->setTick( nTick );
		pProjectile->setNounContext( m_pDefenseProjectile );
		pProjectile->setName( m_pDefenseProjectile->name() );
		pProjectile->setTarget( pTarget );
		pProjectile->setVelocity( vProjDirection * DEFENSE_PROJ_VELOCITY );
		pProjectile->setLife( (fProjDistance / DEFENSE_PROJ_VELOCITY) * TICKS_PER_SECOND );
		pProjectile->setOwner( this );

		zone()->attachNoun( pProjectile );
		break;
	}
}

void NounPlanet::updateControl( dword nTick )
{
	if ( maxUnits() == 0 )
		return;							// gas giants can't be captured..
	if ( nTick < m_nControlTick )
		return;
	m_nControlTick = nTick + (CONTROL_UPDATE * TICKS_PER_SECOND);

	float	fControl = 0.0f;			// change in control
	int		nDefense = 0;				// damage to enemy ships
	bool	bEnemyDetected = false;		// set to true if enemy ships are near

	// clear all previous capture targets before we release our array
	clearCappers();

	m_Contacts.release();
	m_Attackers.release();

	// update the contacts
	Array< GameContext::NounCollision > detect;
	if ( context()->proximityCheck( worldPosition(), PLANET_SCANNER_RANGE, detect ) )
	{
		for(int i=0;i<detect.size();i++)
		{
			Noun * pNoun = detect[i].pNoun;
			if ( canDetect( pNoun ) )
			{
				bool bIsShip = WidgetCast<NounShip>( pNoun ) != NULL;
				if (! bIsShip )
					continue;		// only ships can cap planets

				bool bEnemy = !isFriend( pNoun );
				bool bIsPlatform = bIsShip && WidgetCast<ShipPlatform>( pNoun ) != NULL;
				
				if ( bIsPlatform && ((ShipPlatform *)pNoun)->isBuilding() )
					bIsShip = false;		// don't let incomplete platforms contribute to control of planets.

				NounShip * pShipContact = (NounShip *)pNoun;
				if ( !bEnemyDetected && bEnemy && !bIsPlatform && pShipContact->userId() != 0 )
					bEnemyDetected = true;

				if ( detect[i].fDistance <= (CAPTURE_RANGE + radius()) 
					&& (pShipContact->flags() & NounShip::FLAG_CLOAKED) == 0 )		// cloaked ships can't add/subtract control..
				{
					// set the capture target in the ship.. yes, I know a ship may be capping more than one planet, but we're not
					// going to work about that issue right now.
					pShipContact->setCaptureTarget( this );

					m_Cappers.push( pShipContact );
					
					float fCapPoints = ( SHIP_CAPTURE_RATE * ( pShipContact->controlWeight() * pShipContact->calculateModifier( MT_CAPTURE_WEIGHT ) ) ) * CONTROL_UPDATE;

					if ( bEnemy )
						fControl -= fCapPoints;
					else 
						fControl += fCapPoints;
				}
				
				if ( detect[i].fDistance <= (ATTACK_RANGE + radius()) 
					&& (pShipContact->flags() & NounShip::FLAG_CLOAKED) == 0 )		// cloaked ships shouldn't be attacked if they cannot influence control..		
				{
					m_Attackers.push( pShipContact );	
				}

				m_Contacts.push( pNoun );
			}
		}
	}
	
	Array< NounUnit::Ref > units;
	for(int i=0;i<childCount();++i)
	{
		Noun * pChild = WidgetCast<Noun>( child(i) );
		if (! pChild )
			continue;

		if ( WidgetCast<NounUnit>( pChild ) != NULL )
		{
			units.push( (NounUnit *)pChild );

			if ( isEnemy( pChild ) )
				fControl -= UNIT_CAPTURE_RATE * CONTROL_UPDATE;
			else 
				fControl += ( UNIT_CAPTURE_RATE + ( allegiance() * ( UNIT_CAPTURE_RATE * ALLEGIANCE_EFFECT ) ) ) * CONTROL_UPDATE;
		}
		else if ( WidgetCast<NounStructure>( pChild ) != NULL )
		{
			NounStructure * pStructure = (NounStructure *)pChild;
			if ( pStructure->active() )
			{
				fControl += ((NounStructure *)pChild)->control() * CONTROL_UPDATE;
				nDefense += ((NounStructure *)pChild)->defense() * CONTROL_UPDATE;
			}
		}
	}

	if ( nDefense > 0 && m_Attackers.size() > 0 )
	{
		// enable updateDefense() function for the faux AA fire..
		m_bDefenseEnabled = true;

		// split the damage based on the number of enemy ships
		float fDamageReduction = (m_Attackers.size() - 1) * CAPTURE_DAMAGE_SPLIT;
		if ( fDamageReduction > MAX_DAMAGE_SPLIT )
			fDamageReduction = MAX_DAMAGE_SPLIT;
		nDefense -= nDefense * fDamageReduction;
		
		// add 10% damage per allegiance level
		nDefense += allegiance() * ( nDefense * ALLEGIANCE_EFFECT );

		// inflict damage on enemy ships in attack range..
		for(int i=0;i<m_Attackers.size();++i)
		{
			NounShip * pShip = WidgetCast<NounShip>( m_Attackers[i] );
			if (! pShip )
				continue;
			if ( isFriend( pShip ) )
				continue;		// don't damage friendly ships, duh..

			Vector3 vDirection( worldPosition() - pShip->worldPosition() );
			vDirection.normalize();

			pShip->inflictDamage( tick(), this, nDefense * pShip->planetDefenseModifier(), 
				DAMAGE_KINETIC|DAMAGE_ENERGY, pShip->worldFrame() * vDirection );
		}
	}
	else
	{
		// disable faux AA fire..
		m_bDefenseEnabled = false;
	}

	// limit the change in control, so planets can't be capped in 30 seconds with tons of players..
	fControl = Clamp( fControl, 
		-(MAX_CAPTURE_RATE * CONTROL_UPDATE), 
		(MAX_CAPTURE_RATE * CONTROL_UPDATE) );
		
	// only reduce control if the planet is capturable
	if ( ( flags() & FLAG_NOT_CAPTURABLE ) == 0 )
	{
		// update our control 
		m_fControl += fControl;
		if ( m_fControl > maxControl() )
			m_fControl = maxControl();		// cap the control amount
	}

	if ( isServer() )
	{
		// if control goes below 0.0f, then the planet is captured..
		if ( m_fControl <= 0.0f )
		{
			// set our control points to a reasonable amount, so we don't flip-flop too quickly..
			m_fControl = CAPTURE_CONTROL_POINTS;
			
			// reset allegiance and allegiance XP to zero
			setAllegiance( 0 );
			setAllegianceXP( 0 );

			// who has the most points currently.. they now own the planet.
			IntFloatMap factionScores, teamScores;
			for(int i=0;i<m_Cappers.size();++i)
			{
				NounShip * pShip = WidgetCast<NounShip>( m_Cappers[i] );
				factionScores[ m_Cappers[i]->factionId() ] += SHIP_CAPTURE_RATE * pShip->controlWeight();
				teamScores[ m_Cappers[i]->teamId() ] += SHIP_CAPTURE_RATE * pShip->controlWeight();
			}
			for(int i=0;i<units.size();++i)
			{
				factionScores[ units[i]->factionId() ] += UNIT_CAPTURE_RATE;
				teamScores[ units[i]->teamId() ] += UNIT_CAPTURE_RATE;
			}
			
			// determine the highest faction, they will now control the planet..
			float fHighestFaction = 0.0f;
			int nHighestFaction = -1;
			for( IntFloatMap::iterator iFaction = factionScores.begin(); 
				iFaction != factionScores.end(); ++iFaction )
			{
				if ( iFaction->second > fHighestFaction )
				{
					nHighestFaction = iFaction->first;
					fHighestFaction = iFaction->second;
				}
			}

			// find the highest ranked team, they get the ownership of the planet..
			float fHighestTeam = 0.0f;
			int nHighestTeam = -1;
			for( IntFloatMap::iterator iTeam = teamScores.begin();
				iTeam != teamScores.end(); ++iTeam )
			{
				if ( context()->teamFactionId( iTeam->first ) != nHighestFaction )
					continue;
				
				if ( iTeam->second > fHighestTeam )
				{
					nHighestTeam = iTeam->first;
					fHighestTeam = iTeam->second;
				}
			}

			CharString sPlayers;
			// award capture points to the victors..
			for(int i=0;i<m_Cappers.size();++i)
				if ( m_Cappers[i]->factionId() == nHighestFaction && m_Cappers[i]->userId() != 0 )
				{
					NounShip * pShip = WidgetCast<NounShip>( m_Cappers[i] );
					if ( pShip != NULL && ( value() * AWARD_SCALAR ) >= 1 )
					{
						int nPrestigeAward = ( value() * AWARD_SCALAR ) * pShip->rank();
						gameContext()->gameUser()->onBonusPrestige( m_Cappers[i], nPrestigeAward );
					}
					
					gameContext()->gameUser()->onPlanetsCaptured( m_Cappers[i], 1.0f );
					sPlayers += CharString().format( "@%d ", m_Cappers[i]->userId() );
				}

			setTeamId( nHighestTeam );
			Verb::Ref( new VerbCapturePlanet( this, nHighestTeam ) );
			LOG_STATUS( "NounPlanet", CharString().format( "Planet '%s' has been captured by '%s'.", name(), teamName()) );

			if ( sPlayers.length() > 0 )
				LOG_STATUS( "NounPlanet", "Players involved in capturing %s: %s.", name(), sPlayers.cstr() );
			else
				LOG_STATUS( "NounPlanet", "No players involved in the capturing of %s.", name() );
		}

		// handle the enemy detected flag
		if ( bEnemyDetected && (flags() & FLAG_ENEMY) == 0 )
		{
			Verb::Ref( new VerbPlanetEvent( this, VerbPlanetEvent::ENEMY, true ) );
			// don't spam distress if the flag is toggled quickly
			if ( nTick >= m_nDistressTick )
			{
				m_nDistressTick = nTick + DISTRESS_MESSAGE_DELAY;
				Verb::Ref( new VerbDistress( this, VerbDistress::ENEMY ) );
			}
		}
		else if ( !bEnemyDetected && (flags() & FLAG_ENEMY) != 0 )
		{
			Verb::Ref( new VerbPlanetEvent( this, VerbPlanetEvent::ENEMY, false ) );
		}
	}
}

void NounPlanet::updateProduction( dword nTick )
{
	if ( nTick < m_nProductionTick )
		return;
	m_nProductionTick = nTick + PRODUCTION_UPDATE;

	// push the path finding cache for this planet, to save some memory...
	if ( m_pMap.valid() )
		m_pMap->flushPathCache();

	// check for workers / power
	int technology = (int)floor( m_fTechnology );
	int workers = (int)floor( m_fPopulation );
	int workerSum = 0;
	int power = powerAvailable();
	int powerSum = 0;

	// if this is a habitable planet, register this planet with our root star
	if ( maxUnits() > 0 )
	{
		NounStar * pStar = WidgetCast<NounStar>( orbitRoot() );
		if ( pStar != NULL )
			pStar->addPlanet( this );
	}

	bool bTechnologyShortage = false;
	// clear all structure flags
	dword nStructureFlags = 0;

	// check structures on the planet
	for(int j=0;j<childCount();j++)
	{
		NounStructure * pStructure = WidgetCast<NounStructure>( child(j) );
		if ( pStructure != NULL )
		{
			workerSum += pStructure->workers();
			powerSum += pStructure->power();

			if ( pStructure->flags() & NounStructure::FLAG_ACTIVE )
			{
				// structure is on, make sure it can be on...
				if ( pStructure->workers() > 0 )
				{
					if ( pStructure->workers() <= workers )
						pStructure->clearFlags( NounStructure::FLAG_WORKER_SHORT );
					else
						pStructure->addFlags( NounStructure::FLAG_WORKER_SHORT );
				}
				if ( pStructure->power() < 0 )
				{
					if ( (pStructure->power() + power) >= 0 )
						pStructure->clearFlags( NounStructure::FLAG_POWER_SHORT );
					else
						pStructure->addFlags( NounStructure::FLAG_POWER_SHORT );
				}
				if ( pStructure->technology() > 0 )
				{
					if ( pStructure->technology() > technology )
					{
						bTechnologyShortage = true;
						pStructure->addFlags( NounStructure::FLAG_TECH_SHORT );
					}
					else
						pStructure->clearFlags( NounStructure::FLAG_TECH_SHORT );
				}
			}

			// OR structure flags if the structure is active
			if ( pStructure->active() )
			{
				nStructureFlags |= pStructure->planetFlags();
				workers -= pStructure->workers();
				power += pStructure->power();
			}

		}
	}

	// have structure flags changed, if true then send out a verb to all clients/servers with the new flags
	if ( isServer() && (flags() & FLAG_STRUCTURES) != nStructureFlags )
	{
		clearFlags( FLAG_STRUCTURES );
		addFlags( nStructureFlags );

		// send verb to all clients to update the flags for this planet
		Verb::Ref( new VerbSetFlags( this, flags() ) );
	}

	if ( isServer() )
	{
		bool powerShortage = powerSum < 0;
		bool workerShortage = workerSum > m_fPopulation;

		if ( powerShortage && (flags() & FLAG_POWER_SHORT) == 0 )
			Verb::Ref( new VerbPlanetEvent( this, VerbPlanetEvent::POWER_SHORT, true) );
		else if ( !powerShortage && (flags() & FLAG_POWER_SHORT) != 0 )
			Verb::Ref( new VerbPlanetEvent( this, VerbPlanetEvent::POWER_SHORT, false) );

		if ( workerShortage && (flags() & FLAG_WORKER_SHORT) == 0 )
			Verb::Ref( new VerbPlanetEvent( this, VerbPlanetEvent::WORKER_SHORT, true) );
		else if ( !workerShortage && (flags() & FLAG_WORKER_SHORT) != 0 )
			Verb::Ref( new VerbPlanetEvent( this, VerbPlanetEvent::WORKER_SHORT, false) );

		if ( bTechnologyShortage && (flags() & FLAG_TECH_SHORT) == 0 )
			Verb::Ref( new VerbPlanetEvent( this, VerbPlanetEvent::TECH_SHORT, true ) );
		else if ( !bTechnologyShortage && (flags() & FLAG_TECH_SHORT) != 0 )
			Verb::Ref( new VerbPlanetEvent( this, VerbPlanetEvent::TECH_SHORT, false ) );
	}

	// do mine production
	int nResources;
	nResources = mining() * production();
	nResources += nResources * ( allegiance() * ALLEGIANCE_EFFECT );
	addResources( nResources );
	// update our status lines displayed in our target info..
	updateStatusLines();
}

void NounPlanet::updateAllegiance( dword nTick )
{
	// other factors can affect allegiance levels, so we calculate the XP required here
	m_nAllegianceXPRequired = 1 << ( allegiance() + 2 );
	m_nAllegianceXPRequired *= ALLEGIANCE_XP_MOD;

	if ( nTick < m_nAllegianceTick )
		return;
			
	m_nAllegianceTick = nTick + ALLEGIANCE_UPDATE;
	
	// only increase allegiance if we're at 90% control or more	
	// and if we're not already at max allegiance
	if ( ( ( control() * 1.1 ) > maxControl() ) && maxAllegiance() > allegiance() )
	{
		m_nAllegianceXP += ALLEGIANCE_UPDATE;		
		if ( m_nAllegianceXP >= m_nAllegianceXPRequired )
		{
			// we may have more than the required XP to level the planet, put that back into the pool
			m_nAllegianceXP = m_nAllegianceXP - m_nAllegianceXPRequired;
			m_nAllegiance++;
		}
	}
}

void NounPlanet::updateEconomy( dword nTick )
{
	if ( nTick < m_nEconomyTick )
		return;
	m_nEconomyTick = nTick + ENCONOMY_UPDATE;

	// maximum technology level is based on the amount of available research
	int nMaxTechnology = research();

	// if plague, then kill of 25% of the population
	if ( (flags() & FLAG_PLAGUE) != 0 && m_fPopulation > 0 )
		killPopulation( m_fPopulation * PLAGUE_POPULATION_KILLED );

	int foodProduction = food();
	int foodNeed = foodNeeded();
	int nMaxPopulation = habitat();
	bool bFoodShortage = foodProduction < foodNeed;

	if (!bFoodShortage )
	{
		float expPopGrowth = m_fPopulation * POPULATION_EXPONENTIAL_GROWTH;
		// grow population
		if ( foodProduction > foodNeed && m_fPopulation < nMaxPopulation )
			m_fPopulation = Clamp<float>( expPopGrowth + 1.0f, 0.0f, nMaxPopulation );		// food surplus, grow population...

		// increase moral
		m_fMoral = Min( m_fMoral + PLANET_MORAL_INCREASE, 1.0f );
	}
	else 
	{
		// if not enough food is being produced...
		killPopulation( 1 );
	}


	// clamp population, it can be no larger than the total available habitat
	if ( m_fPopulation > nMaxPopulation )
		killPopulation( m_fPopulation - nMaxPopulation );

	// increase technology level based on the planet population
	if ( m_fTechnology > nMaxTechnology )
	{
		// decay the technology level ...
		m_fTechnology -= m_fTechnology * TECHNOLOGY_DECAY_RATE;
	}
	else
	{
		// increase technology level up to max...
		m_fTechnology += (TECHNOLOGY_POPULATION * m_fPopulation) + ((research() - m_fTechnology) / 10);
		if ( m_fTechnology > nMaxTechnology )
			m_fTechnology = nMaxTechnology;
	}
	
	//check to see if any paused structures can be turned back on
	for(int z = 0; z < childCount(); z++)
	{
		NounStructure * pStructure = WidgetCast<NounStructure>(child(z));

		if( pStructure != NULL && pStructure->isPaused() )
		{
			bool needsPower = pStructure->power() < 0;
			bool needsWorkers = pStructure->workers() > 0;

			//basically, if pStructure takes power or workers and theirs no shortage of what it requires, turn it on.
			if( needsPower && isPowerShortage() )
				continue;
			else if( needsWorkers && isWorkerShortage() )
				continue;
			//if we got this far, turn the structure on
			pStructure->setActive(true);
		}
	}

	//// repair structures
	//for(int j=0;j<childCount();j++)
	//{
	//	NounStructure * pStructure = WidgetCast<NounStructure>( child(j) );
	//	if ( pStructure != NULL && pStructure->active() )
	//		pStructure->repair();
	//}

	// disband excess ground units
	int unitCount = 0;
	for(int j=0;j<childCount();j++)
	{
		NounUnit * pUnit = WidgetCast<NounUnit>( child(j) );
		if ( pUnit != NULL && isFriend( pUnit ) )
		{
			unitCount++;
			if ( unitCount > maxUnits() )
				Verb::Ref( new VerbDisband( pUnit, this ) );
		}
	}

	// update the demand for resources
	if ( maxResources() > 0 )
	{
		m_fResourceDemand = PLANET_BASE_DEMAND;
		m_fResourceDemand += structureCount() * PLANET_STRUCTURE_DEMAND;
		m_fResourceDemand += population() * PLANET_POPULATION_DEMAND;
		//m_fResourceDemand += friendlyUnitCount() * PLANET_UNIT_DEMAND;      
		//m_fResourceDemand += factoryCount() * PLANET_FACTORY_DEMAND; 
		
		if ( testFlags( FLAG_HAS_SHIPYARD ) )
			m_fResourceDemand += PLANET_SHIPYARD_DEMAND;

		// scale the resource demand inversly by the number of resources..
		m_fResourceDemand *= 1.0f - (resources() / maxResources());
	}
	else
		m_fResourceDemand = 0.0f;

	// remove invalid trade routes
	for(int j=tradeRoutes()-1;j>=0;j--)
	{
		NounPlanet * pRoute = tradeRoute( j );
		if (! VerbTradeRoute::canTradeRoute( this, pRoute, NULL ) )
			removeTradeRoute( j );
	}

	// server-side logic...
	if ( isServer() )
	{
		// check the lock rank, lower the level to the highest ranking player in the current faction...
		if ( m_nLockRank > 0 )
		{
			int nHigestRank = gameContext()->gameUser()->getHighestRank( factionId() );
			if ( nHigestRank >= 0 && m_nLockRank > nHigestRank )
				setLockRank( nHigestRank );
		}

		if ( bFoodShortage && (flags() & FLAG_FOOD_SHORT) == 0 )
			Verb::Ref( new VerbPlanetEvent( this, VerbPlanetEvent::FOOD_SHORT, true) );
		else if ( !bFoodShortage && (flags() & FLAG_FOOD_SHORT) != 0 )
			Verb::Ref( new VerbPlanetEvent( this, VerbPlanetEvent::FOOD_SHORT, false) );

		// end blockades when control is high enought
		if ( (flags() & FLAG_BLOCKADE) != 0 && m_fControl >= (maxControl() * PLANET_BLOCKADE_PERCENT) )
		{
			Verb::Ref( new VerbPlanetEvent( this, VerbPlanetEvent::BLOCKADE, false ) );
		}
		else if ( (flags() & FLAG_BLOCKADE) == 0 && m_fControl < (maxControl() * PLANET_BLOCKADE_PERCENT) )
		{
			Verb::Ref( new VerbPlanetEvent( this, VerbPlanetEvent::BLOCKADE, true ) );
			Verb::Ref( new VerbDistress( this, VerbDistress::BLOCKADE ) );
		}

		// send distress signal if enemy units are on the planet
		int enemyUnits = enemyUnitCount();
		if ( enemyUnits > 0 )
			Verb::Ref( new VerbDistress( this, VerbDistress::INVASION ) );

		// check for random events every 5 minutes
		if ( (m_nLastEvent + EVENT_UPDATE) < tick() )
		{
			m_nLastEvent = tick();

			// only let plague run for one round, always stop, other events are more long term
			if ( (flags() & FLAG_PLAGUE) != 0 )
				Verb::Ref( new VerbPlanetEvent( this, VerbPlanetEvent::PLAGUE, false ) );

			int check = rand() % 7;
			switch( check )
			{
			case 0:
				// PLAGUE event
				if ( (rand() % 100) > 98 && m_fPopulation > 50 )
					Verb::Ref( new VerbPlanetEvent( this, VerbPlanetEvent::PLAGUE, true ) );	// plague!
				break;
			case 1:
				if ( ( flags() & FLAG_NOT_CAPTURABLE ) == 0 )
				{
					if ( (flags() & FLAG_REVOLUTION) == 0 )
					{
						int roll = rand() % 100;
						int chance = (m_fMoral * 100) + 40;
						if ( roll > chance )
							Verb::Ref( new VerbPlanetEvent( this, VerbPlanetEvent::REVOLUTION, true ) );
					}
					else
					{
						int roll = rand() % 100;
						int chance = (m_fMoral * 100) + 50;
						if ( ( roll > chance ) && allegiance() <= 0 )
							Verb::Ref( new VerbRevolt( this, -1 ) );
						else if ( ( roll > chance ) && allegiance() >= 1 )
						{
							setAllegiance ( allegiance() - 1 );
							setAllegianceXP ( 0 ); 
						}
						else if ( roll < 25 )
							Verb::Ref( new VerbPlanetEvent( this, VerbPlanetEvent::REVOLUTION, false ) );
					}
				}
				break;
			case 2:
				// RESESSION event
				if ( (flags() & FLAG_RESESSION) == 0 )
				{
					int roll = rand() % 100;
					int chance = (m_fMoral * 100) + 50;
					if ( roll > chance )
						Verb::Ref( new VerbPlanetEvent( this, VerbPlanetEvent::RESESSION, true ) );	// resession
				}
				else
					Verb::Ref( new VerbPlanetEvent( this, VerbPlanetEvent::RESESSION, false ) );	// resession
				break;
			case 3:
				// STRIKE event
				if ( (flags() & FLAG_STRIKE) == 0 )
				{
					int roll = rand() % 100;
					int chance = (m_fMoral * 100) + 50;
					if ( roll > chance )
						Verb::Ref( new VerbPlanetEvent( this, VerbPlanetEvent::STRIKE, true ) );
				}
				else
					Verb::Ref( new VerbPlanetEvent( this, VerbPlanetEvent::STRIKE, false ) );
				break;
			case 4:
				// BOOM event
				if ( (flags() & FLAG_BOOM) == 0 )
				{
					int roll = rand() % 100;
					int chance = (m_fMoral * 100) - 97;
					if ( roll < chance )
						Verb::Ref( new VerbPlanetEvent( this, VerbPlanetEvent::BOOM, true) );
				}
				else
					Verb::Ref( new VerbPlanetEvent( this, VerbPlanetEvent::BOOM, false) );
				break;
			case 5:
				// SMUGGLERS event
				if ( (flags() & FLAG_SMUGGLERS) == 0 )
				{
					int roll = rand() % 100;
					int chance = (m_fMoral * 100) + 50;
					if ( roll > chance )
						Verb::Ref( new VerbPlanetEvent( this, VerbPlanetEvent::SMUGGLERS, true) );
				}
				else
					Verb::Ref( new VerbPlanetEvent( this, VerbPlanetEvent::SMUGGLERS, false) );
				break;
			case 6:
				// MINING_FIND event
				if ( (flags() & FLAG_MINING_FIND) == 0 )
				{
					int roll = rand() % 100;
					if ( roll > 98 )
						Verb::Ref( new VerbPlanetEvent( this, VerbPlanetEvent::MINING_FIND, true) );
				}
				else
					Verb::Ref( new VerbPlanetEvent( this, VerbPlanetEvent::MINING_FIND, false) );
				break;
			}
		}
	}
}

bool NounPlanet::canPauseStructure(NounStructure * pStructure)
{
	bool needsPower = pStructure->power() < 0;
	bool needsWorkers = pStructure->workers() > 0;

	if( ( needsPower && isPowerShortage() ) || ( needsWorkers && ( isWorkerShortage() || isFoodShortage() ) ) )
		return true;
	return false;
}

void NounPlanet::clearCappers()
{
	for(int i=0;i<m_Cappers.size();++i)
	{
		NounShip * pCapper = WidgetCast<NounShip>( m_Cappers[i] );
		if ( pCapper != NULL )
			pCapper->setCaptureTarget( NULL );
	}
	m_Cappers.release();
}

void NounPlanet::updateStatusLines()
{
	if (! isClient() )
		return;

	m_StatusLines.release();

	if ( m_fControl > 0.0f )
		m_StatusLines.push( CharString().format("Control: %.1f%%", (control() / maxControl()) * 100.0f ) );
	if ( m_nAllegiance > 0 )
		m_StatusLines.push( CharString().format("Allegiance: %.1d", allegiance()) );
	int units = unitCount();
	if ( units > 0 )
		m_StatusLines.push( CharString().format("Units: %d", units ) );

	int structures = 0;
	int shield = 0;
	float fProtection = 0.0f;
	int defense = 0;
	int depots = 0;
	int ports = 0;

	for(int i=0;i<childCount();i++)
	{
		BaseNode * pChild = child(i);
		if ( WidgetCast<NounStructure>( pChild ) )
		{
			NounStructure * pStructure = (NounStructure *)pChild;

			structures++;

			if ( pStructure->active() )
				fProtection += pStructure->protection();
			if ( WidgetCast<StructureDefense>( pChild ) )
			{
				defense++;

				if ( pStructure->active() )
				{
					int shielding = ((StructureDefense *)pStructure)->shielding();
					if ( shielding >= 0 )
					{
						shield += shielding;
						if ( shield > 0 )
							shield = (shield + shielding) >> 1;		// average them together
						else
							shield = shielding;
					}
				}
			}
			else if ( WidgetCast<StructureDepot>( pStructure ) )
				depots++;
			else if ( WidgetCast<StructurePort>( pStructure ) )
				ports++;
		}
	}

	if ( structures > 0 )
	{
		CharString sStructures;
		sStructures += CharString().format("Structures: %d", structures );
		if ( defense > 0 )
			sStructures += CharString().format(", Def: %d", defense);
		if ( depots > 0 )
			sStructures += CharString().format(", Dep: %d", depots );
		if ( ports > 0 )
			sStructures += CharString().format(", Port: %d", ports );

		m_StatusLines.push( sStructures );
	}

	CharString sProtection;
	if ( shield > 0 )
		sProtection += CharString().format("Shield: %d%%", shield);
	if ( fProtection > 0.0f )
		sProtection += CharString().format("%sPD: %d%%", sProtection[0] != 0 ? ", " : "", (int)(fProtection * 100) );
	if ( sProtection[0] != 0 )
		m_StatusLines.push( sProtection );

	if ( resources() > 0 )
		m_StatusLines.push( CharString().format("Resources: %s\n", FormatNumber<char,int>( resources() ).cstr() ) );
}

//----------------------------------------------------------------------------
// EOF
