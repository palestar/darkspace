/*
	NounAsteroid.h
	(c)1999 PaleStar, Richard Lyle
*/

#ifndef NOUN_ASTEROID_H
#define NOUN_ASTEROID_H

#include "NounBody.h"
#include "Constants.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL NounAsteroid : public NounBody
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference< NounAsteroid >				Ref;
	typedef WidgetReference< NounAsteroid >			wRef;

	// Construction
	NounAsteroid();

	// Noun interface
	NounType				nounType() const;				
	bool					canCollide() const;
	bool					collidable() const;

	void					simulate( dword nTick );
	void					collision( Noun * pCollide, 
								const Vector3 & intersect );
	bool					canDamage( dword type ) const;	
	void					inflictDamage( dword nWhen, Noun * pFrom, int damage, 
								dword type, const Vector3 & direction );
	// NounGame interface
	virtual bool			canShipCollide() const;

	// Accessors
	const Vector3 &			velocity() const;
	int						damage() const;

	// Mutators
	void					setVelocity( const Vector3 & velocity );
	// Helpers
	int						maxDamage() const;

protected:
	// Data
	Vector3					m_Velocity;					// current velocity vector
	int						m_Damage;					// current damage

	// non-serilized
	bool					m_bZeroVelocity;			// set to true if the velocity is zero
};

//----------------------------------------------------------------------------

inline const Vector3 & NounAsteroid::velocity() const
{
	return m_Velocity;
}

inline int NounAsteroid::damage() const
{
	return m_Damage;
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
