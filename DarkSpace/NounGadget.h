/*
	NounGadget.h

	This object is the base class for any object that can be mounted onto another noun
	(c)1999 Palestar Development, Richard Lyle
*/

#ifndef NOUN_GADGET_H
#define NOUN_GADGET_H

#include "Render3D/Scene.h"
#include "Constants.h"
#include "NounGame.h"
#include "NounPlanet.h"
#include "GameDll.h"

class NounBody;		// foward declare
class SceneryEffect;

//----------------------------------------------------------------------------

class DLL NounGadget : public NounGame
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference< NounGadget >			Ref;
	typedef WidgetReference< NounGadget >	wRef;

	enum Flags {
		FLAG_ACTIVE				= 0x00000001,		// shared flag for gadget being active..
		FLAG_MOUNT_FORE			= 0x00010000,		// forward mount
		FLAG_MOUNT_RIGHT		= 0x00020000,		// right mounted
		FLAG_MOUNT_LEFT			= 0x00040000,		// left mounted 
		FLAG_MOUNT_AFT			= 0x00080000,		// aft mounted
		FLAG_MOUNT_FULL			= FLAG_MOUNT_FORE|FLAG_MOUNT_RIGHT|FLAG_MOUNT_LEFT|FLAG_MOUNT_AFT,
	};

	enum Type {
		// NounGadget classifications. The main purpose is so the AI can determine how and when to use a device without
		// having to special case the code.

		UNKNOWN,				// unknown device
		DRIVE,					// maneuvers ship and provides energy
		DRIVE_JUMP,				// allows intersteller travel
		WEAPON,					// ship to ship weapons
		WEAPON_CLOSE,			// close range weapon, use only when close
		WEAPON_MEDIUM,			// medium ranged weapon, use only when over 100gu away
		WEAPON_RANGED,			// ranged weapon, use only when over 400gu away
		WEAPON_FIGHTER,			// ranged fighter bay, use only when over 400gu away
		WEAPON_MINE,			// mine type weapon
		WEAPON_BOMB,			// planetary weapon
		ARMOR,					// armor
		SHIELD,					// shields
		SPECIAL,				// special devices
		SPECIAL_DEFENSIVE,		// defensive special devices (cloak, ECM)
		SPECIAL_OFFENSIVE,		// offensive special (ECCM)
		JUMP_GATE,				// jump gate
		INTERDICTOR,			// interdictor device
		BEACON,					// beacon
		DRONE_REPAIR,			// repair drones
		DRONE_BUILD,			// build drones
		MINING_BEAM,			// mining beam
		TRACTOR_BEAM,
		REACTOR,
		AURA,

		TYPE_LAST,				// leave last
		FIRST_WEAPON			= WEAPON,
		LAST_WEAPON				= WEAPON_BOMB,
	};

	enum EnergyClass {
		// determines the order useEnergy() is called on the gadget
		//
		ENERGY_CLASS_DRIVES = 0,			// drives and generators
		ENERGY_CLASS_JUMP,					// jump-drives
		ENERGY_CLASS_WEAPONS,
		ENERGY_CLASS_DEFENSE,
		ENERGY_CLASS_SPECIAL
	};

	// depreciated, all mounts are specified by the noun flags now
	enum Mount {
		FRONT,
		RIGHT,
		LEFT,
		BACK,
		FULL,
	};

	// Construction
	NounGadget();

	// BaseNode interface
	void					render( RenderContext &context, 
								const Matrix33 & frame, 
								const Vector3 & position );
	// Noun interface
	virtual NounType		nounType() const;	

	virtual bool			canBeDetected() const;
	virtual float			baseSignature() const;

	virtual void			setup();
	virtual bool			postInitialize();
	virtual void			inflictDamage( dword nWhen, Noun * pFrom, 
								int damage, dword type, 
								const Vector3 & direction );

	// Accessors
	bool					cooledDown() const;				// if device is not on cooldown
	int						damage() const;					// amount of damage gadget has received
	int						level() const;					// gadget level
	dword					group() const;					// gadget group

	virtual dword			hotkey() const;					// default hotkey assignment, 0 if none
	virtual Type			type() const;					// get gadget type
	virtual EnergyClass		energyClass() const;			// get gadget energy class
	virtual CharString		status() const;					// status of the gadget (displayed on the icon)
	virtual CharString		description() const;			// get description of this gadget

	virtual CharString		useTip( Noun * pTarget, 
								bool shift ) const;			// get the tool tip
	virtual bool			usable( Noun * pTarget,		
								bool shift ) const;			// can gadget be used on target, called before we issue VerbUseGadget
	virtual int				usableWhen() const;				// when can the gadget be used again, value from 0 to 100 which is used to display progress bars
	virtual int				useModeCount() const;			// how many different modes are available
	virtual int				useMode() const;				// what is the current user mode
	virtual bool			useActive() const;				// is the gadget current on or off
	virtual bool			useTarget() const;				// should this gadget be targetted when used?

	virtual bool			canDamage( dword type ) const;	// can this device be damaged
	virtual int				maxDamage() const;				// max damage this gadget can take
	virtual int				durability() const;				// how likely can this device survive a ship explosion

	virtual int				buildTechnology() const;		// required technology level for this device
	virtual int				buildCost() const;				// cost in resources to build
	virtual dword			buildFlags() const;				// planetary flags required to build this device
	virtual int				buildTime() const;				// time in seconds to build

	virtual int				cooldown() const;				// how many ticks for the device to cooldown

	// used by NounShip
	virtual float			addMass() const;				// mass of this gadget
	virtual int				addValue() const;				// value of this gadget
	virtual float			addMaxVelocity() const;			// afterburners...
	virtual float			addThrust() const;				// fusion drive...
	virtual int				addMaxDamage() const;			// armor...
	virtual int				addMaxEnergy() const;			// energy storage...
	virtual float			addSignature() const;			// how much energy is this gadget emitting for detection
	virtual float			addSensor() const;				// increase the ability to detect objects
	virtual float			addVisibility() const;			// how visible is the ship
	virtual int				addCargo() const;				// additional storage for armies and materials

	virtual bool			needReload( Noun * pWhom ) const;				// does this gadget need reloading
	virtual bool			needRepair( Noun * pWhom ) const;				// does this gadget need repairing

	// Mutators
	void					setDamage( int damage );
	void					setLevel( int nLevel );
	void					setGroup( dword group );

	virtual bool			repair( Noun * pWhom, 
								int nDamage );				// repair the gadget
	virtual bool			reload( Noun * pWhom );			// reload gadget

	virtual void			useMode( int mode );			// set use mode
	virtual void			use( dword when, Noun * pTarget, 
								bool shift );				// use gadget, returns created object if any
	virtual int				useEnergy( dword nTick, 
								int energy );
	virtual bool			updateLogic();						// gadget specific AI...

	// Helpers
	void					message( const char * pMessage, bool a_bAllowRemote = false );
	Material *				icon();							// get gadget icon

	Scene *					useEffect() const;
	Scene *					projEffect() const;
	Material *				tracerMaterial() const;
	Scene *					hitEffect() const;

	bool					getUpgrades( int a_nFactionID, Array< NounGadget * > & upgrades ) const;

	bool					checkFacing( const Vector3 & position ) const;
	float					damageRatio() const;			// m_nDamage / m_Points
	float					damageRatioInv() const;			// 1.0f - damageRatio()
	bool					destroyed() const;

	int						useDelay() const;
	SceneryEffect *			createUseEffect( bool attachToGadget = true );

	NounBody *				parentBody() const;				// get the parent body of this gadget
	dword					mountFlags() const;				// returns the mount flags for this gadget, ignores all other flags

	bool					isCloaked() const;				// returns true if our parent is cloaked
	bool					isJumping() const;				// returns true if our parent is jumping
	bool					isSafeZone() const;				// returns true if our parent is inside a safe zone

	//! Returns the enhancement modifier for the given system and modifier..
	//! a_eType = This should be the enhancement type.
	//! if a_bInvert is false, then a 10% modifier would return 1.1f
	//! if a_bInvert is true, then a 10% modifier would return 0.9f
	float					calculateModifier( ModifierType a_eType, 
								bool a_bInvert = false ) const;
	//! This is inokved when any modifiers are changed on our ship..
	virtual void			onModifiersChanged();

	// Static
	static bool				sm_bRenderGadgets;				// render the gadgets

	static dword			typeGroup( Type type );			// get the default group for the type of gadget
	static CharString		mountText( dword nFlags );
	static dword			facingMountFlags( Facing face );	// translate a facing enum to mount flags

protected:
	// Data
	int						m_nDamage;						// amount of damage
	dword					m_nGroup;	

	// non-serialized
	int						m_nLevel;						// gadget level
	Material::Ref			m_pIcon;						// cached icon material
	dword					m_nCooldownTick;				// when will this device be cooled
};

IMPLEMENT_RAW_STREAMING( NounGadget::Mount );
IMPLEMENT_RAW_STREAMING( NounGadget::Type );

//----------------------------------------------------------------------------
inline bool NounGadget::cooledDown() const
{
	return tick() >= m_nCooldownTick ? true : false;
}

inline int NounGadget::damage() const
{
	return m_nDamage;
}

inline int NounGadget::level() const
{
	return m_nLevel;
}

inline dword NounGadget::group() const
{
	return m_nGroup;
}

//----------------------------------------------------------------------------

inline void NounGadget::setDamage( int damage )
{
	m_nDamage = damage;
}

inline void NounGadget::setLevel( int nLevel )
{
	m_nLevel = nLevel;
}

inline void NounGadget::setGroup( dword group )
{
	m_nGroup = group;
}

//----------------------------------------------------------------------------

inline Scene * NounGadget::useEffect() const
{
	return WidgetCast<Scene>( 
		nounContext()->resource( "USE", false ) );
}

inline Scene * NounGadget::projEffect() const
{
	return WidgetCast<Scene>( 
		nounContext()->resource( "PROJECTILE", false ) );
}

inline Material * NounGadget::tracerMaterial() const
{
	return WidgetCast<Material>( 
		nounContext()->resource( "TRACER", false ) );
}

inline Scene * NounGadget::hitEffect() const
{
	return WidgetCast<Scene>( 
		nounContext()->resource( "HIT", false ) );
}

inline float NounGadget::damageRatio() const
{
	return( float( m_nDamage ) / maxDamage() );
}

inline float NounGadget::damageRatioInv() const
{
	return( 1.0f - damageRatio() );
}

inline bool NounGadget::destroyed() const
{
	return( damageRatio() >= 1.0f );
}

//----------------------------------------------------------------------------

inline dword NounGadget::mountFlags() const
{
	return flags() & FLAG_MOUNT_FULL;
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
