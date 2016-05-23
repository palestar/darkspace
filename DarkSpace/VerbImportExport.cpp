/*
	VerbImportExport.cpp
	(c)2012 Palestar Inc, Jack Wallace
*/


#include "Debug/Assert.h"
#include "VerbImportExport.h"
#include "GameContext.h"
#include "resource.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbImportExport, Verb );

BEGIN_PROPERTY_LIST( VerbImportExport, Verb )
	ADD_TRANSMIT_PROPERTY( m_bImport );
	ADD_TRANSMIT_PROPERTY( m_bExport );
END_PROPERTY_LIST();

VerbImportExport::VerbImportExport()
{}

VerbImportExport::VerbImportExport( NounPlanet * pPlanet, bool bImport, bool bExport ) : m_bImport( bImport ), m_bExport( bExport )
{
	attachVerb( pPlanet );
}

//----------------------------------------------------------------------------

Verb::Priority VerbImportExport::priority() const
{
	return MEDIUM;
}

VerbImportExport::Scope VerbImportExport::scope() const
{
	return GLOBAL;
}

bool VerbImportExport::client() const
{
	return true;
}

bool VerbImportExport::canAttach( Noun * pNoun )
{
	return WidgetCast<NounPlanet>( pNoun ) != NULL;
}

void VerbImportExport::onExecute()
{
	NounPlanet * pPlanet = WidgetCast<NounPlanet>( target() );
	if ( validate( pPlanet ) )
	{
		ASSERT( pPlanet );

		if ( pPlanet->importStatus() != m_bImport )
			pPlanet->setImport( m_bImport );

		if ( pPlanet->exportStatus() != m_bExport )
			pPlanet->setExport( m_bExport );
	}
}

//----------------------------------------------------------------------------

bool VerbImportExport::canImport( NounPlanet * pPlanet )
{
	ASSERT( pPlanet );

	if ( pPlanet == NULL )
		return false;		// no planet
		
	return pPlanet->importStatus();
}

bool VerbImportExport::canExport( NounPlanet * pPlanet )
{
	ASSERT( pPlanet );

	if ( pPlanet == NULL )
		return false;		// no planet
		
	return pPlanet->exportStatus();
}

//----------------------------------------------------------------------------
//EOF
