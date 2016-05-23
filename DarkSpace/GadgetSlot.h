/*
	GadgetSlot.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef GADGETSLOT_H
#define GADGETSLOT_H

#include "NounGadget.h"
#include "GameDll.h"



//----------------------------------------------------------------------------

class DLL GadgetSlot : public NounGadget
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef WidgetReference< GadgetSlot >			wRef;

	// Construction
	GadgetSlot();

	// NounGadget interface
	Type					type() const;					// get gadget type
	bool					useTarget() const;

	bool					canDamage( dword type ) const;	// can this device be damaged
	int						durability() const;				// how likely can this device survive a ship explosion

	float					addMass() const;				// mass of this gadget
	int						addValue() const;

	CharString				useTip( Noun * pTarget,
								bool shift ) const;			
	bool					usable( Noun * pTarget, 
								bool shift ) const;			// can gadget be used on target
	void					use( dword when, Noun * pTarget,
								bool shift);				// use gadget
	// Mutators
	void					setType( Type type );			// type of slot

private:
	// Data
	Type					m_Type;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
