/*
	NounShipCargo.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Debug/Trace.h"
#include "Math/Helpers.h"
#include "NounShip.h"
#include "NounPlanet.h"
#include "NounUnit.h"
#include "NounPod.h"
#include "StructurePort.h"
#include "VerbLoad.h"
#include "VerbUnload.h"
#include "GameContext.h"
#include "resource.h"

//----------------------------------------------------------------------------

int NounShip::unitCount() const
{
	int nUnits = 0;
	for(int i=0;i<childCount();i++)
		if ( WidgetCast<NounUnit>( child(i) ) )
			nUnits += 1;

	return nUnits;
}

int	NounShip::friendlyUnitCount() const
{
	int nUnits = 0;
	for(int i=0;i<childCount();i++)
	{
		NounUnit * pUnit = WidgetCast<NounUnit>( child(i) );
		if ( pUnit && isFriend( pUnit ) )
			nUnits += 1;
	}

	return nUnits;
}

int NounShip::enemyUnitCount() const
{
	int nUnits = 0;
	for(int i=0;i<childCount();i++)
	{
		NounUnit * pUnit = WidgetCast<NounUnit>( child(i) );
		if ( pUnit && isEnemy( pUnit ) )
			nUnits += 1;
	}

	return nUnits;
}

int NounShip::cargoCount() const
{
	int cargo = 0;
	for(int i=0;i<childCount();i++)
	{
		BaseNode * pChild = child(i);
		if ( WidgetCast<NounUnit>( pChild ) )
			cargo += 1;
		else if ( WidgetCast<NounCargo>( pChild ) )
			cargo += ((NounCargo *)pChild)->size();
	}

	return cargo;
}

int NounShip::availableCargo() const
{
	int nAvailable = maxCargo() - cargoCount() - m_Pods.size();
	if ( nAvailable < 0 )
		nAvailable = 0;

	return nAvailable;
}

int NounShip::podCount() const
{
	return m_Pods.size();
}

//----------------------------------------------------------------------------

bool NounShip::canLoad( Noun * pLoad )
{
	return VerbLoad::canLoad( pLoad, this );
}

bool NounShip::canUnload( Noun * pTarget, Noun * pUnload ) 
{
	return VerbUnload::canUnload( pTarget, pUnload, this );
}

bool NounShip::verbLoad( Noun * pLoad )
{
	if (! VerbLoad::canLoad( pLoad, this ) )
		return false;

	Verb::Ref( new VerbLoad( this, pLoad ) );
	return true;
}

bool NounShip::verbUnload( Noun * pTarget, Noun * pUnload )
{
	if (! canUnload( pTarget, pUnload ) )
		return false;

	Verb::Ref( new VerbUnload( this, pTarget, pUnload ) );
	return true;
}

bool NounShip::verbJettison( Noun * pJettison )
{
	if (! canUnload( this, pJettison ) )
		return false;

	Verb::Ref( new VerbUnload( this, this, pJettison ) );
	return true;
}

void NounShip::onPodAttached( NounPod * pPod )
{
	TRACE( CharString().format( "NounShip::onPodAttached, pShip = %p, pPod = %p, isFriend = %d", this, pPod, isFriend( pPod ) ) );

	// only account for incoming friendly pods, enemy/neutral pods are ignored and do not consume cargo space
	if ( isFriend( pPod ) )
		m_Pods.push( pPod );
}

void NounShip::onPodDetached( NounPod * pPod )
{
	TRACE( CharString().format( "NounShip::onPodDetached, pShip = %p, pPod = %p", this, pPod ) );
	m_Pods.removeSearch( pPod );
}

//----------------------------------------------------------------------------
//EOF
