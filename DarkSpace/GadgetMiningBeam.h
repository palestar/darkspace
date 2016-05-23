/*
	GadgetMiningBeam.h
	(c)1999 Palestar Development, Richard Lyle
*/

#ifndef GADGET_MINING_BEAM_H
#define GADGET_MINING_BEAM_H

#include "NounGadget.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL GadgetMiningBeam : public NounGadget
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	GadgetMiningBeam();
	// BaseNode interface
	void					render( RenderContext &context, 
								const Matrix33 & frame, 
								const Vector3 & position );
	// Noun interface
	void					release();
	void					simulate( dword nTick );

	// NounGadget interface
	Type					type() const;					// get gadget type
	dword					hotkey() const;					// gadget hotkey

	bool					usable( Noun * pTarget, 
								bool shift ) const;			// can gadget be used on target
	bool					useActive() const;
	void 					use( dword when, Noun * pTarget,
								bool shift);				// use gadget
	int						useEnergy( dword nTick, 
								int energy );
	bool					updateLogic();

	virtual float			mineScale() const = 0;
	virtual int				energyCost() const = 0;			// energy cost to keep the beam active
	virtual float			range() const = 0;				// range of the beam

	void					mineResources( float fAmount );

private:
	// Data
	Noun::wRef				m_Target;						// mining beam target
	// non-serialized
	dword					m_nMineTick;
	float					m_fMined;						// fractional resources mined
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
