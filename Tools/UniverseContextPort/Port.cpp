/*
	Port.cpp
	(c)1998 Palestar, Richard Lyle
*/

#define UNIVERSECONTEXTPORT_DLL
#include "stdafx.h"

#include "Port.h"
#include "PortView.h"
#include "ChildFrame.h"

#include "Resource.h"

#include "File/FileDisk.h"
#include "Resource/Collection.h"
#include "Render3D/NodeParticleField.h"

#include "DarkSpace/NounJumpGate.h"
#include "DarkSpace/NounSpawnShip.h"
#include "DarkSpace/TraitOrbit.h"
#include "DarkSpace/GameZone.h"

#include "Tools/ResourcerDoc/ResourcerDoc.h"
#include "Tools/ScenePort/Port.h"
#include "Tools/TextPort/Port.h"
#include "Tools/NodeParticleField/Port.h"
#include "Tools/ShipContextPort/Port.h"
#include "Tools/NounJumpGatePort/Port.h"
#include "Tools/NounPlanetPort/Port.h"
#include "Tools/NounBodyPort/Port.h"
#include "Tools/NounSpawnPort/Port.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( UniverseContextPort, Port );
REGISTER_FACTORY_KEY( UniverseContextPort, 4037388579725782390 );

BEGIN_PROPERTY_LIST( UniverseContextPort, Port )
	ADD_PROPERTY( m_Name );
	ADD_PROPERTY( m_Description );
	ADD_PROPERTY( m_Type );
	ADD_PORT_PROPERTY( m_Script, TextPort );
	ADD_PORT_PROPERTY( m_Scenery, ScenePort );
	ADD_PROPERTY( m_Position );
	ADD_PROPERTY( m_Zones );
	ADD_PROPERTY( m_Teams );
	ADD_PROPERTY( m_Conditions );
	ADD_PROPERTY( m_TimeLimit );
	// deprecated
	ADD_PORT_PROPERTY( m_UniversePort, ScenePort );
END_PROPERTY_LIST();

UniverseContextPort::UniverseContextPort() : Port()
{
	m_Name = "Context Name";
	m_Description = "Context Description";
	m_Type = 0;
	m_Position = Vector3( 0.0f, 0.0f, 0.0f );
	m_TimeLimit = TICKS_PER_HOUR * 2;
}

//----------------------------------------------------------------------------

const int VERSION_082100 = 82100;
const int VERSION_022301 = 22301;
const int VERSION_040501 = 40501;
const int VERSION_073101 = 73101;
const int VERSION_082701 = 82701;
const int VERSION_092001 = 92001;
const int VERSION_102401 = 102401;

bool UniverseContextPort::read( const InStream &input )
{
	if (! Port::read( input ) )
		return false;

	if ( m_UniversePort.length() > 0 )
	{
		Zone zone;
		zone.sScene = m_UniversePort;
		zone.vPosition = m_Position;

		m_Zones.push( zone );
		m_UniversePort = "";
		m_Position = Vector3( 0.0f, 0.0f, 0.0f );
	}
	
	return true;
}

//----------------------------------------------------------------------------

bool UniverseContextPort::autoLoad()
{
	return false;
}

void UniverseContextPort::dependencies( DependentArray & dep )
{
	dep.push( m_Scenery );
	dep.push( m_Script );

	for(int i=0;i<m_Zones.size();++i)
		dep.push( m_Zones[i].sScene );

	for(int i=0;i<m_Teams.size();i++)
	{
		dep.push( m_Teams[i].logo );
		dep.push( m_Teams[i].ships );
	}
	for(int i=0;i<m_Conditions.size();i++)
		dep.push( m_Conditions[i].next );
}

Resource::Ref UniverseContextPort::createResource()
{
	// turn off jump gate linking while building the resource.
	NounJumpGate::sm_bBuildMode = true;

	// load the scenery scene
	Scene::Link pScenery = Scene::Link( portResource( m_Scenery ) );
	Text::Link pScript = Text::Link( portResource( m_Script ) );

	BoxHull UniverseHull;

	// the list of all nouns to get added into the universe..
	Array< NounBody::Ref > nouns;
	for(int i=0;i<m_Zones.size();++i)
	{
		const Zone & zone = m_Zones[ i ];

		Scene::Link pScene = Scene::Link( portResource( zone.sScene ) );
		if (! pScene.valid() )
		{
			report( CharString().format( "Failed to load zone %s", zone.sScene ) );
			continue;
		}

		Vector3 vOffset( zone.vPosition * GALACTIC_UNIT_SIZE );
		BoxHull SceneHull( pScene->calculateHull() * 2.0f, vOffset );
		UniverseHull.addBox( SceneHull );
		if ( UniverseHull.radius() > 5000000.0f )
			report( CharString().format( "%s; Scene is increased map radius to %f gu.", zone.sScene.cstr(), UniverseHull.radius() ) );

		// push all nouns in this zone into a master list, update the position of each noun into would space..
		std::list< BaseNode * > nodes;
		nodes.push_back( pScene->node() );

		while( nodes.size() > 0 )
		{
			BaseNode * pNode = nodes.front();
			nodes.pop_front();

			if (! pNode )
				continue;

			// push all children into the stack..
			for(int j=0;j<pNode->childCount();++j)
				nodes.push_back( pNode->child(j) );

			NounBody * pNoun = WidgetCast<NounBody>( pNode );
			if (! pNoun )
				continue;

			pNoun->setPosition( pNoun->worldPosition() + vOffset );
			pNoun->setup();

			nouns.push( pNoun );
		}
	}

	TRACE( CharString().format("UniverseContextPort::createResource() - hull = %s/%s, nouns = %d", 
		ConvertType<CharString>( UniverseHull.min ), 
		ConvertType<CharString>( UniverseHull.max ), 
		nouns.size() ) );


	// create the universeContext object
	GameContext::Ref pContext = new GameContext();
	pContext->setPosition( m_Position * GALACTIC_UNIT_SIZE );
	pContext->setName( m_Name );
	pContext->setDescription( m_Description );
	pContext->setType( (GameContext::ContextType)m_Type );
	pContext->setScript( pScript.valid() ? CharString( pScript->text() ) : "" );

	for(int i=0;i<m_Teams.size();i++)
	{
		const Team & t = m_Teams[i];

		WorldContext::Team team;
		team.nTeamId = i + 1;
		team.clanId = 0;
		team.factionId = t.faction;
		team.flags = t.flags;
		team.name = t.name;
		team.longName = t.name;
		team.about = t.description;
		team.pTemplates = Collection::Link( portResource( t.ships ) );

		pContext->addTeam( team );
	}

	for(int i=0;i<m_Conditions.size();i++)
	{
		GameContext::Condition newCondition;
		newCondition.conditionId = i + 1;
		newCondition.script = "";
		newCondition.message = m_Conditions[i].message;
		newCondition.next = portResource( m_Conditions[i].next ).key();

		for(int j=0;j<m_Conditions[i].objectives.size();j++)
		{
			GameContext::Objective newObjective;
			newObjective.description = m_Conditions[i].objectives[j].description;
			newObjective.factionId = m_Conditions[i].objectives[j].faction;
			newObjective.type = m_Conditions[i].objectives[j].type;

			CharString sObjectiveName = m_Conditions[i].objectives[j].noun;

			bool bObjectiveFound = false;
			for(int k=0;k<nouns.size() && !bObjectiveFound;++k)
			{
				if ( _stricmp( sObjectiveName, nouns[k]->name() ) == 0 )
				{
					bObjectiveFound = true;
					newObjective.noun = nouns[k]->key();
					newCondition.objectives.push( newObjective );
				}
			}

			if (! bObjectiveFound )
				report( CharString().format("Failed to find objective %s, ignoring objective!",sObjectiveName)  );
		}

		if ( newCondition.objectives.size() > 0 )
			pContext->addCondition( newCondition );
	}
	pContext->setTimeLimit( m_TimeLimit );

	if (! pContext->createZones( UniverseHull, pScenery ) )
		report( "Failed to create zones!" );

	// lastly, add the nouns to the universe...
	for(int k=0;k<nouns.size();++k)
	{
		Noun * pNoun = nouns[k];
		if (! pNoun )
			continue;

		TRACE( CharString().format( "UniverseContextPort::createResource() - Adding noun %s at %s",
			pNoun->name(), ConvertType<CharString>( pNoun->position() ) ) );

		if (! pContext->attachNoun( pNoun ) )
			report( CharString().format( "Failed to attach noun %s...", pNoun->name() ) );
	}

	report( CharString().format( "%d Zones, %d Nouns", pContext->zoneCount(), nouns.size() ) );

	// restore build mode..
	NounJumpGate::sm_bBuildMode = false;
	return pContext.pointer();
}

CFrameWnd * UniverseContextPort::createView()
{
	static CMultiDocTemplate docTemplate( IDR_PORT, NULL, 
		RUNTIME_CLASS(CChildFrame), RUNTIME_CLASS(CPortView) );

    CChildFrame *pFrame = dynamic_cast<CChildFrame *>( docTemplate.CreateNewFrame( this, NULL ) );
	ASSERT( pFrame );

	docTemplate.InitialUpdateFrame(pFrame, this);

	return( pFrame );
}

// Import a DSM file from the editor...
const int VERSION_110901 = 110901;

bool UniverseContextPort::importFile( const char * fileName )
{
	return false;	// return true of successful import, false on failure
}

void UniverseContextPort::shellOpen() const
{
	MessageBox( NULL, _T("shellOpen() is not valid for this port"), NULL, MB_OK);
}

int UniverseContextPort::factionCount()
{
	return FACTION_COUNT;
}

const char * UniverseContextPort::factionName( int faction )
{
	static char * FACTION_NAMES[] = 
	{
		"Unknown",			// 0
		"Neutral",			// 1
		"UGTO",				// 2
		"ICC",				// 3
		"K'Luth",			// 4
		"Human Alliance",	// 5
		"Pirate",			// 6
		"MI",				// 7
		"Teros",				// 8
	};

	return FACTION_NAMES[ faction ];
}

Tech UniverseContextPort::factionTech( int faction )
{
	switch( faction )
	{
	case FACTION_NEUTRAL:
		return TECH_NEUTRAL;
	case FACTION_UGTO:
		return TECH_UGTO;
	case FACTION_ICC:
		return TECH_ICC;
	case FACTION_KLUTH:
		return TECH_KLUTH;
	case FACTION_PIRATE:
		return TECH_PIRATE;
	case FACTION_MI:
		return TECH_MI;
	case FACTION_TEROS:
		return TECH_TEROS;
	}

	return TECH_UNKNOWN;
}

Race UniverseContextPort::factionRace( int faction )
{
	switch( faction )
	{
	case FACTION_UGTO:
	case FACTION_ICC:
	case FACTION_HUMAN_ALLIANCE:
	case FACTION_PIRATE:
	case FACTION_TEROS:
		return RACE_HUMAN;
	case FACTION_KLUTH:
		return RACE_KLUTH;
	case FACTION_MI:
		return RACE_MI;
	}

	return RACE_NONE;
}

//-------------------------------------------------------------------------------
// EOF

