/*
	NounNebula.h
	(c)1999 PaleStar Development, Richard Lyle
*/

#ifndef NOUN_NEBULA_H
#define NOUN_NEBULA_H

#include "NounBody.h"
#include "NounShip.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL NounNebula : public NounBody
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference< NounNebula >				Ref;
	typedef WidgetReference< NounNebula >		wRef;

	// Construction
	NounNebula();
	// Noun interface
	NounType			nounType() const;	
	void				release();
	void				simulate( dword nTick );

private:
	// non-serialized
	dword				m_nUpdateAffected;
	Array< NounGame::wRef >	
						m_Affect;				// affected objects
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
