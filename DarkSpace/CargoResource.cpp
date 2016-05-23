/*
	CargoResource.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Standard/Limits.h"
#include "NounPlanet.h"
#include "VerbLoad.h"
#include "NounShip.h"
#include "CargoResource.h"
#include "StructurePort.h"
#include "ShipPlatform.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

int				MAX_RESOURCES = 5000;
const int		CREDITS_PER_RESOURCE = 100;
const int		RESOURCE_DECAY_RATE = 20;
const dword		CARGO_STEP_RATE = TICKS_PER_SECOND * 10;

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( CargoResource, NounCargo );
REGISTER_FACTORY_KEY( CargoResource, 4286650252182582086LL );

BEGIN_PROPERTY_LIST( CargoResource, NounCargo )
END_PROPERTY_LIST();

CargoResource::CargoResource() : m_nUpdateTick( 0 )
{}

CargoResource::CargoResource( int nQuantity, bool bClamp /*= true*/ ) : m_nUpdateTick( 0 )
{
	setQuantity( bClamp ? Min( nQuantity,  MAX_RESOURCES ) : nQuantity );
}

CargoResource::CargoResource( NounPlanet * pPlanet ) : m_nUpdateTick( 0 )
{
	ASSERT( pPlanet );

	// take resources from the planet
	int nQuantity = Min<int>( pPlanet->resources(), MAX_RESOURCES );
	// set the type and quantity
	setQuantity( pPlanet->useResources( nQuantity ) );
}

//----------------------------------------------------------------------------

bool CargoResource::read( const InStream & input )
{
	if (! NounCargo::read( input ) )
		return false;
	updateName();
	return true;
}

//----------------------------------------------------------------------------

void CargoResource::onAttached()
{
	NounCargo::onAttached();

	if ( isServer() )
	{
		if ( WidgetCast<NounPlanet>( parent() ) ) 		// merge all resources into the planet
		{
			NounPlanet * pPlanet = (NounPlanet *)parent();
			pPlanet->addResources( quantity() );
			setDetach();
		}
		else if (WidgetCast<ShipPlatform>( parent() ) )
		{
			ShipPlatform * pPlatform = (ShipPlatform *)parent();
			pPlatform->addResouces( quantity() );
			setDetach();
		}
		else if ( WidgetCast<StructurePort>( parent() ) )
		{
			StructurePort * pPort = (StructurePort *)parent();

			// NOTE: Check if the port already has this resource on attachment, detach if i
			// firstly, add our resources to the planet
			NounPlanet * pPlanet = pPort->planet();
			if ( validateNoun( pPlanet ) )
			{
				// add the resources to the planet
				pPlanet->addResources( quantity() );
				// match the amount on the planet
				m_Quantity = pPlanet->resources();

				if ( pPort->findCargoResource() != this )
					setDetach();
			}
			else
				setDetach();
		}
#if 0
		else if ( WidgetCast<NounShip>( parent() ) && userId() != 0 )
		{
			NounShip * pShip = (NounShip *)parent();
			gameContext()->gameUser()->onResources( pShip, quantity() );
			setDetach();
		}
#endif
	}
}

void CargoResource::onDetaching()
{
	NounCargo::onDetaching();
}

//----------------------------------------------------------------------------

void CargoResource::initialize()
{
	m_nUpdateTick = tick() + seed( CARGO_STEP_RATE );
	NounCargo::initialize();
}

void CargoResource::simulate( dword nTick )
{
	if ( nTick >= m_nUpdateTick )
	{
		m_nUpdateTick += CARGO_STEP_RATE;

		if ( isServer() )
		{
			BaseNode * pParent = parent();
			if ( WidgetCast<NounShip>( pParent ) )
			{
				// merge our resources into other containers, detach from the ship
				NounShip * pShip = (NounShip *)pParent;
				for(int i=0;i<pShip->childCount();i++)
				{
					CargoResource * pSibling = WidgetCast<CargoResource>( pShip->child(i) );
					if ( pSibling != NULL && pSibling != this && pSibling->key() > key() )
						if ( pSibling->quantity() < pSibling->maxQuantity() )
						{
							int merge = Min( pSibling->maxQuantity() - pSibling->quantity(), quantity() );

							// add our resources into our sibling
							pSibling->setQuantity( pSibling->quantity() + merge );
							// remove our resources sent to our sibling
							setQuantity( quantity() - merge );
						}
				}
			}
			else if ( WidgetCast<StructurePort>( pParent ) != NULL )
			{
				StructurePort * pPort = (StructurePort *)pParent;

				if ( pPort->findCargoResource() != this )
					setDetach();		// we already have another CargoResource in the port for this planet

				// set our quantity to be the same as the planet
				NounPlanet * pPlanet = pPort->planet();
				if ( validateNoun( pPlanet ) )
					m_Quantity = pPlanet->resources();
				else
					m_Quantity = 0;
			}
			else
			{
				// if left in space, slowly go away, once we reach 0 then this will be detached
				setQuantity( Max( quantity() - RESOURCE_DECAY_RATE, 0 ) );
			}
		}
		else
		{
			updateName();
		}
	}

	// NounCargo::simulate( dword nTick ) will remove any cargo objects from the world who have the quantity <= 0
	NounCargo::simulate( nTick );
}

//----------------------------------------------------------------------------

int CargoResource::durability() const
{
	return 100;
}

int CargoResource::value() const
{
	return 10 * quantity();
}

int CargoResource::maxQuantity() const
{
	return MAX_RESOURCES;
}

void CargoResource::setQuantity( int quantity )
{
	if ( WidgetCast<StructurePort>( parent() ) )
	{
		// merge our resources into other containers
		StructurePort * pPort = (StructurePort *)parent();
		// set our quantity to be the same as the planet
		NounPlanet * pPlanet = pPort->planet();
		if ( validateNoun( pPlanet ) )
		{
			int nDiff = quantity - m_Quantity;
			if ( nDiff > 0 )
				pPlanet->addResources( nDiff );
			else
				pPlanet->useResources( -nDiff );
		}
	}

	// update the quantity
	m_Quantity = quantity;
	// update the resource name
	updateName();
}

//----------------------------------------------------------------------------

NounPlanet * CargoResource::planet() const
{
	BaseNode * pNode = parent();
	while( pNode != NULL )
	{
		if ( WidgetCast<NounPlanet>( pNode ) )
			return (NounPlanet *)pNode;

		pNode = pNode->parent();
	}

	// not on planet
	return NULL;
}

//----------------------------------------------------------------------------

void CargoResource::updateName()
{
	setName( CharString().format("%u Resources", quantity() ) );
}

//----------------------------------------------------------------------------
//EOF
