/*
	StructureDefense.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Debug/Profile.h"
#include "Math/Helpers.h"
#include "NounPlanet.h"
#include "NounStar.h"
#include "NounProjectile.h"
#include "NounShip.h"
#include "NounGadget.h"
#include "GadgetBeamWeapon.h"
#include "GadgetWeapon.h"
#include "GadgetReactor.h"
#include "GadgetFighterBay.h"
#include "GadgetReload.h"
#include "GadgetShield.h"
#include "GadgetJD.h"
#include "GadgetECM.h"
#include "GadgetECCM.h"
#include "VerbUseGadget.h"
#include "VerbUseSingleGadget.h"
#include "StructureDefense.h"
#include "resource.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

const int MAX_ENERGY = 1000000;
const int USE_GADGETS_INTERVAL = TICKS_PER_SECOND * 1;

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( StructureDefense, NounStructure );
REGISTER_FACTORY_KEY( StructureDefense, 4250298540790984421LL );

BEGIN_PROPERTY_LIST( StructureDefense, NounStructure )
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Children );
END_PROPERTY_LIST();

StructureDefense::StructureDefense() : m_Shielding( -1 ), m_nUseGadgets( 0 )
{}

//----------------------------------------------------------------------------

void StructureDefense::initialize()
{
	NounStructure::initialize();

	// HACK: detach any weapons or beam weapons, we no longer have defense bases that act like ships, any
	// weapon fire will be faux.. this is just here to remove it from defense bases already in-game, 
	// this code can be removed later on.
	for(int i=0;i<childCount();++i)
	{
		Noun * pNoun = WidgetCast<Noun>( child(i) );
		if (! pNoun )
			continue;

		if ( WidgetCast<GadgetWeapon>( pNoun ) != NULL ||
			WidgetCast<GadgetBeamWeapon>( pNoun ) != NULL ||
			WidgetCast<GadgetReactor>( pNoun ) != NULL )
		{
			pNoun->setDetach();
		}
	}
}

void StructureDefense::simulate( dword nTick )
{
	NounPlanet * pPlanet = planet();

	// charge the energy banks
	int nEnergy = MAX_ENERGY;
	for(int i=0;i<childCount();i++)
	{
		NounGadget * pGadget = WidgetCast<NounGadget>( child(i) );
		if ( pGadget != NULL )
			nEnergy = pGadget->useEnergy( nTick, nEnergy );
	}

	if ( ++m_nUseGadgets >= USE_GADGETS_INTERVAL )
	{
		m_nUseGadgets -= USE_GADGETS_INTERVAL;

		int nMaxShield = 0;
		int nShield = 0;

		// use gadgets
		for(int i=0;i<childCount();i++)
		{
			NounGadget * pGadget = WidgetCast<NounGadget>( child(i) );
			if ( pGadget == NULL )
				continue;

			if ( active() && WidgetCast<GadgetShield>( pGadget ) )
			{
				nMaxShield += ((GadgetShield *)pGadget)->maxCharge();
				nShield += ((GadgetShield *)pGadget)->charge();
			}

			// reload and repair gadgets
			pGadget->reload( pPlanet );
			pGadget->repair( pPlanet, 500 );

			if ( isLocal() )
				pGadget->updateLogic();

		} // end for

		flushUse();

		if ( nMaxShield > 0 )
			m_Shielding = (nShield * 100) / nMaxShield;
		else
			m_Shielding = -1;
	}

	NounStructure::simulate( nTick );	
}

int StructureDefense::sortId() const
{
	return 0;
}

int StructureDefense::groupId() const
{
	return 6;
}

int StructureDefense::maxDamage() const
{
	return 15000;
}

int StructureDefense::buildTechnology() const
{
	return 20;
}

int StructureDefense::buildTime() const
{
	return 90;
}

int StructureDefense::buildCost() const
{
	return 500;
}

int StructureDefense::repairRate() const
{
	return 600;
}

int StructureDefense::workers() const
{
	return 1;
}

int StructureDefense::power() const
{
	return -10;
}

int StructureDefense::technology() const
{
	return 20;
}

int StructureDefense::defense() const
{
	return 450 * getTechPercentage();
}

float StructureDefense::protection() const
{
	return 0.05f * getTechPercentage();		// 5% damage reduction
}

CharString StructureDefense::status() const
{
	CharString info( NounStructure::status() );

	if ( active() && m_Shielding >= 0 )
		info += CharString().format( "Shields: %d%%\n", m_Shielding );

	return info;
}

Color StructureDefense::color() const
{
	return ORANGE;
}

bool StructureDefense::canUpgrade( NounStructure * pStructure ) const
{
	if ( WidgetCast<StructureDefense>( pStructure ) )
		return true;
	return false;
}

//----------------------------------------------------------------------------

void StructureDefense::useGadget( Noun * pTarget, NounGadget * pGadget )
{
	if ( m_pUseTarget != pTarget )
		flushUse();

	m_pUseTarget = pTarget;
	m_Use.push( pGadget );
}

void StructureDefense::flushUse()
{
	if ( m_Use.size() > 0 )
		Verb::Ref( new VerbUseGadget( this, m_Use, m_pUseTarget ) );

	m_pUseTarget = NULL;
	m_Use.release();
}

//----------------------------------------------------------------------------

bool StructureDefense::isTargeted( NounPlanet * pPlanet, Noun * pTarget )
{
	bool targeted = false;
	for(int k=0;k<pPlanet->childCount() && !targeted;k++)
	{
		StructureDefense * pDefense = WidgetCast<StructureDefense>( pPlanet->child(k) );
		if ( pDefense != NULL )
		{
			for(int i=0;i<pDefense->childCount() && !targeted;i++)
			{
				GadgetBeamWeapon * pBeam = WidgetCast<GadgetBeamWeapon>( pDefense->child(i) );
				if ( pBeam != NULL && pBeam->target() == pTarget )
					targeted = true;
			}
		}
	}

	return targeted;
}

//----------------------------------------------------------------------------
//EOF
