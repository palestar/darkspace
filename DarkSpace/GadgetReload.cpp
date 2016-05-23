/*
	GadgetReload.cpp
	(c)2000 Palestar, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Math/Helpers.h"
#include "NounPlanet.h"
#include "GadgetReload.h"
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

IMPLEMENT_ABSTRACT_FACTORY( GadgetReload, NounGadget ); 
REGISTER_FACTORY_KEY( GadgetReload, 4744430658981611336LL );

BEGIN_ABSTRACT_PROPERTY_LIST( GadgetReload, NounGadget );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Reloading );
	ADD_REPLICATED_PROPERTY( m_Reloads, TICKS_PER_SECOND * 10 );
END_PROPERTY_LIST();

GadgetReload::GadgetReload() : m_nReloadTick( 0 )
{
	m_DroneCount = 0;
	m_Reloads = 0;
}

//----------------------------------------------------------------------------

void GadgetReload::render( RenderContext & context, const Matrix33 & frame, const Vector3 & position )
{
	NounGadget::render( context, frame, position );
}

//----------------------------------------------------------------------------

void GadgetReload::setup()
{
	NounGadget::setup();
	m_Reloads = maxReloads();
}

void GadgetReload::release()
{
	NounGadget::release();
	m_Reloading = NULL;
}

void GadgetReload::simulate( dword nTick )
{
	if ( m_Reloading.valid() && nTick >= m_nReloadTick )
	{
		NounShip * pReload = reloading();
		ASSERT( pReload );

		m_nReloadTick = nTick + (TICKS_PER_SECOND * calculateModifier( MT_RELOAD, true ));
	
		// get our parent ship (might not be attached to a ship)
		NounBody * pParent = parentBody();

		bool bValid = true;
		if ( WidgetCast<NounShip>( pParent ) 
			&& ((NounShip *)pParent)->testFlags( NounShip::FLAG_CLOAKED ) )
			bValid = false;			// can't reload while cloaked
		else if ( m_Reloads <= 0 || destroyed() )
			bValid = false;			// out of reloads or this device is destroyed
		else if ( pReload->isDestroyed() || pReload->zone() == NULL 
			|| (worldPosition() - pReload->worldPosition()).magnitude() > reloadRange() )
			bValid = false;			// target is destroyed, detached, or out of range

		int nRepairRate = repairRate();
		if ( pReload->isOutOfCombat() )
			nRepairRate *= 1.5;

		if ( bValid && (pReload->reload( pParent ) || pReload->repair( pParent, nRepairRate, true )) )
		{
			m_Reloads--;

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
		}
		else if ( isLocal() )
			Verb::Ref( new VerbGadgetOff( this ) );
	}

	NounGadget::simulate( nTick );
}

//----------------------------------------------------------------------------

NounGadget::Type GadgetReload::type() const
{
	return DRONE_REPAIR;
}

dword GadgetReload::hotkey() const
{
	return 'Y';
}

CharString GadgetReload::status() const
{
	return CharString().format("%d", reloads());
}

bool GadgetReload::usable( Noun * pTarget, bool shift ) const
{
	if ( m_Reloading.valid() )
		return true;		// allow stop reloading
	if (! NounGadget::usable( pTarget, shift ) )
		return false;
	if ( destroyed() )
		return false;		// gadget destroyed
	if ( reloads() <= 0 )
		return false;		// no more reloads 
	if ( WidgetCast<NounShip>( parent() ) && ((NounShip *)parent())->testFlags( NounShip::FLAG_CLOAKED ) )
		return false;		
	if ( shift )
		return true;		// allow self reloads
	if ( WidgetCast<NounShip>( pTarget ) == NULL )
		return false;		// make sure the target is another ship

	NounBody * pParent = parentBody();
	if ( pParent != NULL && !pParent->isFriend( pTarget ) )
		return false;		// reload only friendly targets
	float distance = (worldPosition() - pTarget->worldPosition()).magnitude();
	if ( distance > reloadRange() )
		return false;		// out of range

 	return ((NounShip *)pTarget)->needReload( pParent ) 
		|| ((NounShip *)pTarget)->needRepair( pParent );
}

bool GadgetReload::useActive() const
{
	return m_Reloading.valid();
}

bool GadgetReload::needReload( Noun * pWhom ) const
{
	if ( WidgetCast<NounPlanet>( pWhom ) != NULL 
		|| WidgetCast<ShipPlatform>( pWhom ) )
	{
		return reloads() < maxReloads();
	}

	return false;
}

bool GadgetReload::reload( Noun * pWhom )
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

void GadgetReload::use( dword when, Noun * pTarget, bool shift)
{
	if ( shift )
	{
		message( "<color;ffffff>Engineering: Reloading Self." );
		m_Reloading = (NounShip *)parent();
	}
	else if ( m_Reloading.valid() )
	{
		message( "<color;ffffff>Engineering: Reload Aborted." );
		m_Reloading = NULL;
	}
	else
	{
		ASSERT( WidgetCast<NounShip>( pTarget ) );

		// reload the ship
		m_Reloading = (NounShip *)pTarget;
		message( CharString().format( "<color;ffffff>Engineering: Reloading %s.", m_Reloading->name()) );

		NounGadget::use( when, pTarget, shift );
	}
}

bool GadgetReload::updateLogic()
{
	if ( useActive() || reloads() <= 0 )
		return true;

	if ( WidgetCast<NounShip>( parent() ) )
	{
		NounShip * pShip = (NounShip *)parent();

		Noun * pReloadTarget = pShip->commandTarget();
		if ( !pReloadTarget || !isFriend( pReloadTarget ) || !usable( pReloadTarget, false ) )
		{
			// can't reload command target, look for another ship to reload...
			float fBestRange = 0.0f;
			pReloadTarget = NULL;
			
			for(int k=0;k<pShip->contactCount();++k)
			{
				Noun * pContact = pShip->contact( k );
				if ( isFriend( pContact ) && usable( pContact, false ) )
				{
					float fRange = (pContact->worldPosition() - pShip->worldPosition()).magnitude();
					if ( fRange < fBestRange || !pReloadTarget )
					{
						pReloadTarget = pContact;
						fBestRange = fRange;
					}
				}
			}
		}

		if ( pReloadTarget )
			pShip->useGadget( this, pReloadTarget, false );

		return true;
	}
	else if ( WidgetCast<StructureDefense>( parent() ) )
	{
		StructureDefense * pStructure = (StructureDefense *)parent();
		if (! pStructure->active() )
		{
			// stop reloading if reloading any ship...
			if ( useActive() )
				pStructure->useGadget( NULL, this );
			return true;
		}
		NounPlanet * pPlanet = pStructure->planet();
		if (! pPlanet )
			return false;

		NounShip *	pBestTarget = NULL;
		float		fBestRange = 10000.0f;

		for(int j=0;j<pPlanet->contactCount();j++)
		{
			NounShip * pShip = WidgetCast<NounShip>( pPlanet->contact( j ) );
			if ( pShip != NULL && pPlanet->isFriend( pShip ) )
			{
				float fRange = (worldPosition() - pShip->worldPosition()).magnitude();
				if ( (!pBestTarget || fRange < fBestRange) && usable( pShip, false ) )
				{
					pBestTarget = pShip;
					fBestRange = fRange;
				}
			}
		}

		if ( pBestTarget != NULL )
			pStructure->useGadget( pBestTarget, this );

		return true;
	}

	return false;
}

//----------------------------------------------------------------------------

int GadgetReload::reloads() const
{
	return m_Reloads;
}

GadgetReload * GadgetReload::getGadgetReload( NounShip * pShip, bool bUsable /*= false*/ )
{
	for(int i=0;i<pShip->childCount();i++)
	{
		GadgetReload * pReload = WidgetCast<GadgetReload>( pShip->child(i) );
		if (! pReload )
			continue;
		if ( bUsable && (pReload->useActive() || pReload->reloads() <= 0) )
			continue;

		return pReload;
	}

	return NULL;
}

//----------------------------------------------------------------------------

void GadgetReload::droneUpdate( NounDrone * pDrone )
{
	ASSERT( pDrone != NULL );
	ASSERT( pDrone->context() != NULL );

	float distance = (pDrone->worldPosition() - worldPosition()).magnitude();
	if ( context() == NULL || distance > (reloadRange() * 2) )
	{
		pDrone->setDetach();
		return;
	}

	NounShip * pReload = m_Reloading;
	if ( pDrone->target() != pReload )
	{
		if ( pReload != NULL )
		{
			// pick a new target position on the ship
			Vector3 offset( RandomVector( -1.0f, 1.0f ) );
			offset.normalize();

			pDrone->setTarget( pReload, offset * pReload->radius() );
			pDrone->setState( NounDrone::GO_TARGET );
			pDrone->setDuration( TICKS_PER_SECOND * 2 );
		}
		else
		{
			if ( pDrone->state() != NounDrone::GO_HOME &&
				pDrone->state() != NounDrone::AT_HOME )
				pDrone->setState( NounDrone::GO_HOME );
		}
	}
}

void GadgetReload::droneAction( NounDrone * pDrone )
{
	NounShip * pReload = m_Reloading;
	if ( pReload != NULL )
	{
		if ( reloads() <= 0 )
			return;
		if ( destroyed() || pReload->isDestroyed() )
			return;
		if ( ((worldPosition() - pReload->worldPosition()).magnitude()) > reloadRange() )
			return;
		if (! pReload->needReload( parentBody() ) )
			return;

		// pick a new target position on the ship
		Vector3 offset( RandomVector( -1.0f, 1.0f ) );
		offset.normalize();

		pDrone->setTarget( pReload, offset * pReload->radius() );
		pDrone->setState( NounDrone::GO_TARGET );
		pDrone->setDuration( TICKS_PER_SECOND * 2 );
	}
}

void GadgetReload::droneHome( NounDrone * pDrone )
{
	m_DroneCount--;
}

//----------------------------------------------------------------------------
// EOF
