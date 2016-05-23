/*
	VerbBuildStructure.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "VerbBuildStructure.h"
#include "VerbUseSingleGadget.h"
#include "StructureHub.h"
#include "GameContext.h"

//-------------------------------------------------------------------------------

extern Constant BUILD_POINTS_PER_SECOND;

IMPLEMENT_FACTORY( VerbBuildStructure, Verb );

BEGIN_PROPERTY_LIST( VerbBuildStructure, Verb )
	ADD_TRANSMIT_PROPERTY( m_pTarget );
	ADD_TRANSMIT_PROPERTY( m_pBuild );
END_PROPERTY_LIST();

//---------------------------------------------------------------------------------------------------

VerbBuildStructure::VerbBuildStructure()
{}

VerbBuildStructure::VerbBuildStructure( NounShip * pBuilder, Noun * pTarget, NounStructure * pBuild ) 
	: m_pTarget( pTarget ), m_pBuild( pBuild )
{
	attachVerb( pBuilder );
}

//----------------------------------------------------------------------------

Verb::Priority VerbBuildStructure::priority() const
{
	return MEDIUM;
}

Verb::Scope VerbBuildStructure::scope() const
{
	return LOCAL;
}

bool VerbBuildStructure::client() const
{
	return true;
}

bool VerbBuildStructure::canAttach( Noun * pNoun )
{
	NounShip * pBuilder = WidgetCast<NounShip>( pNoun );
	if (! validate( pBuilder ) || !validate( m_pTarget ) || !m_pBuild.valid() )
		return false;

	return canBuild( pBuilder, m_pTarget, m_pBuild );
}

void VerbBuildStructure::onExecute()
{
	NounShip * pBuilder = WidgetCast<NounShip>( target() );
	if ( validate( pBuilder ) && validate( m_pTarget ) && m_pBuild.valid() )
	{
		NounPlanet * pPlanet = NULL;
		NounDropTarget * pDropTarget = WidgetCast<NounDropTarget>( m_pTarget );
		NounStructure * pUpgrade = WidgetCast<NounStructure>( m_pTarget );		// structure we're upgrading, if any...


		if ( pUpgrade )
			pPlanet = pUpgrade->planet();
		else if ( pDropTarget )
			pPlanet = pDropTarget->planet();

		if (! validate( pPlanet ) )
			return;

		if ( pDropTarget != NULL && pUpgrade == NULL )
		{
			// check for another structure on the same hex, if found, see if we can upgrade it or not..
			for(int i=0;i<pPlanet->childCount() && pUpgrade == NULL;++i)
			{
				NounStructure * pStructure = WidgetCast<NounStructure>( pPlanet->child(i) );
				if (! pStructure )
					continue;
				if ( pStructure->hex() == pDropTarget->hex() )
					pUpgrade = pStructure;
			}
		}

		// give builder construction points
		//((GameContext *)context())->gameUser()->onConstruct( pBuilder, 0.2f );

		// take resources from the planet for this structure first, then from the builders ship..
		int nBuildCost = m_pBuild->buildCost();
		nBuildCost -= pPlanet->useResources( nBuildCost );
		if ( nBuildCost > 0 )
			pBuilder->useResources( nBuildCost );

		// spawn the structure on the server only
		if ( context()->isServer() )
		{
			NounStructure::Ref pStructure = (NounStructure *)m_pBuild->copy();

			// detach any upgraded structure
			if ( pUpgrade != NULL )
			{
				// Fix for BUG#738 - If players upgrade, they avoid the scrap costs... 
				float fPoints = BUILD_POINTS_PER_SECOND * pUpgrade->buildTime();
				((GameContext *)context())->gameUser()->onConstruct( pBuilder, -fPoints );

				pUpgrade->setDetach();
			}
			// initialize the structure
			pStructure->setHex(  pDropTarget != NULL ? pDropTarget->hex() : pUpgrade->hex() );
			pStructure->setTick( tick() );
			pStructure->setup();
			// attach the structure to the planet
			pPlanet->attachNode( pStructure );

			// check for GadgetBuild object on builders ship, if found use it build ...
			for(int i=0;i<pBuilder->childCount();++i)
			{
				GadgetBuild * pBuildDevice = WidgetCast<GadgetBuild>( pBuilder->child(i) );
				if (! pBuildDevice )
					continue;
				if ( !pBuildDevice->useActive() && pBuildDevice->usable( pStructure, false ) )
					Verb::Ref( new VerbUseSingleGadget( pBuildDevice, pStructure, false ) );
			}
		}
	}
}

//----------------------------------------------------------------------------

bool VerbBuildStructure::canBuild( NounShip * pBuilder, Noun * pTarget, NounStructure * pStructure )
{
	if ( pTarget == NULL || pBuilder == NULL || pStructure == NULL )
		return false;		// invalid pointers

	GadgetBuild * pGadget = getGadgetBuild( pBuilder );
	if (! pGadget || pGadget->useActive() )
		return false;

	NounDropTarget * pDrop = WidgetCast<NounDropTarget>( pTarget );
	NounStructure * pUpgrade = WidgetCast<NounStructure>( pTarget );		// structure we're upgrading, if any...
	NounPlanet * pPlanet = NULL;

	if (!pDrop && !pUpgrade )
		return false;			// target must be a drop target or a structure to upgrade..

	if ( pDrop != NULL )
	{
		pPlanet = pDrop->planet();
		if ( !pPlanet || !pPlanet->isHexValid( pDrop->hex() ) )
			return false;		// invalid planet or hex

		// check for another structure on the same hex, if found, see if we can upgrade it or not..
		for(int i=0;i<pPlanet->childCount() && !pUpgrade;++i)
		{
			NounStructure * pStructure = WidgetCast<NounStructure>( pPlanet->child(i) );
			if (! pStructure )
				continue;
			if ( pStructure->hex() == pDrop->hex() )
				pUpgrade = pStructure;
		}
	}

	if ( pUpgrade != NULL )
	{
		if ( pUpgrade->isBuilding() )
			return false;		// do not allow incomplete structures to be upgraded, this would get exploited..
		if (! pStructure->canUpgrade( pUpgrade ) )
			return false;		// nope, new structure can upgrade from old structure..
		if ( pStructure->nounContext() == pUpgrade->nounContext() )
			return false;		// prevent users from upgrading to the same structure ..
		pPlanet = pUpgrade->planet();
	}

	if (! pPlanet )
		return false;		// invalid planet pointer
	if (! pPlanet->canBuild( pBuilder, pUpgrade != NULL ) )
		return false;		// planet disallows the build...
	if (! pStructure->canBuild( pPlanet, pUpgrade != NULL ) )
		return false;		// structure disallows the build...
	if ( pStructure->buildCost() > (pPlanet->resourceCount() + pBuilder->resourceCount()) )
		return false;		// not enough resources to build...

	Array< NounStructure *> structures;
	pGadget->getStructures( structures );

	if ( structures.search( pStructure ) < 0 )
		return false;		// structure not found in the list of structure from the gadget, this shouldn't happen..
	if (! pGadget->inBuildRange( pTarget ) )
		return false;

	return true;
}

GadgetBuild * VerbBuildStructure::getGadgetBuild( NounShip * pBuilder )
{
	return GadgetBuild::getGadgetBuild( pBuilder );
}

// gets all structures available to build technology wise... does not take in account lack of resources
bool VerbBuildStructure::getStructures( NounShip * pBuilder, Noun * pTarget, Array< NounStructure * > & structures )
{
	if (! pTarget || !pBuilder )
		return false;
	// if we have a ship that is within range, get the object we can build on the surface
	GadgetBuild * pGadget = getGadgetBuild( pBuilder );
	if (! pGadget )
		return false;
	if (! pGadget->inBuildRange( pTarget ) )
		return false;

	pGadget->getStructures( structures );
	return structures.size() > 0;
}

static int StructureCompare( NounStructure * e1, NounStructure * e2 )
{
	int nGroup = e1->groupId() - e2->groupId();
	if ( nGroup != 0 )
		return nGroup;
	return e1->sortId() - e2->sortId();
}

void VerbBuildStructure::sortStructures( Array< NounStructure * > & structures )
{
	structures.qsort( StructureCompare );
}

//----------------------------------------------------------------------------
//EOF
