/*
	Port.h

	Port object
	(c)1998 Palestar, Richard Lyle
*/

#ifndef SHIPCONTEXTPORT_H
#define SHIPCONTEXTPORT_H

#include "Standard/Dll.h"
#include "Tools/NounContextPort/Port.h"

//-------------------------------------------------------------------------------

#undef DLL
#ifdef SHIPCONTEXTPORT_DLL
#define DLL		DLL_EXPORT
#else
#define DLL		DLL_IMPORT
#endif

//-------------------------------------------------------------------------------

class DLL ShipContextPort : public Port
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	typedef Reference<ShipContextPort>	Ref;

	// Construction
	ShipContextPort();

	// Widget Interface
	bool						read( const InStream & );
	// Port Interface
	bool						autoLoad();									// enable autoload for this resource
	void						dependencies( DependentArray & dep );		// get the depdendencies
	Resource::Ref				createResource();							// create the resource 
	CFrameWnd *					createView();								// create a view of the port

	bool						importFile( const char * fileName );		// import from file
	void						shellOpen() const;							// open the source object through a shell program

	// Data
	dword						m_Badges;
	int							m_Rank;
	CharString					m_Ship;
};


#endif

//-------------------------------------------------------------------------------
// EOF
