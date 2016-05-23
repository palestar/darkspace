/*
	LayoutUnitBuild.cpp

	This window displays the interface for all units that can be built by the current target
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "DarkSpace/VerbBuildUnit.h"
#include "Interface/GameDocument.h"
#include "Interface/ButtonBuildUnit.h"
#include "Interface/WindowSeperator.h"
#include "Interface/LayoutUnitBuild.h"

//----------------------------------------------------------------------------

Constant UPDATE_UNIT_BUILD_LAYOUT( "UPDATE_UNIT_BUILD_LAYOUT", 5.0f );

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( LayoutUnitBuild, WindowLayout );
REGISTER_FACTORY_KEY( LayoutUnitBuild, 4543244270911110555 );

LayoutUnitBuild::LayoutUnitBuild()
{}

//----------------------------------------------------------------------------

void LayoutUnitBuild::onActivate()
{
	WindowLayout::onActivate();

	detachAllNodes();
}

void LayoutUnitBuild::onDeactivate()
{
	WindowLayout::onDeactivate();

	m_Target = NULL;
	detachAllNodes();
}

void LayoutUnitBuild::onUpdate( float t )
{
	WindowLayout::onUpdate( t );

	/*
	if ( visible() )
	{
		GameDocument * pDoc = dynamic_cast<GameDocument *>( document() );
		ASSERT( pDoc );
		NounShip * pShip = pDoc->ship();
		if (! pShip )
			return;

		Noun * pTarget = pDoc->planetTarget();
		if ( m_Target != pTarget || activeTime() > UPDATE_UNIT_BUILD_LAYOUT )
		{
			int buttonCount = 0;

			setActiveTime( 0 );

			m_Target = pTarget;
			if ( pTarget != NULL )
			{
				// get all barracks on the planet
				Array< StructureBarracks * > barracks;
				GetNouns<StructureBarracks>( pTarget, barracks );

				if ( barracks.size() > 0 )
				{
					// get all units that can be built by these barracks
					Array< NounUnit * > units;
					for(int i=0;i<barracks.size();i++)
						VerbBuildUnit::getUnits( barracks[i], pShip, units );
					
					// add build buttons for all unit types, use the barracks that has the smallest queue to build each unit
					for(i=0;i<units.size();i++)
					{
						NounUnit * pUnit = units[i];

						// find barracks that can make this unit firstly, then find barracks with the smallest build queue
						StructureBarracks * pBarracks = NULL;
						for(int j=0;j<barracks.size();j++)
						{
							StructureBarracks * pCheck = barracks[j];

							if (! pCheck->canBuild( pUnit, false ) )
								continue;		// this barracks cannot build this unit
							if ( pBarracks == NULL || pCheck->buildQueueCount() < pBarracks->buildQueueCount() )
								pBarracks = pCheck;
						}

						ASSERT( pBarracks != NULL );

						// add/update button to build the unit
						GetButton<ButtonBuildUnit>( this, buttonCount++ )->setButton( pBarracks, pUnit );
					}
				}
				else
					GetButton<WindowSeperator>( this, buttonCount++)->setSeperator( 10, "NO BARRACKS" );
			}

			// remove any extra buttons
			cullChildren( buttonCount );
		}
	}
	*/

}

//----------------------------------------------------------------------------
//EOF
