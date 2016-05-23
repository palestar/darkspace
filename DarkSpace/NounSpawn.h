/*
	NounSpawn.h

	This noun type spawns a noun into the world with random parameters
	(c)2005 Palestar Inc, Richard Lyle
*/

#ifndef NOUNSPAWN_H
#define NOUNSPAWN_H

#include "NounGame.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL NounSpawn : public NounGame
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	NounSpawn();

	// Widget interface
	bool				read( const InStream & );
	// Noun interface
	NounType			nounType() const;
	bool				canBeDetected() const;

	void				simulate( dword nTick );

	// Accessors
	bool				active() const;
	float				spawnDelay() const;			// how long to wait before spawning in seconds
	float				spawnArea() const;

	Scene *				nounTemplate() const;		// the noun that is spawned
	bool				recur() const;				// will the spawn recur
	int					maxSpawn() const;			// maximum number of nouns to spawn
	bool				random() const;				// is spawning random
	int					chance() const;				// chance to spawn out of 100, 100 = spawn always, 0 = spawn never

	int					spawnCount() const;			// number of nouns spawned
	Noun *				spawn( int n ) const;		// get spawned noun n

	// Mutators
	void				setActive( bool bActive );
	void				setSpawnDelay( float fSeconds );	// how many seconds to wait before/between spawning
	void				setSpawnArea( float fArea );

	void				setNounTemplate( Scene * pTemplate );
	void				setRecur( bool bRecur, int nMaxSpawn = 8 );
	void				setRandom( bool bRandom, int nChance = 25 );

protected:
	// Data
	bool				m_bActive;					// is the spawner active or not..
	float				m_fSpawnDelay;
	float				m_fSpawnArea;
	Scene::Link			m_pScene;					// scene containing our noun template
	bool				m_bRecur;
	bool				m_bRandom;
	int					m_nMaxSpawn;
	int					m_nChance;

	Array< Noun::wRef >	m_Spawn;					// our spawn

	// non-serialized
	float				m_fSpawnTimer;

	// Helpers
	bool				canSpawnNounType( Noun * pType ) const;
};

//----------------------------------------------------------------------------

inline bool NounSpawn::active() const
{
	return m_bActive;
}

inline float NounSpawn::spawnDelay() const
{
	return m_fSpawnDelay;
}

inline float NounSpawn::spawnArea() const
{
	return m_fSpawnArea;
}

inline Scene * NounSpawn::nounTemplate() const
{
	return m_pScene;
}

inline bool NounSpawn::recur() const
{
	return m_bRecur;
}

inline int NounSpawn::maxSpawn() const
{
	return m_nMaxSpawn;
}

inline bool NounSpawn::random() const
{
	return m_bRandom;
}

inline int NounSpawn::chance() const
{
	return m_nChance;
}

inline int NounSpawn::spawnCount() const
{
	return m_Spawn.size();
}

inline Noun * NounSpawn::spawn( int n ) const
{
	return m_Spawn[ n ];
}

//----------------------------------------------------------------------------

inline void NounSpawn::setActive( bool bActive )
{
	m_bActive = bActive;
}

inline void NounSpawn::setSpawnDelay( float fSeconds )
{
	m_fSpawnDelay = fSeconds;
}

inline void NounSpawn::setSpawnArea( float fArea )
{
	m_fSpawnArea = fArea;
}

inline void NounSpawn::setNounTemplate( Scene * pTemplate )
{
	m_pScene = pTemplate;
}

inline void NounSpawn::setRecur( bool bRecur, int nMaxSpawn /*= 8*/ )
{
	m_bRecur = bRecur;
	m_nMaxSpawn = nMaxSpawn;
}

inline void NounSpawn::setRandom( bool bRandom, int nChance /*= 25*/ )
{
	m_bRandom = bRandom;
	m_nChance = Clamp( nChance, 0, 100);
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
