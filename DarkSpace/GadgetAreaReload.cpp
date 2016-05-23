/**
	@file GadgetAreaReload.cpp

	(c)2009 Palestar Inc
	@author Richard Lyle @date 6/13/2011 9:49:40 PM
*/

//---------------------------------------------------------------------------------------------------

#include "Debug/Assert.h"
#include "Math/Helpers.h"
#include "NounPlanet.h"
#include "GadgetAreaReload.h"
#include "GadgetWeapon.h"
#include "GadgetArmor.h"
#include "GadgetJumpDrive.h"
#include "GameContext.h"
#include "resource.h"
#include "StructureDefense.h"
#include "ShipPlatform.h"
#include "VerbGadgetOff.h"

//----------------------------------------------------------------------------

static Constant GADGET_REPAIR_POINTS( "GADGET_REPAIR_POINTS", 0.03f );
static Constant GADGET_REPAIR_MAX_DRONES( "GADGET_REPAIR_MAX_DRONES", 5 );

//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_FACTORY( GadgetAreaReload, NounGadget ); 

BEGIN_ABSTRACT_PROPERTY_LIST( GadgetAreaReload, NounGadget );
	ADD_REPLICATED_PROPERTY( m_Flags, TICKS_PER_SECOND );
	ADD_REPLICATED_PROPERTY( m_Reloads, TICKS_PER_SECOND * 10 );
END_PROPERTY_LIST();

GadgetAreaReload::GadgetAreaReload() : 
	m_nReloadTick( 0 ),
	m_nNextReloadIndex( 0 ),
	m_Reloads( 0 ),
	m_DroneCount( 0 )
{}

//----------------------------------------------------------------------------

void GadgetAreaReload::render( RenderContext & context, const Matrix33 & frame, const Vector3 & position )
{
	if ( useActive() )
	{
		Scene * pUseEffect = useEffect();
		if ( pUseEffect != NULL )
			pUseEffect->render( context, frame, position );
	}
}

//----------------------------------------------------------------------------

void GadgetAreaReload::setup()
{
	NounGadget::setup();
	m_Reloads = maxReloads();
}

void GadgetAreaReload::release()
{
	NounGadget::release();
	m_Reloading.release();
}

static int ReloadSort( GadgetAreaReload::NounCollision e1, GadgetAreaReload::NounCollision e2 )
{
	return ((int)e1.pNoun->key()) - ((int)e2.pNoun->key());
}

void GadgetAreaReload::simulate( dword nTick )
{
	if ( nTick >= m_nReloadTick && useActive() )
	{
		m_nReloadTick = nTick + (TICKS_PER_SECOND * calculateModifier( MT_RELOAD, true ));
	
		// get our parent ship (might not be attached to a ship)
		NounBody * pParent = parentBody();

		bool bValid = true;
		if ( WidgetCast<NounShip>( pParent ) 
			&& ((NounShip *)pParent)->testFlags( NounShip::FLAG_CLOAKED ) )
			bValid = false;			// can't reload while cloaked
		else if ( m_Reloads <= 0 || destroyed() )
			bValid = false;			// out of reloads or this device is destroyed

		if ( bValid )
		{
			m_Reloading.release();
			context()->proximityCheck( worldPosition(), reloadRange(), m_Reloading, CLASS_KEY(NounShip) );

			// sort collisions by ID, so we process them in the same order if possible..
			m_Reloading.qsort( ReloadSort );			
			
			bool bReloadedShip = false;
			for(int i=0;i<m_Reloading.size() && !bReloadedShip;++i)
			{
				// reload the next ship...
				if ( m_nNextReloadIndex >= m_Reloading.size() )
					m_nNextReloadIndex = 0;

				if ( m_nNextReloadIndex < m_Reloading.size() )
				{
					NounShip * pReload = WidgetCast<NounShip>( m_Reloading[ m_nNextReloadIndex++ ].pNoun );
					if ( pReload != NULL && pReload != pParent 
						&& pReload->isFriend( pParent ) 
						&& ( pReload->needReload( pParent ) || pReload->needRepair( pParent ) ) )
					{
						if ( pReload != pParent )
							pReload->reload( pParent );

						if ( pReload->isOutOfCombat() )
						{
							// increase repair by 50% if out of combat
							pReload->repair( pParent, gadgetRepairRate() * 1.5, true );
							pReload->repair( pParent, hullRepairRate() * 1.5, false );
						}
						else
						{
							pReload->repair( pParent, gadgetRepairRate(), true );
							pReload->repair( pParent, hullRepairRate(), false );
						}
						--m_Reloads;

						if ( isClient() && m_DroneCount < GADGET_REPAIR_MAX_DRONES )
						{
							m_DroneCount++;

							Vector3 offset( RandomVector( -1.0f, 1.0f ) );
							offset.normalize();

							NounDrone * pDrone = new NounDrone;
							pDrone->setHome( this, Vector3( 0.0f ) );
							pDrone->setPosition( worldPosition() );
							pDrone->setTarget( pReload, offset * pReload->radius() );
							pDrone->setSpeed( 10.0f );
							pDrone->setVelocity( 0.0f );
							pDrone->setDuration( TICKS_PER_SECOND * 2 );

							context()->attachNoun( pDrone );
						}

						if ( pReload != pParent )
							gameContext()->gameUser()->onRepair( pParent, GADGET_REPAIR_POINTS );

						// set flag to true, we only reload/repair one ship per second.. as more ships enter the AOI
						// the longer it will take to repair them.
						bReloadedShip = true;
					}
				}
			}
		}
	}

	NounGadget::simulate( nTick );
}

//----------------------------------------------------------------------------

NounGadget::Type GadgetAreaReload::type() const
{
	return DRONE_REPAIR;
}

dword GadgetAreaReload::hotkey() const
{
	return 'I';
}

CharString GadgetAreaReload::status() const
{
	return CharString().format("%d", reloads());
}

bool GadgetAreaReload::usable( Noun * pTarget, bool shift ) const
{
	if ( useActive() )
		return true;		// allow stop reloading
	if (! NounGadget::usable( pTarget, shift ) )
		return false;

	return true;
}

bool GadgetAreaReload::useActive() const
{
	return (flags() & FLAG_ACTIVE) != 0;
}

bool GadgetAreaReload::needReload( Noun * pWhom ) const
{
	if ( WidgetCast<NounPlanet>( pWhom ) != NULL 
		|| WidgetCast<ShipPlatform>( pWhom ) )
	{
		return reloads() < maxReloads();
	}

	return false;
}

bool GadgetAreaReload::reload( Noun * pWhom )
{
	if ( WidgetCast<NounPlanet>( pWhom ) != NULL 
		|| WidgetCast<ShipPlatform>( pWhom ) )
	{
		m_Reloads = Clamp( m_Reloads + 100, 0, maxReloads() );
		return m_Reloads < maxReloads();
	}

	// only planets can reload this device!
	return false;
}

void GadgetAreaReload::use( dword when, Noun * pTarget, bool shift)
{
	if ( useActive() )
		NounGadget::use( when, pTarget, shift );

	if ( isServer() )
	{
		// set the device active
		setFlags( FLAG_ACTIVE, !useActive() );
		message( CharString().format( "<color;ffffff>Engineering: %s %s.", name(), useActive() ? "Active" : "Inactive" ), true ); 
	}
}

bool GadgetAreaReload::updateLogic()
{
	if ( WidgetCast<NounShip>( parent() ) )
	{
		NounShip * pShip = (NounShip *)parent();

		bool bUsable = false;
		for(int i=0;i<pShip->contactCount() && !bUsable;++i)
		{
			NounShip * pContact = WidgetCast<NounShip>( pShip->contact(i) );
			if ( pContact == NULL )
				continue;		// not a ship
			if (! isFriend( pContact ) )
				continue;		// not a friend
			bUsable = pContact->needReload( pShip ) || pContact->needRepair( pShip );
		}

		if ( bUsable )
		{
			if (! useActive() )
				pShip->useGadget( this, NULL, false );
		}
		else
		{
			if ( useActive() )
				pShip->useGadget( this, NULL, false );		
		}

		return true;
	}
	else if ( WidgetCast<StructureDefense>( parent() ) )
	{
		StructureDefense * pStructure = (StructureDefense *)parent();
		if ( pStructure->active() && !useActive() )
			pStructure->useGadget( NULL, this );		// turn on this device 
		else if ( !pStructure->active() && useActive() )
			pStructure->useGadget( NULL, this );		// turn off this device

		return true;
	}

	return false;
}

//----------------------------------------------------------------------------

GadgetAreaReload * GadgetAreaReload::getGadgetReload( NounShip * pShip, bool bUsable /*= false*/ )
{
	for(int i=0;i<pShip->childCount();i++)
	{
		GadgetAreaReload * pReload = WidgetCast<GadgetAreaReload>( pShip->child(i) );
		if (! pReload )
			continue;
		if ( bUsable && (pReload->useActive() || pReload->reloads() <= 0) )
			continue;

		return pReload;
	}

	return NULL;
}

//----------------------------------------------------------------------------

void GadgetAreaReload::droneUpdate( NounDrone * pDrone )
{
	ASSERT( pDrone != NULL );
	ASSERT( pDrone->context() != NULL );

	float distance = (pDrone->worldPosition() - worldPosition()).magnitude();
	if ( context() == NULL || distance > (reloadRange() * 2) )
	{
		pDrone->setDetach();
		return;
	}
}

void GadgetAreaReload::droneAction( NounDrone * pDrone )
{}

void GadgetAreaReload::droneHome( NounDrone * pDrone )
{
	m_DroneCount--;
}

//---------------------------------------------------------------------------------------------------
//EOF
