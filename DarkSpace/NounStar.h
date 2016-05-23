/*
	NounStar.h
	(c)1999 PaleStar, Richard Lyle
*/

#ifndef NOUN_STAR_H
#define NOUN_STAR_H

#include "NounBody.h"
#include "NounPlanet.h"
#include "Constants.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL NounStar : public NounBody
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference< NounStar >				Ref;
	typedef WidgetReference< NounStar >			wRef;

	enum RenderFlags {
		RENDER_FLARE		= 0x1,
	};

	// Construction
	NounStar();

	// BaseNode interface
	void					render( RenderContext &context, 
								const Matrix33 & frame, 
								const Vector3 & position );

	// Noun interface
	virtual NounType		nounType() const;
	virtual bool			collidable() const;
	virtual float			baseSignature() const;

	virtual void			simulate( dword nTick );
	virtual void			collision( Noun * pCollide, 
								const Vector3 & intersect );
	virtual bool			postInitialize();

	// NounGame interface
    virtual bool			canShipCollide() const;
	virtual bool			canOrbit() const;

	// Accessors
	int						planetCount() const;					// number of planets orbiting this star
	NounPlanet *			planet( int n ) const;

	// Mutators
	void					addPlanet( NounPlanet * pPlanet );		// this is called by the planets orbiting this star
	void					removePlanet( NounPlanet * pPlanet );	// remove a planet

private:
	// Data
	Array< NounPlanet * >	m_Planets;								// planets oribitng this star
	dword					m_nCheckFactionTick;	
};

//----------------------------------------------------------------------------

inline int NounStar::planetCount() const
{
	return m_Planets.size();
}

inline NounPlanet * NounStar::planet( int n ) const
{
	return m_Planets[ n ];
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
