/*
	NounScenery.h
	
	This noun is used to display effects
	(c)1999 PaleStar Development, Richard Lyle
*/

#ifndef NOUN_SCENERY_H
#define NOUN_SCENERY_H

#include "Render3D/Scene.h"
#include "NounGame.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL NounScenery : public NounGame
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference<NounScenery>	Ref;

	// Construction
	NounScenery();
	// BaseNode interface
	void			render( RenderContext &context, 
						const Matrix33 & frame, 
						const Vector3 & position );

	// Noun interface
	NounType		nounType() const;
	// Accessors
	float			time() const;
	// Mutators
	void			setTime( float time );

protected:
	// Data
	float			m_Time;
};

//----------------------------------------------------------------------------

inline float NounScenery::time() const
{
	return( m_Time );
}

inline void NounScenery::setTime( float time )
{
	m_Time = time;
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
