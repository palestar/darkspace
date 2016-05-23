/*
	GadgetAutoRepair.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "GadgetAutoRepair.h"
#include "NounShip.h"
#include "GameContext.h"
#include "ShipPlatform.h"
#include "GadgetArmor.h"

//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_FACTORY( GadgetAutoRepair, NounGadget ); 
REGISTER_FACTORY_KEY( GadgetAutoRepair, 4369462777691072743LL );

BEGIN_ABSTRACT_PROPERTY_LIST( GadgetAutoRepair, NounGadget );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_nSupply );
END_PROPERTY_LIST();

GadgetAutoRepair::GadgetAutoRepair() : m_nSupply( 0 ), m_nRepairTick( 0 )
{}

//----------------------------------------------------------------------------

void GadgetAutoRepair::setup()
{
	NounGadget::setup();
	m_nSupply = maxSupply();
}

void GadgetAutoRepair::simulate( dword nTick )
{
	if ( nTick >= m_nRepairTick && m_nSupply > 0)
	{
		m_nRepairTick = nTick + (TICKS_PER_SECOND * calculateModifier( MT_AUTOREPAIR, true ));

		NounShip * pShip = WidgetCast<NounShip>( parent() );
		if ( pShip != NULL && m_nSupply > 0 )
		{
			clearFlags( FLAG_SHIP_DAMAGE|FLAG_GADGET_DAMAGE );

			if ( pShip->damage() > 0 )
			{
				addFlags( FLAG_SHIP_DAMAGE );

				int repairAmount = (int)( pShip->baseDamage() / hullRepairTime() );
				pShip->repair( this, repairAmount, false );
			}

			for(int i=0;i<pShip->childCount();i++)
			{
				NounGadget * pGadget = WidgetCast<NounGadget>( pShip->child(i) );
				if ( pGadget != NULL )
				{
					int nDamHP, nMaxHP;
					if ( pGadget->type() == NounGadget::ARMOR )
					{
						GadgetArmor * pGadgetArmor = WidgetCast<GadgetArmor>( pShip->child(i) );
						nMaxHP = pGadgetArmor->maxArmor();
						nDamHP = nMaxHP - pGadgetArmor->armor();
					}
					else
					{
						nMaxHP = pGadget->maxDamage();
						nDamHP = pGadget->damage();
					}
					
					if ( nDamHP > 0 )
					{
						addFlags( FLAG_GADGET_DAMAGE );

						int nGadgetRepairAmount = (int)( nMaxHP / gadgetRepairTime() );
						pGadget->repair( this, nGadgetRepairAmount );
					}
				}
			}

			if ( (flags() & (FLAG_GADGET_DAMAGE|FLAG_SHIP_DAMAGE)) != 0 )
				m_nSupply--;
		}
	}

	NounGadget::simulate( nTick );
}

//----------------------------------------------------------------------------

NounGadget::Type GadgetAutoRepair::type() const
{
	return SPECIAL;
}

CharString GadgetAutoRepair::status() const
{
	return CharString().format("%d", m_nSupply);
}

bool GadgetAutoRepair::canDamage( dword type ) const
{
	return false;
}

bool GadgetAutoRepair::needReload( Noun * pWhom ) const
{
	if ( WidgetCast<NounPlanet>( pWhom ) != NULL 
		|| WidgetCast<ShipPlatform>( pWhom ) != NULL )
	{
		return m_nSupply < maxSupply();
	}

	return false;
}

bool GadgetAutoRepair::reload( Noun * pWhom )
{
	if ( WidgetCast<NounPlanet>( pWhom ) != NULL 
		|| WidgetCast<ShipPlatform>( pWhom ) != NULL )
	{
		m_nSupply = Clamp( m_nSupply + reloadRate(), 0, maxSupply() );
		return m_nSupply < maxSupply();
	}

	return false;
}

int GadgetAutoRepair::useEnergy( dword nTick, int nEnergy )
{
	if ( (flags() & (FLAG_SHIP_DAMAGE|FLAG_GADGET_DAMAGE)) != 0 && nEnergy >= energyCost() )
	{
		addFlags( FLAG_ACTIVE );
		nEnergy -= energyCost();
	}
	else
	{
		// no damage or not enough energy, make this device inactive
		clearFlags( FLAG_ACTIVE );
	}

	return nEnergy;
}

//----------------------------------------------------------------------------

void GadgetAutoRepair::setSupply( int nSupply )
{
	m_nSupply = Clamp( nSupply, 0, maxSupply() );
}

//----------------------------------------------------------------------------
//EOF
