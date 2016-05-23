/*
	Port.cpp
	(c)1998 Palestar, Richard Lyle
*/

#define NOUNCONTEXTPORT_DLL
#include "stdafx.h"

#include "Port.h"
#include "Resource.h"

#include "DarkSpace/DropData.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounContextPort, Port );
REGISTER_FACTORY_KEY( NounContextPort, 4283356736651368059 );

BEGIN_PROPERTY_LIST( NounContextPort, Port )
	ADD_PROPERTY( m_Name );
	ADD_PROPERTY( m_Description );
	ADD_CLASSKEY_PROPERTY( m_Class, Noun );
	ADD_CLASSKEY_PROPERTY( m_Type, Noun );
	ADD_PORT_PROPERTY( m_Scene, ScenePort );
	ADD_PROPERTY( m_Mass );
	ADD_PROPERTY( m_bHullOverride );
	ADD_PROPERTY( m_Hull );
	ADD_PROPERTY( m_Flags );
	ADD_PROPERTY( m_Data );
	// DEPRECATED
	ADD_PROPERTY( m_Resources );
		ADD_PROPERTY_FLAGS( m_Resources, PF_NOEDIT );
	ADD_PROPERTY( m_Drops );
		ADD_PROPERTY_FLAGS( m_Drops, PF_NOEDIT );
END_PROPERTY_LIST();

BEGIN_NAMED_PROPERTY_LIST( PLIST_NounContextPort_KeyPort, NounContextPort::KeyPort, PropertyClass );
	ADD_PROPERTY( m_sKey );
	ADD_PORT_PROPERTY( m_sPort, Port );
END_PROPERTY_LIST();

NounContextPort::NounContextPort() : 
	m_Type( CLASS_KEY(Noun) ),
	m_Class( CLASS_KEY(Noun) ),
	m_Mass( 1.0f ),
	m_Flags( 0 ),
	m_bHullOverride( false ),
	m_Hull( true )
{}

//----------------------------------------------------------------------------

const int VERSION_080801 = 80801;
const int VERSION_20041201 = 20041201;

bool NounContextPort::read( const InStream &input )
{
	if (! Port::read( input ) )
		return true;

	// convert any old drops into the new DropData class..
	for(int i=0;i<m_Drops.size();++i)
	{
		Drop * pDrop = m_Drops[i];
		if (! pDrop )
			continue;

		DropData * pNewDrop = new DropData();
		pNewDrop->m_nDropClassKey = pDrop->m_nDropClassKey;
		pNewDrop->m_nBaseAmount = pDrop->m_nBaseAmount;
		pNewDrop->m_nRandomAmount = pDrop->m_nRandomAmount;
		pNewDrop->m_nChance = pDrop->m_nChance;

		m_Data.push( pNewDrop );
	}
	m_Drops.release();

	for(int i=0;i<m_Resources.size();++i)
		m_Data.push( new NounContext::ResourceData( m_Resources[i].m_sKey, m_Resources[i].m_sPort) );
	m_Resources.release();

	if (! m_bHullOverride )
	{
		Scene::Link pScene = Port::portResource( m_Scene );
		if ( pScene )
			m_Hull = pScene->calculateHull();
	}

	return true;
}

//----------------------------------------------------------------------------

bool NounContextPort::autoLoad()
{
	return false;
}

void NounContextPort::dependencies( DependentArray & dep )
{
	dep.push( m_Scene );
	for(int i=0;i<m_Data.size();i++)
	{
		NounContext::ResourceData * pData = WidgetCast<NounContext::ResourceData>( m_Data[i] );
		if ( pData != NULL )
			dep.push( pData->port() );
	}
}

Resource::Ref NounContextPort::createResource()
{
	return initialize( new NounContext() );
}

bool NounContextPort::importFile( const char * fileName )
{
	return true;	// return true of successful import, false on failure
}

void NounContextPort::shellOpen() const
{
	MessageBox( NULL, _T("shellOpen() is not valid for this port"), NULL, MB_OK);
}

//----------------------------------------------------------------------------

static bool FindNouns( BaseNode * pNode )
{
	if (! pNode )
		return false;
	if ( WidgetCast<Noun>( pNode ) )
		return true;

	for(int i=0;i<pNode->childCount();++i)
		if ( FindNouns( pNode->child(i) ) )
			return true;

	return false;
}

NounContext * NounContextPort::initialize( NounContext * pContext )
{
	Scene::Link pScene = Port::portResource( m_Scene );
	if ( pScene )
	{
		// make sure the scene contains NO Noun classes...
		if ( FindNouns( pScene->node() ) )
		{
			report( "Invalid classes found in the scene of a NounContext; Noun classes are NOT allowed!" );
			pScene = NULL;
		}
	}

	pContext->setName( m_Name );
	pContext->setDescription( m_Description );
	pContext->setScene( pScene, !m_bHullOverride );
	if ( m_bHullOverride )
		pContext->setHull( m_Hull );
	pContext->setMass( m_Mass );
	pContext->setFlags( m_Flags );

	for(int i=0;i<m_Data.size();++i)
		pContext->addData( m_Data[i] );

	return pContext;
}

//-------------------------------------------------------------------------------
// EOF

