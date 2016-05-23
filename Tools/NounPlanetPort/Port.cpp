/*
	Port.cpp
	(c)1998 Palestar, Richard Lyle
*/

#define NOUNPLANETPORT_DLL
#include "stdafx.h"

#include "Port.h"
#include "PortView.h"

#include "Tools/ScenePort/ChildFrame.h"
#include "DarkSpace/GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounPlanetPort, NounBodyPort );
REGISTER_FACTORY_KEY( NounPlanetPort, 4083318528338125012 );

BEGIN_PROPERTY_LIST( NounPlanetPort, NounBodyPort )
	ADD_PROPERTY( m_Population );
	ADD_PROPERTY( m_bHydrogen );
	ADD_PROPERTY( m_bOxygen );
	ADD_PROPERTY( m_bMetals );
	ADD_PROPERTY( m_bHeavyMetals );
	ADD_PROPERTY( m_bCyroMetals );
	ADD_PROPERTY( m_bHyperMatter );
	ADD_PROPERTY( m_bDarkMatter );
	ADD_PROPERTY( m_bUrdaniam );
END_PROPERTY_LIST();

NounPlanetPort::NounPlanetPort() : m_Population( 1 ),
	m_bHydrogen(false),
	m_bOxygen(false),
	m_bMetals(false),
	m_bHeavyMetals(false),
	m_bCyroMetals(false),
	m_bHyperMatter(false),
	m_bDarkMatter(false),
	m_bUrdaniam(false)
{
	m_Class = NounPort::m_Type = CLASS_KEY( NounPlanet );
}

//----------------------------------------------------------------------------

const int VERSION_021600 = 021600;
const int VERSION_031401 = 31401;
const int VERSION_20040605 = 20040605;

bool NounPlanetPort::read( const InStream &input )
{
	if (! NounBodyPort::read( input ) )
	{
		int version;
		input >> version;

		switch( version )
		{
		case VERSION_20040605:
			{
				int nCredits;
				input >> m_Population;
				input >> nCredits;
				input >> m_bHydrogen;
				input >> m_bOxygen;
				input >> m_bMetals;
				input >> m_bHeavyMetals;
				input >> m_bCyroMetals;
				input >> m_bHyperMatter;
				input >> m_bDarkMatter;
				input >> m_bUrdaniam;
			}
			break;
		case VERSION_031401:
			{
				int nCredits;
				dword nFlags;
				input >> m_Population;
				input >> nCredits;
				input >> nFlags;

				m_bHydrogen = (nFlags & NounPlanet::FLAG_HYDROGEN) ? true : false;
				m_bOxygen = (nFlags & NounPlanet::FLAG_OXYGEN) ? true : false;
				m_bMetals = (nFlags & NounPlanet::FLAG_METALS) ? true : false;
				m_bHeavyMetals = (nFlags & NounPlanet::FLAG_HEAVY_METALS) ? true : false;
				m_bCyroMetals = (nFlags & NounPlanet::FLAG_CYRO_METALS) ? true : false;
				m_bHyperMatter = (nFlags & NounPlanet::FLAG_HYPER_MATTER) ? true : false;
				m_bDarkMatter = (nFlags & NounPlanet::FLAG_DARK_MATTER) ? true : false;
				m_bUrdaniam = (nFlags & NounPlanet::FLAG_URDANIAM) ? true : false;
			}
			break;
		case VERSION_021600:
			{
				dword nFlags;
				input >> m_Population;
				input >> nFlags;

				m_bHydrogen = (nFlags & NounPlanet::FLAG_HYDROGEN) ? true : false;
				m_bOxygen = (nFlags & NounPlanet::FLAG_OXYGEN) ? true : false;
				m_bMetals = (nFlags & NounPlanet::FLAG_METALS) ? true : false;
				m_bHeavyMetals = (nFlags & NounPlanet::FLAG_HEAVY_METALS) ? true : false;
				m_bCyroMetals = (nFlags & NounPlanet::FLAG_CYRO_METALS) ? true : false;
				m_bHyperMatter = (nFlags & NounPlanet::FLAG_HYPER_MATTER) ? true : false;
				m_bDarkMatter = (nFlags & NounPlanet::FLAG_DARK_MATTER) ? true : false;
				m_bUrdaniam = (nFlags & NounPlanet::FLAG_URDANIAM) ? true : false;
			}
			break;
		}
		return false;
	}

	return true;
}

//-------------------------------------------------------------------------------

void NounPlanetPort::dependencies( DependentArray & dep )
{
	NounBodyPort::dependencies( dep );
}

CFrameWnd * NounPlanetPort::createView()
{
	return NounBodyPort::createView();
}

BaseNode * NounPlanetPort::createNode()
{
	return NounBodyPort::createNode();
}

void NounPlanetPort::initializeNode( BaseNode * pNode )
{
	NounBodyPort::initializeNode( pNode );

	NounPlanet * pPlanet = dynamic_cast<NounPlanet *>( pNode );
	if ( pPlanet != NULL )
	{
		pPlanet->setPopulation( m_Population );

		dword nFlags = 0;
		nFlags |= m_bHydrogen ? NounPlanet::FLAG_HYDROGEN : 0;
		nFlags |= m_bOxygen ? NounPlanet::FLAG_OXYGEN : 0;
		nFlags |= m_bMetals ? NounPlanet::FLAG_METALS : 0;
		nFlags |= m_bHeavyMetals ? NounPlanet::FLAG_HEAVY_METALS : 0;
		nFlags |= m_bCyroMetals ? NounPlanet::FLAG_CYRO_METALS : 0;
		nFlags |= m_bHyperMatter ? NounPlanet::FLAG_HYPER_MATTER : 0;
		nFlags |= m_bDarkMatter ? NounPlanet::FLAG_DARK_MATTER : 0;
		nFlags |= m_bUrdaniam ? NounPlanet::FLAG_URDANIAM : 0;

		pPlanet->setFlags( nFlags );
	}
	else
		report( "Invalid noun for NounPlanetPort" );
}


//-------------------------------------------------------------------------------
// EOF

