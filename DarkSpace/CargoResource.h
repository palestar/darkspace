/*
	CargoResource.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef CARGORESOURCE_H
#define CARGORESOURCE_H

#include "Constants.h"
#include "NounCargo.h"
#include "GameDll.h"

class NounPlanet;		// forward declare

//----------------------------------------------------------------------------

class DLL CargoResource : public NounCargo
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference<CargoResource>	Ref;

	// Construction
	CargoResource();
	CargoResource( int nQuantity, bool bClamp = true );		// create a blank cargo resource object
	CargoResource( NounPlanet * pPlanet );					// create a new resource objects, removes resources from the planet provided

	// Widget interface
	bool			read( const InStream & );
	// BaseNode interface
	void			onAttached();
	void			onDetaching();
	// Noun interface
	void			initialize();
	void			simulate( dword nTick );

	// NounCargo interface
	int				durability() const;				// 0-100, chance of surviving ship explosion
	int				value() const;					// get value of cargo in credits
	int				maxQuantity() const;			// what is the maximum quantity
	void			setQuantity( int quantity );
	// Accessors
	NounPlanet *	planet() const;					// get the planet this cargo is on if any

private:
	// Mutators
	void			updateName();		
	// Data
	dword			m_nUpdateTick;
};

//----------------------------------------------------------------------------




#endif

//----------------------------------------------------------------------------
//EOF
