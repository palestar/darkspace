/*
	Port.h

	ResourcePort object
	(c)1998 Palestar, Richard Lyle
*/

#ifndef NOUNSHIPPORT_H
#define NOUNSHIPPORT_H

#include "Standard/Dll.h"
#include "Tools/NounBodyPort/Port.h"

//-------------------------------------------------------------------------------

#undef DLL
#ifdef NOUNSHIPPORT_DLL
#define DLL			DLL_EXPORT
#else
#define DLL			DLL_IMPORT
#endif

//----------------------------------------------------------------------------

class DLL NounShipPort : public NounBodyPort
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference<NounShipPort>	Ref;

	// Construction
	NounShipPort();

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
