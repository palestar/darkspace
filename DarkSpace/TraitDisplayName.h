/**
	@file TraitDisplayName.h
	@brief This trait allows a custom name to be attached onto a given object.

	(c)2012 Palestar Inc
	@author Richard Lyle @date 11/9/2012 2:31:41 PM
*/

#ifndef TRAITDISPLAYNAME_H
#define TRAITDISPLAYNAME_H

#include "World/Trait.h"
#include "GameDll.h"

//---------------------------------------------------------------------------------------------------

class DLL TraitDisplayName : public Trait
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	//! Accessors
	const CharString &	
				displayName() const;
	//! Mutators
	void		setDisplayName( const char * a_pName );

protected:
	//! Data
	CharString	m_sDisplayName;
};

//---------------------------------------------------------------------------------------------------

inline const CharString & TraitDisplayName::displayName() const
{
	return m_sDisplayName;
}

inline void	TraitDisplayName::setDisplayName( const char * a_pName )
{
	m_sDisplayName = a_pName;
}

//---------------------------------------------------------------------------------------------------

#endif

//---------------------------------------------------------------------------------------------------
//EOF
