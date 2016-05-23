/*
	SceneryEffect.h

	This scenery noun is removed from the area after its life reaches zero. This object
	isn't removed from the area with a verb because it's a scenery object and has no bearing
	on the gameplay.

	This object can also be set to delay the render and update by placing the number of 
	seconds to delay in m_Delay.

	(c)1999 Palestar, Richard Lyle
*/

#ifndef SCENERY_EFFECT_H
#define SCENERY_EFFECT_H

#include "NounScenery.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL SceneryEffect : public NounScenery
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	SceneryEffect();

	// Node interface
	void			preRender( RenderContext & context, 
						const Matrix33 &frame, 
						const Vector3 &position );
	// Noun interface
	void			simulate( dword nTick );
	// accessors
	float			delay() const;
	float			life() const;
	// Mutators
	void			setDelay( float delay );
	void			setLife( float life );
	bool			setSegment( const char * segment );		// sets the m_Life & m_Time based on the Scene segment!!

protected:
	// Data
	float			m_Delay;		// delay in seconds before object renders and starts updating
	float			m_Life;			// lifespan in seconds of this object
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF

