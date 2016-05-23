/*
	SceneryDebris.h

	This object is used to contain a projectile object
	(c)1999 Palestar, Richard Lyle
*/

#ifndef SCENERY_DEBRIS_H
#define SCENERY_DEBRIS_H

#include "SceneryEffect.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL SceneryDebris : public SceneryEffect
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef WidgetReference< SceneryDebris >	wRef;

	// Construction
	SceneryDebris();
	// Noun interface
	void					simulate( dword nTick );
	// Mutators
	void					setVelocity( const Vector3 & velocity );
	void					setRotationVelocity( const Vector3 & velocity );
	void					setEndEffect( Scene * pScene );

protected:
	// Data
	Vector3					m_Velocity;
	Vector3					m_RotationVelocity;	
	Scene::Link				m_EndEffect;								// effect created when debris ends
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF


