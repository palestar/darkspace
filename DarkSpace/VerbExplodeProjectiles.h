/**
	@file VerbExplodeProjectiles.h
	@brief TODO

	(c)2009 Palestar Inc
	@author Richard Lyle @date 5/4/2011 9:15:42 PM
*/

#ifndef VERBEXPLODEPROJECTILES_H
#define VERBEXPLODEPROJECTILES_H

#include "NounProjectile.h"
#include "World/Verb.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbExplodeProjectiles : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbExplodeProjectiles();
	VerbExplodeProjectiles( Noun * pParent, const Array< NounProjectile::wRef > & a_Projectiles );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;		// is this a client verb, meaning okay to receive from a client

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	Array< NounProjectile::wRef > 
							m_Projectiles;
};


#endif

//---------------------------------------------------------------------------------------------------
//EOF
