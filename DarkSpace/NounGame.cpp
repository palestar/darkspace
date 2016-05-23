/**
	@file NounGame.cpp

	(c)2006 Palestar Inc
	@author Richard Lyle @date 4/29/2007 5:03:18 PM
*/

#include "Debug/Assert.h"
#include "NounGame.h"
#include "TraitOrbit.h"
#include "TraitDisplayName.h"
#include "NounStar.h"
#include "NounJumpGate.h"
#include "NounDropTarget.h"
#include "GameContext.h"

//---------------------------------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounGame, Noun );

BEGIN_PROPERTY_LIST( NounGame, Noun );
	ADD_PROPERTY( m_DetectionFlags );
END_PROPERTY_LIST();

//---------------------------------------------------------------------------------------------------

NounGame::NounGame() : m_DetectionFlags( 0xffffffff ), m_Signature( 0 ), m_SignatureTick( 0 ), m_LastSignature( 0 )
{}

//---------------------------------------------------------------------------------------------------

CharString NounGame::displayName( bool a_bAllowTags ) const
{
	TraitDisplayName * pDisplayName = findTrait<TraitDisplayName>();
	if ( pDisplayName != NULL )
		return pDisplayName->displayName();

	return Noun::displayName( a_bAllowTags );
}

void NounGame::update()
{
	Noun::update();
	resetSignature();
}

//----------------------------------------------------------------------------

bool NounGame::inOrbit() const
{
	return orbiting() != NULL;
}

Noun * NounGame::orbiting() const
{
	TraitOrbit * pOrbit = FindTrait<TraitOrbit>( this );
	return pOrbit != NULL ? pOrbit->orbiting() : NULL;	
}

Noun * NounGame::orbitRoot() const
{
	NounGame * pRoot = WidgetCast<NounGame>( zoneParent() );
	if ( pRoot == NULL || !pRoot->enableOrbit() )
		return NULL;

	NounGame * pNext = WidgetCast<NounGame>( pRoot->orbiting() );
	while( pNext != NULL && pNext->enableOrbit() )
	{
		pRoot = pNext;
		pNext = WidgetCast<NounGame>( pRoot->orbiting() );
	}

	return pRoot;
}

const char * NounGame::starSystem() const
{
	Noun * pRoot = orbitRoot();
	if ( WidgetCast<NounStar>( pRoot ) )
		return pRoot->name();		// return the name of the star we are orbiting

	// not orbiting, find a object in our current zone that is...
	NodeZone * pZone = zone();
	if ( pZone != NULL )		// can be NULL if the players ship is detached but still showing the in-game tactical screen..
	{
		for(int i=0;i<pZone->childCount();i++)
		{
			NounGame * pSibling = WidgetCast<NounGame>( pZone->child(i) );
			if (! pSibling )
				continue;

			pRoot = pSibling->orbitRoot();
			if ( WidgetCast<NounStar>( pRoot ) )
				return pRoot->name();
		}
	}

	return "Deep Space";
}

const char * NounGame::serverName() const
{
	GameContext * pContext = gameContext();
	if ( pContext != NULL )
		return pContext->name();
	return "";
}

//---------------------------------------------------------------------------------------------------

CharString NounGame::status() const
{
	return "";
}

bool NounGame::canInteract( NounInteraction nInteraction, Noun * pWith ) const
{
	return false;
}

bool NounGame::canDamage( dword type ) const
{
	return false;
}

bool NounGame::canOrbit() const
{
	return false;
}

bool NounGame::canShipCollide() const
{
	return true;
}

bool NounGame::canBeDetected() const
{
	return false;
}

bool NounGame::enableECM() const
{
	return false;		// by default this object doesn't emit a signatur
}

bool NounGame::enablePD() const
{
	return false;
}

float NounGame::baseSignature() const
{
	return radius();
}

float NounGame::visibility() const
{
	return 1.0f;
}

bool NounGame::isCargo() const
{
	return false;
}

bool NounGame::canAttachCargo( Noun * pCargo ) const
{
	return false;
}

int NounGame::resourceCount() const
{
	int nCount = 0;
	for(int i=0;i<childCount();++i)
	{
		CargoResource * pResource = WidgetCast<CargoResource>( child(i) );
		if (! pResource )
			continue;
		nCount += pResource->quantity();
	}
	return nCount;
}

int NounGame::useResources( int nWanted )
{
	int nUsed = 0;
	for(int i=childCount()-1;nWanted > 0 && i>=0;--i)
	{
		CargoResource * pResource = WidgetCast<CargoResource>( child(i) );
		if (! pResource )
			continue;
		
		int nAvailable = pResource->quantity();
		int nTake = Min( nAvailable, nWanted );
		pResource->setQuantity( nAvailable - nTake );

		nUsed += nTake;
		nWanted -= nTake;
	}

	return nUsed;
}

bool NounGame::canDetect( Noun * pNoun, float sensor, float view ) const
{
	if ( WidgetCast<NounGame>( pNoun ) == NULL )
	{
		LOG_ERROR(  "NounGame", "Noun %p (%s) is not a NounGame type.", pNoun, pNoun != NULL ? pNoun->className() : "NULL" );
		return false;
	}

	if ( pNoun == this )
		return true;		// we can always detect ourselves
	if (! ((NounGame *)pNoun)->canBeDetected() )
		return false;		// object is never detected
	if ( factionId() == pNoun->factionId() )
		return true;		// we can always detect objects from our own faction

	float distance = (worldPosition() - pNoun->worldPosition()).magnitude();
	float visibleRange = (pNoun->radius() * ((NounGame *)pNoun)->viewDetectScale() * ((NounGame *)pNoun)->visibility()) * view;
	float sensorRange = ((NounGame *)pNoun)->signature() * sensor;

	return distance < sensorRange || distance < visibleRange;
}

bool NounGame::validateTarget( const Noun * pNoun ) const
{
	if ( pNoun == NULL )	
		return false;		// invalid pointer
	if ( WidgetCast<NounTarget>( pNoun ) )
		return true;		// NounTarget objects are always valid
	if (! WidgetCast<NounGame>( pNoun ) )
		return false;		// Object must be a NounGame type..
	if ( pNoun->zone() == NULL )
		return false;		// object is detached from the world

	// lastly, make we the object is detected by this objects faction
	return ((NounGame *)pNoun)->isDetected( factionId() );
}

//---------------------------------------------------------------------------------------------------

void NounGame::addSignature( dword when, float sig )
{
	if ( when > m_SignatureTick )
	{
		m_Signature = baseSignature() + sig;
		m_SignatureTick = when;
	}
	else if ( when == m_SignatureTick )
		m_Signature += sig;
}

void NounGame::resetSignature()
{
	m_LastSignature = m_Signature;
	m_Signature = baseSignature();
	m_SignatureTick = 0;
}

void NounGame::setDetectionFlags( dword nFlags )
{
	m_DetectionFlags = nFlags;
}

void NounGame::interact( NounInteraction nInteraction, Noun * pWith, int nTicks /*= 1*/ )
{}

void NounGame::inflictDamage( dword nWhen, Noun * pFrom, int damage, dword type, const Vector3 & direction )
{}

//---------------------------------------------------------------------------------------------------

bool NounGame::enableOrbit() const
{
	return false;		// by default, nouns cannot orbit
}

//---------------------------------------------------------------------------------------------------

Noun * NounGame::findCargoHold( Noun * pNoun, Noun * pCargo )
{
	if ( WidgetCast<NounDropTarget>( pNoun ) )
		pNoun = ((NounDropTarget *)pNoun)->planet();

	if ( !WidgetCast<NounGame>( pNoun ) )
		return NULL;
	// check this object
	if ( ((NounGame *)pNoun)->canAttachCargo( pCargo ) )
		return pNoun;

	// check our parent object
	NounGame * pParent = WidgetCast<NounGame>( pNoun->parentNoun() );
	return pParent != NULL ? findCargoHold( pParent, pCargo ) : NULL;
}

//---------------------------------------------------------------------------------------------------
//EOF
