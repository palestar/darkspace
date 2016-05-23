/*
	Port.h

	ResourcePort object
	(c)1998 Palestar, Richard Lyle
*/

#ifndef NOUNTRAILPORT_H
#define NOUNTRAILPORT_H

#include "Standard/Dll.h"
#include "Tools/NounPort/Port.h"

//-------------------------------------------------------------------------------

#undef DLL
#ifdef NOUNTRAILPORT_DLL
#define DLL			DLL_EXPORT
#else
#define DLL			DLL_IMPORT
#endif

//----------------------------------------------------------------------------

class DLL NounTrailPort : public NounPort
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference<NounTrailPort>	Ref;

	// Construction
	NounTrailPort();

	// Widget Interface
	bool						read( const InStream & );
	// BaseNodePort Interface
	void						dependencies( DependentArray & dep );
	CFrameWnd *					createView();
	BaseNode *					createNode();
	void						initializeNode( BaseNode * pNode );

};


#endif

//-------------------------------------------------------------------------------
// EOF
