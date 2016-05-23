/*
	GadgetScript.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef GADGETSCRIPT_H
#define GADGETSCRIPT_H

#include "Resource/Text.h"
#include "NounGadget.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL GadgetScript : public NounGadget
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	GadgetScript();

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
	void					use( dword when, Noun * pTarget,
								bool shift);				// use gadget
	
	// Mutators
	void					loadScripts();

private:
	// Data
	int						m_Mode;
	// non-serialized
	Array< Text::Ref >		m_Scripts;

	// Helpers
	Text *					getScript() const;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
