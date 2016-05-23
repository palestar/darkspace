/*
	CargoGadget.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef CARGOGADGET_H
#define CARGOGADGET_H

#include "NounGadget.h"
#include "NounCargo.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL CargoGadget : public NounCargo
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference<CargoGadget>					Ref;
	typedef WidgetReference< CargoGadget >			wRef;
	typedef NounGadget::Type						Type;

	// Construction
	CargoGadget();
	CargoGadget( NounGadget * pGadget );

	// Widget interface
	bool				read( const InStream & input );
	// Noun interface
	void				simulate( dword nTick );
	// NounCargo interface
	int					value() const;					// get value of cargo in credits
	int					maxQuantity() const;			// what is the maximum quantity

	// Accessors
	NounGadget *		gadget() const;
	Type				type() const;
	int					damage() const;
	int					level() const;

	// Mutators
	void				setGadget( NounGadget * pGadget );
	// Helpers
	virtual Material *	icon() const;

private:
	// Data
	int					m_nLife;
	NounGadget::Ref		m_pGadget;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
