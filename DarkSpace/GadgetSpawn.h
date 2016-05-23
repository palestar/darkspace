/*
	GadgetSpawn.h
	(c)1999 Palestar Development, Richard Lyle
*/

#ifndef GADGET_SPAWN_H
#define GADGET_SPAWN_H

#include "Resource/Collection.h"
#include "NounGadget.h"
#include "NounShip.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL GadgetSpawn : public NounGadget
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	GadgetSpawn();

	// Noun interface
	void					initialize();
	// NounGadget interface
	Type					type() const;					// get gadget type
	dword					hotkey() const;					// gadget hotkey
	CharString				status() const;					// status of the gadget (displayed on the icon)
	CharString				description() const;			// get description of this gadget
	int						useModeCount() const;			// how many different modes are available
	int						useMode() const;				// what is the current user mode
	bool					useTarget() const;

	void					useMode( int mode );			// set use mode
	bool					usable( Noun * pTarget, 
								bool shift ) const;			// can gadget be used on target
	void 					use( dword when, Noun * pTarget,
								bool shift);				// use gadget
	
	// Mutators
	void					loadSpawns();

private:
	// Data
	int						m_Mode;

	// non-serialized
	Array< Noun::Ref >		m_Spawn;

	// Helpers
	Noun *					getSpawn() const;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
