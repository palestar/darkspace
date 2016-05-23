/*
	Port.cpp
	(c)1998 Palestar, Richard Lyle
*/

#define NOUNSTRUCTUREPORT_DLL
#include "stdafx.h"

#include "Port.h"
#include "Tools/ScenePort/ChildFrame.h"
#include "Tools/ResourcerDoc/Port.h"

#include "DarkSpace/NounStructure.h"
#include "DarkSpace/GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounStructurePort, NounPort );
REGISTER_FACTORY_KEY( NounStructurePort, 4229545680639301327 );

BEGIN_PROPERTY_LIST( NounStructurePort, NounPort )
END_PROPERTY_LIST();

NounStructurePort::NounStructurePort() : NounPort()
{
	m_Class = m_Type = CLASS_KEY( NounStructure );
}

//----------------------------------------------------------------------------

const int VERSION_031601 = 31601;
const int VERSION_080801 = 80801;

bool NounStructurePort::read( const InStream &input )
{
	if (! NounPort::read( input ) )
	{
		int version;
		input >> version;

		CharString	m_Build;
		CharString	m_Destroy;

		switch( version )
		{
		case VERSION_080801:
			break;
		case VERSION_031601:
			{
				input >> m_Build;
				input >> m_Destroy;

				if ( m_pNounContextPort != NULL )
				{
					//m_pNounContextPort->setResource( "DESTROY", m_Destroy );
					//m_pNounContextPort->setResource( "BUILD", m_Build );
					m_pNounContextPort->UpdateAllViews(NULL);
				}
			}
			break;
		}

		return false;
	}

	return true;
}

//-------------------------------------------------------------------------------

void NounStructurePort::dependencies( DependentArray & dep )
{
	NounPort::dependencies( dep );
}

CFrameWnd * NounStructurePort::createView()
{
	return( NounPort::createView() );
}

BaseNode * NounStructurePort::createNode()
{
	return( NounPort::createNode() );
}

void NounStructurePort::initializeNode( BaseNode * pNode )
{
	NounPort::initializeNode( pNode );
}


//-------------------------------------------------------------------------------
// EOF

