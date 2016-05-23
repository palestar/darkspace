/*
	Port.cpp
	(c)1998 Palestar, Richard Lyle
*/

#define NOUNBODYPORT_DLL
#include "stdafx.h"

#include "Port.h"
#include "PortView.h"

#include "Tools/NounContextPort/Port.h"
#include "Tools/ScenePort/ChildFrame.h"
#include "Tools/ScenePort/Port.h"

#include "DarkSpace/TraitOrbit.h"
//#include "DarkSpace/GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounBodyPort, NounPort );
REGISTER_FACTORY_KEY( NounBodyPort, 4040357502270931158 );

BEGIN_PROPERTY_LIST( NounBodyPort, NounPort )
	ADD_PROPERTY( m_nFleetId );
	ADD_PROPERTY( m_Orbiting );
	ADD_PROPERTY( m_OrbitVelocity );
END_PROPERTY_LIST();

NounBodyPort::NounBodyPort() : NounPort()
{
	m_Type = m_Class = CLASS_KEY( NounBody );
	m_nFleetId = -1;
	m_OrbitVelocity = 0.0f;
}

//----------------------------------------------------------------------------

const int VERSION_102399 = 102399;
const int VERSION_022200 = 22200;
const int VERSION_080100 = 80100;
const int VERSION_080801 = 80801;

bool NounBodyPort::read( const InStream &input )
{
	if (! NounPort::read( input ) )
	{
		int version;
		input >> version;

		WidgetKey key;

		int m_Type;		// obsolete
		float m_Mass;

		switch( version )
		{
		case VERSION_080801:
			input >> m_nFleetId;
			input >> m_Orbiting;
			input >> m_OrbitVelocity;
			break;
		case VERSION_080100:
			input >> m_Mass;
			input >> m_nFleetId;
			input >> m_Orbiting;
			input >> m_OrbitVelocity;
			break;
		case VERSION_022200:
			input >> m_Type;
			input >> m_Mass;
			input >> m_nFleetId;
			input >> m_Orbiting;
			input >> m_OrbitVelocity;
			break;
		case VERSION_102399:
			input >> m_Type;
			input >> m_Mass;
			input >> m_nFleetId;
			m_OrbitVelocity = 0.0f;
			break;
		default:
			input.file()->setPosition( input.file()->position() - sizeof(int) );
			m_Mass = 1.0f;
			m_nFleetId = -1;
			m_OrbitVelocity = 0.0f;
			break;
		}

		return false;
	}

	return true;
}

//-------------------------------------------------------------------------------

void NounBodyPort::dependencies( DependentArray & dep )
{
	NounPort::dependencies( dep );
}

CFrameWnd * NounBodyPort::createView()
{
	return NounPort::createView();
}

BaseNode * NounBodyPort::createNode()
{
	return NounPort::createNode();
}

void NounBodyPort::initializeNode( BaseNode * pNode )
{
	NounPort::initializeNode( pNode );

	NounBody * pBody = dynamic_cast< NounBody * > ( pNode );
	if ( pBody != NULL )
	{
		pBody->setTeamId( m_nFleetId + 1 );

		if ( m_Orbiting.length() > 0 )
		{
			ScenePort * pDoc = dynamic_cast<ScenePort *>( parent() );
			ASSERT( pDoc );

			Noun * pOrbit = WidgetCast<Noun>( pDoc->findCachedNode( m_Orbiting ) );
			if ( pOrbit == NULL )
			{
				report( CharString().format("Node %s failed to find orbit node %s...", 
					name(), m_Orbiting.cstr()) );
				return;
			}

			// enter orbit around dummy object, which will connect to the correct object upon loading
			Vector3 vDelta( pBody->worldPosition() - pOrbit->worldPosition() );
			float fDistance = vDelta.magnitude();
			if ( fDistance > 0.0f )
			{
				vDelta.normalize();
				float fPosition = atan2( vDelta.x, vDelta.z );

				// orbit velocity is in gu/s, convert into r/s
				float fRadiansPerSecond = 0.0f;
				if ( fabs( m_OrbitVelocity ) > 0.0f )		// prevent a divide by zero
					fRadiansPerSecond = m_OrbitVelocity / fDistance;

				GetTrait<TraitOrbit>( pBody )->setOrbit( pOrbit, fDistance, fPosition, fRadiansPerSecond, 0 );
			}
		}
	}
	else
		report( "Node not NounBody" );
}


//-------------------------------------------------------------------------------
// EOF

