/*
	GadgetBuild.h
	(c)1999 Palestar Development, Richard Lyle
*/

#ifndef GADGET_BUILD_H
#define GADGET_BUILD_H

#include "NounGadget.h"
#include "NounStructure.h"
#include "NounDrone.h"
#include "ShipPlatform.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL GadgetBuild : public NounGadget, public NounDrone::Action
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	GadgetBuild();

	// BaseNode interface
	void					render( RenderContext &context, 
								const Matrix33 & frame, 
								const Vector3 & position );
	// Noun interface
	virtual void			simulate( dword nTick );

	virtual void			setup();
	virtual void			initialize();
	virtual bool			postInitialize();
	virtual void			release();

	// NounGadget interface
	Type					type() const;					// get gadget type
	void					reset();						// reset gadget to starting state

	bool					usable( Noun * pTarget, 
								bool shift ) const;			// can gadget be used on target
	void					use( dword when, Noun * pTarget,
								bool shift);				// use gadget

	int						usableWhen() const;				// when can the gadget be used again
	bool					useActive() const;				// is the gadget current on or off
	int						durability() const;				// how durable is the gadget

	int						addValue() const;

	// NounDrone::Action interface
	void					droneUpdate( NounDrone * pDrone );
	void					droneAction( NounDrone * pDrone );	
	void					droneHome( NounDrone * pDrone );

	// Accessors
	int						structureCount() const;
	NounStructure *			structure( int n ) const;
	ShipPlatform *			platform( int n ) const;
	Scene *					build() const;

	bool					inBuildRange( Noun * pTarget ) const;

	// Helpers
	void					getStructures( Array< NounStructure * > & structures );
	void					getPlatforms( Array< ShipPlatform * > & platforms );
	// Static
	static GadgetBuild *	getGadgetBuild( NounShip * pShip );

private:
	// Data
	Noun::wRef				m_Building;						// who are we building
	// non-serialize
	int						m_DroneCount;
	Scene::Ref				m_Build;
	dword					m_nBuildTick;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
