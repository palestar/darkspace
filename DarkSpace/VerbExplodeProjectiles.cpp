/**
	@file VerbExplodeProjectiles.cpp

	(c)2009 Palestar Inc
	@author Richard Lyle @date 5/4/2011 9:19:45 PM
*/

#include "VerbExplodeProjectiles.h"

//-------------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbExplodeProjectiles, Verb );

BEGIN_PROPERTY_LIST( VerbExplodeProjectiles, Verb )
	ADD_TRANSMIT_PROPERTY( m_Projectiles );
END_PROPERTY_LIST();

VerbExplodeProjectiles::VerbExplodeProjectiles()
{}

VerbExplodeProjectiles::VerbExplodeProjectiles( Noun * pParent, const Array< NounProjectile::wRef > & a_Projectiles ) : m_Projectiles( a_Projectiles )
{
	attachVerb( pParent );
}

//----------------------------------------------------------------------------

Verb::Priority VerbExplodeProjectiles::priority() const
{
	return CRITICAL;
}

Verb::Scope VerbExplodeProjectiles::scope() const
{
	return LOCAL;
}

bool VerbExplodeProjectiles::client() const
{
	return false;
}

bool VerbExplodeProjectiles::canAttach( Noun * pNoun )
{
	return pNoun != NULL;
}

void VerbExplodeProjectiles::onExecute()
{
	for(int i=0;i<m_Projectiles.size();++i)
	{
		NounProjectile * pProj = m_Projectiles[i];
		if ( pProj != NULL )
			pProj->explode();
	}
}

//---------------------------------------------------------------------------------------------------
//EOF
