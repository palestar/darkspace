/*
	Port.cpp
	(c)1998 Palestar, Richard Lyle
*/

#define NOUNTRAILPORT_DLL
#include "stdafx.h"

#include "Port.h"
#include "Tools/ScenePort/ChildFrame.h"
#include "Tools/MaterialPort/Port.h"
#include "Tools/ResourcerDoc/Port.h"
#include "Tools/NounContextPort/Port.h"

#include "DarkSpace/NounTrail.h"
#include "DarkSpace/GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounTrailPort, NounPort );
REGISTER_FACTORY_KEY( NounTrailPort, 4160064291811611914 );

BEGIN_PROPERTY_LIST( NounTrailPort, NounPort )
END_PROPERTY_LIST();

NounTrailPort::NounTrailPort() : NounPort()
{
	m_Class = NounPort::m_Type = CLASS_KEY( NounTrail );
}

//----------------------------------------------------------------------------

const int VERSION_091000 = 91000;	// TODO: set the current date as the version
const int VERSION_080801 = 80801;	// TODO: set the current date as the version

bool NounTrailPort::read( const InStream &input )
{
	if (! NounPort::read( input ) )
	{
		int version;
		input >> version;

		CharString m_TrailMaterial;
		switch( version )
		{
		case VERSION_080801:
			break;
		case VERSION_091000:
			input >> m_TrailMaterial;
			break;
		default:
			throw BadWidget();
		}

		m_Class = NounPort::m_Type = CLASS_KEY( NounTrail );

		if ( version != VERSION_080801 )
		{
			if ( m_pNounContextPort != NULL )
			{
				//m_pNounContextPort->setResource( "TRAIL", m_TrailMaterial );
				m_pNounContextPort->UpdateAllViews( NULL );
			}
		}
		return false;
	}

	return true;
}

//-------------------------------------------------------------------------------

void NounTrailPort::dependencies( DependentArray & dep )
{
	NounPort::dependencies( dep );
}

CFrameWnd * NounTrailPort::createView()
{
	return( NounPort::createView() );
}

BaseNode * NounTrailPort::createNode()
{
	return( NounPort::createNode() );
}

void NounTrailPort::initializeNode( BaseNode * pNode )
{
	NounPort::initializeNode( pNode );
	if ( dynamic_cast<NounTrail *>( pNode ) == NULL )
		report( "Invalid class for NounTrailPort" );
}


//-------------------------------------------------------------------------------
// EOF

