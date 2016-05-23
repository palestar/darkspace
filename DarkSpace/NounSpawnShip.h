/*
	NounSpawnShip.h
	This Noun object spawns ships into the world... 

	* This object has one or more Spawner objects, a spawner object can have a NULL NounTemplate if that spawner is picked, then
	nothing is spawned that attempt.
	* This object will try to spawn ships until totalSpawnScore() >= maxSpawnScore()
	* 
	
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef NOUNSPAWNSHIP_H
#define NOUNSPAWNSHIP_H

#include "World/NounTemplate.h"
#include "NounShip.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL NounSpawnShip : public NounBody
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference< NounSpawnShip >				Ref;
	typedef WidgetReference< NounSpawnShip >		wRef;

	typedef NounShip::Order							Order;

	// Types
	class DLL Spawner : public Widget
	{
	public:
		DECLARE_WIDGET_CLASS();
		DECLARE_PROPERTY_LIST();

		// Types
		typedef Reference< Spawner >		Ref;

		// Construction
		Spawner();

		// Accessors
		NounTemplate *		nounTemplate() const;		// the noun that is spawned
		const char *		baseName() const;			// base name for spawned objects
		bool				isNumbered() const;			// if true, then a unique number is appended onto the ship name
		bool				isRandom() const;			// if true, then a unique random name is appended onto the base name
		int					limit() const;				// maximu number to spawn of this type, 0 if no limit
		int					chance() const;				// chance this spawn
		int					score() const;				// how many points does this spawn add towards the max spawn
		Order				order() const;				// order for the spawned ships
		const char *		orderTarget() const;		// name of the order target

		int					spawnedCount() const;		// number of ships spawned
		NounShip *			spawned( int n ) const;		// get spawed ship n
		int					spawnedScore() const;		// get the score of all spawned ships 

		// Mutators
		void				setNounTemplate( NounTemplate * a_pTemplate );
		void				setBaseName( const char * a_pBaseName );
		void				setNumbered( bool a_bNumbered );
		void				setRandom( bool a_bRandom );
		void				setLimit( int a_nLimit );
		void				setChance( int a_nChance );
		void				setScore( int a_nScore );
		void				setOrder( Order a_nOrder, const char * a_pOrderTarget );
		void				setFaction( int factionID );

		void				update();
		NounShip::Ref		spawn( WorldContext * a_pContext );		// spawn a ship using this spawner

	protected:
		// Data
		NounTemplate::Link	m_pNounTemplate;
		CharString			m_sBaseName;
		bool				m_bNumbered;
		bool				m_bRandom;
		int					m_nLimit;
		int					m_nChance;
		int					m_nScore;
		int					m_nFaction;
		Order				m_Order;
		CharString			m_sOrderTarget;

		Array< NounShip::wRef >
							m_Spawned;
		int					m_nNextNumber;
	};
	typedef Array< Spawner::Ref >		SpawnerArray;

	enum SpawnType {
		ST_NORMAL,						// spawns ships up to the maxSpawn		
		ST_POPULATION,					// as player population get's higher spawns ships up to the maxSpawn, this is useful for mob spawners
		ST_FACTION,						// as faction population of other factions increase, this spawner spawns more to make up the difference between factions in-game
		ST_ONE_SHOT						// spawns ships up to the max spawn then detaches
	};

	// Construction
	NounSpawnShip();

	// Noun interface
	virtual NounType	nounType() const;
	virtual bool		canBeDetected() const;

	virtual void		initialize();					// called before a noun is attached to a zone
	virtual void		release();
	virtual void		simulate( dword nTick );

	// Accessors
	SpawnType			spawnType() const;				// what kind of spawner is this..
	dword				spawnDelay() const;				// how long to wait before we can spawn in ticks
	float				spawnArea() const;				// radius of the spawn area 
	int					minSpawn() const;
	int					maxSpawn() const;				// maximum spawn score value for this spawner
	int					minPlanets() const;				// used by this object to send VerbRevolt() objects..
	const char *		timeMask() const;				// time mask when this spawner is active, if empty then always active

	int					spawnerCount() const;
	Spawner *			spawner( int n ) const;

	int					totalSpawnScore() const;		// get the total score of all spawned ships

	// Mutators
	void				setSpawnType( SpawnType a_nType );
	void				setSpawnDelay( dword a_nTicks );	// how many ticks to wait before spawning
	void				setSpawnArea( float a_fArea );		// radius of the spawn area
	void				setMinSpawn( int a_nMinSpawn );
	void				setMaxSpawn( int a_nMaxSpawn );
	void				setMinPlanets( int a_nMinPlanets );
	void				setTimeMask( const char * pMask );

	void				addSpawner( Spawner * a_pSpawn );
	void				removeSpawner( int n );
	void				releaseSpawners();

	//! Helpers
	static Ref			makeTeamSpawner( GameContext * a_pContext, 
							int a_nTeamId, Noun * a_pParent, bool a_bGateSpawner );

protected:
	// Data
	int					m_nSpawnType;
	dword				m_nSpawnTick;
	dword				m_nSpawnDelay;
	float				m_fSpawnArea;
	int					m_nMinSpawn;
	int					m_nMaxSpawn;
	int					m_nMinPlanets;
	CharString			m_sTimeMask;
	SpawnerArray		m_Spawners;

	qword				m_nLastTime;

	static const char * pickRandomName( int a_nFactionI, NounShip::Type a_eType );
};

//----------------------------------------------------------------------------

inline NounSpawnShip::SpawnType NounSpawnShip::spawnType() const
{
	return (SpawnType)m_nSpawnType;
}

inline dword NounSpawnShip::spawnDelay() const
{
	return m_nSpawnDelay;
}

inline float NounSpawnShip::spawnArea() const
{
	return m_fSpawnArea;
}

inline int NounSpawnShip::minSpawn() const
{
	return m_nMinSpawn;
}

inline int NounSpawnShip::maxSpawn() const
{
	return m_nMaxSpawn;
}

inline int NounSpawnShip::minPlanets() const
{
	return m_nMinPlanets;
}

inline const char * NounSpawnShip::timeMask() const
{
	return m_sTimeMask;
}

inline int NounSpawnShip::spawnerCount() const
{
	return m_Spawners.size();
}

inline NounSpawnShip::Spawner * NounSpawnShip::spawner( int n ) const
{
	return m_Spawners[ n ];
}

inline int NounSpawnShip::totalSpawnScore() const
{
	int nTotal = 0;
	for(int i=0;i<spawnerCount();++i)
		nTotal += spawner(i)->spawnedScore();
	return nTotal;
}

//----------------------------------------------------------------------------

inline void NounSpawnShip::setSpawnType( SpawnType a_nSpawnType )
{
	m_nSpawnType = a_nSpawnType;
}

inline void NounSpawnShip::setSpawnDelay( dword a_nSpawnDelayTicks )
{
	m_nSpawnDelay = a_nSpawnDelayTicks;
}

inline void NounSpawnShip::setSpawnArea( float a_fSpawnArea )
{
	m_fSpawnArea = a_fSpawnArea;
}
inline void NounSpawnShip::setMinSpawn( int a_nMinSpawn )
{
	m_nMinSpawn = a_nMinSpawn;
}

inline void NounSpawnShip::setMaxSpawn( int a_nMaxSpawn )
{
	m_nMaxSpawn = a_nMaxSpawn;
}

inline void NounSpawnShip::setMinPlanets( int a_nMinPlanets )
{
	m_nMinPlanets = a_nMinPlanets;
}

inline void NounSpawnShip::setTimeMask( const char * pMask )
{
	m_sTimeMask = pMask;
}

//---------------------------------------------------------------------------------------------------

inline NounSpawnShip::Spawner::Spawner() :
	m_bNumbered( true ),
	m_bRandom( true ),
	m_nLimit( 0 ),
	m_nChance( 1 ), 
	m_nScore( 1 ), 
	m_Order( NounShip::NOORDER ),
	m_nNextNumber( 1 )
{}

inline NounTemplate * NounSpawnShip::Spawner::nounTemplate() const
{
	return m_pNounTemplate;
}

inline const char * NounSpawnShip::Spawner::baseName() const
{
	return m_sBaseName;
}

inline bool NounSpawnShip::Spawner::isNumbered() const
{
	return m_bNumbered;
}

inline bool NounSpawnShip::Spawner::isRandom() const
{
	return m_bRandom;
}

inline int NounSpawnShip::Spawner::limit() const
{
	return m_nLimit;
}

inline int NounSpawnShip::Spawner::chance() const
{
	return m_nChance;
}

inline int NounSpawnShip::Spawner::score() const
{
	return m_nScore;
}

inline NounShip::Order NounSpawnShip::Spawner::order() const
{
	return m_Order;
}

inline const char * NounSpawnShip::Spawner::orderTarget() const
{
	return m_sOrderTarget;
}

inline int NounSpawnShip::Spawner::spawnedCount() const
{
	return m_Spawned.size();
}

inline NounShip * NounSpawnShip::Spawner::spawned( int n ) const
{
	return m_Spawned[ n ];
}

inline int NounSpawnShip::Spawner::spawnedScore() const
{
	return m_Spawned.size() * m_nScore;
}

//---------------------------------------------------------------------------------------------------

inline void NounSpawnShip::Spawner::setNounTemplate( NounTemplate * a_pTemplate )
{
	m_pNounTemplate = a_pTemplate;
}

inline void NounSpawnShip::Spawner::setBaseName( const char * a_pBaseName )
{
	m_sBaseName = a_pBaseName;
}

inline void NounSpawnShip::Spawner::setNumbered( bool a_bNumbered )
{
	m_bNumbered = a_bNumbered;
}

inline void NounSpawnShip::Spawner::setRandom( bool a_bRandom )
{
	m_bRandom = a_bRandom;
}

inline void NounSpawnShip::Spawner::setLimit( int a_nLimit )
{
	m_nLimit = a_nLimit;
}

inline void NounSpawnShip::Spawner::setChance( int a_nChance )
{
	m_nChance = a_nChance;
}

inline void NounSpawnShip::Spawner::setScore( int a_nScore )
{
	m_nScore = a_nScore;
}

inline void NounSpawnShip::Spawner::setOrder( Order a_nOrder, const char * a_pOrderTarget )
{
	m_Order = a_nOrder;
	m_sOrderTarget = a_pOrderTarget;
}

inline void NounSpawnShip::Spawner::setFaction( int factionID )
{
	m_nFaction = factionID;
}


//----------------------------------------------------------------------------

#endif

//----------------------------------------------------------------------------
//EOF
