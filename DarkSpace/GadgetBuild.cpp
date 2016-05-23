/*
	GadgetBuild.h
	(c)1999 Palestar Development, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Math/Helpers.h"
#include "GameContext.h"
#include "CargoResource.h"
#include "GadgetBuild.h"
#include "resource.h"
#include "NounDropTarget.h"

//----------------------------------------------------------------------------

const int	BUILD_DRONES = 8;

Constant MAX_BUILD_RANGE( "MAX_BUILD_RANGE", 250.0f );
Constant BUILD_POINTS_PER_SECOND( "BUILD_POINTS_PER_SECOND", 0.05f );

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( GadgetBuild, NounGadget );
REGISTER_FACTORY_KEY( GadgetBuild, 4229590722668897107LL );

BEGIN_PROPERTY_LIST( GadgetBuild, NounGadget )
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Building );
END_PROPERTY_LIST();

GadgetBuild::GadgetBuild() : m_nBuildTick( 0 )
{
	m_DroneCount = 0;
}

//----------------------------------------------------------------------------

void GadgetBuild::render( RenderContext &context, 
						const Matrix33 & frame, 
						const Vector3 & position )
{
	NodeTransform::render( context, frame, position );
}

//----------------------------------------------------------------------------

void GadgetBuild::simulate( dword nTick )
{
	if ( nTick >= m_nBuildTick )
	{
		m_nBuildTick = nTick + (TICKS_PER_SECOND * calculateModifier( MT_BUILD_SPEED, true ));
		if ( m_Building.valid() )
		{
			NounShip * pBuilder = WidgetCast<NounShip>( parent() );
			if (! pBuilder )
				return;

			bool bComplete = false;
			bool bValid = false;

			if ( !pBuilder->testFlags( NounShip::FLAG_CLOAKED ) && !destroyed() )
			{
				if ( WidgetCast<NounStructure>( m_Building ) )
				{
					NounStructure * pStructure = (NounStructure *)m_Building.pointer();

					bValid = pStructure->planet() == pBuilder->orbiting() || 
						(worldPosition() - pStructure->worldPosition()).magnitude() < MAX_BUILD_RANGE;
					if ( bValid )
					{
						bComplete = pStructure->build();

						// reparir structure as well if damaged..
						if ( pStructure->damage() > 0 )
						{
							pStructure->repair();
							bComplete = pStructure->damage() == 0;
						}
					}
				}
				else if ( WidgetCast<ShipPlatform>( m_Building ) )
				{
					ShipPlatform * pPlatform = (ShipPlatform *)m_Building.pointer();
					bValid = (pPlatform->worldPosition() - worldPosition()).magnitude() < MAX_BUILD_RANGE;
					if ( bValid )
					{
						bComplete = pPlatform->build();

						if ( pPlatform->damage() > 0 )
							bComplete = !pPlatform->repair( pBuilder, 300, true );
					}
				}
			}

			if ( bValid )
			{
				// give builder construction points
				if ( pBuilder != NULL )
				{
					float fPoints = BUILD_POINTS_PER_SECOND * calculateModifier( MT_BUILD_SPEED, false );
					gameContext()->gameUser()->onConstruct( pBuilder, fPoints );
				}

				// update the drones...
				if ( isClient() && m_DroneCount < BUILD_DRONES )
				{
					m_DroneCount++;

					NounDrone * pDrone = new NounDrone;
					pDrone->setPosition( worldPosition() + (RandomVector( -0.1f, 0.1f ) * parent()->radius()) );
					pDrone->setTarget( m_Building, RandomVector( -0.5f, 0.5f ) * m_Building->radius() );
					pDrone->setHome( this, Vector3( 0.0f )  );
					pDrone->setSpeed( 30.0f );
					pDrone->setVelocity( 0.0f );
					pDrone->setDuration( TICKS_PER_SECOND );

					context()->attachNoun( pDrone );
				}

				if ( bComplete )
				{
					message( "<color;ffffff>Comms: Build Complete." );
					m_Building = NULL;
				}
			}
			else
			{
				message( "<color;ffffff>Comms: Aborting Build." );
				m_Building = NULL;
			}

		}
	}

	NounGadget::simulate( nTick );
}


void GadgetBuild::setup()
{
	NounGadget::setup();
	// reset the drone count
	m_DroneCount = 0;
}

void GadgetBuild::initialize()
{
	NounGadget::initialize();
}

bool GadgetBuild::postInitialize()
{
	if (! NounGadget::postInitialize() )
		return false;

	if ( m_NounContext.valid() )
	{
		// load the Scene containing all the build objects
		m_Build = WidgetCast<Scene>( m_NounContext->resource( "BUILD", false ) );
		if (! m_Build.valid() )
			return false;

		// sort the nodes by name
		if ( m_Build.valid() && m_Build->node() != NULL )
			m_Build->node()->sortNodes();
	}

	return true;
}

void GadgetBuild::release()
{
	NounGadget::release();
	// clear any structure we are building
	m_Building = NULL;
}

//----------------------------------------------------------------------------

NounGadget::Type GadgetBuild::type() const
{
	return DRONE_BUILD;
}

bool GadgetBuild::usable( Noun * pTarget, bool shift ) const
{
	if ( m_Building.valid() )
		return true;
	if (! NounGadget::usable( pTarget, shift ) )
		return false;
	if ( destroyed() )
		return false;
	if ( WidgetCast<NounShip>( parent() ) && ((NounShip *)parent())->testFlags( NounShip::FLAG_CLOAKED ) )
		return false;		
	if (! isFriend( pTarget ) )
		return false;

	if ( WidgetCast<NounStructure>( pTarget ) )
	{
		NounStructure * pStructure = (NounStructure *)pTarget;
		if (! pStructure->isBuilding() && pStructure->damage() == 0 )
			return false;		// structure is finished building and is not damaged

		return true;
	}
	else if ( WidgetCast<ShipPlatform>( pTarget ) )
	{
		ShipPlatform * pPlatform = (ShipPlatform *)pTarget;
		if (! pPlatform->isBuilding() && pPlatform->damage() == 0 )
			return false;		// platform is done building..
		return true;
	}

	return false;
}

void GadgetBuild::use( dword when, Noun * pTarget, bool shift )
{
	if ( m_Building.valid() )
	{
		message( "<color;ffffff>Comms: Aborting Build." );
		m_Building = NULL;
	}
	else if ( WidgetCast<NounStructure>( pTarget ) )
	{
		NounStructure * pStructure = (NounStructure *)pTarget;
		if ( pStructure->isBuilding() )
			message( CharString().format( "<color;ffffff>Comms: Building %s.", pStructure->name()) );
		else
			message( CharString().format( "<color;ffffff>Comms: Repairing %s.", pStructure->name()) );
		m_Building = pStructure;

		NounGadget::use( when, pTarget, shift );
	}
	else if (WidgetCast<ShipPlatform>( pTarget ) )
	{
		ShipPlatform * pPlatform = (ShipPlatform *)pTarget;
		if ( pPlatform->isBuilding() )
			message( CharString().format( "<color;ffffff>Comms: Building %s.", pPlatform->name()) );
		else
			message( CharString().format( "<color;ffffff>Comms: Repairing %s.", pPlatform->name()) );
		m_Building = pPlatform;

		NounGadget::use( when, pTarget, shift );
	}
}

int GadgetBuild::usableWhen() const
{
	if ( WidgetCast<NounStructure>( m_Building ) )
	{
		NounStructure * pStructure = (NounStructure *)m_Building.pointer();
		if ( pStructure->isBuilding() )
			return 100 - pStructure->buildComplete();
		return (pStructure->damage() * 100) / pStructure->maxDamage();
	}
	else if ( WidgetCast<ShipPlatform>( m_Building ) )
	{
		ShipPlatform * pPlatform = (ShipPlatform *)m_Building.pointer();
		if ( pPlatform->isBuilding() )
			return 100 - pPlatform->buildPercent();
		return (pPlatform->damage() * 100) / pPlatform->maxDamage();
	}

	return 0;
}

bool GadgetBuild::useActive() const
{
	return m_Building.valid();
}

int GadgetBuild::durability() const
{
	return 0;	// never survive ship death
}

int GadgetBuild::addValue() const
{
	return 10000;
}

//----------------------------------------------------------------------------

void GadgetBuild::droneUpdate( NounDrone * pDrone )
{
	ASSERT( pDrone != NULL );
	ASSERT( pDrone->context() != NULL );

	// TODO: Also don't detach if still in orbit & building, or the building just finished
	float distance = (pDrone->worldPosition() - worldPosition()).magnitude();
	if ( context() == NULL || distance > (MAX_BUILD_RANGE * 4) )
	{
		pDrone->setDetach();
		m_DroneCount--;
	}
}

void GadgetBuild::droneAction( NounDrone * pDrone )
{}

void GadgetBuild::droneHome( NounDrone * pDrone )
{
	m_DroneCount--;
}

//----------------------------------------------------------------------------

int GadgetBuild::structureCount() const
{
	return build()->node()->childCount();
}

NounStructure * GadgetBuild::structure( int n ) const
{
	return WidgetCast<NounStructure>( build()->node()->child( n ) );
}

ShipPlatform * GadgetBuild::platform( int n ) const
{
	return WidgetCast<ShipPlatform>( build()->node()->child( n ) );
}

Scene * GadgetBuild::build() const
{
	return m_Build;
}

bool GadgetBuild::inBuildRange( Noun * pTarget ) const
{
	if ( WidgetCast<NounDropTarget>( pTarget ) )
	{
		NounGame * pParent = WidgetCast<NounGame>( parentNoun() );
		if (! pParent )
			return false;
		NounDropTarget * pDropTarget = (NounDropTarget *)pTarget;
		if ( pDropTarget->planet() == pParent->orbiting() )
			return true;
	}
	else if ( WidgetCast<NounStructure>( pTarget ) )
	{
		NounGame * pParent = WidgetCast<NounGame>( parentNoun() );
		if (! pParent )
			return false;
		NounStructure * pStructure = (NounStructure *)pTarget;
		if ( pStructure->planet() == pParent->orbiting() )
			return true;
	}

	if ( (pTarget->worldPosition() - worldPosition()).magnitude() <= MAX_BUILD_RANGE )
		return true;

	return false;
}

//----------------------------------------------------------------------------

void GadgetBuild::getStructures(Array< NounStructure * > & structures )
{
	BaseNode * pRoot = build()->node();
	for(int i=0;i<pRoot->childCount();i++)
	{
		NounStructure * pStructure = WidgetCast<NounStructure>( pRoot->child(i) );
		if ( pStructure != NULL )
			structures.push( pStructure );
	}
}

void GadgetBuild::getPlatforms(Array< ShipPlatform * > & platforms )
{
	BaseNode * pRoot = build()->node();
	for(int i=0;i<pRoot->childCount();i++)
	{
		ShipPlatform * pPlatform = WidgetCast<ShipPlatform>( pRoot->child(i) );
		if ( pPlatform )
			platforms.push( pPlatform );
	}
}

//----------------------------------------------------------------------------

GadgetBuild * GadgetBuild::getGadgetBuild( NounShip * pShip )
{
	for(int i=0;i<pShip->childCount();i++)
		if ( WidgetCast<GadgetBuild>( pShip->child(i) ) )
			return (GadgetBuild *)pShip->child(i);

	return NULL;
}

//----------------------------------------------------------------------------
// EOF
