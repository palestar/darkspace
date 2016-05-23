/*
	GadgetTractorBeam.h
	(c)1999 Palestar Development, Richard Lyle
*/

#ifndef GADGET_TRACTOR_BEAM_H
#define GADGET_TRACTOR_BEAM_H

#include "NounGadget.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL GadgetTractorBeam : public NounGadget
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	GadgetTractorBeam();

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
	bool					useActive() const;
	void					use( dword when, Noun * pTarget,
								bool shift);				// use gadget
	int						useEnergy( dword nTick, 
								int energy );

	virtual int				energyCost() const = 0;			// energy cost to keep the beam active
	//  virtual int				energyFalloff() const = 0;		// energy falloff 

	virtual float			length() const = 0;				// length (range) of the beam

	// Helpers
	bool					validateTarget( Noun * pTarget ) const;

private:
	// Data
	NounBody::wRef			m_Target;						// tractor beam target
	Vector3					m_vOffset;						// position offset between this device and the target 

	// non-serialized

};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
