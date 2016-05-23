/*
	NounField.h

	A field is a collidable root object, that cannot be detected usually. It's position is linked to another noun, possibly a child of
	another object (i.e. GadgetJD on a ship)

	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef NOUNFIELD_H
#define NOUNFIELD_H

#include <map>
#include <set>

#include "NounGame.h"
#include "GameDll.h"		// include last always

//----------------------------------------------------------------------------

class DLL NounField : public NounGame
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference< NounField >				Ref;
	typedef WidgetReference< NounField >		wRef;

	// Construction
	NounField();

	// Noun interface
	virtual NounType	
					nounType() const;
	virtual bool	collidable() const;				// can this object collide
	virtual bool	canBeDetected() const;			// can this object be detected
	
	virtual bool	postInitialize();
	virtual void	initialize();
	virtual void	release();
	virtual void	simulate( dword nTick );	

	virtual void	collision( Noun * pCollide,
						const Vector3 & intersect );	// handle collision between two colliable objects, intersect position is in world space
	virtual void	update();

	// Mutators
	void			setFieldRadius( float fRadius );
	
	virtual void	onEnterField( Noun * pNoun ) = 0;
	virtual void	onInsideField( Noun * pNoun ) = 0;
	virtual void	onLeaveField( Noun * pNoun ) = 0;

	//! Helper function to remove all fields from the given heirarchy..
	static void		removeAllFields( BaseNode * a_pNode );

protected:
	//! Types
	typedef std::map< WidgetKey, Noun::wRef >		InFieldMap;
	typedef std::set< WidgetKey >					CollisionSet;

	// Data
	float			m_fFieldRadius;

	// non-serialized
	InFieldMap		m_InField;
	CollisionSet	m_CollisionSet;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
