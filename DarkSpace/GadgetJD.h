/*
	GadgetJD.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef GADGETJD_H
#define GADGETJD_H

#include "Constants.h"
#include "NounGadget.h"
#include "NounShip.h"
#include "FieldInterdictor.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL GadgetJD : public NounGadget
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	GadgetJD();

	// BaseNode interface
	void					render( RenderContext &context, 
								const Matrix33 & frame, 
								const Vector3 & position );			
	// Noun interface
	void					initialize();
	void					release();
	// NounGadget interface
	Type					type() const;					// get gadget type
	dword					hotkey() const;					// gadget hotkey
	
	float					addMass() const;				// mass of this gadget	

	float					addSignature() const;			// add signature when active

	bool					usable( Noun * pTarget, 
								bool shift ) const;			// can gadget be used on target
	bool					useActive() const;				// is the gadget current on or off
	void					use( dword when, Noun * pTarget,
								bool shift);				// use gadget
	int						useEnergy( dword nTick, 
								int energy );				// use energy
	bool					updateLogic();

	// Accessors
	bool					active() const;					// how many ticks remaining
	FieldInterdictor *		field() const;	

	virtual int				energyCost() const = 0;
	virtual float			range() const = 0;

private:
	//! Data
	FieldInterdictor::wRef	m_pField;
	// non-serialized
	int						m_nLastFleetId;					// our last fleet id
	float					m_fFieldScale;
};

//----------------------------------------------------------------------------

inline bool GadgetJD::active() const
{
	return (flags() & FLAG_ACTIVE) != 0;
}

inline FieldInterdictor * GadgetJD::field() const
{
	return m_pField;
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
