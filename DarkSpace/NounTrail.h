/*
	NounTrail.h

	This object is used to create a con-trail for a moving game object
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef NOUNTRAIL_H
#define NOUNTRAIL_H

#include "Render3D/Scene.h"
#include "NounGame.h"
#include "Trail.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL NounTrail : public NounGame
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference<NounTrail>	Ref;

	// Construction
	NounTrail();
	NounTrail( const Trail & a_Copy );

	// Node interface
	void			render( RenderContext &context, 
						const Matrix33 & frame, 
						const Vector3 & position );
	// Noun interface
	NounType		nounType() const;

	virtual bool	postInitialize();
	void			simulate( dword nTick );
	void			onWorldTransform( const Vector3 & transform );

	// Accessors
	const Trail &	trail() const;
	Material *		trailMaterial() const;

	// Mutators
	Trail &			trail();
	void			setDetachOnEnd( bool a_bDetachOnEnd );		// if true, trail is detached when the tail reaches the head
	void			setTrailMaterial( Material * pMaterial );
	void			resetTrail();
	void			reset();	

	//! This helper function copies any NounTrail objects into the zone, this is used
	//! to keep trails visible after a ship has been destroyed.
	static void		CopyTrailsIntoZone( BaseNode * pNode );	

protected:
	// Data
	Trail			m_Trail;
	bool			m_bUpdate;
	bool			m_bDetachOnEnd;
};

//----------------------------------------------------------------------------

inline const Trail & NounTrail::trail() const
{
	return m_Trail;
}

inline Material * NounTrail::trailMaterial() const
{
	return m_Trail.trailMaterial();
}

//---------------------------------------------------------------------------------------------------

inline Trail & NounTrail::trail()
{
	return m_Trail;
}

inline void NounTrail::setDetachOnEnd( bool a_bDetachOnEnd )
{
	m_bDetachOnEnd = a_bDetachOnEnd;
}

inline void NounTrail::setTrailMaterial( Material * pMaterial )
{
	m_Trail.setTrailMaterial( pMaterial );
}

inline void NounTrail::reset()
{
	m_Trail.release();
}


//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
