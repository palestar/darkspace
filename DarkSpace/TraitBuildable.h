/*
	TraitBuildable.h

	A noun with this trait can be built
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef TRAITBUILDABLE_H
#define TRAITBUILDABLE_H

#include "Trait.h"
#include "Noun.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL TraitBuildable : public Trait
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	class BuildContext		// abstract interface for buildable nouns
	{
	public:
		virtual float	buildTime() const = 0;						// time in seconds to build
		virtual int		buildCost() const = 0;						// costs to build in resources
	};

	// Construction
	TraitBuildable(); 

	// Trait interface
	void		simulate( dword nTick );					
	
	// Accessors
	int			complete() const;				// percentage complete, 0 - 100
	int			buildTime() const;				// 
	int			buildRate() const;				

	// Mutators

	Noun *		orbiting() const;				// object we are orbiting
	float		orbitDistance() const;			// distance from the object
	float		orbitPosition() const;			// position around the object in radians
	float		orbitVelocity() const;			// orbital velocity

	// Mutators
	void		setOrbit( Noun * pOrbit, float fDistance, 
					float fPosition, float fVelocity, int nTicks = 0 );
	void		breakOrbit();
	void		dynamics( int nTicks );

	// Helpers
	int			buildCost() const;				// cost to build this structure
private:
	// Data
	int			m_nBuild;
	float		m_fBuildTime;
};

//----------------------------------------------------------------------------

inline Noun * TraitBuildable::orbiting() const
{
	return m_pOrbiting;
}

inline float TraitBuildable::orbitDistance() const
{
	return m_fOrbitDistance;
}

inline float TraitBuildable::orbitPosition() const
{
	return m_fOrbitPosition;
}

inline float TraitBuildable::orbitVelocity() const
{
	return m_fOrbitVelocity;
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
