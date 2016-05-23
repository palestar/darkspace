/*
	TraitOrbit.h

	This trait allows the noun to orbit other nouns
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef TRAITORBIT_H
#define TRAITORBIT_H

#include "World/Trait.h"
#include "World/TraitMovement.h"
#include "NounGame.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL TraitOrbit : public Trait
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	TraitOrbit(); 

	//! Widget interface
	bool		read( const InStream & input );
	bool		write( const OutStream & output );

	// Trait interface
	void		initialize();
	void		simulate( dword nTick );					
	
	// Accessors
	Noun *		orbiting() const;				// object we are orbiting
	float		orbitDistance() const;			// distance from the object
	float		orbitPosition() const;			// position around the object in radians
	float		orbitVelocity() const;			// orbital velocity in radians per second
	float		rotationalVelocity() const;		// rotational velocity in radians per second

	// Mutators
	void		setOrbit( Noun * pOrbit, float fDistance, 
					float fPosition, float fVelocity, dword nTick );
	void		setRotationalVelocity( float a_fRotVelocity );
	void		breakOrbit();
	void		dynamics( dword nTick );

private:
	// Data
	Noun::wRef	m_pOrbiting;					// who are we oribiting
	float		m_fOrbitDistance;				// distance from the object
	float		m_fOrbitPosition;				// in radians
	float		m_fOrbitVelocity;				// in r/s
	float		m_fRotVelocity;
	dword		m_nBaseTick;

	Vector3		m_vLastPosition;
	TraitMovement::WeakRef
				m_pMovement;
};

//----------------------------------------------------------------------------

inline Noun * TraitOrbit::orbiting() const
{
	return m_pOrbiting;
}

inline float TraitOrbit::orbitDistance() const
{
	return m_fOrbitDistance;
}

inline float TraitOrbit::orbitPosition() const
{
	return m_fOrbitPosition;
}

inline float TraitOrbit::orbitVelocity() const
{
	return m_fOrbitVelocity;
}

inline float TraitOrbit::rotationalVelocity() const
{
	return m_fRotVelocity;
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
