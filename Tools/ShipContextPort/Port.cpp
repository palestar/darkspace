/*
	Port.cpp
	(c)1998 Palestar, Richard Lyle
*/

#define SHIPCONTEXTPORT_DLL
#include "stdafx.h"

#include "Port.h"
#include "PortView.h"
#include "ChildFrame.h"

#include "Resource.h"
#include "Tools/CollectionPort/Port.h"
#include "Tools/ScenePort/Port.h"
#include "Tools/NounShipPort/Port.h"
#include "Tools/NounGadgetPort/Port.h"
#include "World/NounTemplate.h"
//#include "DarkSpace/NounShip.h"
//#include "DarkSpace/GameContext.h"

#include "Tools/ResourcerDoc/SelectResource.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( ShipContextPort, Port );
REGISTER_FACTORY_KEY( ShipContextPort, 4040641423645276594 );

BEGIN_PROPERTY_LIST( ShipContextPort, Port )
	ADD_PROPERTY( m_Badges );
	ADD_PROPERTY( m_Rank );
	ADD_PROPERTY( m_Ship );
END_PROPERTY_LIST();

ShipContextPort::ShipContextPort() : Port()
{
	m_Badges = 0;
	m_Rank = 0;
}

//----------------------------------------------------------------------------

const int VERSION_080801 = 80801;
const int VERSION_022403 = 22403;

bool ShipContextPort::read( const InStream &input )
{
	if (! Port::read( input ) )
	{
		int version;
		input >> version;

		switch( version )
		{
		case VERSION_022403:
			{
				input >> m_Badges;
				input >> m_Rank;
				input >> m_Ship;
			}
			break;
		case VERSION_080801:
			{
				CharString gadgets;

				input >> m_Badges;
				input >> m_Rank;
				input >> m_Ship;
				input >> gadgets;
			}
			break;
		default:
			throw BadWidget();
		}
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------

bool ShipContextPort::autoLoad()
{
	return false;
}

void ShipContextPort::dependencies( DependentArray & dep )
{
	Port::dependencies( dep );
	dep.push( m_Ship );
}

Resource::Ref ShipContextPort::createResource()
{
	Scene::Link pScene = portResource( m_Ship );

	if ( pScene.valid() )
	{
		BaseNode * pRoot = pScene->node();
		if ( pRoot != NULL )
		{
			// verify all nodes in the scene are nouns!
			std::list< BaseNode * > nodes;
			for( int i=0;i<pRoot->childCount();++i)
				nodes.push_back( pRoot->child(i) );

			while( nodes.size() > 0 )
			{
				BaseNode * pNode = nodes.front();
				nodes.pop_front();

				if ( pNode == NULL )
				{
					report( "NULL node in the scene!" );
					continue;
				}
				else if ( WidgetCast<Noun>( pNode ) == NULL )
				{
					report( CharString().format( "Node %s is not a Noun type!", pNode->name() ) );
				}

				for(int i=0;i<pNode->childCount();++i)
					nodes.push_back( pNode->child(i) );
			}
		}
	}

	NounTemplate * pTemplate = new NounTemplate();
	pTemplate->setFlags( m_Badges );
	pTemplate->setRank( m_Rank );
	pTemplate->setScene( Scene::Link( portResource( m_Ship ) ) );

	return pTemplate;
}

CFrameWnd * ShipContextPort::createView()
{
	static CMultiDocTemplate docTemplate( IDR_PORT, NULL, 
		RUNTIME_CLASS(CChildFrame), RUNTIME_CLASS(CPortView) );

    CChildFrame *pFrame = dynamic_cast<CChildFrame *>( docTemplate.CreateNewFrame( this, NULL ) );
	ASSERT( pFrame );

	docTemplate.InitialUpdateFrame(pFrame, this);

	return( pFrame );
}

bool ShipContextPort::importFile( const char * fileName )
{
	return false;	// return true of successful import, false on failure
}

void ShipContextPort::shellOpen() const
{
	MessageBox( NULL, _T("shellOpen() is not valid for this port"), NULL, MB_OK);
}

//-------------------------------------------------------------------------------
// EOF

