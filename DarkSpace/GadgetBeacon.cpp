/*
	GadgetBeacon.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "GadgetBeacon.h"
#include "GameContext.h"
#include "NounBeacon.h"
#include "NounShip.h"

//----------------------------------------------------------------------------

const dword BEACON_LIFE = TICKS_PER_SECOND * 300;	

//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_FACTORY( GadgetBeacon, GadgetWeapon ); 
REGISTER_FACTORY_KEY( GadgetBeacon, 4346008654386196641LL );

BEGIN_ABSTRACT_PROPERTY_LIST( GadgetBeacon, GadgetWeapon );
END_PROPERTY_LIST();

GadgetBeacon::GadgetBeacon()
{}

//----------------------------------------------------------------------------

NounGadget::Type GadgetBeacon::type() const
{
	return BEACON;
}

//----------------------------------------------------------------------------

bool GadgetBeacon::hit( Noun * pTarget, NounProjectile * pProj, const Vector3 & intersect )
{	
	if (! GadgetWeapon::hit( pTarget, pProj, intersect ) )
		return false;

	NounBody * pTargetBody = WidgetCast<NounBody>( pTarget );
	if ( pTargetBody == NULL )
		return true;

	if ( isServer() )
	{
		Noun * pParent = parentNoun();
		// ASSERT( pParent ); User could remove device after shooting, so there will be no parent when it hits.
		
		// Limit the number of beacons which can be placed on an object
		int nBeaconCount = 0;
		NounBeacon * pFirstBeacon = NULL;
		for(int i=0;i<pTargetBody->childCount();i++)
		{
			NounBeacon * pBeacon = WidgetCast<NounBeacon>( pTargetBody->child(i) );
			if ( pBeacon != NULL )
			{
				nBeaconCount++;
				if( pFirstBeacon == NULL )
					pFirstBeacon = pBeacon;
			}
		}
		// kill old beacon if limit is exceeded
		if( nBeaconCount >= 3 )
			pFirstBeacon->setDetach();

		// get the beacon position in object space of the target we hit
		Vector3 offset( pTarget->worldFrame() * (intersect - pTarget->worldPosition()) );
		// create a beacon object and attach it to the target
		pTarget->attachNode( new NounBeacon( offset, this ) );
		// give scout points if target was an enemy ship only. Also only first 2 beacons are rewarded
		if ( pParent && WidgetCast<NounShip>( pTarget ) && nBeaconCount < 2 && isEnemy( pTarget ) )
			gameContext()->gameUser()->onScout( pParent, 1.0f );
	}

	return true;
}

//----------------------------------------------------------------------------

GadgetBeacon * GadgetBeacon::getGadgetBeacon( NounShip * pShip )
{
	for(int i=0;i<pShip->childCount();i++)
		if ( WidgetCast<GadgetBeacon>( pShip->child(i) ) )
			return (GadgetBeacon *)pShip->child(i);

	return NULL;
}

//----------------------------------------------------------------------------
//EOF
