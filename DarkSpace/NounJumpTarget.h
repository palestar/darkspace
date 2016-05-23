/*
	NounJumpTarget.h
	(c)1999 PaleStar Development, Richard Lyle
*/

#ifndef NOUN_JUMP_TARGET_H
#define NOUN_JUMP_TARGET_H

#include "World/NounZoneTarget.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL NounJumpTarget : public NounZoneTarget
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference< NounJumpTarget >		Ref;

	// Construction
	NounJumpTarget();
	NounJumpTarget( const Vector3 & vPosition, float fRadius, WorldContext * pContext );
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF

