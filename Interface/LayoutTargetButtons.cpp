/*
	LayoutTargetButtons.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "World/NounZoneTarget.h"
#include "Interface/GameDocument.h"
#include "Interface/LayoutTargetButtons.h"
#include "Interface/ButtonBuildStructure.h"
#include "Interface/ButtonBuildPlatform.h"
#include "Interface/ButtonUnload.h"
#include "Interface/ButtonBuildUnit.h"
#include "Interface/ButtonRefit.h"
#include "Interface/WindowSeperator.h"
#include "DarkSpace/VerbBuildStructure.h"
#include "DarkSpace/VerbBuildPlatform.h"
#include "DarkSpace/VerbBuildUnit.h"
#include "DarkSpace/VerbRefitGadget.h"

//----------------------------------------------------------------------------

Constant UPDATE_TARGET_BUTTON_LAYOUT( "UPDATE_TARGET_BUTTON_LAYOUT", 1.0f );

static int CompareCargo( NounCargo * p1, NounCargo * p2 )
{
	return _stricmp( p1->name(), p2->name() );
}

static int CompareGadgets( NounGadget * p1, NounGadget * p2 )
{
	return _stricmp( p1->name(), p2->name() );
}

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( LayoutTargetButtons, WindowLayout );
REGISTER_FACTORY_KEY( LayoutTargetButtons, 4360885711807859734 );

LayoutTargetButtons::LayoutTargetButtons()
{}

//----------------------------------------------------------------------------

void LayoutTargetButtons::onActivate()
{
	m_LastTarget = NULL;
	detachAllNodes();
}

void LayoutTargetButtons::onDeactivate()
{
	detachAllNodes();
	m_LastTarget = NULL;
}

void LayoutTargetButtons::onUpdate( float t )
{
	WindowLayout::onUpdate( t );

	if ( visible() )
	{
		GameDocument * pDoc = WidgetCast<GameDocument>( document() );
		ASSERT( pDoc );

		Noun * pTarget = pDoc->target();
		if ( pTarget != m_LastTarget || activeTime() > UPDATE_TARGET_BUTTON_LAYOUT )
		{
			// set the last target
			m_LastTarget = pTarget;
			// number of buttons added
			int nButtonCount = 0;
			// reset active time
			setActiveTime( 0.0f );

			NounShip * pShip = pDoc->ship();
			if ( pShip != NULL && pShip->validateTarget( pTarget ) )
			{
				if ( WidgetCast<NounDropTarget>( pTarget ) )
				{
					NounDropTarget * pDrop = (NounDropTarget *)pTarget;
					ASSERT( pDrop );

					// get the planet
					NounPlanet * pPlanet = pDrop->planet();
					if ( pPlanet != NULL && pShip->isFriend( pPlanet ) )
					{
						// make a list of all structures that can be built at this location
						Array< NounStructure * > build;
						if ( VerbBuildStructure::getStructures( pShip, pDrop, build ) )
						{
							VerbBuildStructure::sortStructures( build );

							// create/update buttons
							for(int i=0;i<build.size();i++)
							{
								if ( i > 0 && build[i]->groupId() != build[i-1]->groupId() )
									GetButton<WindowSeperator>( this, nButtonCount++ );
								GetButton<ButtonBuildStructure>( this, nButtonCount++ )->setButton( pDrop, build[i] );
							}

							GetButton<WindowSeperator>( this, nButtonCount++ );
						}
					}
				}
				if ( WidgetCast<NounStructure>( pTarget ) )
				{
					NounStructure * pStructure = (NounStructure *)pTarget;
					ASSERT( pStructure );

					// get the planet
					NounPlanet * pPlanet = pStructure->planet();
					if ( pPlanet != NULL && pShip->isFriend( pPlanet ) )
					{
						// make a list of all structures that can be built at this location
						Array< NounStructure * > build;
						if ( VerbBuildStructure::getStructures( pShip, pStructure, build ) )
						{
							VerbBuildStructure::sortStructures( build );

							// create/update buttons
							for(int i=0;i<build.size();i++)
							{
								if ( i > 0 && build[i]->groupId() != build[i-1]->groupId() )
									GetButton<WindowSeperator>( this, nButtonCount++ );
								GetButton<ButtonBuildStructure>( this, nButtonCount++ )->setButton( pStructure, build[i] );
							}

							GetButton<WindowSeperator>( this, nButtonCount++ );
						}
					}
				}
				if ( WidgetCast<NounZoneTarget>( pTarget ) )
				{
					NounZoneTarget * pZoneTarget = (NounZoneTarget *)pTarget;

					Array< ShipPlatform *> platforms;
					if ( VerbBuildPlatform::getPlatforms( pShip, pZoneTarget, platforms ) )
					{
						// create/update buttons
						for(int i=0;i<platforms.size();i++)
							GetButton<ButtonBuildPlatform>( this, nButtonCount++ )->setButton( pZoneTarget, platforms[i] );

						GetButton<WindowSeperator>( this, nButtonCount++ );
					}
				}
				if ( WidgetCast<StructureBarracks>( pTarget ) )
				{
					StructureBarracks * pBarracks = (StructureBarracks *)pTarget;
					ASSERT( pBarracks );

					if ( pShip->isFriend( pBarracks ) )
					{
						Array< NounUnit * > units;
						VerbBuildUnit::getUnits( pBarracks, pShip, units );

						// create/update buttons
						for(int i=0;i<units.size();i++)
							GetButton<ButtonBuildUnit>( this, nButtonCount++)->setButton( pBarracks, units[i] );
					}
				}
				if ( WidgetCast<NounGadget>( pTarget ) && pTarget->parent() == pShip )
				{
					NounGadget * pGadget = (NounGadget *)pTarget;

					// get our available upgrades
					Array< NounGadget * > upgrades;
					if ( pGadget->getUpgrades( pShip->factionId(), upgrades ) )
					{
						upgrades.qsort( CompareGadgets );
						for(int i=0;i<upgrades.size();i++)
							GetButton<ButtonRefit>( this, nButtonCount++)->setButton( pGadget, upgrades[i] );
					}
				}
			} 

			// remove any extra buttons
			cullChildren( nButtonCount );
		}
	} // if (visible())

}

//----------------------------------------------------------------------------
//EOF
