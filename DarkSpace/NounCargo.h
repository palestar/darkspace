/*
	NounCargo.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef NOUNCARGO_H
#define NOUNCARGO_H

#include "NounGame.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL NounCargo : public NounGame
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference<NounCargo>			Ref;
	typedef WidgetReference<NounCargo>		wRef;

	// Construction
	NounCargo();
	// BaseNode interface
	void			preRender( RenderContext &context, 
						const Matrix33 & frame, 
						const Vector3 & position );
	// Noun interface
	NounType		nounType() const;				
	bool			collidable() const;				// can this object collide
	bool			canDamage( dword type ) const;	// can this object be damaged by the specified damage type
	
	bool			canBeDetected() const;
	bool			isCargo() const;

	virtual bool	postInitialize();
	void			simulate( dword nTick );

	void			collision( Noun * pCollide, 
						 const Vector3 & intersect );
	void			inflictDamage( dword nWhen, Noun * pFrom, 
						int damage, dword type, 
						const Vector3 & direction );

	// Accessors
	int				quantity() const;

	virtual int		durability() const;				// 0-100, chance of surviving ship explosion
	virtual int		value() const;					// get value of cargo in credits
	virtual int		size() const;					// how much cargo space is needed 
	virtual int		maxQuantity() const;			// what is the maximum quantity

	// Mutators
	virtual void	setQuantity( int quantity );
	virtual CharString		
					status() const;					// get the target status of this object
	
	void			explode();

protected:
	// Data
	int				m_Quantity;
};

//----------------------------------------------------------------------------




#endif

//----------------------------------------------------------------------------
//EOF

