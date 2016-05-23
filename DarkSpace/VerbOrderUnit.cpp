/*
	VerbOrderUnit.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "resource.h"
#include "World/NounTarget.h"
#include "VerbOrderUnit.h"
#include "GameContext.h"

//-------------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbOrderUnit, Verb );

BEGIN_PROPERTY_LIST( VerbOrderUnit, Verb )
	ADD_TRANSMIT_PROPERTY( m_Order );
	ADD_TRANSMIT_PROPERTY( m_pWho );
	ADD_TRANSMIT_PROPERTY( m_pTarget );
END_PROPERTY_LIST();

VerbOrderUnit::VerbOrderUnit()
{}

VerbOrderUnit::VerbOrderUnit( NounUnit * pUnit, Order order, Noun * pTarget, NounShip * pWho ) 
	: m_Order( order ), m_pTarget( pTarget ), m_pWho( pWho )
{
	attachVerb( pUnit );
}

//----------------------------------------------------------------------------

Verb::Priority VerbOrderUnit::priority() const
{
	return MEDIUM;
}

Verb::Scope VerbOrderUnit::scope() const
{
	return LOCAL;
}

bool VerbOrderUnit::client() const
{
	return true;
}

bool VerbOrderUnit::canAttach( Noun * pNoun )
{
	if ( pNoun->isServer() )
		return canOrder( WidgetCast<NounUnit>( pNoun ), m_pWho );
	return true;
}

void VerbOrderUnit::onExecute()
{
	NounUnit * pUnit = WidgetCast<NounUnit>( target() );
	if ( validate( pUnit ) )
	{
		pUnit->setOrder( m_Order );
		pUnit->setTarget( m_pTarget );

		if ( validate( m_pWho ) )
		{
			m_pWho->message( CharString().format( "<color;ffffff>Comms: Sent Order to %s, %s %s",
				pUnit->name(), NounUnit::orderText( m_Order ), m_pTarget.valid() ? m_pTarget->name() : "" ) );
		}
	}
}

//----------------------------------------------------------------------------

bool VerbOrderUnit::canOrder( Noun * pTarget, NounShip * pWho )
{
	NounUnit * pUnit = WidgetCast<NounUnit>( pTarget );
	if ( pUnit == NULL || pWho == NULL )
		return false;		// invalid pointers
	if (! pWho->isFriend( pUnit ) )
		return false;		// cannot order an enemy unit

	return true;
}

//----------------------------------------------------------------------------
// EOF
