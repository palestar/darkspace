/**
	@file NounShipEnhancement.cpp

	(c)2006 Palestar Inc
	@author Richard Lyle @date 12/21/2008 5:29:55 PM
*/

#include "NounShip.h"
#include "GameContext.h"
#include "Network/LogClient.h"		// LogLevels
#include "VerbApplyEnhancement.h"
#include "DefaultLootTables.h"
#include "CargoEnhancement.h"
#include "DropData.h"

//---------------------------------------------------------------------------------------------------

IMPLEMENT_FACTORY( ShipModifier, EnhancementModifier );
BEGIN_PROPERTY_LIST( ShipModifier, EnhancementModifier );
	ADD_ENUM_PROPERTY( m_eType );
		ADD_PROPERTY_FLAGS( m_eType, PF_TRANSMIT );
		ADD_ENUM_OPTION( m_eType, MT_UNKOWN );
		ADD_ENUM_OPTION( m_eType, MT_DRIVE_VELOCITY );
		ADD_ENUM_OPTION( m_eType, MT_DRIVE_THRUST );
		ADD_ENUM_OPTION( m_eType, MT_BEAM_RANGE );
		ADD_ENUM_OPTION( m_eType, MT_BEAM_ENERGY );
		ADD_ENUM_OPTION( m_eType, MT_BEAM_DAMAGE );
		ADD_ENUM_OPTION( m_eType, MT_WEAPON_RANGE );
		ADD_ENUM_OPTION( m_eType, MT_WEAPON_ENERGY );
		ADD_ENUM_OPTION( m_eType, MT_WEAPON_DAMAGE );
		ADD_ENUM_OPTION( m_eType, MT_WEAPON_COOLDOWN );
		ADD_ENUM_OPTION( m_eType, MT_WEAPON_AMMO );
		ADD_ENUM_OPTION( m_eType, MT_DEFENSE );
		ADD_ENUM_OPTION( m_eType, MT_DEFENSE_RECHARGE );
		ADD_ENUM_OPTION( m_eType, MT_AUTOREPAIR );
		ADD_ENUM_OPTION( m_eType, MT_BUILD_SPEED );
		ADD_ENUM_OPTION( m_eType, MT_CLOAK );
		ADD_ENUM_OPTION( m_eType, MT_EWAR_RANGE );
		ADD_ENUM_OPTION( m_eType, MT_EWAR_STRENGTH );
		ADD_ENUM_OPTION( m_eType, MT_EWAR_ENERGY );
		ADD_ENUM_OPTION( m_eType, MT_JUMPDRIVE_RANGE );
		ADD_ENUM_OPTION( m_eType, MT_JUMPGATE_RANGE );
		ADD_ENUM_OPTION( m_eType, MT_MINING );
		ADD_ENUM_OPTION( m_eType, MT_PULSESHIELD );
		ADD_ENUM_OPTION( m_eType, MT_REACTOR );
		ADD_ENUM_OPTION( m_eType, MT_RELOAD );
		ADD_ENUM_OPTION( m_eType, MT_SCANNER );
		ADD_ENUM_OPTION( m_eType, MT_TRACTOR );
		ADD_ENUM_OPTION( m_eType, MT_TURNRATE );
		ADD_ENUM_OPTION( m_eType, MT_SHIPENERGY );
		ADD_ENUM_OPTION( m_eType, MT_DAMAGE_REDUCTION );
		ADD_ENUM_OPTION( m_eType, MT_CAPTURE_DEFENSE );
		ADD_ENUM_OPTION( m_eType, MT_SIGNATURE );
		ADD_ENUM_OPTION( m_eType, MT_JUMPCOOLDOWN );
		ADD_ENUM_OPTION( m_eType, MT_JUMPSPEED );
		ADD_ENUM_OPTION( m_eType, MT_MASS );
		ADD_ENUM_OPTION( m_eType, MT_CAPTURE_WEIGHT );
	ADD_PROPERTY( m_nValue );
		ADD_PROPERTY_FLAGS( m_nValue, PF_TRANSMIT );
END_PROPERTY_LIST();

ShipModifier::ShipModifier( ModifierType a_eType /*= MT_UNKNOWN*/, int a_nValue /*= 0*/ ) 
	: m_eType( a_eType ), m_nValue( a_nValue )
{}

CharString ShipModifier::getDescription( NounShip * a_pShip )
{
	int nVal = m_nValue;

	int nClamp = NounShip::modifierTypeClamp( m_eType );
	if ( nClamp != 0 && (a_pShip == NULL || (a_pShip->flags() & NounShip::FLAG_ADMIN) == 0) )
		nVal = Clamp<int>( nVal, -nClamp, nClamp );
			
	if ( nVal > 0 )
		return CharString().format( "Increases %s by %d%%", NounShip::modifierTypeText( m_eType ), nVal );
	else
		return CharString().format( "Decreases %s by %d%%", NounShip::modifierTypeText( m_eType ), nVal );
}

bool ShipModifier::canAttach( NounShip * a_pShip )
{
	return true;
}

bool ShipModifier::onApply( NounShip * a_pShip  )
{
	a_pShip->addModifier( m_eType, m_nValue );
	return true;
}

void ShipModifier::onRemove( NounShip * a_pShip )
{
	a_pShip->subtractModifier( m_eType, m_nValue );
}

//---------------------------------------------------------------------------------------------------

float NounShip::calculateModifier( NounEnhancement::Type a_eType, bool a_bInvert ) const
{
	int nVal = m_Modifiers[a_eType];

	int nClamp = modifierTypeClamp( a_eType );
	if ( nClamp != 0 && (flags() & NounShip::FLAG_ADMIN) == 0 )
		nVal = Clamp<int>( nVal, -nClamp, nClamp );

	float fAdjust = ((float)nVal) / 100.0f;
	if ( a_bInvert )
	{
		if ( fAdjust < 1.0f )
			return 1.0f - fAdjust;
		else
			return 0.0f;		// don't allow negative modifiers!
	}

	return 1.0f + fAdjust;
}

void NounShip::addModifier( NounEnhancement::Type a_eType, int a_nValue )
{
	m_Modifiers[ a_eType ] += a_nValue;
	onModifiersChanged();
}

void NounShip::subtractModifier( NounEnhancement::Type a_eType, int a_nValue )
{
	m_Modifiers[ a_eType ] -= a_nValue;
	onModifiersChanged();
}

int NounShip::getModifier( ModifierType a_eType )
{
	return m_Modifiers[ a_eType ];
}

void NounShip::resetModifiers()
{
	for(int i=0;i<MT_COUNT;++i)
		m_Modifiers[i] = 0;
}

void NounShip::onModifiersChanged()
{
	// notify all gadgets about the change as well..
	for(int i=0;i<m_Gadgets.size();++i)
		m_Gadgets[i]->onModifiersChanged();
}

//---------------------------------------------------------------------------------------------------

void NounShip::dropLoot()
{
	// platforms do not drop enhancements
	if(type() == PLATFORM || type() == PLATFORM_SUPPLY)
		return;

	CargoEnhancement::Ref pCargoDrop;

	Array< DropData *> drops;
	if( m_NounContext->findData<DropData>( drops ) > 0)
	{
		// add up the chances for each drop
		int nTotalChance = 0;
		for(int i=0;i<drops.size();i++)
			nTotalChance += drops[i]->m_nChance;

		if ( nTotalChance > 0 )
		{
			// pick a random number between 0 and total chance
			int randomDrop = rand() % nTotalChance;

			int nTotal = 0;
			// grab that drop
			DropData * pSelectedDrop = NULL;
			for(int i=0;i<drops.size() && pSelectedDrop == NULL;i++)
			{
				DropData * pDrop = drops[i];
				int nThisValue = nTotal+pDrop->m_nChance;
				if(randomDrop > nTotal && randomDrop < nThisValue)
					pSelectedDrop = pDrop;
				nTotal = nThisValue;
			}

			// if we have a drop, dump it onto a cargo and put it into space
			if( pSelectedDrop )
				pCargoDrop = new CargoEnhancement( pSelectedDrop->m_nDropClassKey );
		}
	}

	// this ship has no drops, so lets just use a default set of drops for now
	if (! pCargoDrop.valid() || pCargoDrop->enhancement() == NULL )
	{
		ClassKey key = DefaultLootTables::selectLoot(this);
		if(key != ClassKey( (qword)0 ) )
			pCargoDrop = new CargoEnhancement(key);
	}

	if ( pCargoDrop.valid() && pCargoDrop->enhancement() != NULL )
		addLoot( pCargoDrop );
}

// returns the ship that it was given too
NounShip * NounShip::addLoot( CargoEnhancement *a_pCargo )
{
	// total amount of damage currently done to this ship, minus the slow drain
	int nTotal = 0;
	if(m_ThreatList.size() == 0)
		return NULL;

	// first determine which ships could possibly get the loot
	typedef std::list<NounShip::Ref> ShipList;
	ShipList potentialList;

	// add up the total stored damage
	for(ThreatList::iterator iThreat = m_ThreatList.begin(); 
		iThreat != m_ThreatList.end(); ++iThreat )
	{
		Threat & t = *iThreat;

		if ( t.m_nDamage < (int)(maxDamage() * 0.05f) )
			continue;		// skip ships that have done less than 5% damage to this ship..

		// dont count ai ships or detached ships
		NounShip * pShip = WidgetCast<NounShip>( t.m_pThreat );
		if (! pShip || pShip->userId() == 0 || pShip->parent() == NULL )
			continue;

		potentialList.push_back( pShip );
		nTotal += t.m_nDamage;
	}

	// if no potential targets no player ships were involved in the kill so dont drop anything
	if( potentialList.size() == 0 )
		return NULL;

	// now pick a random ship to award the loot too
	NounShip * pSelectedShip = NULL;
	while( pSelectedShip == NULL )
	{
		// no possible ships can get this item, dump it into space
		// most likely this is because all original potential targets had full cargo holds
		if(potentialList.size() == 0)
		{
			Vector3 vPosition( worldPosition() + RandomVector( -radius(), radius() ) );
			vPosition.y = 0.0f;
			a_pCargo->setPosition( vPosition );
			a_pCargo->setTick( tick() );
			context()->attachNoun( a_pCargo );

			// let everyone int the area
			localChat( CharString().format( "Comms:<color;ffffff> No room in holds for <color;%6.6x>%s</color></color>....",  
				a_pCargo->enhancement()->color().RGB(),a_pCargo->enhancement()->getName()) );
			return NULL; 
		}

		// pick a random index from that list
		int selection = rand() % potentialList.size();
		// go through the list
		int v = 0;
		for(ShipList::iterator iShip = potentialList.begin();iShip != potentialList.end();++iShip,++v)
		{
			// is this the ship we want?
			if(selection != v)
				continue;
			// is there room on this ship for the new cargo?
			if((*iShip)->canAttachCargo(a_pCargo))
			{
				Noun * pHold = NounGame::findCargoHold( (*iShip), a_pCargo );
				if ( pHold != NULL )
				{
					a_pCargo->setTick( tick() );
					pHold->attachNode( a_pCargo );

					LOG_STATUS( "NounShip", "[ENHANCEMENT]:Spawn:%u:%s:%s",
						(*iShip)->userId(), a_pCargo->enhancement()->getName(),(*iShip)->name() );
					pSelectedShip = (*iShip);
				}
			}
			else
			{
				// we chose this ship, but it has no room. lets pick another one
				potentialList.erase(iShip);
				break;
			}
		}
	}

	// let everyone know who got the drop and who didnt 
	pSelectedShip->localChat( CharString().format( "Comms: <color;ffffff>%s receives <color;%6.6x>%s</color>...", 
		pSelectedShip->name(), a_pCargo->enhancement()->color().RGB(), a_pCargo->enhancement()->getName()) );
	return pSelectedShip;
}

bool NounShip::addLoot(	const CharString & a_sEnhName )
{
	std::list< ClassKey > classes;
	classes.push_back( NounEnhancement::classKey() );

	while( classes.size() > 0 )
	{
		ClassKey nClass = classes.front();
		classes.pop_front();

		Factory * pFactory = Factory::findFactory( nClass );
		if ( pFactory != NULL )
		{
			if ( !pFactory->isAbstract() && stristr( pFactory->className(), (const char *)a_sEnhName ) != NULL )
			{
				CargoEnhancement::Ref pCargo = new CargoEnhancement( pFactory->classKey() );
				attachNode( pCargo );
				LOG_STATUS( "NounShip", "[ENHANCEMENT]:Spawn:%u:%s:%s:[DEVSPAWN]",
					userId(), pCargo->enhancement()->getName(),NounShip::name() );
				privateChat( CharString().format( "<color;ffffff>DEV: Creating <color;%6.6x>%s</color>",
					pCargo->enhancement()->color().RGB(),pCargo->enhancement()->getName() ) );
				return true;
			}

		}

		// push child classes then..
		int typeCount = Factory::typeCount( nClass );
		for(int i=0;i<typeCount;i++)
			classes.push_back( Factory::type( nClass, i ) );
	}

	return false;
}

//---------------------------------------------------------------------------------------------------

const char * NounShip::modifierTypeText( ModifierType a_eType )
{
	static const char * TYPE_TEXT[] = 
	{
		"Unknown Effect",				// MT_UNKOWN
		"Drive Velocity",				// MT_DRIVE_VELOCITY,
		"Drive Thrust",					// MT_DRIVE_THRUST,
		"Beam Range",					// MT_BEAM_RANGE,
		"Beam Efficiency",				// MT_BEAM_ENERGY,
		"Beam Damage",					// MT_BEAM_DAMAGE,
		"Weapon Range",					// MT_WEAPON_RANGE,
		"Weapon Efficiency",			// MT_WEAPON_ENERGY,
		"Weapon Damage",				// MT_WEAPON_DAMAGE,
		"Weapon Cooldown",				// MT_WEAPON_COOLDOWN,
		"Weapon Ammo",					// MT_WEAPON_AMMO
		"Defense",						// MT_DEFENSE,		
		"Defense Recharge",				// MT_DEFENSE_RECHARGE
		"Auto-Repair Rate",				// MT_AUTOREPAIR,		
		"Build Speed",					// MT_BUILD_SPEED,			
		"Cloak Rate",					// MT_CLOAK,			
		"EWAR Range",					// MT_EWAR_RANGE,			
		"EWAR Strength",				// MT_EWAR_STRENGTH,
		"EWAR Efficiency",				// MT_EWAR_ENERGY
		"Jump Drive Range",				// MT_JUMPDRIVE_RANGE,		
		"Jump Gate Range",				// MT_JUMPGATE_RANGE,			
		"Mining Drones",				// MT_MINING,			
		"Pulse Shield Range",			// MT_PULSESHIELD,		
		"Reactor Boost",				// MT_REACTOR,			
		"Reload Rate",					// MT_RELOAD,			
		"Scanner Strength",				// MT_SCANNER,			
		"Tractor Range",				// MT_TRACTOR,			
		"Turn Rate",					// MT_TURNRATE,
		"Ship Energy Bank",				// MT_SHIPENERGY,
		"Damage Reduction",				// MT_DAMAGE_REDUCTION,
		"Capture Defense",				// MT_CAPTURE_DEFENSE,
		"Signature Reduction",			// MT_SIGNATURE
		"Jump Drive Cooldown",			// MT_JUMPCOOLDOWN
		"Jump Drive Velocity",			// MT_JUMPSPEED
		"Base Mass",					// MT_MASS
		"Planet Cap Rate",				// MT_CAPTURE_WEIGHT
	};

	if ( a_eType < 0 || a_eType >= sizeof(TYPE_TEXT)/sizeof(TYPE_TEXT[0]) )
		return "Unknown Type";

	return TYPE_TEXT[ a_eType ];
}

const int NounShip::modifierTypeClamp( ModifierType a_eType )
{
	static const int TYPE_CLAMP[] = 
	{
		0,					// MT_UNKOWN
		24,					// MT_DRIVE_VELOCITY,
		24,					// MT_DRIVE_THRUST,
		24,					// MT_BEAM_RANGE,
		48,					// MT_BEAM_ENERGY,
		24,					// MT_BEAM_DAMAGE,
		24,					// MT_WEAPON_RANGE,
		48,					// MT_WEAPON_ENERGY,
		24,					// MT_WEAPON_DAMAGE,
		24,					// MT_WEAPON_COOLDOWN,
		0,					// MT_WEAPON_AMMO
		24,					// MT_DEFENSE,		
		24,					// MT_DEFENSE_RECHARGE
		0,					// MT_AUTOREPAIR,		
		0,					// MT_BUILD_SPEED,			
		0,					// MT_CLOAK,			
		48,					// MT_EWAR_RANGE,			
		24,					// MT_EWAR_STRENGTH,
		48,					// MT_EWAR_ENERGY
		0,					// MT_JUMPDRIVE_RANGE,		
		0,					// MT_JUMPGATE_RANGE,			
		0,					// MT_MINING,			
		72,					// MT_PULSESHIELD,		
		0,					// MT_REACTOR,			
		0,					// MT_RELOAD,			
		0,					// MT_SCANNER,			
		0,					// MT_TRACTOR,			
		0,					// MT_TURNRATE,
		0, 					// MT_SHIPENERGY,
		0,					// MT_DAMAGE_REDUCTION,
		0,					// MT_CAPTURE_DEFENSE,
		0,					// MT_SIGNATURE
		0,					// MT_JUMPCOOLDOWN
		0,					// MT_JUMPSPEED
		18,					// MT_MASS
		18,					// MT_CAPTURE_WEIGHT
	};

	if ( a_eType < 0 || a_eType >= sizeof(TYPE_CLAMP)/sizeof(TYPE_CLAMP[0]) )
		return 0;

	return TYPE_CLAMP[ a_eType ];
}


//---------------------------------------------------------------------------------------------------
//EOF
