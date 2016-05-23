/*
	Port.cpp
	(c)1998 Palestar, Richard Lyle
*/

#define NOUNSHIPPORT_DLL
#include "stdafx.h"

#include "Port.h"

#include "DarkSpace/NounShip.h"
#include "DarkSpace/GameContext.h"
#include "Tools/ShipContextPort/Port.h"
#include "Tools/ScenePort/ChildFrame.h"
#include "Tools/ResourcerDoc/Port.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounShipPort, NounBodyPort );
REGISTER_FACTORY_KEY( NounShipPort, 4040224354651462474 );

BEGIN_PROPERTY_LIST( NounShipPort, NounBodyPort )
END_PROPERTY_LIST();

NounShipPort::NounShipPort() : NounBodyPort()
{
	m_Class = m_Type = CLASS_KEY( NounShip );
}

//----------------------------------------------------------------------------

const int VERSION_080701 = 80701;

bool NounShipPort::read( const InStream &input )
{
	if (! NounBodyPort::read( input ) )
	{
		int version;
		input >> version;

		switch( version )
		{
		case VERSION_080701:
			break;
		}
		return false;
	}

	return true;
}

//-------------------------------------------------------------------------------

void NounShipPort::dependencies( DependentArray & dep )
{
	NounBodyPort::dependencies( dep );
}

CFrameWnd * NounShipPort::createView()
{
	return( NounBodyPort::createView() );
}

BaseNode * NounShipPort::createNode()
{
	return NounBodyPort::createNode();
}

void NounShipPort::initializeNode( BaseNode * pNode )
{
	NounBodyPort::initializeNode( pNode );

	NounShip * pShip = WidgetCast<NounShip>( pNode );
	if ( pShip != NULL )
	{
		// make the ship visible in resourcer..
		pShip->setSyncronized( true );
	}
}


//-------------------------------------------------------------------------------
// EOF

