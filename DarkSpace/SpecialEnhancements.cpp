/**
	@file SpecialEnhancements.cpp
	
	(c)2009 Palestar Inc
	@author Richard Lyle @date 5/6/2010 10:04:55 PM
*/

#include "SpecialEnhancements.h"
#include "GameContext.h"
#include "NounTrail.h"
#include "NounSpawnShip.h"

#define ENABLE_ALLY_ENHANCEMENTS			1

//! Max number of ally ships allowed..
static Constant			MAX_ALLY_SHIPS( "MAX_ALLY_SHIPS", 4.0f );
static Constant			MAX_ALLY_TIME( "MAX_ALLY_TIME", TICKS_PER_SECOND * 3600 );				// how long do ally ships stick around

//---------------------------------------------------------------------------------------------------

typedef std::list< NounTrail * >		TrailList;
typedef std::list< NounEnhancement * >	EnhList;

//---------------------------------------------------------------------------------------------------

IMPLEMENT_FACTORY( JumpDriveRefill, NounEnhancement );
IMPLEMENT_FACTORY( EnergyRefill, NounEnhancement );
#if ENABLE_ALLY_ENHANCEMENTS
IMPLEMENT_FACTORY( ScoutAlly, NounEnhancement );
IMPLEMENT_FACTORY( FrigateAlly, NounEnhancement );
IMPLEMENT_FACTORY( DestroyerAlly, NounEnhancement );
IMPLEMENT_FACTORY( CruiserAlly, NounEnhancement );
IMPLEMENT_FACTORY( SupplyAlly, NounEnhancement );
IMPLEMENT_FACTORY( DreadAlly, NounEnhancement );
IMPLEMENT_FACTORY( StationAlly, NounEnhancement );
#endif
IMPLEMENT_FACTORY( UpgradedBuildDrones, NounEnhancement );
IMPLEMENT_FACTORY( DeepPurpleExhaust, NounEnhancement );
IMPLEMENT_FACTORY( ForestGreenExhaust, NounEnhancement );
IMPLEMENT_FACTORY( RedHotExhaust, NounEnhancement );
IMPLEMENT_FACTORY( DarkBlueExhaust, NounEnhancement );
IMPLEMENT_FACTORY( WhiteExhaust, NounEnhancement );
IMPLEMENT_FACTORY( RainbowExhaust, NounEnhancement );
IMPLEMENT_FACTORY( DeepPinkGradExhaust, NounEnhancement );

//---------------------------------------------------------------------------------------------------

REGISTER_FACTORY_ALIAS( JumpDriveRefill, NounEnhancement_FullJumpDrive );
REGISTER_FACTORY_ALIAS( EnergyRefill, NounEnhancement_HalfEnergy );
REGISTER_FACTORY_ALIAS( UpgradedBuildDrones, NounEnhancement_LevelOneBuild );
REGISTER_FACTORY_ALIAS( DeepPurpleExhaust, NounEnhancement_TrailDeepPurple );
REGISTER_FACTORY_ALIAS( ForestGreenExhaust, NounEnhancement_TrailForestGreen );
REGISTER_FACTORY_ALIAS( RedHotExhaust, NounEnhancement_TrailRedHot );
REGISTER_FACTORY_ALIAS( DarkBlueExhaust, NounEnhancement_TrailDarkBlue );
REGISTER_FACTORY_ALIAS( WhiteExhaust, NounEnhancement_TrailWhite );
REGISTER_FACTORY_ALIAS( RainbowExhaust, NounEnhancement_TrailRainbow );
REGISTER_FACTORY_ALIAS( DeepPinkGradExhaust, NounEnhancement_TrailDeepPinkGrad );

//---------------------------------------------------------------------------------------------------

IMPLEMENT_FACTORY( JumpFuelModifier, EnhancementModifier );
BEGIN_PROPERTY_LIST( JumpFuelModifier, EnhancementModifier );
	ADD_TRANSMIT_PROPERTY( m_nPercent );
END_PROPERTY_LIST();

CharString JumpFuelModifier::getDescription( NounShip * a_pShip )
{
	return CharString().format( "Increases ships jump fuel by %d%%", m_nPercent );
}

bool JumpFuelModifier::canAttach( NounShip * a_pShip )
{
	return true;
}

bool JumpFuelModifier::onApply( NounShip * a_pShip )
{
	GadgetJumpDrive * pDrive = a_pShip->jumpDrive();
	if (! pDrive )
		return false;

	pDrive->refuel( m_nPercent );
	return true;
}

void JumpFuelModifier::onRemove( NounShip * a_pShip )
{}

//---------------------------------------------------------------------------------------------------

IMPLEMENT_FACTORY( ShipEnergyModifier, EnhancementModifier );
BEGIN_PROPERTY_LIST( ShipEnergyModifier, EnhancementModifier );
	ADD_TRANSMIT_PROPERTY( m_nPercent );
END_PROPERTY_LIST();

CharString ShipEnergyModifier::getDescription( NounShip * a_pShip )
{
	return CharString().format( "Increases ships energy by %d%%", m_nPercent );
}

bool ShipEnergyModifier::canAttach( NounShip * a_pShip )
{
	return true;
}

bool ShipEnergyModifier::onApply( NounShip * a_pShip )
{
	a_pShip->refuelEnergy( m_nPercent );
	return true;
}

void ShipEnergyModifier::onRemove( NounShip * a_pShip )
{}

//---------------------------------------------------------------------------------------------------

static CharString FormatTickDuration( dword a_nTicks )
{
	float fSeconds = a_nTicks * TICK_DURATION_S;
	if ( fSeconds > (24.0f*60.0f*60.0f) )
		return CharString().format( "%.1f Days", fSeconds / (24.0f*60.0f*60.0f) );
	else if ( fSeconds > (2.0f*60.0f*60.0f) )
		return CharString().format( "%.1f Hours", fSeconds / (60.0f*60.0f) );
	else if ( fSeconds > (5.0f*60.0f) )
		return CharString().format( "%.1f Minutes", fSeconds / 60.0f );

	return CharString().format( "%.0f Seconds", fSeconds );
}

IMPLEMENT_FACTORY( SpawnShipModifier, EnhancementModifier );

CharString SpawnShipModifier::getDescription( NounShip * a_pShip )
{
	return CharString().format( "Use to receive a %s ally for %s.", 
		NounShip::typeText( m_eType ), FormatTickDuration( MAX_ALLY_TIME ).cstr() );
}

bool SpawnShipModifier::canAttach( NounShip * a_pShip )
{
	return true;
}

bool SpawnShipModifier::onApply( NounShip * a_pShip )
{
	// all logic occurs on the server only..
	if ( a_pShip->isServer() )
	{
		GameContext * pContext = (GameContext *)a_pShip->context();
		if (! pContext )
			return false;
		
		if ( a_pShip->validateAllyShips() >= MAX_ALLY_SHIPS )
			return false;
		int nTeamId = a_pShip->teamId();
		NounTemplate * pShipTemplate = pContext->pickShipTemplate( nTeamId, m_eType );
		if( ! pShipTemplate )
			return false;
		NounShip::Ref pSpawnedShip = WidgetCast<NounShip>( pShipTemplate->spawn() );
		if (! pSpawnedShip )
			return false;

		pSpawnedShip->setName( CharString().format( "[ALLY] %s", a_pShip->name() ) );
		pSpawnedShip->setTeamId( nTeamId );
		pSpawnedShip->setRank( 0 );							// user might get demoted, so no rank..
		pSpawnedShip->addFlags( NounShip::FLAG_AI_ALLY );	// flag this ship as an AI ally
		pSpawnedShip->setHome( a_pShip );					// our home is the player that spawns us..

		if ( m_eType == NounShip::SUPPLY )
			pSpawnedShip->setOrder( NounShip::RELOAD, a_pShip, a_pShip );
		else 
			pSpawnedShip->setOrder( NounShip::DEFEND, a_pShip, a_pShip );

		Vector3 vMyPosition( a_pShip->worldPosition() );

		// lastly, try to place the ship out about 3k so it will jump in the ship that summon it..
		int nAttempt = 0;
		while( nAttempt <= 3 )
		{
			Vector3 vDirection( RandomFloat(-1.0f,1.0f), 0.0f, RandomFloat(-1.0f,1.0f) );
			vDirection.normalize();

			Vector3 vPosition( vMyPosition + (vDirection * 3000.0f) );

			Array< GameContext::NounCollision > collisions;
			if (! pContext->proximityCheck( vPosition, pSpawnedShip->radius() * 5.0f, collisions ) )
			{
				pSpawnedShip->setPosition( vPosition );
				pSpawnedShip->setup();
				
				pContext->attachNoun( pSpawnedShip );
				a_pShip->addAlly( pSpawnedShip );

				return true;
			}

			++nAttempt;
		}

		// failed to place ship in area that is clear..
		TRACE( "Failed to find place to spawn ally ship!" );
		return false;
	}

	return true;
}

void SpawnShipModifier::onRemove( NounShip * a_pShip )
{}


//---------------------------------------------------------------------------------------------------

IMPLEMENT_FACTORY( TrailColorModifier, EnhancementModifier );
BEGIN_PROPERTY_LIST( TrailColorModifier, EnhancementModifier );
	ADD_ENUM_PROPERTY( m_eColor );
		ADD_PROPERTY_FLAGS( m_eColor, PF_TRANSMIT );
		ADD_ENUM_OPTION( m_eColor, TRAIL_PURPLE );
		ADD_ENUM_OPTION( m_eColor, TRAIL_GREEN );
		ADD_ENUM_OPTION( m_eColor, TRAIL_RED );
		ADD_ENUM_OPTION( m_eColor, TRAIL_BLUE );
		ADD_ENUM_OPTION( m_eColor, TRAIL_WHITE );
		ADD_ENUM_OPTION( m_eColor, TRAIL_RAINBOW );
		ADD_ENUM_OPTION( m_eColor, TRAIL_PINK );
END_PROPERTY_LIST();

CharString TrailColorModifier::getDescription( NounShip * a_pShip )
{
	switch( m_eColor )
	{
	case TRAIL_PURPLE:
		return "Gives your ship purple engine trails";
	case TRAIL_GREEN:
		return "Gives your ship Green engine trails";
	case TRAIL_RED:
		return "Gives your ship Red engine trails";
	case TRAIL_BLUE:
		return "Gives your ship Blue engine trails";
	case TRAIL_WHITE:
		return "Gives your ship White engine trails";		
	case TRAIL_RAINBOW:
		return "Gives your ship multi-colored engine trails";
	case TRAIL_PINK:
		return "Gives your ship Pink engine trails";		
	}

	return "";
}

bool TrailColorModifier::canAttach( NounShip * a_pShip )
{
	// make sure there are no other Trail's already attached
	EnhList enhancments;
	FindNodeClasses<NounEnhancement>( a_pShip, enhancments );

	for( EnhList::iterator iEnh = enhancments.begin(); iEnh != enhancments.end(); ++iEnh )
	{
		NounEnhancement * pEnh = *iEnh;
		for(int i=0;i<pEnh->modifierCount();++i)
			if ( WidgetCast<TrailColorModifier>( pEnh->modifier(i) ) )
				return false;		// found another trail modifier on the ship, can't attach!
	}

	return true;
}

bool TrailColorModifier::onApply( NounShip * a_pShip )
{
	Material::Link pMat;
	switch( m_eColor )
	{
	case TRAIL_PURPLE:
		loadResource("effects\\materials\\trail_deeppurple.prt",pMat);
		break;
	case TRAIL_GREEN:
		loadResource("effects\\materials\\trail_forestgreen.prt",pMat);
		break;
	case TRAIL_RED:
		loadResource("effects\\materials\\trail_redhot.prt",pMat);
		break;
	case TRAIL_BLUE:
		loadResource("effects\\materials\\trail_darkblue.prt",pMat);
		break;
	case TRAIL_WHITE:
		loadResource("effects\\materials\\trail_white.prt",pMat);
		break;
	case TRAIL_RAINBOW:
		loadResource("effects\\materials\\trail_rainbow.prt",pMat);
		break;
	case TRAIL_PINK:
		loadResource("effects\\materials\\trail_deeppink.prt",pMat);
		break;
	}

	if ( pMat.valid() )
	{
		TrailList trails;
		FindNodeClasses<NounTrail>( a_pShip, trails );

		for( TrailList::iterator iTrail = trails.begin(); iTrail != trails.end(); ++iTrail )
			(*iTrail)->setTrailMaterial( pMat );
	}
	else
	{
		TRACE( "Failed to load trail color modifier material!" );
	}

	return true;
}

void TrailColorModifier::onRemove( NounShip * a_pShip )
{
	TrailList trails;
	FindNodeClasses<NounTrail>( a_pShip, trails );

	for( TrailList::iterator iTrail = trails.begin(); iTrail != trails.end(); ++iTrail )
		(*iTrail)->resetTrail();
}

//---------------------------------------------------------------------------------------------------
//EOF
