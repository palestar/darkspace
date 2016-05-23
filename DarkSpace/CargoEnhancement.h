/*
	CargoEnhancement.h
	(c)2008 Palestar Inc, Jeff Everett
*/

#ifndef CARGOENHANCEMENT_H
#define CARGOENHANCEMENT_H

#include "Constants.h"
#include "NounCargo.h"
#include "NounEnhancement.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

// forward declare
class NounEnhancement;

class DLL CargoEnhancement : public NounCargo
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference<CargoEnhancement>				Ref;
	typedef WidgetReference<CargoEnhancement >		wRef;

	// Construction
	CargoEnhancement();
	CargoEnhancement( qword a_classKey );		// create a cargo item

	// Widget interface
	bool					read( const InStream & );
	// BaseNode interface
	void					onAttached();
	void					onDetaching();
	// Noun interface
	virtual float			baseSignature() const;			// what is the default signature for this noun (by default the radius)
	virtual CharString		status() const;
	virtual void			initialize();
	virtual void			simulate( dword nTick );

	//! Accessors
	const qword &			enhancementKey() const;								// key of the nounenhancement in the cargo
	NounEnhancement *		enhancement() const;
	u8						damage() const;

	//! Mutators
	void					createEnhancement();
	void					setDamage( u8 a_nDamage );

private:
	// Data
	float					m_fRemovalTime;
	qword					m_enhancementClassKey;
	u8						m_nDamage;
	NounEnhancement::Ref	m_pEnhancement;
};

//---------------------------------------------------------------------------------------------------

inline NounEnhancement * CargoEnhancement::enhancement() const
{
	return m_pEnhancement;
}

inline const qword & CargoEnhancement::enhancementKey() const
{
	return m_enhancementClassKey;

}

inline u8 CargoEnhancement::damage() const
{
	return m_nDamage;
}

#endif

//---------------------------------------------------------------------------------------------------
//EOF
