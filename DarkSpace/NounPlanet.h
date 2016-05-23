/*
	NounPlanet.h
	(c)1999 PaleStar, Richard Lyle
*/

#ifndef NOUN_PLANET_H
#define NOUN_PLANET_H

#include "Render3d/NodeComplexMesh2.h"
#include "NounBody.h"
#include "CargoResource.h"
#include "NounStructure.h"
#include "PathMapPlanet.h"
#include "Constants.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL NounPlanet : public NounBody
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference< NounPlanet >				Ref;
	typedef WidgetReference< NounPlanet >		wRef;
 
	enum Flags {

		FLAG_HYDROGEN		= 0x00000001,
		FLAG_OXYGEN			= 0x00000002,
		FLAG_METALS			= 0x00000004,
		FLAG_HEAVY_METALS	= 0x00000008,
		FLAG_CYRO_METALS	= 0x00000010,
		FLAG_HYPER_MATTER	= 0x00000020,
		FLAG_DARK_MATTER	= 0x00000040,
		FLAG_URDANIAM		= 0x00000080,

		FLAG_PLAGUE			= 0x00000100,		// plague
		FLAG_REVOLUTION		= 0x00000200,		// revolution
		FLAG_RESESSION		= 0x00000400,		// enconomic resession
		FLAG_STRIKE			= 0x00000800,		// worker strike
		FLAG_BOOM			= 0x00001000,		// enconomic boom, incresed production
		FLAG_SMUGGLERS		= 0x00002000,		// smugglers, lost resources
		FLAG_MINING_FIND	= 0x00004000,		// mining find, increased resources

		FLAG_FOOD_SHORT		= 0x00008000,		// food shortage
		FLAG_POWER_SHORT	= 0x00010000,		// power shortage
		FLAG_WORKER_SHORT	= 0x00020000,		// worker shortage
		FLAG_TECH_SHORT		= 0x00040000,		// technology shortage
		FLAG_HAS_DEPOT		= 0x00080000,		// depot on planet
		FLAG_HAS_PORT		= 0x00100000,		// starport on planet
		FLAG_HAS_SHIPYARD	= 0x00200000,		// shipyard on planet
		FLAG_HAS_HUB		= 0x00400000,		// colony hub on planet
		FLAG_BLOCKADE		= 0x00800000,		// planet is current under blockade
		FLAG_ENEMY			= 0X01000000,		// enemy detected
		FLAG_NOT_CAPTURABLE	= 0x02000000,		// is planet capturable

		FLAG_STRUCTURES		= FLAG_HAS_DEPOT | FLAG_HAS_PORT | FLAG_HAS_SHIPYARD | FLAG_HAS_HUB,
		STRUCTURE_MASK		= ~FLAG_STRUCTURES,
	};
	enum SceneBits {
		HEX_GRID			= 0x00000001,		// render hex grid overlay
	};

	typedef PathMapPlanet::Node				Hex;
	typedef PathMapPlanet::Path				Path;

	// Construction
	NounPlanet();

	// Widget interface
	virtual bool			read( const InStream & input );

	//! BaseNode interface
	virtual void			render( RenderContext &context, 
								const Matrix33 & frame, 
								const Vector3 & position );

	// Noun interface
	virtual NounType		nounType() const;
	virtual bool			collidable() const;
	virtual bool			canOrbit() const;
	virtual bool			canAttachCargo( Noun * pCargo ) const;
	virtual float			baseSignature() const;

	virtual void			setup();
	virtual void			initialize();					// initialize the noun
	virtual bool			postInitialize();
	virtual void			release();
	virtual void			simulate( dword nTick );

	virtual void			collision( Noun * pCollide,
								 const Vector3 & intersect );
	
	virtual bool			canDamage( dword type ) const;	
	virtual void			inflictDamage( dword nWhen, Noun * pFrom, int damage, 
								dword type, const Vector3 & direction );

	// NounGame interface
	virtual CharString		status() const;
	virtual int				resourceCount() const;							// returns the total number of resources available...
    virtual bool			canShipCollide() const;
	virtual int				useResources( int nWanted );

	// Interface
	virtual const char *	description() const;							// planet type description
	virtual int				baseValue() const;								// what is the base value of this planet
	virtual int				baseHabitat() const;							// what amount of habitat is available
	virtual int				baseFood() const;								// naturally occuring food available
	virtual int				maxUnits() const;								// maximum units allowed
	virtual int				maxStructures() const;							// maximum structures allowed
	virtual int				maxResources() const;							// maximum number of resources
	virtual float			maxControl() const;								// maximum control points
	virtual int				maxAllegiance() const;							// maximum allegiance level

	virtual float			sensor() const;									// planet sensor detection amount
	virtual float			view() const;									// planet view detection amount

	// Accessors
	float					rotVelocity() const;							// rotational velocity of planet in r/s
	float					population() const;								// population of this planet
	float					technology() const;								// current technology level
	float					resources() const;								// available planet resources
	float					control() const;								// current control points
	float					resourceDemand() const;							// demand for resource, CargoResource uses this to calculate value
	int						allegiance() const;								// current allegiance level
	int						allegianceXP() const;							// current allegiance xp
	int						allegianceXPRequired() const;					// current amount of xp required to level planet allegiance

	PathMap *				pathMap() const;								// get the pathmap object for this planet
	bool					isHexValid( int n ) const;						// check if hex is valid
	int						hexCount() const;								// total number of hexes on this planet
	Hex						hex( int n ) const;								// get hex n

	float					moral() const;									// population moral
	int						lockRank() const;								// current lock rank

	int						tradeRoutes() const;							// number of trade routes
	NounPlanet *			tradeRoute( int n ) const;						// access trade route n
	bool					importStatus() const;
	bool					exportStatus() const;

	int						contactCount() const;							// current planet contacts
	Noun *					contact( int n ) const;

	// Mutators
	void					setRotVelocity( float rv );
	void					setPopulation( float fPopulation );
	void					setTechnology( float fTechnology );
	void					setResources( float fResources );
	float					addResources( float fProduced );
	void					setControl( float fControl );
	void					setAllegiance( int nAllegiance );
	void					setAllegianceXP( int nAllegianceXP );

	void					setMoral( float moral );
	void					setLockRank( int rank );

	void					addTradeRoute( NounPlanet * pPlanet );			// add new trade route
	void					removeTradeRoute( int n );						// remove trade route
	void					removeTradeRoute( NounPlanet * pPlanet );
	void					setImport( bool a_bImport );
	void					setExport( bool a_bExport );

	float					killPopulation( float fKill );

	// Helpers
	int						value() const;									// calculate the total value of the planet
	int						workersAvailable() const;						// how many workers are available						
	int						workersNeeded() const;							// how many workers are required for all structures
	int						power() const;									// what is the total power value
	int						powerAvailable() const;							// how much power is generated
	int						powerUsed() const;								// how much power is needed
	int						powerNeeded() const;							// how much power is needed
	int						food() const;									// how much food is being generated
	int						foodNeeded() const;								// how much food is needed
	int						habitat() const;								// space for population
	int						mining() const;									// total mining production
	int						research() const;								// total research production
	int						production() const;								// total production
	bool					isCappable() const;								// can this planet be captured

	bool					isPowerShortage() const;
	bool					isFoodShortage() const;
	bool					isWorkerShortage() const;
	bool					isTechShortage() const;

	bool					canGovern( Noun * pWho ) const;
	bool					canBuild( Noun * pWho, bool bUpgrade ) const;
	bool					canDetect( Noun * pNoun ) const;				// can this planet detect this object
	bool					canPauseStructure(NounStructure *);				// can this structure be paused

	CharString				resourceNames() const;							// get the resource names
	
	int						unitCount() const;								// number of units on this planet
	int						friendlyUnitCount() const;						// number of friendly units on this planet
	int						enemyUnitCount() const;							// number of friendly units on this planet
	int						structureCount() const;							// number of structures on this planet
	bool					isBuildTarget() const;							// returns true if we have space for structures or damaged structures
	int						factoryCount() const;							// number of factories on this planet

	int						findHex( const Vector3 & vNear, bool bStructure );

	static const char *		flagName( dword nFlags );						// get the text names for the provided flag

protected:
	// Data
	float					m_fPopulation;				// planet population
	float					m_fTechnology;				// planet technology level
	float					m_fMoral;					// moral of the population, 0 - 1
	int						m_nLockRank;				// rank required to modify the planet
	Array< wRef >			m_TradeRoutes;				// current supply routes
	float					m_fResources;				// current planetary 
	float					m_fControl;					// current capture points
	float					m_fRotVelocity;				// in r/s how fast does this planet rotate on it's axis
	int						m_nAllegiance;				// current allegiance level of planet
	int						m_nAllegianceXP;			// current allegiance experience of planet
	int						m_nAllegianceXPRequired;	// amount of allegiance required to level
	bool					m_bImportStatus;			// is planet allowed to import
	bool					m_bExportStatus;			// is planet allowed to export

	// non-serialized
	float					m_fResourceDemand;			// demand for resources, affects the price
	BaseNode::Ref			m_pTerrain;					// the model for our planet
	PathMap::Ref			m_pMap;						// our planet map

	dword					m_nLastDistress;			// last distress message sent
	dword					m_nLastEvent;				// last event

	dword					m_nControlTick;
	Array< Noun::wRef >		m_Contacts;					// planet contacts
	Array< Noun::wRef >		m_Cappers;					// ships currently within capture range of this planet
	Array< Noun::wRef >		m_Attackers;				// ships currently in attack range of planet
	dword					m_nProductionTick;
	dword					m_nEconomyTick;
	dword					m_nDistressTick;
	dword					m_nAllegianceTick;

	dword					m_nDefenseTick;
	bool					m_bDefenseEnabled;			// set to true if we have any defense bases
	NounContext::Link		m_pDefenseProjectile;		// used by updateFauxDefense()..
	int						m_nLastDefenseTarget;		// last contact fired at, we attack contacts in round robin

	Array< CharString >		m_StatusLines;				// lines of status text about this planet

	// Mutators
	bool					findTerrain( BaseNode * pBaseNode, const Matrix33 & mFrame, const Vector3 & vPosition );

	void					updateDefense( dword nTick );
	void					updateControl( dword nTick );
	void					updateProduction( dword nTick );
	void					updateEconomy( dword nTick );
	void					updateAllegiance( dword nTick );

	void					clearCappers();
	void					updateStatusLines();

	// Depreciated
public:
	struct TradeRoute
	{
		wRef				planet;			// trade route with whom
		ResourceType		type;			// type of resource to export

		TradeRoute & operator=( const TradeRoute & copy );
	};
};

//----------------------------------------------------------------------------

inline float NounPlanet::rotVelocity() const
{
	return m_fRotVelocity;
}

inline float NounPlanet::population() const
{
	return m_fPopulation;
}

inline float NounPlanet::technology() const
{
	return m_fTechnology;
}

inline float NounPlanet::resources() const
{
	return m_fResources;
}

inline int NounPlanet::allegiance() const
{
	return m_nAllegiance;
}

inline int NounPlanet::allegianceXP() const
{
	return m_nAllegianceXP;
}

inline int NounPlanet::allegianceXPRequired() const
{
	return m_nAllegianceXPRequired;
}

inline float NounPlanet::control() const
{
	return m_fControl;
}

inline float NounPlanet::resourceDemand() const
{
	return m_fResourceDemand;
}

inline PathMap * NounPlanet::pathMap() const
{
	return m_pMap;
}

inline bool NounPlanet::isHexValid( int n ) const
{
	return m_pMap.valid() ? m_pMap->isNodeValid( n ) : false;
}

inline int NounPlanet::hexCount() const
{
	return m_pMap.valid() ? m_pMap->nodeCount() : 0;
}

inline NounPlanet::Hex NounPlanet::hex( int n ) const
{
	return m_pMap->node( n );
}

inline float NounPlanet::moral() const
{
	return m_fMoral;
}

inline int NounPlanet::lockRank() const
{
	return m_nLockRank;
}

inline int NounPlanet::tradeRoutes() const
{
	return m_TradeRoutes.size();
}

inline NounPlanet * NounPlanet::tradeRoute( int n ) const
{
	return m_TradeRoutes[ n ];
}

inline bool NounPlanet::importStatus() const
{
	return m_bImportStatus;
}

inline bool NounPlanet::exportStatus() const
{
	return m_bExportStatus;
}

inline int NounPlanet::contactCount() const
{
	return m_Contacts.size();
}

inline Noun * NounPlanet::contact( int n ) const
{
	return m_Contacts[ n ];
}

inline bool NounPlanet::isCappable() const
{
	return (m_Flags & FLAG_NOT_CAPTURABLE) == 0;
}

//----------------------------------------------------------------------------

inline NounPlanet::TradeRoute & NounPlanet::TradeRoute::operator=( const TradeRoute & copy )
{
	planet = copy.planet;
	type = copy.type;
	return *this;
}

inline const InStream & operator>>( const InStream & input, NounPlanet::TradeRoute & route )
{
	input >> route.planet;
	input >> route.type;
	return input;
}

inline const OutStream & operator<<( const OutStream & output, const NounPlanet::TradeRoute & route )
{
	output << route.planet;
	output << route.type;
	return output;
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
