/*
	Port.cpp
	(c)1998 Palestar, Richard Lyle
*/

#define NOUNPORT_DLL
#include "stdafx.h"

#include "Port.h"
#include "PortView.h"

#include "File/FileDisk.h"
#include "World/Noun.h"
#include "Tools/NounContextPort/Port.h"
#include "Tools/ShipContextPort/Port.h"
#include "Tools/ScenePort/Port.h"
#include "Tools/ResourcerDoc/Port.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounPort, NodePort );
REGISTER_FACTORY_KEY( NounPort, 4040222163408358606 );

BEGIN_PROPERTY_LIST( NounPort, NodePort )
	ADD_PORT_PROPERTY( m_NounContext, NounContextPort );
END_PROPERTY_LIST();

NounPort::NounPort() : NodePort(), m_pNounContextPort( NULL )
{
	m_Type = m_Class = CLASS_KEY( Noun );
}

//----------------------------------------------------------------------------

const int VERSION_080801 = 80801;
const int VERSION_080901 = 80901;
const int VERSION_081101 = 81101;
const int VERSION_20040605 = 20040605;
const int VERSION_102399 = 102399;

bool NounPort::read( const InStream &input )
{
	if (! NodePort::read( input ) )
	{
		int version;
		input >> version;

		CharString		m_Scene;
		ClassKey		m_NounContextType;		

		switch( version )
		{
		case VERSION_20040605:
			input >> m_NounContext;
			break;
		case VERSION_081101:
			input >> m_NounContext;
			input >> m_Class;
			input >> m_Type;
			break;
		case VERSION_080901:
			{
				input >> m_NounContext;

				// extract class / type from NounContextPort
				NounContextPort * pPort = WidgetCast<NounContextPort>( Port::portLoad( m_NounContext ) );
				if ( pPort != NULL )
				{
					m_Class = pPort->m_Class;
					m_Type = pPort->m_Type;
				}
				else
				{
					report( "Failed to load NounContextPort" );
					m_Type = m_Class = CLASS_KEY( Noun );
				}
			}
			break;
		case VERSION_080801:
			input >> m_NounContext;
			input >> m_NounContextType;
			input >> m_Class;
			input >> m_Type;
			break;
		case VERSION_102399:
			input >> m_Class;
			input >> m_Type;
			input >> m_NounContext;
			break;
		}

		return false;
	}
	return true;
}

//-------------------------------------------------------------------------------

void NounPort::dependencies( DependentArray & dep )
{
	dep.push( m_NounContext );
}

CFrameWnd * NounPort::createView()
{
	return NodePort::createView();
}

BaseNode * NounPort::createNode()
{
	return NodePort::createNode();
}

void NounPort::initializeNode( BaseNode * pNode )
{
	NodePort::initializeNode( pNode );

	if ( WidgetCast<Noun>( pNode ) )
		((Noun *)pNode)->setNounContext( Port::portResource( m_NounContext ) );
}


//-------------------------------------------------------------------------------
// EOF

