/*
	NounProjectile.h

	This object is used to contain a projectile object
	(c)1999 Palestar, Richard Lyle
*/

#ifndef NOUN_PROJECTILE_H
#define NOUN_PROJECTILE_H

#include "NounGame.h"
#include "NounBody.h"
#include "GadgetWeapon.h"
#include "Trail.h"
#include "Standard/Queue.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL NounProjectile : public NounGame
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef WidgetReference< NounProjectile >	wRef;
	typedef WeakReference< NounProjectile >		WeakRef;

	enum Flags
	{
		FLAG_COLLIDABLE			= 0x00000001,			// add this projectile to the collision hash so it can be collided into..
		FLAG_DO_TRAIL			= 0x00000002,
	};

	// Construction
	NounProjectile();

	// Widget interface
	bool					read( const InStream & );
	// BaseNode interface
	void					onAttached();
	void					onDetaching();
	void					render( RenderContext &context, 
								const Matrix33 & frame, 
								const Vector3 & position );
	// Noun interface
	NounType				nounType() const;
	bool					canCollide() const;
	bool					collidable() const;
	bool					canBeDetected() const;
	bool					enableECM() const;
	bool					enablePD() const;
	float					baseSignature() const;

	void					initialize();
	void					release();
	void					simulate( dword nTick );

	void					collision( Noun * pCollide, 
								const Vector3 & intersect );

	void					onWorldTransform( const Vector3 & transform );

	// Accessors
	int						life() const;
	GadgetWeapon *			weapon() const;
	Noun *					owner() const;
	Noun *					target() const;

	Noun *					intersect() const;							// what object did we collide with

	// Mutators
	bool					initializeProjectile( GadgetWeapon * pWeapon, Noun * pTarget, 
								const Vector3 & velocity, dword tick );

	void					setLife( int a_nLife );
	void					setTarget( Noun * pTarget );				// setting the target, makes this a guided projectile
	void					setOwner( Noun * Owner );
	virtual void			explode();									// explode projectile, setting it's life to -1

	// Static
	static float			sm_fTrailScalar;
	static bool				sm_bRenderProjectiles;						// render the actual projectile object, only turned on for target views..

protected:
	// Data
	int						m_nLife;				// life in ticks
	float					m_fBaseSignature;
	GadgetWeapon::wRef		m_pWeapon;				// owner of projectile
	Noun::tRef				m_pTarget;				// projectile target
	int						m_nTracerLife;
	int						m_nTracerRate;

	// non-serialized
	Scene::Link				m_pProjectile;			// projectile scene
	Noun::WeakRef			m_pOwner;				// who owns this projectile..
	Noun::WeakRef			m_pIntersect;			// who did this projectile intersect
	Trail					m_Trail;
	float					m_Time;

	// Helpers
	void					intercept( const Vector3 & target,		// target world position
								const Vector3 & velocity,			// target velocity in world space
								Vector3 & intercept,				// the intercept point
								float fOvershoot = 0.0f ) const;		// how much to over-estimate the distance

};

//----------------------------------------------------------------------------

inline int NounProjectile::life() const
{
	return m_nLife;
}

inline GadgetWeapon * NounProjectile::weapon() const
{
	return m_pWeapon;
}

inline Noun * NounProjectile::owner() const
{
	return m_pOwner;
}

inline Noun * NounProjectile::target() const
{
	return m_pTarget;
}

inline Noun * NounProjectile::intersect() const
{
	return m_pIntersect;
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF


