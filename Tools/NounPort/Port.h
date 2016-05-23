/*
	Port.h

	ResourcePort object
	(c)1998 Palestar, Richard Lyle
*/

#ifndef NOUNPORT_H
#define NOUNPORT_H

#include "Standard/Dll.h"
#include "Tools/ScenePort/NodePort.h"
#include "Tools/NounContextPort/Port.h"

//-------------------------------------------------------------------------------

#undef DLL
#ifdef NOUNPORT_DLL
#define DLL			DLL_EXPORT
#else
#define DLL			DLL_IMPORT
#endif

//----------------------------------------------------------------------------

class DLL NounPort : public NodePort
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference<NounPort>	Ref;

	// Construction
	NounPort();

	// Widget Interface
	bool						read( const InStream & );
	// BaseNodePort Interface
	void						dependencies( DependentArray & dep );
	CFrameWnd *					createView();
	BaseNode *					createNode();
	void						initializeNode( BaseNode * pNode );

	// Data
	CharString					m_NounContext;
	// used only to upgrade a NounPort
	NounContextPort	*			m_pNounContextPort;
};


#endif

//-------------------------------------------------------------------------------
// EOF
