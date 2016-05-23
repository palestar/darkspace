/*
	ShipPlatform.h
	(c)2008 Palestar Inc, Richard Lyle
*/

#ifndef SHIPPLATFORM_H
#define SHIPPLATFORM_H

#include "NounShip.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL ShipPlatform : public NounShip
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference< ShipPlatform >					Ref;
	typedef WidgetReference< ShipPlatform >				wRef;

	// Construction
	ShipPlatform();

	// Noun interface
	virtual CharString		status() const;					// status string
	virtual void			setup();
	virtual void			initialize();
	virtual void			simulate( dword nTick );		// called to simulate the noun
	// NounShip interface
	virtual bool			updateLogic( dword nTick );
	// Interface
	virtual int				ticksPerUpdate() const = 0;		// how many ticks for each update...
	virtual int				resourceCost() const = 0;		// cost in resources to upkeep this platform per update
	virtual int				maxResources() const = 0;		// maximum number of resources for this platform
	virtual int				buildTime() const = 0;			// time to build this platform in seconds

	// Accessors
	bool					isBuilding() const;				// returns true if this platform is under construction
	int						buildPercent() const;			// percentage of platform built
	int						resourcesPercent() const;		// percentage of resources available
	int						resources() const;				// number of resources
	// Mutators
	bool					build();						// returns true once build is completed...
	void					addResouces( int nQuantity );	// add resources to this platform

	// Helpers
	Material *				icon() const;					// get unit icon

protected:
	// Data
	int						m_nBuildTime;					// amount of platform that has been built..
	dword					m_nResourceTick;				// tick of the next update
	int						m_nResources;					// number resources this platform has.. when <= 0 then platform dies...
};

//----------------------------------------------------------------------------

inline bool ShipPlatform::isBuilding() const
{
	return m_nBuildTime < buildTime();
}

inline int ShipPlatform::buildPercent() const
{
	return (m_nBuildTime * 100) / buildTime();
}

inline int ShipPlatform::resourcesPercent() const
{
	return (m_nResources * 100) / maxResources();
}

inline int ShipPlatform::resources() const
{
	return m_nResources;
}

#endif

//----------------------------------------------------------------------------
//EOF
