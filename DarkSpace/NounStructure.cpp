/*
	NounStructure.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "World/VerbChat.h"
#include "SceneryEffect.h"
#include "NounPlanet.h"
#include "NounStructure.h"
#include "NounGadget.h"
#include "VerbDestroyStructure.h"
#include "resource.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

bool NounStructure::sm_bRender = true;

//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_FACTORY( NounStructure, NounGame ); 
REGISTER_FACTORY_KEY( NounStructure, 4229501729010438515LL );

BEGIN_ABSTRACT_PROPERTY_LIST( NounStructure, NounGame );
	CLEAR_PROPERTY_FLAGS( m_Position, PF_TRANSMIT );		// don't transmit, our position or orientation is set by our hex...
	CLEAR_PROPERTY_FLAGS( m_vOrientation, PF_TRANSMIT );	
	ADD_TRANSMIT_PROPERTY( m_nHex );
	ADD_REPLICATED_PROPERTY( m_nBuildTime, TICKS_PER_SECOND * 30 );
	ADD_REPLICATED_PROPERTY( m_Damage, TICKS_PER_SECOND * 30 );
END_PROPERTY_LIST();

NounStructure::NounStructure() : m_nHex( 0xffff ), m_nBuildTime( 0 ), m_Damage( 0 )
{
	m_nNodeFlags |= NF_NOSHADOW;
	setFlags( FLAG_ACTIVE );
}

//---------------------------------------------------------------------------------------------------

bool NounStructure::read( const InStream & input )
{
	if (! Noun::read( input ) )
		return false;
	updatePosition();
	return true;
}

//----------------------------------------------------------------------------

void NounStructure::onAttached()
{
	Noun::onAttached();
	updatePosition();
}

void NounStructure::render( RenderContext &context, const Matrix33 & frame, const Vector3 & position )
{
	if ( sm_bRender )
		Noun::render( context, frame, position );
	else
		NodeTransform::render( context, frame, position );
}

//----------------------------------------------------------------------------

NounType NounStructure::nounType() const
{
	return TYPE_LOCAL_OBJECT;
}

bool NounStructure::canBeDetected() const
{
	return true;
}

float NounStructure::baseSignature() const
{
	return 1.0f;
}

bool NounStructure::canDamage( dword type ) const
{
	return (type & (DAMAGE_KINETIC|DAMAGE_ENERGY)) != 0;
}

bool NounStructure::canInteract( NounInteraction nInteraction, Noun * pWith )
{
	return false;
}

void NounStructure::initialize()
{
	Noun::initialize();
	updatePosition();

	for(int i=0;i<childCount();i++)
	{
		NounGadget * pGadget = WidgetCast<NounGadget>( child(i) );
		if (! pGadget )
			continue;
		pGadget->setLevel( gadgetLevel() );
	}
}

void NounStructure::interact( NounInteraction nInterfaction, Noun * pWidth, int nTicks )
{}

void NounStructure::inflictDamage( dword nWhen, Noun * pFrom, int damage, dword type, const Vector3 & direction )
{
	// don't allow more damage than possible
	int possible = maxDamage() - m_Damage;
	if ( damage > possible )
		damage = possible;

	if ( pFrom != NULL)
	{
		float ratio = float( damage ) / maxDamage();
		if ( isFriend( pFrom ) )
			gameContext()->gameUser()->onFriendlyFire( pFrom, ratio );
		else
			gameContext()->gameUser()->onPlanetsDamaged( pFrom, ratio );
	}

	m_Damage += damage;
	if ( isServer() && m_Damage >= maxDamage() )
		Verb::Ref( new VerbDestroyStructure( this, pFrom ) );
}

//----------------------------------------------------------------------------

int NounStructure::maxDamage() const
{
	return 1000;
}

int NounStructure::repairRate() const
{
	return 10;
}

int NounStructure::buildTechnology() const
{
	return 0;
}

int NounStructure::buildTime() const
{
	return 60;
}

int NounStructure::buildCost() const
{
	return 100;
}

dword NounStructure::buildFlags() const
{
	return 0;
}

CharString NounStructure::status() const
{
	CharString info;
	info += "\n";

	if ( damage() > 0 )
		info += CharString().format("<color;0000ff>DAMAGE: %d%%</color>\n", (damage() * 100) / maxDamage() );
	if ( workers() != 0 )
		info += CharString().format( "Workers: %s\n", FormatNumber<char,int>( -workers() ).cstr() );
	if ( power() != 0 )
		info += CharString().format( "Power: %s\n", FormatNumber<char,int>( power() ).cstr() );
	if ( technology() != 0 )
		info += CharString().format( "Technology: %d\n", technology() );
	if ( upkeep() != 0 )
		info += CharString().format( "Upkeep: %d\n", upkeep() );

	if ( active() )
	{
		if ( food() != 0 )
			info += CharString().format( "Food: %s\n", FormatNumber<char,int>( food() ).cstr() );
		if ( habitat() != 0 )
			info += CharString().format( "Habitat: %s\n", FormatNumber<char,int>( habitat() ).cstr() );
		if ( mining() != 0 )
			info += CharString().format( "Mining: %s\n", FormatNumber<char,int>( mining() ).cstr() );
		if ( research() != 0 )
			info += CharString().format( "Research: %s\n", FormatNumber<char,int>( research() ).cstr() );
		if ( production() != 0 )
			info += CharString().format( "Production: %s\n", FormatNumber<char,int>( production() ).cstr() );
		if ( protection() != 0.0f )
			info += CharString().format( "PD: %d%%\n", (int)(protection() * 100) );
	}
	else if ( flags() & FLAG_ACTIVE )
	{
		if ( flags() & FLAG_WORKER_SHORT )
			info += "<color;0000ff>WORKER SHORTAGE!</color>\n";
		if ( flags() & FLAG_POWER_SHORT )
			info += "<color;0000ff>POWER SHORTAGE!</color>\n";
		if ( flags() & FLAG_TECH_SHORT )
			info += "<color;0000ff>TECHNOLOGY SHORTAGE!</color>\n";
	}
	else
	{
		info += "\n<color;ffffff>INACTIVE</color>\n";
	}

	if ( isBuilding() )
		info += CharString().format("\nBuilding...%d%% complete\n", buildComplete() );

	return info;
}

Color NounStructure::color() const
{
	return DARK_GREEN;
}

bool NounStructure::canBuild( NounPlanet * pPlanet, bool bUpgrade ) const
{
	if ( pPlanet == NULL )
		return false;		// invalid pointer

	if ( bUpgrade )
	{
		if ( (pPlanet->structureCount() - 1) >= pPlanet->maxStructures() )
			return false;
	}
	else
	{
		if ( pPlanet->structureCount() >= pPlanet->maxStructures() )
			return false;		// maximum number of structures already built
	}
	if ( pPlanet->technology() < buildTechnology() )
		return false;		// technology level not high enough to build structure
	if ( (pPlanet->flags() & buildFlags()) != buildFlags() )
		return false;		// required planet flags not found

	return true;
}

bool NounStructure::canUpgrade( NounStructure * pStructure ) const
{
	return false;
}

int NounStructure::sortId() const
{
	return 0;
}

int NounStructure::groupId() const
{
	return 0;
}

dword NounStructure::planetFlags() const
{
	return 0;
}

int	NounStructure::workers() const
{
	return 0;
}

int NounStructure::power() const
{
	return 0;
}

int NounStructure::food() const
{
	return 0;
}

int NounStructure::habitat() const
{
	return 0;
}

int	NounStructure::mining() const
{
	return 0;
}

int	NounStructure::research() const
{
	return 0;
}

int NounStructure::production() const
{
	return 0;
}

int NounStructure::technology() const
{
	return 0;
}

int NounStructure::upkeep() const
{
	return 0;
}

int NounStructure::upkeepDamageRate() const
{
	return Max( maxDamage() / 60, 1 );		// take 1 hour before structure is destroyed because of lack of resources..
}

float NounStructure::control() const
{
	return 0.0f;
}

int NounStructure::defense() const
{
	return 0;
}

float NounStructure::protection() const
{
	return 0.0f;
}

int NounStructure::gadgetLevel() const
{
	return 5;
}

bool NounStructure::usable( NounShip * pShip ) const
{
	return false;
}

CharString NounStructure::useTip( NounShip * pShip ) const
{
	return "";
}

bool NounStructure::useConfirm( NounShip * pShip ) const
{
	return false;
}

CharString NounStructure::useConfirmMessage( NounShip * pShip ) const
{
	return "";
}

void NounStructure::use( dword nWhen, NounShip * pShip )
{}


//----------------------------------------------------------------------------
void NounStructure::setHex( u16 nHex )
{
	m_nHex = nHex;
	updatePosition();
}

bool NounStructure::updatePosition()
{
	NounPlanet * pPlanet = planet();
	if (! pPlanet  )
	{
		// structure is not on planet... how odd!
		setPosition( Vector3(0,0,0) );
		return true;
	}

	if (! pPlanet->pathMap() )
		return false;		// hex map isn't initialized yet... 

	if (! pPlanet->isHexValid( m_nHex ) )
		m_nHex = pPlanet->findHex( position(), true );
	if (! pPlanet->isHexValid( m_nHex ) )
		return false;		// don't crash at the very least!
	setPosition( pPlanet->hex( m_nHex ).position );

	Matrix33 frame( Vector3(1,0,0), position(), Vector3(0,0,1 ) );
	frame.orthoNormalizeXZ();

	setFrame( frame );

	return true;
}

void NounStructure::setActive( bool active )
{
	setFlags( FLAG_ACTIVE, active );
}

void NounStructure::setDamage( int damage )
{
	m_Damage = damage;
}

void NounStructure::scrap()
{
	// return resources back to the planet when scrapped
	NounPlanet * pPlanet = planet();
	if ( pPlanet != NULL )
	{
		// return half of used resources
		int nRestore = buildCost() / 2;
		// reduce restored resources by damage
		nRestore -= (nRestore * damage()) / maxDamage();
		// restore resources to planet
		pPlanet->addResources( nRestore );
	}

	m_nBuildTime = 0;
	// detach this object from the planet
	setDetach();
}

bool NounStructure::build()
{
	if (! isBuilding() )
		return true;

	m_nBuildTime += 1;
	if (! isBuilding() )
	{
		// construction complete!
		if ( isServer() && planet() != NULL )
			factionChat( CharString().format( "<color;ffffff>Comms: %s %s constructed", planet()->name(), name() ) );

		return true;
	}
	return false;
}

void NounStructure::repair()
{
	// repair damage to structure
	if ( m_Damage > 0 )
		m_Damage = Max( m_Damage - repairRate(), 0 );
}

//----------------------------------------------------------------------------

void NounStructure::destroy( Noun * pKiller )
{
	// add explosion object for client only
	if ( context()->isClient() )
	{
		Scene * pDestroy = WidgetCast<Scene>( nounContext()->resource( "DESTROY" ) );
		if ( pDestroy != NULL )
		{
			// structure destroyed, create the explosion effect
			SceneryEffect * pEffect = new SceneryEffect;
			pEffect->setNounContext( new NounContext( pDestroy ) );
			pEffect->setContext( context() );
			pEffect->setFrame( frame() );
			pEffect->setPosition( position() );
			pEffect->setDelay( 0.0f );

			if (! pEffect->setSegment( "Effect" ) )
				pEffect->setLife( 15.0f );

			context()->attachNoun( pEffect, parent() );
		}
	}

	// add some resources back into the planet
	NounPlanet * pPlanet = planet();
	if( pPlanet )
        pPlanet->addResources( buildCost() / 10 );

	// lastly, detach this object from the world
	setDetach();
}

//----------------------------------------------------------------------------

NounPlanet * NounStructure::planet() const
{
	return WidgetCast<NounPlanet>( parent() );
}

float NounStructure::getTechPercentage() const
{
	NounPlanet * pPlanet = planet();
	if( !pPlanet )
		return 0.0f;
	if( pPlanet->technology() >= this->technology() || this->technology() == 0  )
		return 1.0f;
	return (float)pPlanet->technology() / (float)this->technology();
}

//----------------------------------------------------------------------------
//EOF
