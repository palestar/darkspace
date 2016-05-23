/*
	Port.h

	ResourcePort object
	(c)1998 Palestar, Richard Lyle
*/

#ifndef NOUNGADGETPORT_H
#define NOUNGADGETPORT_H

#include "Standard/Dll.h"
#include "DarkSpace/NounGadget.h"
#include "Tools/NounPort/Port.h"

//-------------------------------------------------------------------------------

#undef DLL
#ifdef NOUNGADGETPORT_DLL
#define DLL			DLL_EXPORT
#else
#define DLL			DLL_IMPORT
#endif

//----------------------------------------------------------------------------

class DLL NounGadgetPort : public NounPort
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference<NounGadgetPort>	Ref;

	// Construction
	NounGadgetPort();

	// Widget Interface
	bool						read( const InStream & input );
	// BaseNodePort Interface
	void						dependencies( DependentArray & dep );
	CFrameWnd *					createView();
	BaseNode *					createNode();
	void						initializeNode( BaseNode * pNode );

	// Data
	int							m_nDamage;
	bool						m_bMountFore;
	bool						m_bMountRight;
	bool						m_bMountLeft;
	bool						m_bMountAft;
};


#endif

//-------------------------------------------------------------------------------
// EOF
