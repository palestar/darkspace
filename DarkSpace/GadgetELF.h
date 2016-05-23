/*
	GadgetELF.h

	Electromagnetic Leech Field
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef GADGETELF_H
#define GADGETELF_H

#include "Constants.h"
#include "NounGadget.h"
#include "NounShip.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL GadgetELF : public NounGadget
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	GadgetELF();

	// Widget interface
	bool					read( const InStream & );
	// BaseNode interface
	void					render( RenderContext &context, 
								const Matrix33 & frame, 
								const Vector3 & position );
	// Noun interface
	void					release();
	// NounGadget interface
	Type					type() const;					// get gadget type
	dword					hotkey() const;					// gadget hotkey

	bool					usable( Noun * pTarget, 
								bool shift ) const;			// can gadget be used on target
	bool					useActive() const;				// is the gadget current on or off
	void					use( dword when, Noun * pTarget,
								bool shift);				// use gadget
	int						useEnergy( dword nTick, 
								int energy );				// use energy
	bool					updateLogic();

	// Accessors
	bool					active() const;
	virtual float			range() const = 0;

private:
	// non-serialized
	Array< NounShip::wRef >	m_Affect;						// affected objects
	int						m_nUpdateAffected;
};

//----------------------------------------------------------------------------

inline bool GadgetELF::active() const
{
	return (flags() & FLAG_ACTIVE) != 0;
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF

