/*
	Port.h

	ResourcePort object
	(c)1998 Palestar, Richard Lyle
*/

#ifndef NOUNPLANETPORT_H
#define NOUNPLANETPORT_H

#include "Standard/Dll.h"
#include "DarkSpace/NounPlanet.h"
#include "Tools/NounBodyPort/Port.h"

//-------------------------------------------------------------------------------

#undef DLL
#ifdef NOUNPLANETPORT_DLL
#define DLL			DLL_EXPORT
#else
#define DLL			DLL_IMPORT
#endif

//----------------------------------------------------------------------------

class DLL NounPlanetPort : public NounBodyPort
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference<NounPlanetPort>	Ref;

	// Construction
	NounPlanetPort();

	// Widget Interface
	bool						read( const InStream & input );
	// BaseNodePort Interface
	void						dependencies( DependentArray & dep );
	CFrameWnd *					createView();
	BaseNode *					createNode();
	void						initializeNode( BaseNode * pNode );

	int							m_Population;
	bool						m_bHydrogen;
	bool						m_bOxygen;
	bool						m_bMetals;
	bool						m_bHeavyMetals;
	bool						m_bCyroMetals;
	bool						m_bHyperMatter;
	bool						m_bDarkMatter;
	bool						m_bUrdaniam;
};


#endif

//-------------------------------------------------------------------------------
// EOF
