/*
	NounFighter.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef NOUNFIGHTER_H
#define NOUNFIGHTER_H

#include "NounSmartProjectile.h"
#include "GadgetBeamWeapon.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL NounFighter : public NounSmartProjectile
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef WidgetReference< NounFighter >	wRef;

	enum State
	{
		ATTACK,				// attacking current target
		EVADE,				// moving away from the current target
		ROLL,				// roll ship right
		LOOP,				// loop ship
		RETURN,				// returning home
		PROTECT,			// protect allied ship
	};

	// Construction
	NounFighter();

	// Widget interface
	virtual bool			read( const InStream & );
	// BaseNode interface
	void					onDetaching();
	// Noun interface
	NounType				nounType() const;	

	void					collision( Noun * pCollide,
								 const Vector3 & intersect );
	void					simulate( dword nTick );
	// NounProjectile interface
	void					explode();									// explode projectile, setting it's life to -1

	// Accessors
	State					state() const;
	// Mutators
	void					setState( State state );

protected:
	// Data
	u8						m_nState;
	bool					m_bDestroyed;
	dword					m_nUpdateTick;

	void					updatePointDefense( GadgetBeamWeapon * pCheck );
};

IMPLEMENT_RAW_STREAMING( NounFighter::State );

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
