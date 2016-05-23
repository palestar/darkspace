/*
	StructureShipyard.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTURESHIPYARD_H
#define STRUCTURESHIPYARD_H

#include "NounSpawnShip.h"
#include "NounStructure.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructureShipyard : public NounStructure
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef WidgetReference< StructureShipyard >	wRef;

	struct Build 
	{
		Build() : m_nUserId( 0 ), m_nCost( 0 )
		{}
		Build( dword nUserId, NounShip * pShip, int nCost ) : 
			m_nUserId( nUserId ), m_pShip( pShip ), m_nCost( nCost ), m_nResources( 0 )
		{}

		dword			m_nUserId;
		NounShip::Ref	m_pShip;
		int				m_nCost;
		int				m_nResources;
	};
	typedef std::list< Build >		BuildQueue;

	// Construction
	StructureShipyard();

	//! Noun interface
	void				simulate( dword nTick );

	//! NounStructure interface
	virtual int			sortId() const;
	virtual int			groupId() const;
	int					maxDamage() const;					// how much damage can this structure take
	int					buildTechnology() const;			// technology level required to build
	int					buildTime() const;					// time in seconds to build this structure
	int					buildCost() const;					// cost to build this structure
	int					repairRate() const;					// damaged repaired per second

	Color				color() const;
	
	dword				planetFlags() const;
	int					workers() const;
	int					power() const;
	int					technology() const;

	bool				usable( NounShip * pShip ) const;
	CharString			useTip( NounShip * pShip ) const;
	bool				useConfirm( NounShip * pShip ) const;			// should the use of this structure be confirmed?
	CharString			useConfirmMessage( NounShip * pShip ) const;	// use confirmation message
	void				use( dword nWhen, NounShip * pShip );

	//! Accessors
	int					buildCount() const;
	//! Mutators
	bool				addBuild( dword nUserId, NounShip::Ref pShip, int nCost );
	bool				cancelBuild( dword nUserId );

	//! Static
	static StructureShipyard *
						findYard( NounPlanet * pPlanet );

protected:
	//! Data
	dword				m_nBuildTick;
	BuildQueue			m_BuildQueue;

	// non-serialized
	NounSpawnShip::Ref	m_pSpawner;			// AI ship spawner for this yard
};

//----------------------------------------------------------------------------

inline int StructureShipyard::buildCount() const
{
	return m_BuildQueue.size();
}


#endif

//----------------------------------------------------------------------------
//EOF

