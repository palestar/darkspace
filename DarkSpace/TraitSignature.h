/*
	TraitSignature.h

	This trait means the object has a signature and can be detected by sensors
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef TRAITSIGNATURE_H
#define TRAITSIGNATURE_H

#include "World/Trait.h"
#include "NounGame.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL TraitSignature : public Trait
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	TraitSignature(); 

	// Trait interface
	void		initialize();
	void		simulate( dword nTick );					
	
	// Accessors
	float		baseSignature() const;			// base signature for this object
	float		signature() const;				// this objects current signature

	// Mutators
	void		addSignature( dword when, 
					float sig );				// add signature to this noun
	void		resetSignature();				// finalize the current signature, and reset to the baseSignature()
	void		setBaseSignature( float fBase );

private:
	// Data
	float		m_fSignature;					// objects current signature
	dword		m_nSignatureTick;
	float		m_fLastSignature;				// set when resetSignature() is called
	float		m_fBaseSignature;
};

//----------------------------------------------------------------------------

inline float TraitSignature::signature() const
{
	return m_fLastSignature;
}

inline float TraitSignature::baseSignature() const
{
	return m_fBaseSignature;
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
