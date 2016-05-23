/*
	NounDropTarget.h

	Target object
	(c)1999 PaleStar Development, Richard Lyle
*/

#ifndef NOUN_DROP_TARGET_H
#define NOUN_DROP_TARGET_H

#include "World/NounTarget.h"
#include "NounPlanet.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL NounDropTarget : public NounTarget
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference< NounDropTarget >			Ref;
	typedef TargetReference< NounDropTarget >	tRef;

	// Construction
	NounDropTarget();
	NounDropTarget( NounPlanet * pPlanet, int hex );

	// Widget interface
	bool				read( const InStream & );
	// Noun interface
	virtual void		calculateWorld() const;
	// Accessors
	NounPlanet *		planet() const;
	int					hex() const;

private:
	// Data
	NounPlanet::wRef	m_Planet;
	int					m_Hex;
};

//----------------------------------------------------------------------------

inline NounPlanet * NounDropTarget::planet() const
{
	return m_Planet;
}

inline int NounDropTarget::hex() const
{
	return m_Hex;
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF

