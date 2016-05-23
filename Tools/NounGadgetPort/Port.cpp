/*
	Port.cpp
	(c)1998 Palestar, Richard Lyle
*/

#define NOUNGADGETPORT_DLL
#include "stdafx.h"

#include "Port.h"
#include "PortView.h"

#include "DarkSpace/NounGadget.h"
#include "DarkSpace/GameContext.h"
#include "Tools/NounContextPort/Port.h"
#include "Tools/ScenePort/ChildFrame.h"
#include "Tools/ResourcerDoc/Port.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounGadgetPort, NounPort );
REGISTER_FACTORY_KEY( NounGadgetPort, 4041045685045868362 );

BEGIN_PROPERTY_LIST( NounGadgetPort, NounPort )
	ADD_PROPERTY( m_nDamage );
	ADD_PROPERTY( m_bMountFore );
	ADD_PROPERTY( m_bMountRight );
	ADD_PROPERTY( m_bMountLeft );
	ADD_PROPERTY( m_bMountAft );
END_PROPERTY_LIST();

NounGadgetPort::NounGadgetPort() : NounPort()
{
	m_Class = m_Type = CLASS_KEY(NounGadget);
	m_nDamage = 0;
	m_bMountFore = true;
	m_bMountRight = m_bMountLeft = m_bMountAft = false;
}

//----------------------------------------------------------------------------

const int VERSION_080801 = 80801;
const int VERSION_20041201 = 20041201;
const int VERSION_20041222 = 20041222;

bool NounGadgetPort::read( const InStream &input )
{
	if (! NounPort::read( input ) )
	{
		int version;
		input >> version;
		
		int nLevel;
		switch( version )
		{
		case VERSION_20041222:
			{
				input >> m_nDamage;
				input >> nLevel;
				input >> m_bMountFore;
				input >> m_bMountRight;
				input >> m_bMountLeft;
				input >> m_bMountAft;
			}
			break;
		case VERSION_20041201:
			{
				input >> m_nDamage;
				input >> m_bMountFore;
				input >> m_bMountRight;
				input >> m_bMountLeft;
				input >> m_bMountAft;
			}
			break;
		case VERSION_080801:
			{
				input >> m_nDamage;
				NounGadget::Mount nMount;
				input >> nMount;

				m_bMountFore = m_bMountRight = m_bMountLeft = m_bMountAft = false;
				switch( nMount )
				{
				case NounGadget::FRONT:
					m_bMountFore = true;
					break;
				case NounGadget::RIGHT:
					m_bMountRight = true;
					break;
				case NounGadget::LEFT:
					m_bMountLeft = true;
					break;
				case NounGadget::BACK:
					m_bMountAft = true;
					break;
				case NounGadget::FULL:
					m_bMountFore = m_bMountRight = m_bMountLeft = m_bMountAft = true;
					break;
				}
			}
			break;
		default:
			throw BadWidget();
		}
		return false;
	}

	return true;
}

//-------------------------------------------------------------------------------

void NounGadgetPort::dependencies( DependentArray & dep )
{
	NounPort::dependencies( dep );
}

CFrameWnd * NounGadgetPort::createView()
{
	return NounPort::createView();
}

BaseNode * NounGadgetPort::createNode()
{
	return NounPort::createNode();
}

void NounGadgetPort::initializeNode( BaseNode * pNode )
{
	NounPort::initializeNode( pNode );

	NounGadget * pGadget = dynamic_cast< NounGadget * >( pNode );
	if ( pGadget != NULL )
	{
		pGadget->setDamage( m_nDamage );
		pGadget->clearFlags( NounGadget::FLAG_MOUNT_FULL );
		if ( m_bMountFore )
			pGadget->addFlags( NounGadget::FLAG_MOUNT_FORE );
		if ( m_bMountRight )
			pGadget->addFlags( NounGadget::FLAG_MOUNT_RIGHT );
		if ( m_bMountLeft )
			pGadget->addFlags( NounGadget::FLAG_MOUNT_LEFT );
		if ( m_bMountAft )
			pGadget->addFlags( NounGadget::FLAG_MOUNT_AFT );
	}
}


//-------------------------------------------------------------------------------
// EOF

