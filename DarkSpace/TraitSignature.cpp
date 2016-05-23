/*
	TraitSignature.cpp
	(c)2006 Palestar Inc, Richard Lyle
*/

#include "TraitSignature.h"
#include "GameContext.h"

//---------------------------------------------------------------------------------------------------

IMPLEMENT_FACTORY( TraitSignature, Trait );

BEGIN_PROPERTY_LIST( TraitSignature, Trait );
END_PROPERTY_LIST();

//---------------------------------------------------------------------------------------------------

TraitSignature::TraitSignature()
{}

void TraitSignature::initialize()
{
	//m_fBaseSignature = parent()->nounContext()->getAttribute( "BaseSignature", 0.0f );
}

void TraitSignature::simulate( dword nTick )
{
	Trait::simulate( nTick );
}

//---------------------------------------------------------------------------------------------------

void TraitSignature::addSignature( dword when, float fAdd )
{
	if ( when > m_nSignatureTick )
	{
		m_fSignature = baseSignature() + fAdd;
		m_nSignatureTick = when;
	}
	else if ( when == m_nSignatureTick )
		m_fSignature += fAdd;
}

void TraitSignature::resetSignature()
{
	m_fLastSignature = m_fSignature;
	m_fSignature = baseSignature();
	m_nSignatureTick = 0;
}

void TraitSignature::setBaseSignature( float fBase )
{
	m_fBaseSignature = fBase;
}

//---------------------------------------------------------------------------------------------------
//EOF
