/*
	VerbImportExport.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VerbImportExport_H
#define VerbImportExport_H

#include "World/Verb.h"
#include "NounPlanet.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbImportExport : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbImportExport();
	VerbImportExport( NounPlanet * pPlanet, bool bImport, bool bExport );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;	

	bool					canAttach( Noun * pNoun );
	void					onExecute();
	
	// Data
	bool					m_bImport;
	bool					m_bExport;

	static bool				canImport( NounPlanet * pPlanet );
	static bool				canExport( NounPlanet * pPlanet );
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
