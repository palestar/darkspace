/*
	VerbCapturePlanet.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "resource.h"
#include "VerbCapturePlanet.h"
#include "StructureDefense.h"
#include "GadgetReload.h"
#include "GameContext.h"

//-------------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbCapturePlanet, Verb );

BEGIN_PROPERTY_LIST( VerbCapturePlanet, Verb )
	ADD_TRANSMIT_PROPERTY( m_nFleetId );
END_PROPERTY_LIST();

VerbCapturePlanet::VerbCapturePlanet()
{}

VerbCapturePlanet::VerbCapturePlanet( NounPlanet * pPlanet, int nFleetId ) 
	: m_nFleetId( nFleetId )
{
	attachVerb( pPlanet );
}

//----------------------------------------------------------------------------

Verb::Priority VerbCapturePlanet::priority() const
{
	return MEDIUM;
}

Verb::Scope VerbCapturePlanet::scope() const
{
	return GLOBAL;
}

bool VerbCapturePlanet::client() const
{
	return false;
}

bool VerbCapturePlanet::canAttach( Noun * pNoun )
{
	return WidgetCast<NounPlanet>( pNoun ) != NULL;
}

void VerbCapturePlanet::onExecute()
{
	NounPlanet * pPlanet = WidgetCast<NounPlanet>( target() );
	if ( validate( pPlanet ) )
	{
		// notify the planet's current owners the planet has been captured..
		CharString sMessage;
		if ( pPlanet->context()->isTeamValid( m_nFleetId ) )
		{
			sMessage.format( "<color;ffffff>NEWS: %s has been captured by the %s.", 
				pPlanet->name(), pPlanet->context()->team( m_nFleetId ).name.cstr() );
		}
		else
		{
			sMessage.format( "<color;ffffff>NEWS: %s has been captured by rebels.", pPlanet->name() );
		}
		pPlanet->clientChat( sMessage );

		// planet captured set the owner
		pPlanet->setTeamId( m_nFleetId );
		// reset lock rank
		pPlanet->setLockRank( 0 );
		// reduce the moral of the planet
		if ( pPlanet->population() > 0 && pPlanet->factionId() != 0 )
			pPlanet->setMoral( Clamp( pPlanet->moral() - 0.25f, 0.2f, 1.0f ) );

		// stop reloading any friendly ships
		if ( pPlanet->isServer() )
		{
			for(int i=0;i<pPlanet->childCount();i++)
			{
				StructureDefense * pDefense = WidgetCast<StructureDefense>( pPlanet->child(i) );
				if ( pDefense == NULL )
					continue;
				pDefense->setTeamId( 0 );		// HACK: Somehow defense bases are getting their teamId set..

				for(int j=0;j<pDefense->childCount();j++)
				{
					GadgetReload * pReload = WidgetCast<GadgetReload>( pDefense->child(j) );
					if ( pReload != NULL && pReload->reloading() != NULL && !pPlanet->isFriend( pReload->reloading() ) )
						pDefense->useGadget( NULL, pReload );
				}
			}
		}

		// send message to the new owners of this planet as well...
		pPlanet->clientChat( sMessage );
	}
}

//----------------------------------------------------------------------------
//EOF
