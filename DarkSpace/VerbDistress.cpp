/*
	VerbDistress.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "VerbDistress.h"
#include "resource.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

Noun::wRef	VerbDistress::sm_LastDistress = NULL;

//-------------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbDistress, Verb );

BEGIN_PROPERTY_LIST( VerbDistress, Verb )
	ADD_TRANSMIT_PROPERTY( m_Type );
END_PROPERTY_LIST();

VerbDistress::VerbDistress()
{}

VerbDistress::VerbDistress( Noun * pTarget, Type type ) 
	: m_Type( type )
{
	attachVerb( pTarget );
}

//----------------------------------------------------------------------------

Verb::Priority VerbDistress::priority() const
{
	return MEDIUM;
}

Verb::Scope VerbDistress::scope() const
{
	return FACTION;
}

bool VerbDistress::client() const
{
	return true;
}

void VerbDistress::onExecute()
{
	Noun * pTarget = target();
	if ( validate( pTarget ) )
	{
		GameContext * pGameContext = WidgetCast<GameContext>( context() );
		if ( pGameContext != NULL )
			pGameContext->onVerbDistress( this );

		switch( m_Type )
		{
		case INVASION:
			pTarget->clientChat( CharString().format( "<color;00ffff>DISTRESS: '%s' has been invaded...</color>", pTarget->name() ) );
			break;
		case ATTACKED:
			pTarget->clientChat( CharString().format( "<color;00ffff>DISTRESS: '%s' is being attacked...</color>", pTarget->name() ) );
			break;
		case SUPPLY:
			pTarget->clientChat( CharString().format( "<color;00ffff>DISTRESS: '%s' needs resupply...</color>", pTarget->name() ) );
			break;
		case ENEMY:
			pTarget->clientChat( CharString().format( "<color;00ffff>DISTRESS: Enemy detected at '%s'...</color>", pTarget->name() ) );
			break;
		case BLOCKADE:
			pTarget->clientChat( CharString().format( "<color;00ffff>DISTRESS: '%s' is under blockade...</color>", pTarget->name() ) );
			break;
		}

		// set the last distress message
		sm_LastDistress = pTarget;
	}
}

//----------------------------------------------------------------------------
//EOF
