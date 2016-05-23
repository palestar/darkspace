/*
	Port.h

	ResourcePort object
	(c)1998 Palestar, Richard Lyle
*/

#ifndef NOUNBODYPORT_H
#define NOUNBODYPORT_H

#include "Standard/Dll.h"
#include "DarkSpace/NounBody.h"

#include "Tools/NounPort/Port.h"

//-------------------------------------------------------------------------------

#undef DLL
#ifdef NOUNBODYPORT_DLL
#define DLL			DLL_EXPORT
#else
#define DLL			DLL_IMPORT
#endif

//----------------------------------------------------------------------------

class DLL NounBodyPort : public NounPort
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference<NounBodyPort>	Ref;
	
	// Construction
	NounBodyPort();

	// Widget Interface
	bool						read( const InStream & );
	// BaseNodePort Interface
	void						dependencies( DependentArray & dep );
	CFrameWnd *					createView();
	BaseNode *					createNode();
	void						initializeNode( BaseNode * pNode );

	// Data
	int							m_nFleetId;
	CharString					m_Orbiting;
	float						m_OrbitVelocity;
};


#endif

//-------------------------------------------------------------------------------
// EOF
