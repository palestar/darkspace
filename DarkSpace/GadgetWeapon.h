/*
	GadgetWeapon.h

	Base class for any type of weapon that shoots a projectile
	(c)2000 Palestar, Richard Lyle
*/

#ifndef GADGET_WEAPON_H
#define GADGET_WEAPON_H

#include "NounGadget.h"
#include "NounPlanet.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class NounProjectile;		// forward declare

//----------------------------------------------------------------------------

class DLL GadgetWeapon : public NounGadget
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef WidgetReference< GadgetWeapon >	wRef;

	// Construction
	GadgetWeapon();

	// BaseNode interface
	void						onDetaching();
	// Noun interface
	void						setup();
	void						initialize();
	void						release();
	void						inflictDamage( dword nWhen,
									Noun * pFrom, int damage, dword type, 
									const Vector3 & direction );

	// NounGadget interface
	Type						type() const;					// get gadget type
	EnergyClass					energyClass() const;
	dword						hotkey() const;					// gadget hotkey
	CharString					status() const;

	int							addValue() const;
	float						addSignature() const;
	virtual CharString			useTip( Noun * pTarget,
									bool shift ) const;
	bool						needReload( Noun * pWhom ) const;

	bool						usable( Noun * pTarget, 
									bool shift ) const;			// can gadget be used on target
	int							usableWhen() const;

	bool						reload( Noun * pWhom );
	void 						use( dword when, Noun * pTarget,
									bool shift);				// use gadget
	int							useEnergy( dword nTick,
									int energy );
	bool						updateLogic();

	virtual void				onModifiersChanged();

	// Accessors
	virtual int					energyCost() const = 0;			// how much energy is needed to fire the weapon
	virtual int					energyCharge() const = 0;		// how much energy can this weapon take
	virtual int					ammoCost() const = 0;			// does the weapon require ammo, 0 if no ammo needed
	virtual int					ammoMax() const = 0;			// maximum ammount of ammo
	virtual int					ammoReload() const = 0;			// how fast does the ammo recharge when at a base
	virtual int					ammoResources() const = 0;		// how many resources does each ammo cost
	virtual bool				needsTarget() const = 0;		// does this weapon need a target? (mines don't need a target to be used)
	virtual bool				needsLock() const = 0;			// does this weapon need a lock?
	virtual int					lockTime() const = 0;			// if it needs a lock, how long does it take to aquire the lock
	virtual dword				lockType() const = 0;			// what does the weapon lock onto
	virtual bool				checkRange() const = 0;			// check the max range
	virtual bool				inheritVelocity() const = 0;	// should the projectile inherit the parent velocity
	virtual bool				turret() const = 0;				// if true, then projectile is fired at target, otherwise direction is locked
	virtual int					maxProjectiles() const = 0;		// maximum number of active projectils at once
	virtual int					projectileCount() const = 0;	// how many projectile objects are created
	virtual int					projectileDelay() const = 0;	// delay between multiple projectiles
	virtual bool				projectileSmart() const = 0;	// is projectile smart, default = false
	virtual bool				isMine() const = 0;				// is projectile a mine, if true then it only explodes when target is enemy
	virtual float				projectileVelocity() const = 0;	// how fast does the projectile travel
	virtual float				projectileLife() const = 0;		// how long does the projectile last, in ticks
	virtual float				projectileSig() const = 0;		// what signature does the projectile generate
	virtual bool				projectileCollidable() const = 0;	// are our projectiles collidable (i.e. bombs, missiles, fighters)
	virtual bool				projectileAutoTarget() const = 0;	// will projectile automatically find a target, useful for smart projectiles only..
	virtual float				interceptMaxRadius() const;		// when calculating an intercept point, this controls how much we can fudge the accuracy
	virtual float				turnRate() const = 0;			// turning rate of smart projectiles
	virtual int					tracerRate() const = 0;			// rate in ticks at which the tracer segments are created
	virtual int					tracerLife() const = 0;			// life span in ticks of each tracer segment
	virtual dword				damageType() const = 0;			// type of damage caused
	virtual int					damage() const = 0;				// how much damage does this weapon cause
	virtual int					damageRandom() const = 0;		// how much random range is added
	virtual float				damageFalloff() const = 0;		// how much does the damage falloff with range
	virtual bool				reverseFalloff() const = 0;		// is the falloff reversed (does it increase with distance)
	virtual int					repairRate() const = 0;			// how much gadget damage is repaired
	virtual int					areaDamage() const = 0;			// how much area damage is caused when projectile explodes
	virtual int					areaDamageRandom() const = 0;	// how much area damage is caused when projectile explodes
	virtual float				areaDamageRadius() const = 0;	// how large is the area of damage
	virtual float				armTime() const = 0;			// how much time before the projectile is armed in seconds

	int							energy() const;
	int							ammo() const;					// available ammo
	Array< Noun::wRef >			projectiles() const;

	// Mutators			
	void						setEnergy( int energy );
	void						setAmmo( int ammo );

	// NounProjectile interface
	virtual void				onProjectileActive( Noun * pProj );
	virtual void				onProjectileDead( Noun * pProj );
	virtual bool				hit( Noun * pTarget,
									NounProjectile * pProj,
									const Vector3 & intersect );// projectile hit
	virtual void				explode( NounProjectile * pProj );

	// Helpers			
	float						maxRange() const;				// calculate the range of this weapon
	float						minRange() const;				// calculated min range of this weapon
	void						intercept( Noun * pTarget,
									Vector3 & intercept ) const;
	void						intercept( const Vector3 & target,		// target world position
									const Vector3 & velocity,	// target velocity in world space
									Vector3 & intercept ) const;	// the intercept point

	virtual NounProjectile *	projectileFactory();				// overridable

	virtual NounProjectile *	createProjectile( dword when,		// overridable
									Noun * pTarget );
	NounProjectile *			createProjectile( dword when, 
									const Vector3 & velocity, 
									Noun * pTarget );

protected:
	// Data
	int							m_Energy;						// how much energy does this weapon have
	int							m_Ammo;							// how much ammo

	// non-serialized
	int							m_nMaxAmmo;
	dword						m_nEnergyTick;
	float						m_Signature;
	Noun::wRef					m_Target;
	int							m_RepeatCount;
	float						m_fRepeatDelay;
	float						m_fChargeRate;

	Array< Noun::wRef >			m_Projectiles;					// active projectiles
};

//----------------------------------------------------------------------------

inline float GadgetWeapon::interceptMaxRadius() const
{
	return 10.0f;
}

inline Array< Noun::wRef > GadgetWeapon::projectiles() const
{
	return m_Projectiles;
}

inline int GadgetWeapon::energy() const
{
	return m_Energy;
}

inline int GadgetWeapon::ammo() const
{
	return m_Ammo;
}

//----------------------------------------------------------------------------

inline void GadgetWeapon::setEnergy( int energy )
{
	m_Energy = Clamp( energy, 0, energyCost() );
}

inline void GadgetWeapon::setAmmo( int ammo )
{
	m_Ammo = Clamp( ammo, 0, m_nMaxAmmo );
}

//----------------------------------------------------------------------------

inline float GadgetWeapon::maxRange() const
{
	return projectileVelocity() * (projectileLife() * calculateModifier( MT_WEAPON_RANGE ));
}

inline float GadgetWeapon::minRange() const
{
	return armTime() * projectileVelocity();
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
