/*
	NounPod.h

	This noun is used to transport cargo from one noun to another
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef NOUNPOD_H
#define NOUNPOD_H

#include "Factory/WidgetWrap.h"
#include "Trail.h"
#include "NounGame.h"
#include "NounShip.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL NounPod : public NounGame
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	static NounContext::Link	sm_pPodContext;

	// Types
	typedef Reference< NounPod >		Ref;
	// Construction
	NounPod();

	// Node interface
	void			render( RenderContext &context, 
						const Matrix33 & frame, 
						const Vector3 & position );

	// Noun interface
	NounType		nounType() const;
	bool			canCollide() const;				// can this object hit
	bool			collidable() const;				// can this object collide
	bool			canDamage( dword type ) const;	// can this object be damaged by the specified damage type
	bool			canBeDetected() const;			// can this object be detected
	bool			enableECM() const;
	bool			enablePD() const;
	float			baseSignature() const;			// what is the default signature for this noun (by default the radius)
	
	void			initialize();
	void			release();
	void			simulate( dword nTick );						// update noun forward in time
	void			collision( Noun * pCollide,
						const Vector3 & intersect );	// handle collision between two colliable objects, intersect position is in world space
	void			onWorldTransform( 
						const Vector3 & transform );
	void			inflictDamage( dword nWhen, Noun * pFrom, 
						int damage, dword type, 
						const Vector3 & direction );

	// Accessors
	int				life() const;					// life of this pod in ticks
	int				damage() const;					// how much damage inflicted on this object
	Noun *			target() const;					// our target
	
	// Mutators
	void			setLife( int life );		
	void			setDamage( int damage );
	void			setCargo( Noun * pCargo );
	void			setTarget( Noun * pTarget );
	void			setFrom( NounShip * pFrom );

protected:
	// Data
	int				m_Life;
	int				m_Damage;
	WidgetWrap		m_Cargo;
	Noun::tRef		m_Target;
	NounShip::WeakRef
					m_From;

	// non-serialized
	Trail			m_Trail;

	// Mutators
	void			explode();						// kill this pod
};



#endif

//----------------------------------------------------------------------------
//EOF
