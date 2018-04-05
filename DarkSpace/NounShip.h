/*
	NounShip.h

	This noun is a star-ship
	(c)1999 Palestar Development, Richard Lyle
*/

#ifndef NOUN_SHIP_H
#define NOUN_SHIP_H

#include "Render3D/Scene.h"
#include "NounPlanet.h"
#include "NounGadget.h"
#include "NounProjectile.h"
#include "GadgetJumpDrive.h"
#include "GadgetJumpGate.h"
#include "GadgetCloak.h"
#include "NounBody.h"
#include "EnhancementModifier.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class NounJumpGate;
class NounPlanet;
class NounPod;
class GadgetShield;
class GadgetArmor;
class CargoEnhancement;
class NounEnhancement;
class NounProjectile;

//----------------------------------------------------------------------------

class DLL NounShip : public NounBody
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference< NounShip >					Ref;
	typedef WeakReference< NounShip >				WeakRef;
	typedef WidgetReference< NounShip >				wRef;
	typedef Array< NounGadget::wRef >				RepairQueue;
	typedef NounGadget::EnergyClass					EnergyClass;

	enum Type
	{
		UNKNOWN,		// 0
		FIGHTER,		// 1
		ENGINEER,		// 2
		SUPPLY,			// 3
		TRANSPORT,		// 4
		SCOUT,			// 5
		FRIGATE,		// 6
		DESTROYER,		// 7
		CRUISER,		// 8
		DREAD,			// 9
		STATION,		// 10
		PLATFORM,		// 11
		PLATFORM_SUPPLY,	// 12

		TYPE_COUNT
	};
	enum Order				// high level
	{
		NOORDER,

		ATTACK,				// attack target
		DEFEND,				// defend target
		CAPTURE,			// capture target
		MOVE,				// move to target
		RELOAD,				// reload/repair target
		BEACON,				// attach beacon to target
		HOLD,				// stay at current position
		TRADE,				// transfer resources
		RECON,				// scout target
		BUILD,				// engineer target
		FALLBACK,			// fallback to target until repaired

		ORDER_COUNT
	};
	enum Command			// low level AI
	{
		NOCOMMAND,			// default free flight mode
		
		ORBIT,				// enter orbit around target
		FOLLOW,				// follow target
		SHADOW,				// match velocity and heading of target
		EVADE,				// move away from target
		ENGAGE,				// engage enemy
		STOP,				// stop
		REFUEL,				// refuel jump drive
		SELL,				// sell all cargo
		BUY,				// buy cargo

		COMMAND_COUNT
	};

	enum SceneBits		// the scene attached to this ship should support these bits
	{
		DESTROYED			= 0x00000001,			// destroyed version
		THRUST				= 0x00000002,			// display thrusters
		THRUST_JUMP			= 0x00000004,			// jump thrust

		DAMAGE_1			= 0x00000020,			// visual ship damage
		DAMAGE_2			= 0x00000040,
		DAMAGE_3			= 0x00000080,
		DAMAGE_4			= 0x00000100,

		BUILD_1				= 0x00000200,
		BUILD_2				= 0x00000400,
		BUILD_3				= 0x00000800,
		BUILD_4				= 0x00001000,
	};
	enum When
	{
		NOW,
		SOONER,
		LATER,
		LAST,
	};
	enum Flags
	{
		FLAG_IN_SAFE_ZONE		= 0x00000002,			// is the ship currently in a safe zone
		FLAG_JUMP_DISABLED		= 0x00000010,			// jump drive disabled
		FLAG_CONTROL_DISABLED	= 0x00000020,			// ship controls are ignored
		FLAG_DYNAMICS_DISABLED	= 0x00000040,			// updateDynamics() is not called by simulate( dword nTick )
		FLAG_COMMAND_DISABLED	= 0x00000080,			// updateCommand() is not called by updateShip()
		FLAG_CLOAKED			= 0x00000100,			// is this ship currently cloaked?
		FLAG_JUMPING			= 0x00000200,			// jump drives engaged
		FLAG_AI_ALLY			= 0x00000400,			// ship is an spawned AI ally ship
		FLAG_NO_BEST_ORDER		= 0x00000800,			// findBestOrder is disabled.
		FLAG_NO_EXPIRE_ORDER	= 0x00001000,			// order never expires due to time limit
		FLAG_DISTRESS			= 0x00002000,			// set when the ships sends a distress signal
		FLAG_SUPPLY_DISTRESS	= 0x00004000,			// set when the ships sends a supply distress signal

		FLAG_SUBSCRIBER			= 0x10000000,			// ship belongs to a subscriber
		FLAG_ADMIN				= 0x20000000,			// ship belongs to a admin

		FLAG_ALL				= FLAG_IN_SAFE_ZONE
									|FLAG_JUMP_DISABLED
									|FLAG_CONTROL_DISABLED
									|FLAG_DYNAMICS_DISABLED
									|FLAG_COMMAND_DISABLED
									|FLAG_CLOAKED
									|FLAG_JUMPING
									|FLAG_DISTRESS
									|FLAG_SUPPLY_DISTRESS,
	};

	//  Data
	static bool		sm_bShipSelection;				// if true, then we are selecting a ship and any effects shouldn't be rendered

	// Construction
	NounShip();

	// Widget interface
	virtual bool	read( const InStream & );

	// BaseNode interface
	virtual void	onAttach( BaseNode * pChild );
	virtual void	onChildDetached();
	virtual void	onAttached();
	virtual void	onDetaching();

	// Node interface
	virtual void	preRender( RenderContext &renderContext, 
						const Matrix33 & frame, 
						const Vector3 & position );
	virtual void	render( RenderContext &context, 
						const Matrix33 & frame, 
						const Vector3 & position );

	// Noun interface
	virtual CharString	displayName( bool a_bAllowTags ) const;
	virtual NounType	nounType() const;
	virtual bool	canCollide() const;
	virtual bool	collidable() const;
	virtual bool	enableECM() const;
	virtual bool	canAttachCargo( Noun * pCargo ) const;

	virtual void	initialize();	
	virtual void	release();
	virtual void	simulate( dword nTick );
	virtual void	update();

	virtual void	setup();

	virtual void	collision( Noun * pCollide, 
						 const Vector3 & intersect );

	virtual bool	canDamage( dword type ) const;				// can this object be damaged
	virtual void	inflictDamage( dword nWhen, Noun * pFrom, int damage, 
						dword type, const Vector3 & direction );	

	// NounGame interface
	virtual CharString status() const;
	virtual float	viewDetectScale() const;

	// Accessors
	virtual Type	type() const = 0;							// type of ship
	virtual int		buildTechnology() const = 0;				// technology level required to build this ship
	virtual int		buildCost() const = 0;						// cost to build this ship in resources
	virtual dword	buildFlags() const = 0;						// required planet flags to build this ship
	virtual int		baseValue() const = 0;						// base value of this ship
	virtual int		baseEnergy() const = 0;						// base energy this ship can carry
	virtual	int		baseDamage() const = 0;						// hull points of this ship
	virtual float	baseVelocity() const = 0;					// the top velocity of this hull
	virtual int		baseCargo() const = 0;						// base cargo 
	virtual float	baseSensor() const = 0;						// base sensor rating
	virtual float	baseView() const = 0;						// how far out can this ship see 1 object with a radius of 1.0
	virtual float	baseThrust() const = 0;						// base thrust 
	virtual float	baseYaw() const = 0;						// base turn rate
	virtual int		maxRepair() const = 0;						// maximum amount of damage that can be repaired per tick
	virtual int		explodeDamage() const = 0;					// damaged caused when this ship explodes
	virtual float	explodeArea() const  = 0;					// damage radius from ship explosion

	virtual float	velocityModifier() const = 0;				// max velocity modifier for devices which addVelocity() to this ship
	virtual float	cloakEnergyModifier() const = 0;			// energy use modifier for cloaking
	virtual float	cloakTime() const = 0;						// how long does it take for this ship to cloak in seconds
	virtual float	uncloakTime() const = 0;					// how long does it take for this ship to uncloak in seconds
	virtual float	maxCloakTime() const = 0;					// how long this ship can remain cloaked for
	virtual float	shieldEnergyModifier() const = 0;			// energy use modifier for shields
	virtual float	armorModifier() const = 0;					// armor modifier
	virtual float	shieldModifier() const = 0;					// shield damage modifier
	virtual int		gadgetLevel() const = 0;					// level for any gadgets attached to this ship
	virtual float	planetDefenseModifier() const = 0;			// modifier for damage inflicted by planetary defenses

	virtual bool	isMonster() const;							// is this ship a space monster
	virtual bool	enableGateSpawn() const;					// can this ship spawn for free from a jumpgate
	virtual bool	enableRollDynamics() const;					// should this ship roll when turning
	virtual dword	despawnTime() const;						// how many ticks to despawn this ship at a SY..
	virtual float	controlWeight() const;						// how much the control weight of this ship is multiplied by when capturing a planet

	int				value() const;								// value of this ship
	float			sensor() const;								// get the ship sensor ability
	float			view() const;								// get the ships view ability
	float			visibility() const;							// how visible is this ship
	int				maxDamage() const;							// amount of damage this hull can take
	int				maxEnergy() const;							// get the maximum energy
	float			maxVelocity() const;						// get the maximum velocity
	int				maxCargo() const;							// get the maximum cargo
	float			maxThrust() const;							// calculate available thrust
	float			maxAcceleration() const;					// calculate maximum acceleration
	float			maxYaw() const;								
	float			velocityRange() const;						// velocity range between max and base

	Scene *			destroyEffect() const;
	int				damage() const;								// amount of damage sustained
	int				energy() const;

	float			velocity() const;							// current velocity
	float			setVelocity() const;						// current set velocity
	float			heading() const;							// current heading
	float			setHeading() const;							// current set heading
	float			yaw() const;								// current yaw

	int				credits() const;							// amount of credits this ship has
	int				rank() const;								// our rank
	bool			isOutOfCombat() const;
	float			OutOfCombatTimer() const;
	int				repairCount() const;						// current number of gadgets in the repair queue
	NounGadget *	repair( int n ) const;						// get gadget from repair queue

	Order			order() const;								// current order
	Noun *			orderTarget() const;						// current order target
	NounShip *		orderFrom() const;							// current order sender
	dword			orderTime() const;							

	Command			command() const;							// current command
	Noun *			commandTarget() const;						// current command target

	int				contactCount() const;
	Noun *			contact( int n ) const;
	Noun *			captureTarget() const;						// what planet, if any, are we currently capturing

	int				maxEnhancements() const;					// get the maximum enhancements
	int				enhancementCount() const;					// how many enhancements do we have
	NounEnhancement *
					enhancement(int n) const;					// get this enhancement

	int				allyCount() const;							// number of active AI ally units
	NounShip *		ally( int n ) const;						// get ally n

	Noun *			home() const;
	
	bool			despawning() const;							// returns true if ship is despanwing
	dword			despawnTicks() const;						// returns the number of ticks until the ship is despawned

	Noun *			lastFireTarget() const;						// last attack target that was checked
	float			lastFireDistance() const;					// what was the distance to the target
	bool			isWeaponsFree() const;						// do we have a clear shot

	// get the priority level for an energy class, lowest number is highest priority
	int				energyClassPriority( EnergyClass nClass ) const;

	Noun *	randomGadget(dword when,int deviant);				//Selects a random gadget using seed when + deviant

	// Mutators
	void			updateCharacter();							// update ship characteristics, such as max damage, etc..
	void			initializeGadgets();						// initialize internal lists of systems, so they will be rebuilt
	void			initializeEnhancements();					// initialize internal list of enhacements
	void			initializeUnits();							// make sure all units onboard are friendly

	void			setDamage( int damage );
	void			setOutOfCombat( dword timer = 0 );
	void			setEnergy( int energy );
	void			setVelocity( float fVelocity );
	void			setHeading( float fHeading );
	void			setCredits( int a_nCredits );
	void			setRank( int rank );

	bool			needRepair( Noun * pWhom );					// returns true if ship needs repairing
	bool			repair( Noun * pFrom, 
						int nRepairRate,
						bool bGadgetRepair );						// repair hull damage only...
	virtual bool	needReload( Noun * pWhom );					// returns true if ship needs reloading..
	virtual bool	reload( Noun * pFrom );						// repair and supply this ship, returns false if not needed..

	void			repairGadget( NounGadget * pRepair, 
						When when  );							// called by VerbRepairGadget() only
	void			destroyShip( Noun * pKiller, 
							bool bZeroVelocity );				// called by VerbDestroyShip()

	void			addAlly( NounShip * pAlly );				// add a ship as an ally, when this ship is deatched
																// any ally ships will be detached as well
	int				validateAllyShips();						// remove any invalid ships from the ally array, 
																// returns the number of active ally ships
	void			setHome( Noun * pHome );
	void			setCaptureTarget( Noun * pTarget );			// called by NounPlanet to set this ships capture target
	void			despawn( dword nTicks = 0 );				// despawn this ship in the specified number of ticks
	void			cancelDespawn();							// cancel the despawn of this ship
	
	void			addIncoming( NounProjectile * a_pProj );
	void			removeIncoming( NounProjectile * a_pProj );

	// NounShipControl.cpp
	bool			controlHeadingVelocity( float fHeading,		// client interface
						float fVelocity );	
	bool			controlYawVelocity( float fYaw,
						float fVelocity );
	bool			controlVelocity( float fVelocity );	
	bool			controlHeading( float fHeading );
	bool			controlYaw( float fYaw );

	bool			canAlpha( Noun * pTarget );					// returns true if are in range to alpha
	bool			verbAlpha( Noun * pTarget );				// alpha strike target
	bool			canBomb( Noun * pTarget );
	bool			verbBomb( Noun * pTarget );					// bomb target
	void			useGadget( NounGadget * pGadget, 
						Noun * pTarget, bool shift );			// main interface for using a ship gadget, pushes VerbUseGadget()

	bool			togglePointDefense();
	int				toggleShields();

	// NounShipLogic.cpp
	static bool		isOrderValid( Order nOrder, Noun * pTarget,
						NounShip * pFrom );							// can we send this order to another ship

	float			shouldOrder( Order nOrder, Noun * pTarget,
						NounShip * pFrom );							// return a score for if this ship can perform this order, higher the score the better
	bool			canOrder( NounShip * pFrom );					// check if the given ship can order this ship
	bool			canOrder( Order nOrder, Noun * pTarget,
						NounShip * pFrom, bool bNewOrder = true );	// can we call verbOrder()
	void			verbOrder( Order nOrder, Noun * pTarget );		// attach VerbOrderShip() to this ship
	void			verbOrder( Order nOrder, Noun * pTarget, 
						NounShip * pFrom );							// attach VerbOrderShip() to this ship
	void			setOrder( Order nOrder, const Noun::tRef & pTarget, 
						const NounShip::wRef & pFrom );				// directly set ship order
	
	bool			canSendOrder( Order nOrder, Noun * pTarget );
	NounShip *		sendOrder( Order nOrder, Noun * pTarget );		// send an order, returns the ship the order was sent to if any, NULL if none
	
	Order			bestOrder( Noun * pTarget );					// get the default order for the target

	bool			canCommand( Command nCommand, Noun * pTarget );	// can we do this command
	void			verbCommand( Command nCommand, Noun * pTarget );	// attach VerbCommandShip()
	bool			isCommand( Command nCommand, Noun * pTarget );	// test the current command
	bool			setCommand( Command nCommand, const Noun::tRef & pTarget );
	NounPlanet *	isTargetPlanet( Noun * pTarget );

	// NounShipCargo.cpp
	int				unitCount() const;								// how many combat units do we have.
	int				friendlyUnitCount() const;						// how many friendly units on this ship
	int				enemyUnitCount() const;							// how many enemy units on this ship
	int				cargoCount() const;								// how many units & cargo do we have
	int				availableCargo() const;							// how much more cargo can we carry
	int				podCount() const;								// number of pods being loaded

	bool			canLoad( Noun * pLoad );						// can we load this object
	bool			canUnload( Noun * pTarget, Noun * pUnload );	// can we unload object onto planet

	bool			verbLoad( Noun * pLoad );						// load object onto this ship
	bool			verbUnload( Noun * pTarget, Noun * pUnload );	// unload object
	bool			verbJettison( Noun * pJettison );				// jettison cargo
	
	void			onPodAttached( NounPod * pPod );
	void			onPodDetached( NounPod * pPod );

	void			refuelEnergy(int a_percent);				// refuel energy by percent

	// NounShipEnhancement.cpp

	//! This returns the modifier for the given type, 1.0f is returned by default.
	//! a_eType = This should be the type of modifier type.
	//! if a_bInvert is false, then a 10% modifier would return 1.1f
	//! if a_bInvert is true, then a 10% modifier would return 0.9f
	float			calculateModifier( ModifierType a_eType, 
						bool a_bInvert = false ) const;
	void			addModifier( ModifierType a_eType, int a_nValue );
	void			subtractModifier( ModifierType a_eType, int a_nValue );
	int				getModifier( ModifierType a_eType );
	void			resetModifiers();

	//! This is invoked when a modifier is added or removed from this ship...
	virtual void	onModifiersChanged();

	void			dropLoot();									// drops go right to cargo if thier is room
	NounShip *		addLoot( CargoEnhancement *a_pCargo );		// Try to place the CargoEnhancement on a ship that damaged this ship...																			
	bool			addLoot( const CharString & a_sEnhName );	// used for creating a specific drop

	static const char *		
					modifierTypeText( ModifierType a_eType );
	static const int	
					modifierTypeClamp( ModifierType a_eType );
	// Helpers
	void			message( const char * pMessage, 
						bool a_bAllowRemote = false );				
	void			protection( Facing face, 					// get the armor/shield status for a facing, 
						int & max, int & damage );				// max = maximum damage, damage = amount remaining
	void			protection( Facing face, 					 
						int & shieldmax, int & shielddamage,    //same idea, just split up
						int & armormax, int & armordamage);				

	float			damageRatio() const;						// ratio = damage / maxDamage
	float			damageRatioInv() const;						// ratio = 1.0f - damageRatio();
	float			energyRatio() const;						// ratio = energy / maxEnergy
	bool			isDestroyed() const;						// damageRatio() >= 1.0f


	GadgetJumpDrive * jumpDrive();								// get the jump drive
	GadgetJumpGate * jumpGate();								// get the jump gate/wormhole
	GadgetCloak * gadgetCloak();								// get the cloaking device

	bool			canDetect( Noun * pNoun ) const;			// can this ship detect this object
	bool			canOrbit( Noun * pTarget );					// can we orbit this object

	Facing			getFacing( float direction, 
						bool bTowards = true );					// get facing from direction, direction is in world space
	Facing			getFacing( const Vector3 & vWorld );
	Vector3			intercept( Noun * pIntercept,
						float fVelocity );						// calculate intecept point for a target

	CharString		commandString();						// get the current command string
	CharString		orderString();							// get the current order string


	static const char *
					commandText( Command command );
	static const char *	
					orderText( Order order );
	static Color	orderColor( Order order );
	static const char *
					typeText( Type type );
	
	static int		buildTechnology( NounShip * pShip );		// get the meximum required technology level to build this ship
	static int		buildCost( NounShip * pShip );				// calculate the build costs including the costs of the gagdets
	static dword	buildFlags( NounShip * pShip );				// get the required build flags for this ship

	void			setBuildTarget(NounStructure * structure);  // Set the build structure to target when we receive it from the server
	NounStructure * getBuildTarget();							// get the build structure to target

protected:
	// Types
	struct CollisionInfo {
		Noun *		m_pCollide;
		Vector3		m_vIntersect;
		float		m_fDistance;

		CollisionInfo() : m_pCollide( NULL ), m_fDistance( 0.0f )
		{}
	};

	struct Threat {
		Threat() : m_nDamage( 0 ), m_fStrength( 1.0f ), m_fOptimialRange( 0.0f ), m_fThreat( 0.0f )
		{}

		Noun::WeakRef
					m_pThreat;				// enemy object pointer (ship, planet, platform, etc)
		int			m_nDamage;				// amount of damage received from this threat
		float		m_fStrength;			// score which indicates the strength of against this ship, > 1.0 stronger, < 1.0 weaker
		float		m_fOptimialRange;		// what is the optimal weapon range for this threat
		float		m_fThreat;				// = (m_nDamage / N) + m_fStrength
	};
	typedef std::list< Threat >					ThreatList;

	enum MoveResult
	{
		MOVE_ERROR			= -2,
		MOVE_OUT_OF_RANGE	= -1,
		MOVE_MOVING			= 0,
		MOVE_DONE			= 1,
	};
	typedef std::list< NounShip::WeakRef >		ShipCollisionList;
	typedef std::vector< NounShip::wRef >		AllyVector;

	// Data
	int				m_Damage;				// how much damage has this ship taken
	int				m_Energy;
	float			m_fVelocity;			// current velocity
	float			m_fSetVelocity;
	float			m_fHeading;				// current heading
	float			m_fSetHeading;
	float			m_fYaw;	

	int				m_nCredits;				// our credits
	int				m_Rank;					// our rank
	bool			m_bOutOfCombat;			// our combat status
	RepairQueue		m_RepairQueue;			// our repair queue

	Order			m_Order;				// current order
	Noun::tRef		m_OrderTarget;			// current order target
	wRef			m_pOrderFrom;			// who sent us this order
	dword			m_nOrderTime;
	Command			m_Command;				// current command
	Noun::tRef		m_CommandTarget;
	Noun::wRef		m_pHome;				// home object
	dword			m_nAllyTime;			// how long in ticks has this ship been an ally

	// non-serialized
	bool			m_bShipEnabled;	
	bool			m_bDynamicsEnabled;
	bool			m_bDespawning;
	dword			m_nDespawnTicks;

	dword			m_nLogicTick;
	dword			m_nDynamicsTick;
	dword			m_nRepairTick;
	dword			m_nUpdateTick;
	dword			m_nIgnoreCollisionTick;	// all collsions are ignored until this tick
	dword			m_nCombatTick;

	float			m_fPlane;				// what is our Y plane.. updated by updateShipPlane()
	float			m_Sensor;				// sensor rating for this ship
	float			m_View;					// view rating
	float			m_fFade;				// fade value of this ship, 0.0 = invisible, 1.0 = visible
	float			m_Visibility;			// visibility of this ship
	int				m_Alpha;				// alpha of this ship, caluculated from the visibility
	int				m_MaxDamage;			// maxmimum damage ship can sustain
	int				m_MaxEnergy;			// maxmumum energy ship can have
	float			m_MaxVelocity;			// maximum velocity
	int				m_MaxCargo;				// number of items ship can carry
	float			m_MaxThrust;			// maximum thrust
	float			m_MaxAcceleration;		// maximum acceleration
	float			m_MaxYaw;				// maximum yaw
	int				m_nRepairedDamage;		// amount of damage repaired this tick

	float			m_fRoll;				// for effect only, doesn't affect ship position or heading
	float			m_fPitch;	
	AllyVector		m_Allies;				// ally ships


	float			m_fLastSetHeading;		// last heading sent by controlHeading()
	float			m_fLastSetVelocity;		// last velocity sent by controlVelocity()
	float			m_fLastYaw;				// last yaw sent by controlYaw()

	ShipCollisionList
					m_ShipCollisions;		// ships we are currently colliding with..

	Array< Noun::WeakRef >
					m_Contacts;
	Noun::WeakRef	m_pCaptureTarget;		// what planet are we currently trying to capture
	Array< Noun::WeakRef >
					m_Pods;					// incoming pods
	bool			m_bIncoming;
	Array< NounProjectile::WeakRef >
					m_Incoming;				// incoming projectiles, used by avoidCollisions()..

	Array< NounGadget * >
					m_Gadgets;				// gadgets attached to this ship, in order of energy priority
	Array< GadgetShield * >
					m_Shields;
	Array< GadgetArmor * >
					m_Armor;
	Array< NounEnhancement * >
					m_Enhancements;
	Array< NounGadget * >
					m_UseGadgets;			// use gadget queue
	Noun::wRef		m_UseTarget;			// target of the use
	bool			m_UseShift;				
	int				m_UseGadgetTick;		// delay when next VerbUseGadget should be sent

	dword			m_DamageBits;			// current damage bits

	GadgetJumpDrive::wRef
					m_JumpDrive;			// our jump drive

	NounStructure * m_BuildTarget;			// A structure being built we want to target once we receive it from the server

	// AI state variables
	int				m_nOrderState;			// internal variable used by orders to control execution of an order
	float			m_fCunning;				// value between 0.0f and 1.0f that control how hard this AI is to fight
	dword			m_nLastDistress;		// last tick when a distress signal was sent
	dword			m_nLastJumpCheck;		// last tick when a AI jump was checked
	Noun::wRef		m_pLastJumpTarget;		// target we checked against
	Noun::wRef		m_pLastJumpCollision;	// object we are trying to avoid
	bool			m_bLoadUnits;			// true when ships needs to load units for capture
	bool			m_bAttacked;			// true if we've been attacked
	Noun::wRef		m_pLastAttacker;		// pointer to our attacker
	dword			m_nLastEvadeVector;		// last time evade vector was randomized
	Vector3			m_vEvadeVector;			// current evade vector..

	dword			m_nLastFireCheck;		// last time we checked our firing vector to our target
	Noun::wRef		m_pLastFireTarget;		// what target did we check
	float			m_fLastFireDistance;	// distance to our attack target
	bool			m_bWeaponsFree;			// true if we are free to fire our weapons

	ThreatList		m_ThreatList;			// list of threats to this ship

	// enhancements
	int				m_MaxEnhancements;				// number of items ship can carry

	// this is where the modifier totals are figured out
	int				m_Modifiers[MT_COUNT];

	// Mutators
	void			updateShip( dword nTick );
	void			updateCombat( dword nTick );
	void			updateDespawn();
	void			updateFireCheck();		// if we are in engage mode, this checks that we can fire at our target without hitting a friendly 
											// or if our weapon fire would be blocked by an obstruction
	void			updateDamageBits();
	void			updateRepairQueue();
	void			updateContacts();
	void			updateGadgets( dword nTick );

	void			useGadgets();			// use gadgets

	// NounShipControl.cpp

	//! Move to target, keep distance between self and target, returns true when inside "within" radius
	//! pTarget = Target to move towards
	//! outside = Distance to stay away from target
	//! within = Distance to get within of the target
	//! speed = Value between 0.0 and 1.0 to control how fast we are willing to move
	//! bSlowToTarget = If true, then the ship will slow once it reaches it's target.
	//! bForceJump = If true, then the jump drives will be used no matter what the distance
	//! nDepth = moveToTarget is called recursive, this is used to limit how deep we will recurse.
	MoveResult		moveToTarget( Noun * pTarget, 
						float outside, 
						float within, 
						float speed, 
						bool bSlowToTarget, 
						bool bForceJump, 
						int nDepth = 0 );				
	bool			updateDynamics( dword nTick  );	// update the ship dynamics
	void			updateShipPlane();

	// NounShipLogic.cpp
	virtual bool	updateLogic( dword nTick );
	virtual bool	updateAttackOrder();
	virtual bool	updateDefendOrder();
	virtual bool	updateCaptureOrder();
	virtual bool	updateReconOrder();
	virtual bool	updateMoveOrder();
	virtual bool	updateReloadOrder();
	virtual bool	updateBeaconOrder();
	virtual bool	updateHoldOrder();
	virtual bool	updateTradeOrder();
	virtual bool	updateBuildOrder();
	virtual bool	updateFallbackOrder();
	virtual bool	findBestOrder();
	virtual bool	returnHome();
	virtual bool	updateCommand();
	virtual void	updateOrbitCommand();
	virtual void	updateShadowCOmmand();
	virtual void	updateFollowCommand();
	virtual void	updateEvadeCommand();
	virtual void	updateEngageCommand();
	virtual void	updateStopCommand();
	virtual void	updateRefuelCommand();
	virtual void	updatePointDefense();

	bool			avoidCollision();							// returns true if this ship is taking evasive action to avoid a collision
	bool			checkForCollision( Noun * a_pTarget,		// check for a collision, returns true if a collision did occur between
						CollisionInfo * a_pInfo = NULL,			// this ships current position and the target position.
						bool a_bIgnoreShips = true );	
	void			calculateAvoidVector( Noun * a_pTarget,
						Vector3 & a_vAvoid );					// this calculates the new target vector for this ship to avoid a collision with the object

	float			scoreTarget( Noun * pTarget );				// measure a possible target, return 0.5 if equal, 1.0 if target is more powerful

	bool			canAttack( Noun * pTarget );
	bool			isMaxFuel();								// do we have the maximum jump fuel
	bool			needFuel();									// returns true if the ship needs to refuel jump drives
	bool			needAmmo();									// returns true if the ship needs more ammo for combat
	bool			sellCargo();								// sell cargo to planet currently orbiting, returns true when done
	bool			buyCargo();									// buy cargo from planet oribiting, returns true when done.

	NounShip *		findEnemyShip( float bravery, float within ) const;		// look for enemy ship to attack, bravery 0.0 (coward) - 1.0 (conan)
	bool			acquireReloadTarget();
	bool			acquirePlatformReloadTarget();				// platform look for repairs
	bool			acquireReconTarget();						// look for a planet/ship to beacon..
	bool			acquireCombatTarget();						// look for a friendly ship / planet to defend
	bool			acquireBuildTarget();						// look for a build target
	bool			acquireCaptureTarget();						// find a target for capture
	bool			acquireTradeTarget();						// look for a planet to trade for..
	bool			acquireRestTarget();						// find a friendly planet to rest, repair, and refuel this ship

	Noun *			findDepot() const;							// find a place to rest and repair
	NounPlanet *	findPlanet( bool bFriendly ) const;			// look for the closest planet...
	NounPlanet *	findUnits() const;							// looks for the closest planet with enough ground units
	NounJumpGate *	findHomeJumpGate() const;					// look for a jump gate that returns the ship home
	NounJumpGate *	findJumpGate( Noun * pDestination );		// find a nearby jump gate that will get us closer to our destination
	NounPlanet *	findSellTarget();							// find a planet to sell resources too from the given planet..
	NounPlanet *	findBuyTarget();							// find a planet to buy resources from for the given planet..

	virtual void	onAttacked( Noun * pFrom, int nDamage, dword damageType );
	bool			addThreat( Noun * pFrom, int nDamage );
	float			calculateStrength( Noun * a_pThreat );		// this returns a float value representing the strength of this object compared to this ship
																// > 1.0 is stronger, < 1.0 is weaker
	float			calculateWeaponRange(Noun * a_pThreat );	// this returns the optimal weapon range for this ship against the given target
	void			updateThreats();
};

IMPLEMENT_RAW_STREAMING( NounShip::Order );
IMPLEMENT_RAW_STREAMING( NounShip::Command );
IMPLEMENT_RAW_STREAMING( NounShip::When );

//---------------------------------------------------------------------------------------------------

class ShipModifier : public EnhancementModifier
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	//! Construction
	ShipModifier( ModifierType a_eType = MT_UNKOWN, int a_nValue = 0);

	//! Modifier interface
	virtual CharString	getDescription( NounShip * a_pShip );
	virtual bool		canAttach( NounShip * a_pShip );
	virtual bool		onApply( NounShip * a_pShip );
	virtual void		onRemove( NounShip * a_pShip );

	//! Accessors
	ModifierType		type() const;
	int					value() const;
protected:
	//! Data
	ModifierType		m_eType;
	int 				m_nValue;
};

//---------------------------------------------------------------------------------------------------

inline ModifierType ShipModifier::type() const
{
	return m_eType;
}

inline int ShipModifier::value() const
{
	return m_nValue;
}

//----------------------------------------------------------------------------

inline float NounShip::sensor() const
{
	return m_Sensor;
}

inline float NounShip::view() const
{
	return m_View;
}

inline float NounShip::visibility() const
{
	return m_Visibility;
}

inline int NounShip::maxDamage() const
{
	return m_MaxDamage;
}

inline int NounShip::maxEnergy() const
{
	return m_MaxEnergy;
}

inline float NounShip::maxVelocity() const
{
	return m_MaxVelocity;
}

inline int NounShip::maxCargo() const
{
	return m_MaxCargo;
}

inline float NounShip::maxThrust() const
{
	return m_MaxThrust;
}

inline float NounShip::maxAcceleration() const
{
	return m_MaxAcceleration;
}

inline float NounShip::maxYaw() const
{
	return m_MaxYaw;
}

inline float NounShip::velocityRange() const
{
	return (maxVelocity() - baseVelocity());
}

inline Scene * NounShip::destroyEffect() const
{
	return WidgetCast<Scene>( 
		nounContext()->resource( "DESTROY" ) ); 
}

inline int NounShip::damage() const
{
	return( m_Damage );
}

inline int NounShip::energy() const
{
	return( m_Energy );
}

inline float NounShip::velocity() const
{
	return( m_fVelocity );
}

inline float NounShip::setVelocity() const
{
	return m_fSetVelocity;
}

inline float NounShip::heading() const
{
	return( m_fHeading );
}

inline float NounShip::setHeading() const
{
	return m_fSetHeading;
}

inline float NounShip::yaw() const
{
	return m_fYaw;
}

inline int NounShip::rank() const
{
	return( m_Rank );
}

inline bool NounShip::isOutOfCombat() const
{
	return( m_bOutOfCombat );
}

inline float NounShip::OutOfCombatTimer() const
{
	return ( m_nCombatTick * TICK_DURATION_S );
}

inline int NounShip::repairCount() const
{
	return( m_RepairQueue.size() );
}

inline NounGadget *	NounShip::repair( int n ) const
{
	return( m_RepairQueue[ n ] );
}

inline NounShip::Order NounShip::order() const
{
	return( m_Order );
}

inline Noun * NounShip::orderTarget() const
{
	return( m_OrderTarget );
}

inline NounShip * NounShip::orderFrom() const
{
	return m_pOrderFrom;
}

inline dword NounShip::orderTime() const
{
	return m_nOrderTime;
}

inline NounShip::Command NounShip::command() const
{
	return( m_Command );
}

inline Noun * NounShip::commandTarget() const
{
	return( m_CommandTarget );
}

inline int NounShip::contactCount() const
{
	return m_Contacts.size();
}

inline Noun * NounShip::contact( int n ) const
{
	return m_Contacts[ n ];
}

inline Noun * NounShip::captureTarget() const
{
	return m_pCaptureTarget;
}

inline 	int NounShip::maxEnhancements() const		// get the maximum enhancements
{
	return m_MaxEnhancements;
}

inline int NounShip::enhancementCount() const
{
	return m_Enhancements.size();
}

// get this enhancement
inline NounEnhancement * NounShip::enhancement(int n) const							
{
	return m_Enhancements[ n ];
}

inline int NounShip::allyCount() const
{
	return static_cast<int>( m_Allies.size() );
}

inline NounShip * NounShip::ally( int n ) const
{
	if ( n < 0 || n >= (int)m_Allies.size() )
		return NULL;
	return m_Allies[ n ];
}

//----------------------------------------------------------------------------

inline int NounShip::credits() const
{
	return m_nCredits;
}

inline Noun * NounShip::home() const
{
	return m_pHome;
}

inline bool NounShip::despawning() const
{
	return m_bDespawning;
}

inline dword NounShip::despawnTicks() const
{
	return m_nDespawnTicks;
}

inline Noun * NounShip::lastFireTarget() const
{
	return m_pLastFireTarget;
}

inline float NounShip::lastFireDistance() const
{
	return m_fLastFireDistance;
}

inline bool NounShip::isWeaponsFree() const
{
	return m_bWeaponsFree;
}

//----------------------------------------------------------------------------

inline void	NounShip::setDamage( int damage )
{
	m_Damage = Clamp( damage, 0, maxDamage() );
}

inline void NounShip::setEnergy( int energy )
{
	m_Energy = Clamp( energy, 0, maxEnergy() );
}

inline void NounShip::setVelocity( float velocity )
{
	m_fLastSetVelocity = m_fSetVelocity = m_fVelocity = Clamp( velocity, 0.0f, maxVelocity() );
}

inline void NounShip::setHeading( float heading )
{
	m_fLastSetHeading = m_fSetHeading = m_fHeading = heading;
	m_fLastYaw = m_fYaw = 0.0f;
}

inline void NounShip::setCredits( int a_nCredits )
{
	m_nCredits = a_nCredits;
}

inline void NounShip::setRank( int rank )
{
	m_Rank = rank;
}

// refuel energy by percent
inline void NounShip::refuelEnergy(int a_percent)
{
	float fAddAmount = maxEnergy() * (float(a_percent * 0.01));
	setEnergy(energy() + (int)fAddAmount);
}

//----------------------------------------------------------------------------

inline float NounShip::damageRatio() const
{
	return( float( damage() ) / maxDamage() );
}

inline float NounShip::damageRatioInv() const
{
	return( 1.0f - damageRatio() );
}

inline float NounShip::energyRatio() const
{
	return( float( energy() ) / maxEnergy() );
}

inline bool NounShip::isDestroyed() const
{
	return( damageRatio() >= 1.0f );
}

//----------------------------------------------------------------------------

#endif

//----------------------------------------------------------------------------
// EOF
