/*
	Port.h

	ResourcePort object
	(c)1998 Palestar, Richard Lyle
*/

#ifndef NOUNJUMPGATEPORT_H
#define NOUNJUMPGATEPORT_H

#include "Standard/Dll.h"
#include "Tools/NounBodyPort/Port.h"

//-------------------------------------------------------------------------------

#undef DLL
#ifdef NOUNJUMPGATEPORT_DLL
#define DLL			DLL_EXPORT
#else
#define DLL			DLL_IMPORT
#endif

//----------------------------------------------------------------------------

class DLL NounJumpGatePort : public NounBodyPort
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference<NounJumpGatePort>	Ref;

	// Construction
	NounJumpGatePort();

	// NounBodyPort Interface
	void						dependencies( DependentArray & dep );
	CFrameWnd *					createView();
	BaseNode *					createNode();
	void						initializeNode( BaseNode * pNode );

	CharString					m_Destination;

	bool						m_bActive;
	bool						m_bAutoActivate;
	bool						m_bSafeZone;
	float						m_fSafeZoneDistance;
	bool						m_bDuration;
	dword						m_nDuration;
	bool						m_bInterval;
	dword						m_nInterval;
};


#endif

//-------------------------------------------------------------------------------
// EOF
