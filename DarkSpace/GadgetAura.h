/**
	@file GadgetAura.h
	@brief This gadgets is used to apply a FieldEnahancement to all ships in a area around the ship with this gadget.

	(c)2012 Palestar Inc
	@author Richard Lyle @date 7/2/2012 7:57:34 PM
*/

#ifndef GADGETAURA_H
#define GADGETAURA_H

//---------------------------------------------------------------------------------------------------

#include "Constants.h"
#include "NounGadget.h"
#include "NounShip.h"
#include "FieldEnhancement.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL GadgetAura : public NounGadget
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	GadgetAura();

	// BaseNode interface
	void					render( RenderContext &context, 
								const Matrix33 & frame, 
								const Vector3 & position );
	// Noun interface
	void					initialize();
	void					release();
	// NounGadget interface
	virtual CharString		useTip( Noun * pTarget,
								bool shift ) const;	
	Type					type() const;					// get gadget type

	//! Accessors
	FieldEnhancement *		getField() const;
	//! Mutators
	void					setField( FieldEnhancement * a_pField );

private:
	//! Data
	FieldEnhancement::Ref	m_pField;
	Scene::Ref				m_pUseEffect;
};

//---------------------------------------------------------------------------------------------------

inline FieldEnhancement * GadgetAura::getField() const
{
	return m_pField;
}

inline void GadgetAura::setField( FieldEnhancement * a_pField )
{
	m_pField = a_pField;
}

//---------------------------------------------------------------------------------------------------

#endif

//---------------------------------------------------------------------------------------------------
//EOF
