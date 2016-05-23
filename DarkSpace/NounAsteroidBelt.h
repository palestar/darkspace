/*
	NounAsteroidBelt.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef NOUNASTEROIDBELT_H
#define NOUNASTEROIDBELT_H

#include "NounAsteroid.h"
#include "Constants.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL NounAsteroidBelt : public NounBody
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference< NounAsteroidBelt >				Ref;
	typedef WidgetReference< NounAsteroidBelt >			wRef;

	// Construction
	NounAsteroidBelt();
	// Noun interface
	NounType				nounType() const;	
	bool					collidable() const;

	bool					canBeDetected() const;

	void					release();
	void					simulate( dword nTick );

	Array< NounAsteroid::wRef >
							m_Asteroids;
	dword					m_nSpawnTick;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
