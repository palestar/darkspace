/*
	Port.cpp
	(c)1998 Palestar, Richard Lyle
*/

#define NOUNSPAWNPORT_DLL
#include "stdafx.h"

#include "Port.h"
#include "PortView.h"

#include "Tools/ScenePort/ChildFrame.h"
#include "Tools/ScenePort/Port.h"
#include "Tools/ResourcerDoc/Port.h"

#include "DarkSpace/NounSpawnShip.h"
//#include "DarkSpace/GameContext.h"

//---------------------------------------------------------------------------------------------------

IMPLEMENT_NAMED_WIDGET_TYPES( TYPES_Spawner, NounSpawnPort::Spawner );
IMPLEMENT_NAMED_FACTORY( FACTORY_Spawner, NounSpawnPort::Spawner, Widget, true );
BEGIN_NAMED_PROPERTY_LIST( PLIST_Spawner, NounSpawnPort::Spawner, Widget );
	ADD_PORT_PROPERTY( m_sNounTemplate, ShipContextPort );
	ADD_PROPERTY( m_sBaseName );
	ADD_PROPERTY( m_bNumbered );
	ADD_PROPERTY( m_bRandom );
	ADD_PROPERTY( m_nLimit );
	ADD_PROPERTY( m_nChance );
	ADD_PROPERTY( m_nScore );
	ADD_ENUM_PROPERTY( m_nOrder );
		ADD_ENUM_OPTION( m_nOrder, NounShip::NOORDER );
		ADD_ENUM_OPTION( m_nOrder, NounShip::ATTACK );
		ADD_ENUM_OPTION( m_nOrder, NounShip::DEFEND );
		ADD_ENUM_OPTION( m_nOrder, NounShip::CAPTURE );
		ADD_ENUM_OPTION( m_nOrder, NounShip::MOVE );
		ADD_ENUM_OPTION( m_nOrder, NounShip::RELOAD );
		ADD_ENUM_OPTION( m_nOrder, NounShip::BEACON );
		ADD_ENUM_OPTION( m_nOrder, NounShip::HOLD );
		ADD_ENUM_OPTION( m_nOrder, NounShip::TRADE );
		ADD_ENUM_OPTION( m_nOrder, NounShip::RECON );
		ADD_ENUM_OPTION( m_nOrder, NounShip::BUILD );
	ADD_PROPERTY( m_sOrderTarget );				// TODO: Implement node selection
END_PROPERTY_LIST();

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounSpawnPort, NounBodyPort );
REGISTER_FACTORY_KEY( NounSpawnPort, 4153054435397921983 );

BEGIN_PROPERTY_LIST( NounSpawnPort, NounBodyPort )
	ADD_ENUM_PROPERTY( m_nSpawnType );
		ADD_ENUM_OPTION( m_nSpawnType, NounSpawnShip::ST_NORMAL );
		ADD_ENUM_OPTION( m_nSpawnType, NounSpawnShip::ST_POPULATION );
		ADD_ENUM_OPTION( m_nSpawnType, NounSpawnShip::ST_FACTION );
		ADD_ENUM_OPTION( m_nSpawnType, NounSpawnShip::ST_ONE_SHOT );
	ADD_PROPERTY( m_SpawnDelay );
	ADD_PROPERTY( m_SpawnArea );
	ADD_PROPERTY( m_MinSpawn );
	ADD_PROPERTY( m_MaxSpawn );
	ADD_PROPERTY( m_MinPlanets );
	ADD_PROPERTY( m_sTimeMask );
		SET_PROPERTY_NOTE( m_sTimeMask, "Sets the time mask when this spawner is active (M/D/Y H:M:S)" );
	ADD_PROPERTY( m_Spawners );
	// depreciated
	ADD_NOEDIT_PROPERTY( m_ShipContext );
END_PROPERTY_LIST();

NounSpawnPort::NounSpawnPort() : NounBodyPort(), 
	m_SpawnDelay( 30 ), 
	m_nSpawnType( NounSpawnShip::ST_NORMAL )
{
	m_Class = NounPort::m_Type = CLASS_KEY( NounSpawnShip );
	m_SpawnArea = 0.0f;
	m_MinSpawn = 0;
	m_MaxSpawn = 1;
	m_MinPlanets = 0;
}

//----------------------------------------------------------------------------

bool NounSpawnPort::read( const InStream &input )
{
	if (! NounBodyPort::read( input ) )
		return false;

	if ( m_ShipContext.length() > 0 )
	{
		Spawner & spawner = m_Spawners.push();
		spawner.m_sBaseName = name();
		spawner.m_sNounTemplate = m_ShipContext;

		m_ShipContext.release();
	}

	return true;
}

//-------------------------------------------------------------------------------

void NounSpawnPort::dependencies( DependentArray & dep )
{
	NounBodyPort::dependencies( dep );

	for(int i=0;i<m_Spawners.size();++i)
        dep.push( m_Spawners[i].m_sNounTemplate );
}

CFrameWnd * NounSpawnPort::createView()
{
	return NounBodyPort::createView();
}

BaseNode * NounSpawnPort::createNode()
{
	return NounBodyPort::createNode();
}

void NounSpawnPort::initializeNode( BaseNode * pNode )
{
	NounBodyPort::initializeNode( pNode );

	NounSpawnShip * pSpawn = WidgetCast<NounSpawnShip>( pNode );
	if ( pSpawn != NULL )
	{
		pSpawn->setSpawnType( (NounSpawnShip::SpawnType)m_nSpawnType );
		pSpawn->setSpawnDelay( m_SpawnDelay * TICKS_PER_SECOND );
		pSpawn->setSpawnArea( m_SpawnArea );
		pSpawn->setMinSpawn( m_MinSpawn );
		pSpawn->setMaxSpawn( m_MaxSpawn );
		pSpawn->setMinPlanets( m_MinPlanets );
		pSpawn->setTimeMask( m_sTimeMask );

		for(int i=0;i<m_Spawners.size();++i)
		{
			Spawner & spawner = m_Spawners[i];

			NounTemplate::Link pTemplate = Port::portResource( spawner.m_sNounTemplate );
			if (! pTemplate )
			{
				report( CharString().format( "Failed to get noun template %s", spawner.m_sNounTemplate ) );
				continue;
			}

			NounSpawnShip::Spawner * pNewSpawner = new NounSpawnShip::Spawner();
			pNewSpawner->setKey( spawner.key() );		// set the same key so we don't cause the WOB's to change between each build..
			pNewSpawner->setNounTemplate( pTemplate );
			pNewSpawner->setBaseName( spawner.m_sBaseName );
			pNewSpawner->setNumbered( spawner.m_bNumbered );
			pNewSpawner->setRandom( spawner.m_bRandom );
			pNewSpawner->setLimit( spawner.m_nLimit );
			pNewSpawner->setChance( spawner.m_nChance );
			pNewSpawner->setScore( spawner.m_nScore );
			pNewSpawner->setOrder( (NounShip::Order)spawner.m_nOrder, spawner.m_sOrderTarget );

			pSpawn->addSpawner( pNewSpawner );
		}
	}
	else
		report( "Invalid noun for NounSpawnPort!" );
}


//-------------------------------------------------------------------------------
// EOF

