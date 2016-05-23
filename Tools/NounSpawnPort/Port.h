/*
	Port.h

	ResourcePort object
	(c)1998 Palestar, Richard Lyle
*/

#ifndef NOUNSPAWNPORT_H
#define NOUNSPAWNPORT_H

#include "Standard/Dll.h"
#include "Tools/NounBodyPort/Port.h"
#include "DarkSpace/NounShip.h"

//-------------------------------------------------------------------------------

#undef DLL
#ifdef NOUNSPAWNPORT_DLL
#define DLL			DLL_EXPORT
#else
#define DLL			DLL_IMPORT
#endif

//----------------------------------------------------------------------------

class DLL NounSpawnPort : public NounBodyPort
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference<NounSpawnPort>	Ref;

	class DLL Spawner : public Widget
	{
	public:
		DECLARE_WIDGET_CLASS();
		DECLARE_PROPERTY_LIST();

		typedef Reference< Spawner >		Ref;

		Spawner() : 
			m_bNumbered( true ), 
			m_bRandom( false ),
			m_nLimit( 0 ), 
			m_nChance( 1 ), 
			m_nScore( 1 ),
			m_nOrder( NounShip::NOORDER )
		{}

		CharString					m_sNounTemplate;
		CharString					m_sBaseName;
		bool						m_bNumbered;
		bool						m_bRandom;
		int							m_nLimit;
		int							m_nChance;
		int							m_nScore;
		int							m_nOrder;
		CharString					m_sOrderTarget;
	};
	typedef Array< Spawner >		SpawnerArray;

	// Construction
	NounSpawnPort();

	// Widget Interface
	bool						read( const InStream & );
	// BaseNodePort Interface
	void						dependencies( DependentArray & dep );
	CFrameWnd *					createView();
	BaseNode *					createNode();
	void						initializeNode( BaseNode * pNode );

	int							m_nSpawnType;
	int							m_SpawnDelay;
	float						m_SpawnArea;
	int							m_MinSpawn;
	int							m_MaxSpawn;
	int							m_MinPlanets;
	CharString					m_sTimeMask;
	SpawnerArray				m_Spawners;

	// depreciated
	CharString					m_ShipContext;
};


#endif

//-------------------------------------------------------------------------------
// EOF
