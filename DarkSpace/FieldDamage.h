/*
	FieldDamage.h

	This field type inflicts damage on any nouns that come into this field.
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef FIELDDAMAGE_H
#define FIELDDAMAGE_H

#include "NounField.h"

//----------------------------------------------------------------------------

class FieldDamage : public NounField
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference< FieldDamage >				Ref;
	typedef WidgetReference< FieldDamage >			wRef;

	// Construction
	FieldDamage();
	
	// Noun interface
	void		simulate( dword nTick );					

	// NounField interface
	void		onEnterField( Noun * pNoun );
	void		onInsideField( Noun * pNoun );
	void		onLeaveField( Noun * pNoun );

	// Accessors
	float		life() const;					// field life in seconds
	int			inflictDamage() const;			// damage caused per second inside the field
	dword		inflictDamageType() const;		// type of damage caused

	// Mutators
	void		setLife( float fLife );
	void		setDamage( int nDamage, dword nType );

private:
	// Data
	float		m_fLife;
	int			m_nInflictDamage;
	dword		m_nInflictDamageType;
};

//----------------------------------------------------------------------------

inline float FieldDamage::life() const
{
	return m_fLife;
}

inline int FieldDamage::inflictDamage() const
{
	return m_nInflictDamage;
}

inline dword FieldDamage::inflictDamageType() const
{
	return m_nInflictDamageType;
}

inline void FieldDamage::setLife( float fLife )
{
	m_fLife = fLife;
}

inline void FieldDamage::setDamage( int nDamage, dword nType )
{
	m_nInflictDamage = nDamage;
	m_nInflictDamageType = nType;
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF

