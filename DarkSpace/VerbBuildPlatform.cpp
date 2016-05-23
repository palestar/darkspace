/**
	@file VerbBuildPlatform.cpp

	(c)2006 Palestar Inc
	@author Richard Lyle @date 5/27/2008 11:43:09 PM
*/

#include "Debug/Assert.h"
#include "VerbBuildPlatform.h"
#include "VerbUseSingleGadget.h"
#include "StructureHub.h"
#include "GameContext.h"
#include "DarkSpace/Constants.h"
#include "GameZone.h"



//-------------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbBuildPlatform, Verb );

BEGIN_PROPERTY_LIST( VerbBuildPlatform, Verb )
	ADD_TRANSMIT_PROPERTY( m_pTarget );
	ADD_TRANSMIT_PROPERTY( m_pBuild );
END_PROPERTY_LIST();

VerbBuildPlatform::VerbBuildPlatform()
{}

VerbBuildPlatform::VerbBuildPlatform( NounShip * pBuilder, NounZoneTarget * pTarget, ShipPlatform * pBuild ) 
	: m_pTarget( pTarget ),m_pBuild( pBuild )
{
	attachVerb( pBuilder );
}

//----------------------------------------------------------------------------

Verb::Priority VerbBuildPlatform::priority() const
{
	return MEDIUM;
}

Verb::Scope VerbBuildPlatform::scope() const
{
	return LOCAL;
}

bool VerbBuildPlatform::client() const
{
	return true;
}

bool VerbBuildPlatform::canAttach( Noun * pNoun )
{
	NounShip * pBuilder = WidgetCast<NounShip>( pNoun );
	if (! validate( pNoun ) || !validate( m_pTarget ) || !m_pBuild.valid() )
		return false;

	return canBuild( pBuilder, m_pTarget, m_pBuild );
}

void VerbBuildPlatform::onExecute()
{
	NounShip * pBuilder = WidgetCast<NounShip>( target() );
	if ( validate( pBuilder ) && validate( m_pTarget ) && m_pBuild.valid() )
	{
		// spawn the platform on the server only...
		if ( context()->isServer() )
		{
			ShipPlatform::Ref pPlatform = WidgetCast<ShipPlatform>( m_pBuild->copy() );
			if ( !pPlatform.valid() )
				return;

			// take resources from the builders ship..
			pBuilder->useResources( m_pBuild->buildCost() );

			// initialize the structure
			pPlatform->setPosition( m_pTarget->position() );
			pPlatform->setTick( tick() );
			pPlatform->setRank( 0/*255*/ );		// changed to 0 so players can give platforms orders.. 
			pPlatform->setOrder( NounShip::DEFEND, pPlatform.pointer(), pBuilder );
			pPlatform->setTeamId( pBuilder->teamId() );
			pPlatform->setup();

			m_pTarget->zone()->attachNode( pPlatform );

			// check for GadgetBuild object on builders ship, if found use it build ...
			for(int i=0;i<pBuilder->childCount();++i)
			{
				GadgetBuild * pBuildDevice = WidgetCast<GadgetBuild>( pBuilder->child(i) );
				if (! pBuildDevice )
					continue;
				if ( !pBuildDevice->useActive() && pBuildDevice->usable( pPlatform, false ) )
					Verb::Ref( new VerbUseSingleGadget( pBuildDevice, pPlatform, false ) );
			}
		}
	}
}

//----------------------------------------------------------------------------

bool VerbBuildPlatform::canBuild( NounShip * pBuilder, NounZoneTarget * pTarget, ShipPlatform * pPlatform )
{
	if ( !pTarget || !pBuilder || !pPlatform )
		return false;		// invalid pointers

	GadgetBuild * pGadget = GadgetBuild::getGadgetBuild( pBuilder );
	if (! pGadget || pGadget->useActive() )
		return false;
	GameZone * pZone = WidgetCast<GameZone>( pTarget->zone() );
	if (! pZone )
		return false;
	GameContext *pGameContext  = WidgetCast<GameContext>(pBuilder->context());
	if(pGameContext)
	{
		if(pGameContext->factionPlatforms(pBuilder->factionId()) >= MAX_PLATFORMS_PER_FACTION)
			return false;
	}
	if ( pPlatform->buildCost() > pBuilder->resourceCount() )
		return false;		// not enough resources to build...

	Array< ShipPlatform *> platforms;
	if (! getPlatforms( pBuilder, pTarget, platforms ) )
		return false;
	if ( platforms.search( pPlatform ) < 0 )
		return false;

	return true;
}

bool VerbBuildPlatform::getPlatforms( NounShip * pBuilder, NounZoneTarget * pTarget, Array< ShipPlatform * > & platforms )
{
	if (! pTarget || !pTarget->zone() || !pBuilder )
		return false;
	// if we have a ship that is within range, get the object we can build on the surface
	GadgetBuild * pGadget = GadgetBuild::getGadgetBuild( pBuilder );
	if (! pGadget )
		return false;
	if (! pGadget->inBuildRange( pTarget ) )
		return false;

	pGadget->getPlatforms( platforms );
	return platforms.size() > 0;
}


//---------------------------------------------------------------------------------------------------
//EOF
