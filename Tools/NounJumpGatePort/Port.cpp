/*
	Port.cpp
	(c)1998 Palestar, Richard Lyle
*/

#define NOUNJUMPGATEPORT_DLL
#include "stdafx.h"

#include "Port.h"
#include "PortView.h"

#include "Tools/ScenePort/ChildFrame.h"
#include "Tools/ScenePort/Port.h"
#include "Tools/ResourcerDoc/Port.h"
#include "Tools/NounContextPort/Port.h"

#include "DarkSpace/NounJumpGate.h"
#include "DarkSpace/GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounJumpGatePort, NounBodyPort );
REGISTER_FACTORY_KEY( NounJumpGatePort, 4151568964789216494 );

BEGIN_PROPERTY_LIST( NounJumpGatePort, NounBodyPort )
	ADD_PROPERTY( m_Destination );				// TODO: Implement node selection
	ADD_PROPERTY( m_bActive );
	ADD_PROPERTY( m_bAutoActivate );
	ADD_PROPERTY( m_bSafeZone );
	ADD_PROPERTY( m_fSafeZoneDistance );
	ADD_PROPERTY( m_bDuration );
	ADD_PROPERTY( m_nDuration );
	ADD_PROPERTY( m_bInterval );
	ADD_PROPERTY( m_nInterval );
END_PROPERTY_LIST();

NounJumpGatePort::NounJumpGatePort() : NounBodyPort(), 
	m_bActive( true ), 
	m_bAutoActivate( false ), 
	m_bSafeZone( false ), 
	m_fSafeZoneDistance( 0 ),
	m_bDuration( false ),
	m_nDuration( 0 ), 
	m_bInterval( false ),
	m_nInterval( 0 )
{
	m_Class = NounPort::m_Type = CLASS_KEY( NounJumpGate );
}

//-------------------------------------------------------------------------------

void NounJumpGatePort::dependencies( DependentArray & dep )
{
	NounBodyPort::dependencies( dep );
	//dep.push( m_Context );
}

CFrameWnd * NounJumpGatePort::createView()
{
	return NounBodyPort::createView();
}

BaseNode * NounJumpGatePort::createNode()
{
	return NounBodyPort::createNode();
}

void NounJumpGatePort::initializeNode( BaseNode * pNode )
{
	NounBodyPort::initializeNode( pNode );

	NounJumpGate * pJumpGate = dynamic_cast<NounJumpGate *>( pNode );
	if ( pJumpGate != NULL )
	{
		pJumpGate->setActive( m_bActive );
		pJumpGate->setAutoActivate( m_bAutoActivate );
		pJumpGate->setSafeZone( m_bSafeZone, m_fSafeZoneDistance );
		pJumpGate->setDuration( m_bDuration, m_nDuration );
		pJumpGate->setInterval( m_bInterval, m_nInterval );
		pJumpGate->setDestination( m_Destination );
	}
	else
		report( "Invalid noun for NounJumpGatePort." );
}


//-------------------------------------------------------------------------------
// EOF

