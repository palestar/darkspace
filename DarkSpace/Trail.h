/*
	Trail.h

	This object is used to create a con-trail for a moving game object
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef TRAIL_H
#define TRAIL_H

#include "Render3D/Scene.h"
#include "GameDll.h"



//----------------------------------------------------------------------------

class DLL Trail
{
public:
	// Construction
	Trail();
	Trail( const Trail & a_Copy );

	// Accessors
	bool			isEnded() const;						// returns true if the tail is at the head
	Material *		trailMaterial() const;
	int				trailLife() const;
	int				trailRate() const;

	// Mutators
	void			release();

	void			setTrailMaterial( Material * pMaterial );
	void			setTrailLife( int life );
	void			setTrailRate( int rate );

	void			updateTrail( const Vector3 & worldPosition );
	void			renderTrail( RenderContext &context, 
						const Matrix33 & frame, 
						const Vector3 & position,
						const Vector3 & headPosition );		// headPosition = worldPosition()
	void			renderProjectileTrail( RenderContext &context, 
						const Matrix33 & frame, 
						const Vector3 & position,
						const Vector3 & headPosition );		// headPosition = worldPosition()

	void			offsetTrail( const Vector3 & add );

	// static 
	static float	sm_fTrailScalar;
	static float	sm_fTrailFarClip;
	static float	sm_fTrailFadeDistance;					// this should less than sm_fTrailFarClip

protected:
	// Types
	struct Segment
	{
		int		life;
		Vector3	position;
	};
	typedef std::list< Segment >		SegmentList;

	// Data
	Material::Link			m_TrailMaterial;				// trail material
	int						m_TrailLife;					// life span of each segment
	int						m_TrailRate;					// how often to create new segments

	int						m_TrailUpdate;					// next trail update time in ticks
	float					m_TrailRadius;					// size of the trail
	SegmentList				m_Trail;						// trail segments
};

//----------------------------------------------------------------------------

inline bool Trail::isEnded() const
{
	return m_TrailRadius < 0.05f;
}

inline Material * Trail::trailMaterial() const
{
	return m_TrailMaterial;
}

inline int Trail::trailLife() const
{
	return m_TrailLife;
}

inline int Trail::trailRate() const
{
	return m_TrailRate;
}

//----------------------------------------------------------------------------

inline void Trail::release()
{
	m_Trail.clear();
}

inline void Trail::setTrailMaterial( Material * pMaterial )
{
	m_TrailMaterial = pMaterial;
}

inline void Trail::setTrailLife( int life )
{
	m_TrailLife = life;
}

inline void Trail::setTrailRate( int rate )
{
	m_TrailRate = rate;
	m_TrailUpdate = rate;
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
